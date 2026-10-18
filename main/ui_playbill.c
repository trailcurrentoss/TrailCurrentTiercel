/*
 * The Playbill screen. See ui_playbill.h.
 *
 * WHAT A REMOTE CAN AND CANNOT DO (TrailCurrentPlaybill controller/src)
 *
 *   - Commands are JSON with an `action`, validated strictly: send exactly
 *     the documented fields or Playbill drops the command (and says so only
 *     in its own log). Every payload below is copied from the schema or from
 *     Headwaters' PWA, which is the reference remote.
 *   - transport.next / previous SKIP +30 s / -10 s -- there is no queue. The
 *     icons are skip glyphs because that is what the PWA shows too.
 *   - There is no "switch to media" command: media needs an item, which only
 *     Playbill's own GUI can pick. "Media" stops the radio and resumes mpv.
 *   - The radio is tuned by frequency only. No seek, no RDS, no names;
 *     presets cannot be read. Previous/next jump between the stations of the
 *     last band scan when there is one, and step 1 MHz when there is not.
 *
 * RATE. Every volumeSet spawns `wpctl` and every tune restarts `rtl_fm`
 * (~100-500 ms) on the Playbill, so knob detents are coalesced: volume goes
 * out at most every 100 ms with the latest value, and a tune is sent once the
 * knob has been still for 350 ms. The rim moves at once regardless --
 * tiercel_playbill echoes the local value until Playbill's report catches up.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "esp_log.h"
#include "esp_timer.h"

#include "tiercel_board.h"
#include "tiercel_mqtt.h"
#include "tiercel_playbill.h"
#include "ui_nav.h"
#include "ui_playbill.h"

#include "screens.h"
#include "ui_lv.h"

static const char *TAG = "ui.playbill";

#define VOL_STEP          2
#define VOL_SEND_MS       100
#define TUNE_SETTLE_MS    350
#define FM_MIN_HZ         87900000u
#define FM_MAX_HZ         107900000u
#define FM_STEP_HZ        200000u      /* US FM grid, as Playbill's scan */
#define FM_DEFAULT_HZ     98700000u

#define G_PLAY    "\xEF\x81\x8B"   /* 0xF04B */
#define G_PAUSE   "\xEF\x81\x8C"   /* 0xF04C */
#define G_STOP    "\xEF\x81\x8D"   /* 0xF04D */
#define G_PREV    "\xEF\x81\x88"   /* 0xF048 backward-step */
#define G_NEXT    "\xEF\x81\x91"   /* 0xF051 forward-step  */
#define G_REWIND  "\xEF\x81\x8A"   /* 0xF04A backward      */
#define G_FORWARD "\xEF\x81\x8E"   /* 0xF04E forward       */
#define G_VOLUME  "\xEF\x80\xA8"   /* 0xF028 volume-high   */
#define G_RADIO   "\xEF\xA3\x97"   /* 0xF8D7 radio         */

typedef enum { VIEW_MEDIA = 0, VIEW_RADIO = 1 } view_t;

static view_t   s_view;
static bool     s_view_forced;    /* a tap chose the view; status not caught up */
static int64_t  s_view_forced_at;
static bool     s_tune_knob = true;   /* radio: knob tunes (design default) */
static int      s_vol_pending = -1;
static int64_t  s_vol_sent_at;
static uint32_t s_freq_pending;
static int64_t  s_freq_at;
static uint32_t s_last_freq = FM_DEFAULT_HZ;
static lv_point_precise_t s_marker[2];

/* ---------------------------------------------------------------------- */

static void send(const char *feature, const char *json)
{
    if (tiercel_mqtt_playbill_command(feature, json) < 0) {
        ESP_LOGW(TAG, "not sent (%s): %s", feature, json);
    }
}

static void send_tune(uint32_t hz)
{
    char buf[96];
    snprintf(buf, sizeof(buf),
             "{\"action\":\"radio.tune\",\"value\":{\"band\":\"fm\","
             "\"frequencyHz\":%lu}}", (unsigned long)hz);
    send("radio", buf);
    s_last_freq = hz;
}

static void send_volume(int pct)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "{\"action\":\"transport.volumeSet\",\"percent\":%d}",
             pct);
    send("volume", buf);
    /* As Peregrine does: raising the volume on a muted Playbill unmutes it,
     * or the knob would appear to do nothing. */
    tiercel_playbill_t pb;
    tiercel_playbill_get(&pb);
    if (pct > 0 && pb.muted) {
        send("volume", "{\"action\":\"transport.muteOff\"}");
    }
    s_vol_sent_at = esp_timer_get_time();
}

static void fmt_time(char *out, size_t len, uint32_t ms)
{
    const unsigned s = ms / 1000, h = s / 3600, m = (s / 60) % 60;
    if (h) {
        snprintf(out, len, "%u:%02u:%02u", h, m, s % 60);
    } else {
        snprintf(out, len, "%u:%02u", m, s % 60);
    }
}

static void upper(char *s)
{
    for (; *s; s++) {
        if (*s >= 'a' && *s <= 'z') {
            *s -= 'a' - 'A';
        }
    }
}

/* The marker on the tuning rim, from r 138 to 170 at the frequency's angle
 * (design P:150) -- a Line whose points are content, like a clock hand. */
static void set_marker(uint32_t hz)
{
    if (!objects.pb_tune_marker) {
        return;
    }
    float frac = ((float)hz - 87500000.0f) / (108000000.0f - 87500000.0f);
    frac = frac < 0 ? 0 : frac > 1 ? 1 : frac;
    const float a = (135.0f + frac * 270.0f) * (float)M_PI / 180.0f;
    const lv_point_precise_t p0 = { (lv_value_precise_t)lroundf(180 + 138 * cosf(a)),
                                    (lv_value_precise_t)lroundf(180 + 138 * sinf(a)) };
    const lv_point_precise_t p1 = { (lv_value_precise_t)lroundf(180 + 170 * cosf(a)),
                                    (lv_value_precise_t)lroundf(180 + 170 * sinf(a)) };
    if (p0.x == s_marker[0].x && p0.y == s_marker[0].y) {
        return;
    }
    s_marker[0] = p0;
    s_marker[1] = p1;
    lv_line_set_points(objects.pb_tune_marker, s_marker, 2);
}

/* ---------------------------------------------------------------------- */

void ui_playbill_refresh(void)
{
    tiercel_playbill_t pb;
    tiercel_playbill_get(&pb);

    /* The view follows Playbill's reported source, except for a moment after
     * a tap chose it -- so the screen answers the tap at once instead of
     * waiting for the round trip, and status still wins after that. */
    const bool radio_src = strcmp(pb.source, "radio") == 0 || pb.radio_running;
    if (s_view_forced &&
        (esp_timer_get_time() - s_view_forced_at > 4000000 ||
         (s_view == VIEW_RADIO) == radio_src)) {
        s_view_forced = false;
    }
    if (!s_view_forced) {
        s_view = radio_src ? VIEW_RADIO : VIEW_MEDIA;
    }
    if (pb.frequency_hz) {
        s_last_freq = pb.frequency_hz;
    }
    const bool radio = s_view == VIEW_RADIO;
    const bool online = pb.known && pb.online;
    const uint32_t freq = s_freq_pending ? s_freq_pending : s_last_freq;

    ui_lv_set_state_in(objects.pb_src_media, LV_STATE_CHECKED, radio ? 0 : LV_STATE_CHECKED);
    ui_lv_set_state_in(objects.pb_src_media_label, LV_STATE_CHECKED, radio ? 0 : LV_STATE_CHECKED);
    ui_lv_set_state_in(objects.pb_src_radio, LV_STATE_CHECKED, radio ? LV_STATE_CHECKED : 0);
    ui_lv_set_state_in(objects.pb_src_radio_label, LV_STATE_CHECKED, radio ? LV_STATE_CHECKED : 0);
    ui_lv_set_hidden(objects.pb_media, radio);
    ui_lv_set_hidden(objects.pb_radio, !radio);

    char room[TIERCEL_PB_NAME_MAX];
    snprintf(room, sizeof(room), "%s", pb.name[0] ? pb.name : "Playbill");
    upper(room);
    char buf[96];

    bool playing;
    if (radio) {
        snprintf(buf, sizeof(buf), "%s \xC2\xB7 %s",
                 pb.radio_running ? "FM RADIO" : "RADIO OFF", room);
        ui_lv_set_text(objects.pb_station, online ? buf : "PLAYBILL OFFLINE");
        ui_lv_set_state_in(objects.pb_station, LV_STATE_CHECKED,
                           pb.radio_running ? LV_STATE_CHECKED : 0);
        snprintf(buf, sizeof(buf), "%.1f", freq / 1000000.0);
        ui_lv_set_text(objects.pb_freq, buf);
        playing = pb.radio_running;
    } else {
        if (!online) {
            ui_lv_set_text(objects.pb_kind, "PLAYBILL");
            ui_lv_set_text(objects.pb_title, "Offline");
            ui_lv_set_text(objects.pb_artist, "Waiting for Playbill");
        } else if (!pb.has_media) {
            snprintf(buf, sizeof(buf), "PLAYBILL \xC2\xB7 %s", room);
            ui_lv_set_text(objects.pb_kind, buf);
            ui_lv_set_text(objects.pb_title, "Nothing playing");
            ui_lv_set_text(objects.pb_artist, "Choose something on Playbill");
        } else {
            char src[TIERCEL_PB_SOURCE_MAX];
            snprintf(src, sizeof(src), "%s", pb.source[0] ? pb.source : "media");
            upper(src);
            snprintf(buf, sizeof(buf), "%s \xC2\xB7 %s", src, room);
            ui_lv_set_text(objects.pb_kind, buf);
            ui_lv_set_text(objects.pb_title, pb.title[0] ? pb.title : "Untitled");
            ui_lv_set_text(objects.pb_artist, pb.subtitle);
        }
        /* Progress only for something with a length; live streams have none. */
        const bool timed = online && pb.has_media && pb.duration_ms > 0;
        ui_lv_set_hidden(objects.pb_progress, !timed);
        ui_lv_set_hidden(objects.pb_pos, !timed);
        ui_lv_set_hidden(objects.pb_len, !timed);
        if (timed) {
            lv_bar_set_value(objects.pb_progress,
                             (int32_t)((uint64_t)pb.position_ms * 1000 / pb.duration_ms),
                             LV_ANIM_OFF);
            fmt_time(buf, sizeof(buf), pb.position_ms);
            ui_lv_set_text(objects.pb_pos, buf);
            fmt_time(buf, sizeof(buf), pb.duration_ms);
            ui_lv_set_text(objects.pb_len, buf);
        }
        playing = pb.has_media && !pb.paused;
    }

    ui_lv_set_text(objects.pb_play_icon, radio ? (playing ? G_STOP : G_PLAY)
                                               : (playing ? G_PAUSE : G_PLAY));
    ui_lv_set_state_in(objects.pb_play, LV_STATE_CHECKED, playing ? LV_STATE_CHECKED : 0);
    ui_lv_set_text(objects.pb_prev_icon, radio ? G_REWIND : G_PREV);
    ui_lv_set_text(objects.pb_next_icon, radio ? G_FORWARD : G_NEXT);

    /* The knob chip and the rims. */
    const bool tune = radio && s_tune_knob;
    const int vol = s_vol_pending >= 0 ? s_vol_pending : pb.volume_pct;
    ui_lv_set_state_in(objects.pb_knob, LV_STATE_CHECKED, radio ? LV_STATE_CHECKED : 0);
    ui_lv_set_text(objects.pb_knob_icon, tune ? G_RADIO : G_VOLUME);
    if (tune) {
        snprintf(buf, sizeof(buf), "Knob \xC2\xB7 Tune");
    } else if (!pb.volume_known) {
        snprintf(buf, sizeof(buf), radio ? "Knob \xC2\xB7 Volume" : "Volume");
    } else {
        snprintf(buf, sizeof(buf), radio ? "Knob \xC2\xB7 Volume %d" : "Volume %d", vol);
    }
    ui_lv_set_text(objects.pb_knob_label, buf);
    ui_lv_set_hidden(objects.pb_vol, tune);
    ui_lv_set_hidden(objects.pb_tune, !tune);
    ui_lv_set_hidden(objects.pb_tune_marker, !tune);
    if (!tune && objects.pb_vol && lv_arc_get_value(objects.pb_vol) != vol) {
        lv_arc_set_value(objects.pb_vol, vol);
    }
    if (tune) {
        set_marker(freq);
    }
}

void ui_playbill_summary(char *out, size_t len)
{
    tiercel_playbill_t pb;
    tiercel_playbill_get(&pb);
    if (!pb.known || !pb.online) {
        snprintf(out, len, "Offline");
    } else if (pb.radio_running) {
        snprintf(out, len, "Radio \xC2\xB7 %.1f FM", pb.frequency_hz / 1000000.0);
    } else if (pb.has_media) {
        snprintf(out, len, "%s \xC2\xB7 %s", pb.paused ? "Paused" : "Playing",
                 pb.title[0] ? pb.title : "Untitled");
    } else {
        snprintf(out, len, "Nothing playing");
    }
}

/* Flushes coalesced volume and tuning. 50 ms, always running: a turn just
 * before leaving the screen still has to reach the Playbill. */
static void flush_timer_cb(lv_timer_t *t)
{
    (void)t;
    const int64_t now = esp_timer_get_time();
    if (s_vol_pending >= 0 && (now - s_vol_sent_at) / 1000 >= VOL_SEND_MS) {
        const int v = s_vol_pending;
        s_vol_pending = -1;
        send_volume(v);
    }
    if (s_freq_pending && (now - s_freq_at) / 1000 >= TUNE_SETTLE_MS) {
        send_tune(s_freq_pending);
        s_freq_pending = 0;
    }
    if (ui_nav_current() == TIERCEL_SCREEN_PLAYBILL) {
        ui_playbill_refresh();
    }
}

void ui_playbill_init(void)
{
    lv_timer_create(flush_timer_cb, 50, NULL);
}

void ui_playbill_enter(void)
{
    s_view_forced = false;
    ui_playbill_refresh();
}

void ui_playbill_leave(void) { }

void ui_playbill_rotate(int diff)
{
    tiercel_playbill_t pb;
    tiercel_playbill_get(&pb);

    if (s_view == VIEW_RADIO && s_tune_knob) {
        const uint32_t cur = s_freq_pending ? s_freq_pending : s_last_freq;
        int64_t f = (int64_t)cur + (int64_t)diff * FM_STEP_HZ;
        if (f < FM_MIN_HZ || f > FM_MAX_HZ) {
            f = f < FM_MIN_HZ ? FM_MIN_HZ : FM_MAX_HZ;
            if ((uint32_t)f == cur) {
                tiercel_board_haptic(TIERCEL_HAPTIC_LIMIT);
                return;
            }
        }
        s_freq_pending = (uint32_t)f;
        s_freq_at = esp_timer_get_time();
        tiercel_playbill_echo_frequency(s_freq_pending);
        ui_playbill_refresh();
        return;
    }

    const int cur = s_vol_pending >= 0 ? s_vol_pending : pb.volume_pct;
    int v = cur + diff * VOL_STEP;
    if (v < 0 || v > 100) {
        v = v < 0 ? 0 : 100;
        if (v == cur) {
            tiercel_board_haptic(TIERCEL_HAPTIC_LIMIT);
            return;
        }
    }
    s_vol_pending = v;
    tiercel_playbill_echo_volume(v);
    ui_playbill_refresh();
}

void ui_playbill_source(int src)
{
    tiercel_playbill_t pb;
    tiercel_playbill_get(&pb);
    const view_t want = src ? VIEW_RADIO : VIEW_MEDIA;
    if (want == s_view) {
        return;
    }
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    s_view = want;
    s_view_forced = true;
    s_view_forced_at = esp_timer_get_time();
    if (want == VIEW_RADIO) {
        s_tune_knob = true;
        send_tune(s_last_freq);
    } else {
        send("radio", "{\"action\":\"radio.stop\"}");
        send("transport", "{\"action\":\"transport.play\"}");
    }
    ui_playbill_refresh();
}

/* The next scanned station past `from` in direction `dir`, or 0. */
static uint32_t adjacent_station(const tiercel_playbill_t *pb, uint32_t from, int dir)
{
    if (dir > 0) {
        for (int i = 0; i < pb->station_count; i++) {
            if (pb->stations_hz[i] > from + 50000u) {
                return pb->stations_hz[i];
            }
        }
        return pb->station_count ? pb->stations_hz[0] : 0;
    }
    for (int i = pb->station_count - 1; i >= 0; i--) {
        if (pb->stations_hz[i] + 50000u < from) {
            return pb->stations_hz[i];
        }
    }
    return pb->station_count ? pb->stations_hz[pb->station_count - 1] : 0;
}

static void radio_jump(int dir)
{
    tiercel_playbill_t pb;
    tiercel_playbill_get(&pb);
    const uint32_t cur = s_freq_pending ? s_freq_pending : s_last_freq;
    uint32_t to = adjacent_station(&pb, cur, dir);
    if (!to) {
        int64_t f = (int64_t)cur + dir * 1000000;
        to = (uint32_t)(f < FM_MIN_HZ ? FM_MIN_HZ : f > FM_MAX_HZ ? FM_MAX_HZ : f);
    }
    s_freq_pending = 0;
    tiercel_playbill_echo_frequency(to);
    send_tune(to);
}

void ui_playbill_prev(void)
{
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    if (s_view == VIEW_RADIO) {
        radio_jump(-1);
    } else {
        send("transport", "{\"action\":\"transport.previous\"}");
    }
    ui_playbill_refresh();
}

void ui_playbill_next(void)
{
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    if (s_view == VIEW_RADIO) {
        radio_jump(+1);
    } else {
        send("transport", "{\"action\":\"transport.next\"}");
    }
    ui_playbill_refresh();
}

void ui_playbill_play(void)
{
    tiercel_playbill_t pb;
    tiercel_playbill_get(&pb);
    tiercel_board_haptic(TIERCEL_HAPTIC_CONFIRM);
    if (s_view == VIEW_RADIO) {
        if (pb.radio_running) {
            send("radio", "{\"action\":\"radio.stop\"}");
        } else {
            send_tune(s_last_freq);
        }
    } else {
        send("transport", "{\"action\":\"transport.toggle\"}");
    }
}

void ui_playbill_knob_toggle(void)
{
    if (s_view != VIEW_RADIO) {
        return;             /* on media the chip only reports */
    }
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    s_tune_knob = !s_tune_knob;
    ui_playbill_refresh();
}
