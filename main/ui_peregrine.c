/*
 * The Peregrine screen. See ui_peregrine.h.
 *
 * Every look the design gives the orb -- rest, listening, thinking, speaking
 * -- is an authored state on pg_orb (the PgOrb style in the generator), so
 * the screen a designer opens in EEZ Studio is the one the panel shows. C
 * moves states, text, the ring's two section ranges and the spinner's
 * rotation, all of which are content.
 */

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_timer.h"

#include "tiercel_audio.h"
#include "tiercel_board.h"
#include "tiercel_config.h"
#include "tiercel_peregrine.h"
#include "ui_nav.h"
#include "ui_peregrine.h"

#include "screens.h"
#include "ui_lv.h"

static const char *TAG = "ui.peregrine";

#define VOL_STEP        5
#define VOL_SHOW_MS     1500      /* the rim stays up this long after a turn */
#define ANIM_MS         40        /* ring and spinner frame rate */
#define RING_HALF       30        /* ticks per side of 12 o'clock */

#define G_MIC    "\xEF\x84\xB0"   /* 0xF130 microphone  */
#define G_VOLUME "\xEF\x80\xA8"   /* 0xF028 volume-high */

/* Orb looks, as PgOrb states. Borrowed states are safe: the orb is inert;
 * the tap lands on pg_tap above it. */
#define ORB_LISTEN LV_STATE_CHECKED
#define ORB_SPEAK  LV_STATE_PRESSED
#define ORB_THINK  LV_STATE_DISABLED
#define ORB_MASK   (ORB_LISTEN | ORB_SPEAK | ORB_THINK)

static int64_t s_vol_at;
static bool    s_vol_dirty;
static int32_t s_ring;           /* lit ticks per side, 0..RING_HALF */

bool ui_peregrine_busy(void)
{
    tiercel_pg_status_t st;
    tiercel_peregrine_status(&st);
    return st.phase == TIERCEL_PG_LISTENING || st.phase == TIERCEL_PG_THINKING ||
           st.phase == TIERCEL_PG_SPEAKING;
}

int32_t ui_peregrine_ring_a_max(void) { return s_ring > 0 ? s_ring - 1 : -1; }
int32_t ui_peregrine_ring_b_min(void) { return s_ring > 0 ? 60 - s_ring : 60; }

int32_t ui_peregrine_spin(void)
{
    /* The design's 0.22 deg/ms -- one lap in about 1.6 s. */
    return (int32_t)((esp_timer_get_time() / 1000 * 22 / 100) % 360);
}

void ui_peregrine_summary(char *out, size_t len)
{
    tiercel_pg_status_t st;
    tiercel_peregrine_status(&st);
    const char *s = !st.configured                    ? "Not set up"
                  : st.phase == TIERCEL_PG_LISTENING  ? "Listening"
                  : st.phase == TIERCEL_PG_THINKING   ? "Thinking"
                  : st.phase == TIERCEL_PG_SPEAKING   ? "Speaking"
                                                      : "Tap to talk";
    snprintf(out, len, "%s", s);
}

static void paint(void)
{
    tiercel_pg_status_t st;
    tiercel_peregrine_status(&st);

    const char *eyebrow = "PEREGRINE", *q = "", *a = "", *hint = "";
    lv_state_t orb = 0, q_st = 0;
    bool live = false, spin = false;
    const char *glyph = G_MIC;

    switch (st.phase) {
    case TIERCEL_PG_IDLE:
        q = st.configured ? "Tap to talk"
                          : "Peregrine is not set up -- add it in Headwaters";
        hint = "Turn for voice volume";
        break;
    case TIERCEL_PG_LISTENING:
        eyebrow = "LISTENING"; live = true; orb = ORB_LISTEN; q_st = LV_STATE_CHECKED;
        q = st.heard_speech ? "Listening..." : "Go ahead";
        hint = st.heard_speech ? "Tap to send" : "Tap to stop";
        break;
    case TIERCEL_PG_THINKING:
        eyebrow = "THINKING"; orb = ORB_THINK; spin = true; glyph = "";
        hint = "Tap to cancel";
        break;
    case TIERCEL_PG_SPEAKING:
        eyebrow = "SPEAKING"; live = true; orb = ORB_SPEAK; glyph = G_VOLUME;
        q = st.transcript; a = st.answer; q_st = LV_STATE_DISABLED;
        hint = "Tap to stop";
        break;
    case TIERCEL_PG_DONE:
        q = st.transcript; a = st.answer; q_st = LV_STATE_DISABLED;
        hint = "Tap to ask again";
        break;
    case TIERCEL_PG_ERROR:
        q = st.error;
        hint = "Tap to try again";
        break;
    }

    ui_lv_set_text(objects.pg_eyebrow, eyebrow);
    ui_lv_set_state_in(objects.pg_eyebrow, LV_STATE_CHECKED,
                       live || spin ? LV_STATE_CHECKED : 0);
    ui_lv_set_state_in(objects.pg_orb, ORB_MASK, orb);
    ui_lv_set_state_in(objects.pg_orb_icon, ORB_SPEAK, orb == ORB_SPEAK ? ORB_SPEAK : 0);
    ui_lv_set_text(objects.pg_orb_icon, glyph);
    ui_lv_set_hidden(objects.pg_spin_arc, !spin);
    ui_lv_set_text(objects.pg_q, q);
    ui_lv_set_state_in(objects.pg_q, LV_STATE_CHECKED | LV_STATE_DISABLED, q_st);
    ui_lv_set_text(objects.pg_a, a);

    /* The volume rim takes the hint line's place while the knob is moving. */
    const bool vol = (esp_timer_get_time() - s_vol_at) / 1000 < VOL_SHOW_MS;
    ui_lv_set_hidden(objects.pg_vol, !vol);
    if (vol) {
        char buf[24];
        snprintf(buf, sizeof(buf), "Voice volume %u", tiercel_audio_get_volume());
        ui_lv_set_text(objects.pg_hint, buf);
    } else {
        ui_lv_set_text(objects.pg_hint, hint);
    }

    /* Ring: lit ticks grow out from 12 o'clock with the level. */
    s_ring = live ? (int32_t)tiercel_peregrine_level() * RING_HALF / 100 : 0;
}

static void anim_timer_cb(lv_timer_t *t)
{
    (void)t;
    if (ui_nav_current() == TIERCEL_SCREEN_PEREGRINE) {
        paint();
    } else {
        s_ring = 0;
    }
}

void ui_peregrine_init(void)
{
    lv_timer_create(anim_timer_cb, ANIM_MS, NULL);
}

void ui_peregrine_enter(void)
{
    tiercel_peregrine_reset();
    s_vol_at = 0;
    if (objects.pg_vol) {
        lv_arc_set_value(objects.pg_vol, tiercel_audio_get_volume());
    }
    paint();
}

void ui_peregrine_leave(void)
{
    tiercel_peregrine_cancel();
    s_ring = 0;
    if (s_vol_dirty) {
        tiercel_display_cfg_t d;
        tiercel_config_get_display(&d);
        d.voice_volume = tiercel_audio_get_volume();
        if (tiercel_config_set_display(&d) == ESP_OK) {
            s_vol_dirty = false;
        }
    }
}

void ui_peregrine_rotate(int diff)
{
    int v = (int)tiercel_audio_get_volume() + diff * VOL_STEP;
    if (v < 0 || v > 100) {
        v = v < 0 ? 0 : 100;
        if (v == tiercel_audio_get_volume()) {
            tiercel_board_haptic(TIERCEL_HAPTIC_LIMIT);
            s_vol_at = esp_timer_get_time();   /* keep the rim up */
            return;
        }
    }
    tiercel_audio_set_volume((uint8_t)v);
    s_vol_dirty = true;
    s_vol_at = esp_timer_get_time();
    if (objects.pg_vol) {
        lv_arc_set_value(objects.pg_vol, v);
    }
    paint();
}

void ui_peregrine_tap(void)
{
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    tiercel_peregrine_tap();
    paint();
    ESP_LOGD(TAG, "orb tapped");
}
