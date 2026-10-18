/*
 * TrailCurrent Tiercel -- touch-first rotary controller.
 *
 * Boot order matters:
 *   1. NVS, then settings -- everything after reads them.
 *   2. The board: panel, touch, knob, haptics, LVGL. Backlight stays off.
 *   3. Audio and the Peregrine client, configured from the services
 *      Headwaters last sent (kept in NVS).
 *   4. Wi-Fi: rejoin the saved network, or go to the setup portal.
 *   5. The data model, MQTT and discovery.
 *   6. The UI -- or, before the first EEZ Studio export, the bring-up screen.
 *   7. Backlight on, once a frame has been drawn.
 *   8. The service task, which drains MQTT into the model.
 */

#include <string.h>

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "discovery.h"
#include "tiercel_audio.h"
#include "tiercel_board.h"
#include "tiercel_bt.h"
#include "tiercel_config.h"
#include "tiercel_model.h"
#include "tiercel_mqtt.h"
#include "tiercel_peregrine.h"
#include "tiercel_playbill.h"
#include "tiercel_wifi.h"

#ifndef TIERCEL_HAVE_UI
#  error "TIERCEL_HAVE_UI is not defined -- main/CMakeLists.txt must set it"
#endif

#if TIERCEL_HAVE_UI
#  include "ui.h"
#  include "ui_clock.h"
#  include "ui_data.h"
#  include "ui_nav.h"
#  include "ui_peregrine.h"
#  include "ui_playbill.h"
#  include "ui_settings.h"
#  include "ui_setup.h"
#else
#  include "bringup.h"
#endif

static const char *TAG = "tiercel";

#define SERVICE_TASK_STACK 6144
#define SERVICE_TASK_PRIO  4

static bool s_needs_setup;

#if TIERCEL_HAVE_UI
/*
 * ui_tick() reads every expression-bound property, and nothing calls it for
 * us on the device: without this the screens draw once and never change.
 * Runs in the LVGL task, which already holds the display lock.
 */
static void ui_tick_timer_cb(lv_timer_t *t)
{
    (void)t;
    ui_tick();
}
#endif

static void on_discovery_trigger(void *ctx)
{
    (void)ctx;
    discovery_handle_trigger();
}

/*
 * Keeps Peregrine pointed at whatever Headwaters last said. The services
 * topic can change at any time (the user edits it in the PWA), and the
 * config component is the one place that knows; this copies across when it
 * changes rather than having the voice client poll NVS.
 */
static void sync_services(void)
{
    static tiercel_services_cfg_t s_seen;
    static uint32_t s_seen_ca_gen;
    static bool s_first = true;
    tiercel_services_cfg_t now;
    tiercel_config_get_services(&now);
    const uint32_t ca_gen = tiercel_config_peregrine_ca_gen();
    if (!s_first && ca_gen == s_seen_ca_gen &&
        memcmp(&now, &s_seen, sizeof(now)) == 0) {
        return;
    }
    /* The CA is up to 2 KB: borrowed from PSRAM for the hand-over only,
     * never on this task's stack. The client keeps its own copy. */
    char *ca = heap_caps_malloc(TIERCEL_CA_MAX_LEN, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!ca) {
        return;                         /* retried on the next tick */
    }
    tiercel_config_get_peregrine_ca(ca, TIERCEL_CA_MAX_LEN);
    s_first = false;
    s_seen = now;
    s_seen_ca_gen = ca_gen;
    tiercel_peregrine_configure(now.peregrine_url, ca);
    heap_caps_free(ca);
}

static void service_task(void *arg)
{
    (void)arg;
    uint32_t ticks = 0;
    while (true) {
#if TIERCEL_HAVE_UI
        /* While a discovery window is open the broker is down on purpose --
         * reconnecting here would undo the handshake's teardown. */
        if (!discovery_is_running()) {
            ui_data_service_tick();
        }
#endif
        tiercel_mqtt_process();
        tiercel_mqtt_check_watchdogs();
        sync_services();

        if (++ticks % 500 == 0) {      /* every 10 s */
            ESP_LOGI(TAG, "service alive: %u B stack free, internal %u B free "
                          "/ %u B largest, mqtt %s",
                     (unsigned)uxTaskGetStackHighWaterMark(NULL),
                     (unsigned)heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                     (unsigned)heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
                     tiercel_mqtt_state_name(tiercel_mqtt_state()));
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG, "NVS needs erasing (%s)", esp_err_to_name(err));
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    if (tiercel_config_init() != ESP_OK) {
        ESP_LOGE(TAG, "settings failed to load -- using defaults");
    }
    tiercel_display_cfg_t disp;
    tiercel_config_get_display(&disp);

    ESP_ERROR_CHECK(tiercel_board_init());
    tiercel_board_haptics_enable(disp.haptics);
    tiercel_board_knob_set_ratio(disp.knob_ratio);

    if (tiercel_audio_init() != ESP_OK) {
        ESP_LOGE(TAG, "audio did not start -- Peregrine will be unavailable");
    }
    tiercel_audio_set_volume(disp.voice_volume);
    tiercel_peregrine_init();
    /* Headphones only: silent unless the co-processor runs Tiercel's
     * firmware, and it prompts for nothing -- pairing is Settings >
     * Bluetooth. */
    tiercel_bt_init();

    const esp_err_t wifi_err = tiercel_wifi_init();
    if (wifi_err != ESP_OK) {
        ESP_LOGE(TAG, "wifi init failed (%s)", esp_err_to_name(wifi_err));
    } else {
        tiercel_wifi_cfg_t w;
        tiercel_config_get_wifi(&w);
        if (w.configured && w.ssid[0]) {
            ESP_LOGI(TAG, "rejoining '%s'", w.ssid);
            tiercel_wifi_connect();
        } else {
            /* New or factory-reset: straight to the setup portal, raised once
             * the Setup screen exists to show its name and password. */
            ESP_LOGI(TAG, "no saved network -- entering setup mode");
            s_needs_setup = true;
        }
    }

    tiercel_model_init();
    {
        tiercel_services_cfg_t svc;
        tiercel_config_get_services(&svc);
        tiercel_playbill_set_target(svc.playbill_device);
    }
    sync_services();
    if (tiercel_mqtt_init() != ESP_OK) {
        ESP_LOGE(TAG, "mqtt init failed -- no data will arrive");
    }
    discovery_init();
    tiercel_mqtt_set_discovery_callback(on_discovery_trigger, NULL);

    if (tiercel_board_lock(0)) {
#if TIERCEL_HAVE_UI
        const size_t before = esp_get_free_heap_size();
        ui_init();
        ui_settings_apply_theme();
        lv_timer_create(ui_tick_timer_cb, 20, NULL);
        ui_peregrine_init();
        ui_playbill_init();
        ui_nav_init();
        ui_data_init();
        if (s_needs_setup) {
            ui_setup_enter();
        }
        ui_clock_init();
        ESP_LOGI(TAG, "UI built -- %u KB heap used, %u KB free",
                 (unsigned)((before - esp_get_free_heap_size()) / 1024),
                 (unsigned)(esp_get_free_heap_size() / 1024));
#else
        bringup_screen();
        ESP_LOGW(TAG, "no generated UI -- showing the bring-up screen. Export "
                      "GUI/Tiercel.eez-project from EEZ Studio (Ctrl+B).");
#endif
        tiercel_board_unlock();
    }

    vTaskDelay(pdMS_TO_TICKS(60));
    tiercel_board_backlight_set(disp.backlight_percent);

    if (xTaskCreate(service_task, "tiercel_svc", SERVICE_TASK_STACK, NULL,
                    SERVICE_TASK_PRIO, NULL) != pdPASS) {
        ESP_LOGE(TAG, "service task did not start -- MQTT will not be parsed");
    }
    ESP_LOGI(TAG, "running");
}
