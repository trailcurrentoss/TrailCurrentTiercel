/*
 * Persistent settings, stored in NVS. See tiercel_config.h for the contract.
 */

#include <string.h>          /* strncpy/memset -- included explicitly rather
                                than inherited from an esp_* header */

#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "tiercel_config.h"

static const char *TAG = "config";

/*
 * Only this namespace is erased by a factory reset, so the Wi-Fi driver's
 * own calibration and PHY data survive it.
 */
#define NS "tiercel"

/*
 * NVS keys are capped at 15 characters, so these are abbreviated rather
 * than descriptive. Do not lengthen them without checking the limit -- an
 * over-long key fails at runtime with ESP_ERR_NVS_KEY_TOO_LONG, not at
 * compile time.
 */
#define K_WIFI_SSID  "w_ssid"
#define K_WIFI_PASS  "w_pass"
#define K_WIFI_SEC   "w_sec"
#define K_WIFI_OK    "w_ok"
#define K_MQTT_HOST  "m_host"
#define K_MQTT_PORT  "m_port"
#define K_MQTT_USER  "m_user"
#define K_MQTT_PASS  "m_pass"
#define K_MQTT_OK    "m_ok"
#define K_DISP_C     "d_celsius"
#define K_DISP_LVLMM "d_lvl_mm"
#define K_DISP_24H   "d_24h"
#define K_DISP_BACK  "d_back"
#define K_DISP_HAPT  "d_haptics"
#define K_DISP_VOICE "d_voice_vol"
#define K_DISP_FACE  "d_face"
#define K_DISP_DARK  "d_dark"
#define K_DISP_BL    "d_backlight"
#define K_DISP_IDLE  "d_idle"
#define K_DISP_SNOOZE "d_snooze"
#define K_SERVICES   "services"
#define K_PG_CA      "pg_ca"
#define K_CONTROLS   "ctrls"
#define K_ALARMS     "alarms"
#define K_MODE       "mode"

static SemaphoreHandle_t      s_lock;
static tiercel_wifi_cfg_t     s_wifi;
static tiercel_mqtt_cfg_t     s_mqtt;
static tiercel_display_cfg_t  s_display;
static tiercel_services_cfg_t s_services;
/* Peregrine's CA: TIERCEL_CA_MAX_LEN bytes in PSRAM, allocated once at init.
 * Kept out of s_services -- see tiercel_config.h. */
static char                  *s_pg_ca;
static uint32_t               s_pg_ca_gen;
static tiercel_controls_t     s_controls;
static tiercel_alarms_t       s_alarms;
static tiercel_mode_t         s_mode;

static const char *const s_sec_names[TIERCEL_WIFI_SEC_COUNT] = {
    [TIERCEL_WIFI_SEC_OPEN]          = "Open",
    [TIERCEL_WIFI_SEC_WEP]           = "WEP",
    [TIERCEL_WIFI_SEC_WPA_PSK]       = "WPA",
    [TIERCEL_WIFI_SEC_WPA2_PSK]      = "WPA2",
    [TIERCEL_WIFI_SEC_WPA_WPA2_PSK]  = "WPA/WPA2",
    [TIERCEL_WIFI_SEC_WPA3_PSK]      = "WPA3",
    [TIERCEL_WIFI_SEC_WPA2_WPA3_PSK] = "WPA2/WPA3",
};

static const char *const s_mode_names[TIERCEL_MODE_COUNT] = {
    [TIERCEL_MODE_CAMPING] = "camping",
    [TIERCEL_MODE_DRIVING] = "driving",
    [TIERCEL_MODE_STORAGE] = "storage",
};

const char *tiercel_mode_name(tiercel_mode_t mode)
{
    if (mode < 0 || mode >= TIERCEL_MODE_COUNT || !s_mode_names[mode]) {
        return "camping";
    }
    return s_mode_names[mode];
}

tiercel_mode_t tiercel_mode_from_name(const char *name, tiercel_mode_t fallback)
{
    if (!name) {
        return fallback;
    }
    for (int i = 0; i < TIERCEL_MODE_COUNT; i++) {
        if (s_mode_names[i] && strcmp(name, s_mode_names[i]) == 0) {
            return (tiercel_mode_t)i;
        }
    }
    /* An unknown mode keeps whatever we had rather than silently becoming
     * camping. A newer Headwaters adding a fourth mode should not quietly
     * re-interpret every alarm on an older panel. */
    return fallback;
}

static const char *const s_verdict_names[] = {
    [TIERCEL_VERDICT_NONE] = "none",
    [TIERCEL_VERDICT_HIGH] = "high",
    [TIERCEL_VERDICT_LOW]  = "low",
};

const char *tiercel_verdict_name(tiercel_verdict_t v)
{
    if (v < 0 || v > TIERCEL_VERDICT_LOW || !s_verdict_names[v]) {
        return "none";
    }
    return s_verdict_names[v];
}

tiercel_verdict_t tiercel_verdict_from_name(const char *name)
{
    if (name) {
        for (int i = 0; i <= TIERCEL_VERDICT_LOW; i++) {
            if (s_verdict_names[i] && strcmp(name, s_verdict_names[i]) == 0) {
                return (tiercel_verdict_t)i;
            }
        }
    }
    /* Unknown means quiet. A verdict this firmware does not understand must
     * not become an alarm that cannot be turned off from the PWA. */
    return TIERCEL_VERDICT_NONE;
}

const char *tiercel_wifi_sec_name(tiercel_wifi_sec_t sec)
{
    if (sec < 0 || sec >= TIERCEL_WIFI_SEC_COUNT || !s_sec_names[sec]) {
        return "Unknown";
    }
    return s_sec_names[sec];
}

/* ------------------------------------------------------------------ */

static void load_defaults(void)
{
    memset(&s_wifi, 0, sizeof(s_wifi));
    memset(&s_mqtt, 0, sizeof(s_mqtt));

    s_wifi.security = TIERCEL_WIFI_SEC_WPA2_PSK;   /* the common case */

    /* Host, username and password are deliberately empty. There is no
     * sensible default for "which rig is this" -- the user supplies it, and
     * a baked-in guess would just point the device at somebody else's
     * broker. Only the port has a right answer. */
    s_mqtt.port = CONFIG_TIERCEL_MQTT_DEFAULT_PORT;

    s_display.celsius = 
#ifdef CONFIG_TIERCEL_DEFAULT_UNITS_CELSIUS
        true;
#else
        false;
#endif
    /* Metric heights with metric temperatures by default; either can then
     * be changed on its own in Settings > Locale. */
    s_display.level_mm = s_display.celsius;
    s_display.dark_theme =
#ifdef CONFIG_TIERCEL_DEFAULT_THEME_DARK
        true;
#else
        false;
#endif
    s_display.backlight_percent = 100;
    s_display.idle_timeout_s    = CONFIG_TIERCEL_IDLE_TIMEOUT_S;
    s_display.alarm_snooze_min  = CONFIG_TIERCEL_ALARM_SNOOZE_MIN;

    /* No controls and no alarms until Headwaters sends some. An empty dial is
     * a correct first-boot state, not a fault -- and disarmed is the only
     * safe default, since an alarm invented locally would be a false one. */
    memset(&s_controls, 0, sizeof(s_controls));
    memset(&s_alarms, 0, sizeof(s_alarms));
    s_mode = TIERCEL_MODE_CAMPING;

    s_display.back_gesture = TIERCEL_BACK_SWIPE;   /* the design's default */
    s_display.haptics      = true;
    s_display.voice_volume = 60;

    memset(&s_services, 0, sizeof(s_services));
    if (s_pg_ca) {
        s_pg_ca[0] = '\0';
    }
    s_pg_ca_gen++;
}

/* Read a string, leaving the destination untouched if the key is absent so
 * the default survives. */
static void read_str(nvs_handle_t h, const char *key, char *dst, size_t cap)
{
    size_t len = cap;
    if (nvs_get_str(h, key, dst, &len) != ESP_OK) {
        return;
    }
    dst[cap - 1] = '\0';
}

static void read_u8(nvs_handle_t h, const char *key, uint8_t *dst)
{
    uint8_t v;
    if (nvs_get_u8(h, key, &v) == ESP_OK) {
        *dst = v;
    }
}

static void read_u16(nvs_handle_t h, const char *key, uint16_t *dst)
{
    uint16_t v;
    if (nvs_get_u16(h, key, &v) == ESP_OK) {
        *dst = v;
    }
}

static void read_bool(nvs_handle_t h, const char *key, bool *dst)
{
    uint8_t v;
    if (nvs_get_u8(h, key, &v) == ESP_OK) {
        *dst = (v != 0);
    }
}

static esp_err_t load_from_nvs(void)
{
    nvs_handle_t h;
    esp_err_t err = nvs_open(NS, NVS_READONLY, &h);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        /* First boot: the namespace does not exist yet. Not an error. */
        ESP_LOGI(TAG, "no saved settings; using defaults");
        return ESP_OK;
    }
    ESP_RETURN_ON_ERROR(err, TAG, "nvs_open failed");

    read_str(h, K_WIFI_SSID, s_wifi.ssid,     sizeof(s_wifi.ssid));
    read_str(h, K_WIFI_PASS, s_wifi.password, sizeof(s_wifi.password));
    uint8_t sec = (uint8_t)s_wifi.security;
    read_u8(h, K_WIFI_SEC, &sec);
    s_wifi.security = (sec < TIERCEL_WIFI_SEC_COUNT)
                    ? (tiercel_wifi_sec_t)sec
                    : TIERCEL_WIFI_SEC_WPA2_PSK;
    read_bool(h, K_WIFI_OK, &s_wifi.configured);

    read_str(h, K_MQTT_HOST, s_mqtt.host,     sizeof(s_mqtt.host));
    read_str(h, K_MQTT_USER, s_mqtt.username, sizeof(s_mqtt.username));
    read_str(h, K_MQTT_PASS, s_mqtt.password, sizeof(s_mqtt.password));
    read_u16(h, K_MQTT_PORT, &s_mqtt.port);
    read_bool(h, K_MQTT_OK, &s_mqtt.configured);

    /* Same blob rule as the controls below: a size mismatch is a struct
     * from another firmware, and Headwaters will resend it. */
    size_t svc_len = sizeof(s_services);
    tiercel_services_cfg_t svc;
    if (nvs_get_blob(h, K_SERVICES, &svc, &svc_len) == ESP_OK &&
        svc_len == sizeof(s_services)) {
        svc.peregrine_url[TIERCEL_URL_MAX_LEN - 1]     = '\0';
        svc.playbill_device[TIERCEL_SLUG_MAX_LEN - 1]  = '\0';
        s_services = svc;
    }
    if (s_pg_ca) {
        read_str(h, K_PG_CA, s_pg_ca, TIERCEL_CA_MAX_LEN);
    }

    /* One blob, not a key per control: 15-character keys leave no room for
     * an index plus a field name, and a partial list read back after a
     * power cut mid-write would be worse than none. A size mismatch means
     * the struct changed across a firmware update -- discard rather than
     * reinterpret, the retained topic will refill it within seconds of the
     * broker connecting. */
    size_t ctrl_len = sizeof(s_controls);
    tiercel_controls_t ctrls;
    if (nvs_get_blob(h, K_CONTROLS, &ctrls, &ctrl_len) == ESP_OK &&
        ctrl_len == sizeof(s_controls) &&
        ctrls.count <= TIERCEL_MAX_CONTROLS) {
        s_controls = ctrls;
    }

    size_t alarm_len = sizeof(s_alarms);
    tiercel_alarms_t alarms;
    if (nvs_get_blob(h, K_ALARMS, &alarms, &alarm_len) == ESP_OK &&
        alarm_len == sizeof(s_alarms) &&
        alarms.count <= TIERCEL_MAX_ALARMS) {
        s_alarms = alarms;
    }

    uint8_t mode = (uint8_t)s_mode;
    read_u8(h, K_MODE, &mode);
    if (mode < TIERCEL_MODE_COUNT) {
        s_mode = (tiercel_mode_t)mode;
    }

    read_bool(h, K_DISP_C,    &s_display.celsius);
    read_bool(h, K_DISP_LVLMM, &s_display.level_mm);
    read_bool(h, K_DISP_24H,   &s_display.clock_24h);
    uint8_t back = (uint8_t)s_display.back_gesture;
    read_u8(h, K_DISP_BACK, &back);
    s_display.back_gesture = back < TIERCEL_BACK_COUNT
                           ? (tiercel_back_gesture_t)back : TIERCEL_BACK_SWIPE;
    read_bool(h, K_DISP_HAPT, &s_display.haptics);
    read_u8(h,   K_DISP_VOICE, &s_display.voice_volume);
    if (s_display.voice_volume > 100) {
        s_display.voice_volume = 100;
    }
    read_u8(h,   K_DISP_FACE, &s_display.clock_face);
    read_bool(h, K_DISP_DARK, &s_display.dark_theme);
    read_u8(h,   K_DISP_BL,   &s_display.backlight_percent);
    read_u16(h,  K_DISP_IDLE, &s_display.idle_timeout_s);
    read_u16(h,  K_DISP_SNOOZE, &s_display.alarm_snooze_min);
    if (s_display.alarm_snooze_min == 0) {
        /* 0 would mean re-raise on the very next refresh -- an alarm that
         * cannot be acknowledged. Treat it as unset. */
        s_display.alarm_snooze_min = CONFIG_TIERCEL_ALARM_SNOOZE_MIN;
    }

    nvs_close(h);
    return ESP_OK;
}

esp_err_t tiercel_config_init(void)
{
    if (!s_lock) {
        s_lock = xSemaphoreCreateMutex();
        ESP_RETURN_ON_FALSE(s_lock, ESP_ERR_NO_MEM, TAG, "mutex alloc failed");
    }
    if (!s_pg_ca) {
        s_pg_ca = heap_caps_calloc(1, TIERCEL_CA_MAX_LEN, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        ESP_RETURN_ON_FALSE(s_pg_ca, ESP_ERR_NO_MEM, TAG, "CA buffer alloc failed");
    }

    load_defaults();
    esp_err_t err = load_from_nvs();

    /*
     * Log what was loaded, but NEVER the credentials. A passphrase in a
     * serial log outlives the session it was captured in.
     */
    ESP_LOGI(TAG, "peregrine: %s%s, playbill: %s",
             s_services.peregrine_url[0] ? s_services.peregrine_url : "<unset>",
             s_pg_ca[0] ? " (CA)" : "",
             s_services.playbill_device[0] ? s_services.playbill_device : "<auto>");
    /* The CA is a ~1.4 KB string (about 45 entries, plus as many again
     * while a replacement is written), in a partition shared with the
     * Wi-Fi driver. Headroom is logged so a full NVS is visible before a
     * CA save fails on it. */
    nvs_stats_t st;
    if (nvs_get_stats(NULL, &st) == ESP_OK) {
        ESP_LOGI(TAG, "nvs: %u of %u entries used, %u free",
                 (unsigned)st.used_entries, (unsigned)st.total_entries,
                 (unsigned)st.free_entries);
    }
    ESP_LOGI(TAG, "device controls: %u, alarms: %u, mode: %s",
             (unsigned)s_controls.count, (unsigned)s_alarms.count,
             tiercel_mode_name(s_mode));
    ESP_LOGI(TAG, "wifi: %s (%s), mqtt: %s:%u %s",
             s_wifi.configured ? s_wifi.ssid : "<unset>",
             tiercel_wifi_sec_name(s_wifi.security),
             s_mqtt.configured ? s_mqtt.host : "<unset>",
             (unsigned)s_mqtt.port,
             s_mqtt.username[0] ? "(auth)" : "(no user)");
    return err;
}

/* ------------------------------------------------------------------ */

/* Variadic so a body with a bare comma in it (an initialiser list, a
 * declaration of two variables) is still one body, not a macro error. */
#define WITH_LOCK(...)                                   \
    do {                                                 \
        xSemaphoreTake(s_lock, portMAX_DELAY);           \
        __VA_ARGS__;                                     \
        xSemaphoreGive(s_lock);                          \
    } while (0)

void tiercel_config_get_wifi(tiercel_wifi_cfg_t *out)
{
    if (!out) { return; }
    WITH_LOCK(*out = s_wifi);
}

void tiercel_config_get_mqtt(tiercel_mqtt_cfg_t *out)
{
    if (!out) { return; }
    WITH_LOCK(*out = s_mqtt);
}

void tiercel_config_get_display(tiercel_display_cfg_t *out)
{
    if (!out) { return; }
    WITH_LOCK(*out = s_display);
}

void tiercel_config_get_services(tiercel_services_cfg_t *out)
{
    if (!out) { return; }
    WITH_LOCK(*out = s_services);
}

esp_err_t tiercel_config_set_services(const tiercel_services_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg, ESP_ERR_INVALID_ARG, TAG, "null services");

    /* Normalised so an unterminated tail cannot make memcmp report a change
     * that is not one -- see tiercel_config_set_controls(). */
    tiercel_services_cfg_t want;
    memset(&want, 0, sizeof(want));
    strncpy(want.peregrine_url, cfg->peregrine_url, TIERCEL_URL_MAX_LEN - 1);
    strncpy(want.playbill_device, cfg->playbill_device, TIERCEL_SLUG_MAX_LEN - 1);

    bool unchanged;
    WITH_LOCK(unchanged = (memcmp(&want, &s_services, sizeof(want)) == 0));
    if (unchanged) {
        return ESP_OK;
    }

    nvs_handle_t h;
    ESP_RETURN_ON_ERROR(nvs_open(NS, NVS_READWRITE, &h), TAG, "nvs_open failed");
    esp_err_t err = nvs_set_blob(h, K_SERVICES, &want, sizeof(want));
    if (err == ESP_OK) {
        err = nvs_commit(h);
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "services save failed");

    WITH_LOCK(s_services = want);
    ESP_LOGI(TAG, "services saved: peregrine %s, playbill %s",
             want.peregrine_url[0] ? want.peregrine_url : "<unset>",
             want.playbill_device[0] ? want.playbill_device : "<auto>");
    return ESP_OK;
}

size_t tiercel_config_get_peregrine_ca(char *out, size_t cap)
{
    if (!out || cap == 0) {
        return 0;
    }
    size_t n = 0;
    WITH_LOCK({
        const char *src = s_pg_ca ? s_pg_ca : "";
        n = strlen(src);
        if (n >= cap) {
            n = cap - 1;
        }
        memcpy(out, src, n);
        out[n] = '\0';
    });
    return n;
}

uint32_t tiercel_config_peregrine_ca_gen(void)
{
    uint32_t g;
    WITH_LOCK(g = s_pg_ca_gen);
    return g;
}

esp_err_t tiercel_config_set_peregrine_ca(const char *pem)
{
    ESP_RETURN_ON_FALSE(s_pg_ca, ESP_ERR_INVALID_STATE, TAG, "config not initialised");
    if (!pem) {
        pem = "";
    }
    const size_t len = strlen(pem);
    ESP_RETURN_ON_FALSE(len < TIERCEL_CA_MAX_LEN, ESP_ERR_INVALID_SIZE, TAG,
                        "peregrine CA is %u B, over %u", (unsigned)len,
                        (unsigned)(TIERCEL_CA_MAX_LEN - 1));

    bool unchanged;
    WITH_LOCK(unchanged = (strcmp(pem, s_pg_ca) == 0));
    if (unchanged) {
        return ESP_OK;
    }

    nvs_handle_t h;
    ESP_RETURN_ON_ERROR(nvs_open(NS, NVS_READWRITE, &h), TAG, "nvs_open failed");
    esp_err_t err = len ? nvs_set_str(h, K_PG_CA, pem) : nvs_erase_key(h, K_PG_CA);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        err = ESP_OK;                   /* clearing what was never saved */
    }
    if (err == ESP_OK) {
        err = nvs_commit(h);
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "peregrine CA save failed");

    WITH_LOCK({
        memcpy(s_pg_ca, pem, len + 1);
        s_pg_ca_gen++;
    });
    if (len) {
        ESP_LOGI(TAG, "peregrine CA saved (%u B)", (unsigned)len);
    } else {
        ESP_LOGI(TAG, "peregrine CA cleared");
    }
    return ESP_OK;
}

void tiercel_config_get_controls(tiercel_controls_t *out)
{
    if (!out) { return; }
    WITH_LOCK(*out = s_controls);
}

esp_err_t tiercel_config_set_controls(const tiercel_controls_t *controls)
{
    ESP_RETURN_ON_FALSE(controls, ESP_ERR_INVALID_ARG, TAG, "null controls");
    ESP_RETURN_ON_FALSE(controls->count <= TIERCEL_MAX_CONTROLS,
                        ESP_ERR_INVALID_ARG, TAG, "too many controls (%u)",
                        (unsigned)controls->count);

    /*
     * Normalise before comparing. The caller fills only `count` entries, so
     * whatever is in the tail is its business -- but if the tail differs the
     * memcmp below reports a change that isn't one, and the retained topic
     * would then rewrite flash on every single broker reconnect.
     */
    tiercel_controls_t want;
    memset(&want, 0, sizeof(want));
    want.count = controls->count;
    for (uint8_t i = 0; i < controls->count; i++) {
        want.items[i].id = controls->items[i].id;
        strncpy(want.items[i].name, controls->items[i].name,
                TIERCEL_CONTROL_NAME_MAX - 1);
        strncpy(want.items[i].icon, controls->items[i].icon,
                TIERCEL_CONTROL_ICON_MAX - 1);
    }

    bool unchanged;
    WITH_LOCK(unchanged = (memcmp(&want, &s_controls, sizeof(want)) == 0));
    if (unchanged) {
        return ESP_OK;
    }

    nvs_handle_t h;
    ESP_RETURN_ON_ERROR(nvs_open(NS, NVS_READWRITE, &h), TAG, "nvs_open failed");
    esp_err_t err = nvs_set_blob(h, K_CONTROLS, &want, sizeof(want));
    if (err == ESP_OK) {
        err = nvs_commit(h);
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "control save failed");

    WITH_LOCK(s_controls = want);
    ESP_LOGI(TAG, "device controls saved: %u", (unsigned)want.count);
    return ESP_OK;
}

void tiercel_config_get_alarms(tiercel_alarms_t *out)
{
    if (!out) { return; }
    WITH_LOCK(*out = s_alarms);
}

esp_err_t tiercel_config_set_alarms(const tiercel_alarms_t *alarms)
{
    ESP_RETURN_ON_FALSE(alarms, ESP_ERR_INVALID_ARG, TAG, "null alarms");
    ESP_RETURN_ON_FALSE(alarms->count <= TIERCEL_MAX_ALARMS,
                        ESP_ERR_INVALID_ARG, TAG, "too many alarms (%u)",
                        (unsigned)alarms->count);

    /* Normalised before comparing, exactly as the controls are: the caller
     * fills only `count` entries, and an unzeroed tail would report a change
     * on every retained redelivery and rewrite flash for nothing. */
    tiercel_alarms_t want;
    memset(&want, 0, sizeof(want));
    want.count = alarms->count;
    for (uint8_t i = 0; i < alarms->count; i++) {
        want.items[i].src    = alarms->items[i].src;
        want.items[i].addr   = alarms->items[i].addr;
        want.items[i].sensor = alarms->items[i].sensor;
        for (int m = 0; m < TIERCEL_MODE_COUNT; m++) {
            want.items[i].modes[m] = alarms->items[i].modes[m];
        }
        strncpy(want.items[i].name, alarms->items[i].name,
                TIERCEL_ALARM_NAME_MAX - 1);
        strncpy(want.items[i].icon, alarms->items[i].icon,
                TIERCEL_ALARM_ICON_MAX - 1);
    }

    bool unchanged;
    WITH_LOCK(unchanged = (memcmp(&want, &s_alarms, sizeof(want)) == 0));
    if (unchanged) {
        return ESP_OK;
    }

    nvs_handle_t h;
    ESP_RETURN_ON_ERROR(nvs_open(NS, NVS_READWRITE, &h), TAG, "nvs_open failed");
    esp_err_t err = nvs_set_blob(h, K_ALARMS, &want, sizeof(want));
    if (err == ESP_OK) {
        err = nvs_commit(h);
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "alarm save failed");

    WITH_LOCK(s_alarms = want);
    ESP_LOGI(TAG, "alarms saved: %u", (unsigned)want.count);
    return ESP_OK;
}

tiercel_mode_t tiercel_config_get_mode(void)
{
    tiercel_mode_t m;
    WITH_LOCK(m = s_mode);
    return m;
}

esp_err_t tiercel_config_set_mode(tiercel_mode_t mode)
{
    ESP_RETURN_ON_FALSE(mode >= 0 && mode < TIERCEL_MODE_COUNT,
                        ESP_ERR_INVALID_ARG, TAG, "bad mode %d", (int)mode);

    bool unchanged;
    WITH_LOCK(unchanged = (s_mode == mode));
    if (unchanged) {
        return ESP_OK;
    }

    nvs_handle_t h;
    ESP_RETURN_ON_ERROR(nvs_open(NS, NVS_READWRITE, &h), TAG, "nvs_open failed");
    esp_err_t err = nvs_set_u8(h, K_MODE, (uint8_t)mode);
    if (err == ESP_OK) {
        err = nvs_commit(h);
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "mode save failed");

    WITH_LOCK(s_mode = mode);
    ESP_LOGI(TAG, "rig mode: %s", tiercel_mode_name(mode));
    return ESP_OK;
}

bool tiercel_config_is_provisioned(void)
{
    bool ok;
    WITH_LOCK(ok = s_wifi.configured && s_mqtt.configured);
    return ok;
}

/* ------------------------------------------------------------------ */

esp_err_t tiercel_config_set_wifi(const tiercel_wifi_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg, ESP_ERR_INVALID_ARG, TAG, "null cfg");

    nvs_handle_t h;
    ESP_RETURN_ON_ERROR(nvs_open(NS, NVS_READWRITE, &h), TAG, "nvs_open failed");

    esp_err_t err = ESP_OK;
    err |= nvs_set_str(h, K_WIFI_SSID, cfg->ssid);
    err |= nvs_set_str(h, K_WIFI_PASS, cfg->password);
    err |= nvs_set_u8(h,  K_WIFI_SEC,  (uint8_t)cfg->security);
    err |= nvs_set_u8(h,  K_WIFI_OK,   1);
    if (err == ESP_OK) {
        err = nvs_commit(h);   /* commit before returning: a power cut right
                                  after "Save" must not lose the value */
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "wifi save failed");

    WITH_LOCK({
        s_wifi = *cfg;
        s_wifi.configured = true;
    });
    ESP_LOGI(TAG, "wifi saved: %s (%s)", cfg->ssid,
             tiercel_wifi_sec_name(cfg->security));
    return ESP_OK;
}

esp_err_t tiercel_config_set_mqtt(const tiercel_mqtt_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg, ESP_ERR_INVALID_ARG, TAG, "null cfg");

    nvs_handle_t h;
    ESP_RETURN_ON_ERROR(nvs_open(NS, NVS_READWRITE, &h), TAG, "nvs_open failed");

    esp_err_t err = ESP_OK;
    err |= nvs_set_str(h,  K_MQTT_HOST, cfg->host);
    err |= nvs_set_u16(h,  K_MQTT_PORT, cfg->port);
    err |= nvs_set_str(h,  K_MQTT_USER, cfg->username);
    err |= nvs_set_str(h,  K_MQTT_PASS, cfg->password);
    err |= nvs_set_u8(h,   K_MQTT_OK,   1);
    if (err == ESP_OK) {
        err = nvs_commit(h);
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "mqtt save failed");

    WITH_LOCK({
        s_mqtt = *cfg;
        s_mqtt.configured = true;
    });
    ESP_LOGI(TAG, "mqtt saved: %s:%u", cfg->host, (unsigned)cfg->port);
    return ESP_OK;
}

esp_err_t tiercel_config_set_display(const tiercel_display_cfg_t *cfg)
{
    ESP_RETURN_ON_FALSE(cfg, ESP_ERR_INVALID_ARG, TAG, "null cfg");

    nvs_handle_t h;
    ESP_RETURN_ON_ERROR(nvs_open(NS, NVS_READWRITE, &h), TAG, "nvs_open failed");

    esp_err_t err = ESP_OK;
    err |= nvs_set_u8(h,  K_DISP_C,    cfg->celsius ? 1 : 0);
    err |= nvs_set_u8(h,  K_DISP_LVLMM, cfg->level_mm ? 1 : 0);
    err |= nvs_set_u8(h,  K_DISP_24H,   cfg->clock_24h ? 1 : 0);
    err |= nvs_set_u8(h,  K_DISP_BACK, (uint8_t)cfg->back_gesture);
    err |= nvs_set_u8(h,  K_DISP_HAPT, cfg->haptics ? 1 : 0);
    err |= nvs_set_u8(h,  K_DISP_VOICE, cfg->voice_volume);
    err |= nvs_set_u8(h,  K_DISP_FACE, cfg->clock_face);
    err |= nvs_set_u8(h,  K_DISP_DARK, cfg->dark_theme ? 1 : 0);
    err |= nvs_set_u8(h,  K_DISP_BL,   cfg->backlight_percent);
    err |= nvs_set_u16(h, K_DISP_IDLE, cfg->idle_timeout_s);
    err |= nvs_set_u16(h, K_DISP_SNOOZE, cfg->alarm_snooze_min);
    if (err == ESP_OK) {
        err = nvs_commit(h);
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "display save failed");

    WITH_LOCK(s_display = *cfg);
    return ESP_OK;
}

esp_err_t tiercel_config_factory_reset(void)
{
    ESP_LOGW(TAG, "factory reset: erasing all saved settings");

    nvs_handle_t h;
    esp_err_t err = nvs_open(NS, NVS_READWRITE, &h);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        /* Nothing saved -- already in the factory state. */
        WITH_LOCK(load_defaults());
        return ESP_OK;
    }
    ESP_RETURN_ON_ERROR(err, TAG, "nvs_open failed");

    err = nvs_erase_all(h);
    if (err == ESP_OK) {
        err = nvs_commit(h);
    }
    nvs_close(h);
    ESP_RETURN_ON_ERROR(err, TAG, "erase failed");

    WITH_LOCK(load_defaults());
    ESP_LOGW(TAG, "factory reset complete -- reboot to apply");
    return ESP_OK;
}
