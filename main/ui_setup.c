/*
 * Setup mode.
 *
 * The portal component owns the AP, the DNS responder and the HTTP
 * server. This owns only what the panel shows while that is happening,
 * and the decision about when it is over.
 */

#include "esp_timer.h"
#include "esp_log.h"

#include "tiercel_config.h"
#include "tiercel_portal.h"
#include "tiercel_wifi.h"
#include "ui_nav.h"
#include "ui_setup.h"

#ifndef TIERCEL_HAVE_UI
#  error "TIERCEL_HAVE_UI is not defined -- main/CMakeLists.txt must set it"
#endif

#if TIERCEL_HAVE_UI

#include "screens.h"
#include "ui_lv.h"
#include "ui.h"

static const char *TAG = "ui.setup";

/*
 * How long to keep the portal up after credentials are saved.
 *
 * The phone has just been told "saved, you can close this page". Tearing
 * the AP down in the same instant drops that phone's connection before
 * the message renders, so the user sees the page die and assumes it
 * failed. A couple of seconds is enough for the response to land and be
 * read.
 */
#define LINGER_MS 2500

static bool    s_active;
static int64_t s_saved_ms;

static void set(lv_obj_t *label, const char *text)
{
    ui_lv_set_text(label, text ? text : "");
}

void ui_setup_enter(void)
{
    if (s_active) {
        return;
    }
    if (tiercel_portal_start() != ESP_OK) {
        ESP_LOGE(TAG, "could not start the setup portal");
        return;
    }
    s_active = true;
    s_saved_ms = 0;

    char url[40];
    snprintf(url, sizeof(url), "http://%s", tiercel_wifi_ap_ip());

    set(objects.setup_ssid, tiercel_wifi_ap_ssid());
    set(objects.setup_pass, tiercel_wifi_ap_password());
    set(objects.setup_url, url);
    set(objects.setup_status, "Waiting for phone...");

    ESP_LOGI(TAG, "setup mode: '%s' / '%s'",
             tiercel_wifi_ap_ssid(), tiercel_wifi_ap_password());

    ui_nav_goto(TIERCEL_SCREEN_SETUP);
}

void ui_setup_exit(void)
{
    if (!s_active) {
        return;
    }
    tiercel_portal_stop();
    s_active = false;
    ESP_LOGI(TAG, "setup mode ended");
}

bool ui_setup_active(void)
{
    return s_active;
}

void ui_setup_tick(void)
{
    if (!s_active) {
        return;
    }

    /* Drives the provisioning check. /save only starts it; without this
     * nothing is ever verified and nothing is ever written to NVS. */
    tiercel_portal_tick();

    if (tiercel_portal_got_credentials()) {
        if (s_saved_ms == 0) {
            s_saved_ms = (int64_t)lv_tick_get();
            set(objects.setup_status, "Verified and saved -- connecting...");
            return;
        }
        if ((int64_t)lv_tick_elaps((uint32_t)s_saved_ms) < LINGER_MS) {
            return;     /* let the phone read the confirmation first */
        }

        ui_setup_exit();

        /* Join with what was just saved. The service task picks the
         * broker up once there is an IP; discovery then finds the unit and
         * Headwaters sends the rest (services, controls, alarms). */
        tiercel_wifi_connect();
        ui_nav_goto(TIERCEL_SCREEN_SETTINGS);
        return;
    }

    /* Tell the user the hard part worked. Before this the screen looked
     * identical whether or not the phone had joined. */
    set(objects.setup_status,
        tiercel_portal_client_count() > 0
            ? "Phone connected -- open the page"
            : "Waiting for phone...");
}

#else

void ui_setup_enter(void) { }
void ui_setup_exit(void) { }
bool ui_setup_active(void) { return false; }
void ui_setup_tick(void) { }

#endif
