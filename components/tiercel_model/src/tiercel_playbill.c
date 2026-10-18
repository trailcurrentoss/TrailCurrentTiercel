/*
 * Playbill state. See tiercel_playbill.h.
 */

#include <string.h>

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "tiercel_playbill.h"

static const char *TAG = "playbill";

/*
 * How long a value the knob just set outranks Playbill's own report of it.
 *
 * Volume and tuning are sent as the knob turns, and Playbill reports back a
 * few hundred ms later -- by which time the knob has moved on. Taking every
 * report at face value made the rim snap backwards mid-turn and then jump
 * forwards again. Holding off briefly after a local change lets the turn
 * finish; the first report after the hold-off is authoritative.
 */
#define ECHO_HOLD_US (1500 * 1000)

static SemaphoreHandle_t  s_lock;
static tiercel_playbill_t s_pb;
static char               s_target[TIERCEL_PB_SLUG_MAX];   /* "" = auto */
static int64_t            s_pos_at_us;     /* when position_ms was reported */
static int64_t            s_vol_echo_us;
static int64_t            s_freq_echo_us;

#define LOCK()   xSemaphoreTake(s_lock, portMAX_DELAY)
#define UNLOCK() xSemaphoreGive(s_lock)

static void copy(char *dst, size_t cap, const char *src)
{
    strncpy(dst, src ? src : "", cap - 1);
    dst[cap - 1] = '\0';
}

static void forget_locked(void)
{
    memset(&s_pb, 0, sizeof(s_pb));
    s_pos_at_us = 0;
}

void tiercel_playbill_init(void)
{
    if (!s_lock) {
        s_lock = xSemaphoreCreateMutex();
    }
    LOCK();
    forget_locked();
    UNLOCK();
}

void tiercel_playbill_set_target(const char *slug)
{
    LOCK();
    if (strncmp(s_target, slug ? slug : "", sizeof(s_target)) != 0) {
        copy(s_target, sizeof(s_target), slug);
        forget_locked();
        if (s_target[0]) {
            copy(s_pb.slug, sizeof(s_pb.slug), s_target);
            s_pb.known = true;
        }
        ESP_LOGI(TAG, "following %s", s_target[0] ? s_target : "<first online>");
    }
    UNLOCK();
}

bool tiercel_playbill_accept(const char *slug, bool online_hint)
{
    if (!slug || !slug[0] || strcmp(slug, "all") == 0) {
        return false;
    }
    bool ok;
    LOCK();
    if (s_pb.known) {
        ok = strcmp(slug, s_pb.slug) == 0;
    } else if (!s_target[0] && online_hint) {
        /* Auto-adopt only on a presence report saying it is online, so a
         * stale retained status from a Playbill that has since been removed
         * cannot win the race. */
        copy(s_pb.slug, sizeof(s_pb.slug), slug);
        s_pb.known = true;
        ok = true;
        ESP_LOGI(TAG, "adopted Playbill '%s' (none named by Headwaters)", slug);
    } else {
        ok = false;
    }
    UNLOCK();
    return ok;
}

void tiercel_playbill_set_presence(bool online, const char *name)
{
    LOCK();
    s_pb.online = online;
    if (name && name[0]) {
        copy(s_pb.name, sizeof(s_pb.name), name);
    }
    UNLOCK();
}

void tiercel_playbill_set_source(const char *source)
{
    LOCK();
    copy(s_pb.source, sizeof(s_pb.source), source);
    UNLOCK();
}

void tiercel_playbill_set_transport(bool has_media, bool paused,
                                    const char *title, const char *subtitle,
                                    int64_t position_ms, int64_t duration_ms)
{
    LOCK();
    s_pb.has_media = has_media;
    s_pb.paused    = paused;
    copy(s_pb.title, sizeof(s_pb.title), title);
    copy(s_pb.subtitle, sizeof(s_pb.subtitle), subtitle);
    s_pb.position_ms = position_ms > 0 ? (uint32_t)position_ms : 0;
    s_pb.duration_ms = duration_ms > 0 ? (uint32_t)duration_ms : 0;
    s_pos_at_us = esp_timer_get_time();
    UNLOCK();
}

void tiercel_playbill_set_volume(int pct, bool muted)
{
    LOCK();
    s_pb.muted = muted;
    if (esp_timer_get_time() - s_vol_echo_us > ECHO_HOLD_US) {
        s_pb.volume_pct   = (uint8_t)(pct < 0 ? 0 : pct > 100 ? 100 : pct);
        s_pb.volume_known = true;
    }
    UNLOCK();
}

void tiercel_playbill_set_radio(bool running, const char *band,
                                uint32_t frequency_hz)
{
    LOCK();
    s_pb.radio_running = running;
    if (band) {
        s_pb.band = strcmp(band, "fm") == 0 ? TIERCEL_PB_BAND_FM
                  : strcmp(band, "am") == 0 ? TIERCEL_PB_BAND_AM
                                            : TIERCEL_PB_BAND_OTHER;
    }
    if (frequency_hz && esp_timer_get_time() - s_freq_echo_us > ECHO_HOLD_US) {
        s_pb.frequency_hz = frequency_hz;
    }
    UNLOCK();
}

void tiercel_playbill_set_stations(const uint32_t *hz, int count)
{
    uint32_t tmp[TIERCEL_PB_STATIONS];
    int n = 0;
    for (int i = 0; i < count && n < TIERCEL_PB_STATIONS; i++) {
        if (hz[i] >= 87000000u && hz[i] <= 108500000u) {
            tmp[n++] = hz[i];
        }
    }
    for (int i = 1; i < n; i++) {               /* insertion sort, n <= 24 */
        const uint32_t v = tmp[i];
        int j = i - 1;
        while (j >= 0 && tmp[j] > v) {
            tmp[j + 1] = tmp[j];
            j--;
        }
        tmp[j + 1] = v;
    }
    LOCK();
    memcpy(s_pb.stations_hz, tmp, n * sizeof(uint32_t));
    s_pb.station_count = (uint8_t)n;
    UNLOCK();
}

void tiercel_playbill_echo_volume(int pct)
{
    LOCK();
    s_pb.volume_pct   = (uint8_t)(pct < 0 ? 0 : pct > 100 ? 100 : pct);
    s_pb.volume_known = true;
    s_pb.muted        = s_pb.muted && pct == 0;
    s_vol_echo_us     = esp_timer_get_time();
    UNLOCK();
}

void tiercel_playbill_echo_frequency(uint32_t frequency_hz)
{
    LOCK();
    s_pb.frequency_hz = frequency_hz;
    s_pb.band         = TIERCEL_PB_BAND_FM;
    s_freq_echo_us    = esp_timer_get_time();
    UNLOCK();
}

void tiercel_playbill_get(tiercel_playbill_t *out)
{
    if (!out) {
        return;
    }
    LOCK();
    *out = s_pb;
    const int64_t at = s_pos_at_us;
    UNLOCK();

    if (out->has_media && !out->paused && at) {
        const int64_t ms = out->position_ms + (esp_timer_get_time() - at) / 1000;
        out->position_ms = (uint32_t)(out->duration_ms && ms > out->duration_ms
                                      ? out->duration_ms : ms);
    }
}
