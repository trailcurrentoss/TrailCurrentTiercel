/*
 * Screen navigation, input routing and the back gesture. See ui_nav.h.
 *
 * Forked from Capstan's ui_nav.c, whose selection model it keeps: one index
 * per screen, the overshoot never stored. What changed is the input. Capstan
 * is ring-only with touch off; Tiercel's knob only turns and everything else
 * is a tap or a gesture on the glass.
 */

#include "esp_log.h"
#include "esp_timer.h"

#include "tiercel_board.h"
#include "tiercel_config.h"
#include "ui_alerts.h"
#include "ui_bluetooth.h"
#include "ui_climate.h"
#include "ui_clock.h"
#include "ui_devices.h"
#include "ui_icons.h"
#include "ui_nav.h"
#include "ui_peregrine.h"
#include "ui_playbill.h"
#include "ui_settings.h"

#include "screens.h"
#include "ui.h"
#include "ui_lv.h"

static const char *TAG = "nav";

/* ---------------------------------------------------------------------- *
 * The policy table: where back goes, and whether the screen is an "app"
 * screen with the back overlay (chip, swipe arc, hold ring) authored on it.
 * Idle and Menu are home; Alert is dismissed by a tap; Setup has nowhere to
 * go back to by design -- on a new unit there is no "before".
 * ---------------------------------------------------------------------- */
typedef struct {
    tiercel_screen_t back;
    bool             app;
    const char      *name;
} screen_policy_t;

static const screen_policy_t s_policy[TIERCEL_SCREEN_COUNT] = {
    [TIERCEL_SCREEN_IDLE]         = { TIERCEL_SCREEN_IDLE,     false, "idle" },
    [TIERCEL_SCREEN_MENU]         = { TIERCEL_SCREEN_IDLE,     false, "menu" },
    [TIERCEL_SCREEN_CLIMATE]      = { TIERCEL_SCREEN_MENU,     true,  "climate" },
    [TIERCEL_SCREEN_CLIMATE_MODE] = { TIERCEL_SCREEN_CLIMATE,  true,  "climate.mode" },
    [TIERCEL_SCREEN_DEVICES]      = { TIERCEL_SCREEN_MENU,     true,  "devices" },
    [TIERCEL_SCREEN_PEREGRINE]    = { TIERCEL_SCREEN_MENU,     true,  "peregrine" },
    [TIERCEL_SCREEN_PLAYBILL]     = { TIERCEL_SCREEN_MENU,     true,  "playbill" },
    [TIERCEL_SCREEN_ENERGY]       = { TIERCEL_SCREEN_MENU,     true,  "energy" },
    [TIERCEL_SCREEN_WATER]        = { TIERCEL_SCREEN_MENU,     true,  "water" },
    [TIERCEL_SCREEN_AIR]          = { TIERCEL_SCREEN_MENU,     true,  "air" },
    [TIERCEL_SCREEN_LEVEL]        = { TIERCEL_SCREEN_MENU,     true,  "level" },
    [TIERCEL_SCREEN_SETTINGS]     = { TIERCEL_SCREEN_MENU,     true,  "settings" },
    [TIERCEL_SCREEN_ALERT]        = { TIERCEL_SCREEN_MENU,     false, "alert" },
    [TIERCEL_SCREEN_SETUP]        = { TIERCEL_SCREEN_SETUP,    false, "setup" },
    [TIERCEL_SCREEN_LOCALE]       = { TIERCEL_SCREEN_SETTINGS, true,  "locale" },
    [TIERCEL_SCREEN_CLOCKFACE]    = { TIERCEL_SCREEN_SETTINGS, true,  "clockface" },
    [TIERCEL_SCREEN_BRIGHTNESS]   = { TIERCEL_SCREEN_SETTINGS, true,  "brightness" },
    [TIERCEL_SCREEN_BLUETOOTH]    = { TIERCEL_SCREEN_SETTINGS, true,  "bluetooth" },
};

/*
 * Our screen enum onto EEZ's. Spelled out rather than `s + 1`: the EEZ order
 * is whatever PAGES in the generator emits, and an arithmetic mapping would
 * silently load the wrong screen the day someone inserts a page.
 */
static const int s_eez_id[TIERCEL_SCREEN_COUNT] = {
    [TIERCEL_SCREEN_IDLE]         = SCREEN_ID_PAGE_IDLE,
    [TIERCEL_SCREEN_MENU]         = SCREEN_ID_PAGE_MENU,
    [TIERCEL_SCREEN_CLIMATE]      = SCREEN_ID_PAGE_CLIMATE,
    [TIERCEL_SCREEN_CLIMATE_MODE] = SCREEN_ID_PAGE_CLIMATE_MODE,
    [TIERCEL_SCREEN_DEVICES]      = SCREEN_ID_PAGE_DEVICES,
    [TIERCEL_SCREEN_PEREGRINE]    = SCREEN_ID_PAGE_PEREGRINE,
    [TIERCEL_SCREEN_PLAYBILL]     = SCREEN_ID_PAGE_PLAYBILL,
    [TIERCEL_SCREEN_ENERGY]       = SCREEN_ID_PAGE_ENERGY,
    [TIERCEL_SCREEN_WATER]        = SCREEN_ID_PAGE_WATER,
    [TIERCEL_SCREEN_AIR]          = SCREEN_ID_PAGE_AIR,
    [TIERCEL_SCREEN_LEVEL]        = SCREEN_ID_PAGE_LEVEL,
    [TIERCEL_SCREEN_SETTINGS]     = SCREEN_ID_PAGE_SETTINGS,
    [TIERCEL_SCREEN_ALERT]        = SCREEN_ID_PAGE_ALERT,
    [TIERCEL_SCREEN_SETUP]        = SCREEN_ID_PAGE_SETUP,
    [TIERCEL_SCREEN_LOCALE]       = SCREEN_ID_PAGE_LOCALE,
    [TIERCEL_SCREEN_CLOCKFACE]    = SCREEN_ID_PAGE_CLOCK_FACE,
    [TIERCEL_SCREEN_BRIGHTNESS]   = SCREEN_ID_PAGE_BRIGHTNESS,
    [TIERCEL_SCREEN_BLUETOOTH]    = SCREEN_ID_PAGE_BLUETOOTH,
};

_Static_assert((int)TIERCEL_SCREEN_COUNT == (int)_SCREEN_ID_LAST,
               "screen enums are out of step -- a page was added to ui_nav.h "
               "or to PAGES in gen_eez_project.py but not the other");

/* The back overlay on each app page: chip, swipe arc, hold ring. */
typedef struct {
    lv_obj_t **chip, **swipe, **hold;
} overlay_t;

static overlay_t overlay_of(tiercel_screen_t s)
{
#define OV(p) (overlay_t){ &objects.p##_back_chip, &objects.p##_swipe_arc, \
                           &objects.p##_hold_arc }
    switch (s) {
    case TIERCEL_SCREEN_CLIMATE:      return OV(climate);
    case TIERCEL_SCREEN_CLIMATE_MODE: return OV(cmode);
    case TIERCEL_SCREEN_DEVICES:      return OV(devices);
    case TIERCEL_SCREEN_PEREGRINE:    return OV(pg);
    case TIERCEL_SCREEN_PLAYBILL:     return OV(pb);
    case TIERCEL_SCREEN_ENERGY:       return OV(energy);
    case TIERCEL_SCREEN_WATER:        return OV(water);
    case TIERCEL_SCREEN_AIR:          return OV(air);
    case TIERCEL_SCREEN_LEVEL:        return OV(level);
    case TIERCEL_SCREEN_SETTINGS:     return OV(settings);
    case TIERCEL_SCREEN_LOCALE:       return OV(locale);
    case TIERCEL_SCREEN_CLOCKFACE:    return OV(cfp);
    case TIERCEL_SCREEN_BRIGHTNESS:   return OV(bright);
    case TIERCEL_SCREEN_BLUETOOTH:    return OV(bt);
    default:                          return (overlay_t){ 0 };
    }
#undef OV
}

static tiercel_screen_t s_current = TIERCEL_SCREEN_IDLE;
static int s_sel[TIERCEL_SCREEN_COUNT];

tiercel_screen_t ui_nav_current(void) { return s_current; }

static tiercel_back_gesture_t back_gesture(void)
{
    tiercel_display_cfg_t d;
    tiercel_config_get_display(&d);
    return d.back_gesture;
}

/* ---------------------------------------------------------------------- *
 * The app carousel. Ten items -- MENU_ITEMS in GUI/tmp/screens_layout.py,
 * index for index; the summary line is ui_data.c's refresh_menu().
 * ---------------------------------------------------------------------- */
#define MENU_ITEM_COUNT 10
#define MENU_DOT_SLOTS  (2 * MENU_ITEM_COUNT - 1)

static const struct {
    const char      *icon;
    const char      *title;
    tiercel_screen_t dest;
} s_menu[MENU_ITEM_COUNT] = {
    { UI_ICON_CLIMATE,   "Climate",     TIERCEL_SCREEN_CLIMATE   },
    { UI_ICON_DEVICES,   "Devices",     TIERCEL_SCREEN_DEVICES   },
    { UI_ICON_PEREGRINE, "Peregrine",   TIERCEL_SCREEN_PEREGRINE },
    { UI_ICON_PLAYBILL,  "Playbill",    TIERCEL_SCREEN_PLAYBILL  },
    { UI_ICON_ENERGY,    "Energy",      TIERCEL_SCREEN_ENERGY    },
    { UI_ICON_WATER,     "Water Tanks", TIERCEL_SCREEN_WATER     },
    { UI_ICON_AIR,       "Air Quality", TIERCEL_SCREEN_AIR       },
    { UI_ICON_LEVEL,     "Leveling",    TIERCEL_SCREEN_LEVEL     },
    { UI_ICON_SETTINGS,  "Settings",    TIERCEL_SCREEN_SETTINGS  },
    { UI_ICON_CLOCK,     "Clock",       TIERCEL_SCREEN_IDLE      },
};

static int wrap(int i, int n) { return ((i % n) + n) % n; }

static void apply_menu_carousel(void)
{
    const int n = MENU_ITEM_COUNT;
    const int sel = wrap(s_sel[TIERCEL_SCREEN_MENU], n);
    ui_lv_set_text(objects.menu_hero_icon, s_menu[sel].icon);
    ui_lv_set_text(objects.menu_prev_icon, s_menu[wrap(sel - 1, n)].icon);
    ui_lv_set_text(objects.menu_next_icon, s_menu[wrap(sel + 1, n)].icon);
    ui_lv_set_text(objects.menu_title, s_menu[sel].title);

    lv_obj_t *const dots[MENU_DOT_SLOTS] = {
        objects.menu_dot0,  objects.menu_dot1,  objects.menu_dot2,
        objects.menu_dot3,  objects.menu_dot4,  objects.menu_dot5,
        objects.menu_dot6,  objects.menu_dot7,  objects.menu_dot8,
        objects.menu_dot9,  objects.menu_dot10, objects.menu_dot11,
        objects.menu_dot12, objects.menu_dot13, objects.menu_dot14,
        objects.menu_dot15, objects.menu_dot16, objects.menu_dot17,
        objects.menu_dot18,
    };
    /* Every other slot is a dot; the interstitials stay hidden (they exist
     * so a shorter list can stay centred -- see page_menu()). */
    for (int slot = 0; slot < MENU_DOT_SLOTS; slot++) {
        const bool used = slot % 2 == 0;
        ui_lv_set_hidden(dots[slot], !used);
        ui_lv_set_state_in(dots[slot], LV_STATE_CHECKED,
                           used && slot / 2 == sel ? LV_STATE_CHECKED : 0);
    }
}

int ui_nav_menu_item(void)
{
    return wrap(s_sel[TIERCEL_SCREEN_MENU], MENU_ITEM_COUNT);
}

/* ---------------------------------------------------------------------- *
 * Energy's page dots -- ENERGY_PAGES in screens_layout.py.
 * ---------------------------------------------------------------------- */
#define ENERGY_PAGE_COUNT 3

static void apply_energy_dots(void)
{
    lv_obj_t *const dots[ENERGY_PAGE_COUNT] = {
        objects.energy_dot0, objects.energy_dot1, objects.energy_dot2,
    };
    for (int i = 0; i < ENERGY_PAGE_COUNT; i++) {
        ui_lv_set_state_in(dots[i], LV_STATE_CHECKED,
                           i == s_sel[TIERCEL_SCREEN_ENERGY] ? LV_STATE_CHECKED : 0);
    }
}

/* ---------------------------------------------------------------------- *
 * Selection.
 * ---------------------------------------------------------------------- */
static lv_obj_t *list_container(tiercel_screen_t s)
{
    switch (s) {
    case TIERCEL_SCREEN_CLIMATE_MODE: return objects.cmode_list;
    case TIERCEL_SCREEN_LOCALE:       return objects.locale_list;
    default:                          return NULL;
    }
}

/* True for the three carousels, which wrap and so never hit an end. */
static bool wraps(tiercel_screen_t s)
{
    return s == TIERCEL_SCREEN_MENU || s == TIERCEL_SCREEN_DEVICES ||
           s == TIERCEL_SCREEN_SETTINGS;
}

static int selectable_count(tiercel_screen_t s)
{
    switch (s) {
    case TIERCEL_SCREEN_MENU:     return MENU_ITEM_COUNT;
    case TIERCEL_SCREEN_ENERGY:   return ENERGY_PAGE_COUNT;
    case TIERCEL_SCREEN_SETTINGS: return UI_SETTINGS_ITEM_COUNT;
    case TIERCEL_SCREEN_DEVICES:  return ui_devices_count();
    default:
        break;
    }
    lv_obj_t *c = list_container(s);
    return c ? (int)lv_obj_get_child_count(c) : 0;
}

static void refresh_selection(tiercel_screen_t s)
{
    switch (s) {
    case TIERCEL_SCREEN_MENU:     apply_menu_carousel(); return;
    case TIERCEL_SCREEN_SETTINGS: ui_settings_refresh(); return;
    case TIERCEL_SCREEN_ENERGY:   apply_energy_dots();   return;
    case TIERCEL_SCREEN_DEVICES:  ui_devices_refresh();  return;
    default:
        break;
    }
    lv_obj_t *c = list_container(s);
    if (!c) {
        return;
    }
    const uint32_t n = lv_obj_get_child_count(c);
    for (uint32_t i = 0; i < n; i++) {
        ui_lv_set_state_in(lv_obj_get_child(c, i), LV_STATE_CHECKED,
                           (int)i == s_sel[s] ? LV_STATE_CHECKED : 0);
    }
}

static void clamp_selection(tiercel_screen_t s)
{
    const int n = selectable_count(s);
    if (s_sel[s] > n - 1) { s_sel[s] = n > 0 ? n - 1 : 0; }
    if (s_sel[s] < 0)     { s_sel[s] = 0; }
}

void ui_nav_selection_changed(void)
{
    clamp_selection(s_current);
    refresh_selection(s_current);
}

int ui_nav_selection(void)
{
    return selectable_count(s_current) > 0 ? s_sel[s_current] : -1;
}

int ui_nav_selection_of(tiercel_screen_t s)
{
    return (s >= 0 && s < TIERCEL_SCREEN_COUNT) ? s_sel[s] : 0;
}

/* ---------------------------------------------------------------------- *
 * Transitions.
 * ---------------------------------------------------------------------- */
static void show_overlay(tiercel_screen_t s)
{
    const overlay_t o = overlay_of(s);
    if (!o.chip) {
        return;
    }
    ui_lv_set_hidden(*o.chip, back_gesture() != TIERCEL_BACK_CHIP);
    ui_lv_set_hidden(*o.swipe, true);
    ui_lv_set_hidden(*o.hold, true);
}

static void on_leave(tiercel_screen_t from, tiercel_screen_t to)
{
    if (from == to) {
        return;
    }
    switch (from) {
    case TIERCEL_SCREEN_SETTINGS:   ui_settings_disarm_reset();     break;
    case TIERCEL_SCREEN_BRIGHTNESS: ui_settings_brightness_commit(); break;
    case TIERCEL_SCREEN_PEREGRINE:  ui_peregrine_leave();           break;
    case TIERCEL_SCREEN_PLAYBILL:   ui_playbill_leave();            break;
    case TIERCEL_SCREEN_BLUETOOTH:  ui_bluetooth_leave();           break;
    default: break;
    }
}

static void on_enter(tiercel_screen_t s)
{
    switch (s) {
    case TIERCEL_SCREEN_IDLE:       ui_clock_refresh();             break;
    case TIERCEL_SCREEN_CLOCKFACE:  ui_clock_pick_enter();          break;
    case TIERCEL_SCREEN_BRIGHTNESS: ui_settings_brightness_enter(); break;
    case TIERCEL_SCREEN_PEREGRINE:  ui_peregrine_enter();           break;
    case TIERCEL_SCREEN_PLAYBILL:   ui_playbill_enter();            break;
    case TIERCEL_SCREEN_BLUETOOTH:  ui_bluetooth_enter();           break;
    default: break;
    }
}

void ui_nav_goto(tiercel_screen_t screen)
{
    if (screen < 0 || screen >= TIERCEL_SCREEN_COUNT) {
        ESP_LOGE(TAG, "bad screen id %d", (int)screen);
        return;
    }
    on_leave(s_current, screen);
    show_overlay(screen);
    loadScreen((enum ScreensEnum)s_eez_id[screen]);
    s_current = screen;
    clamp_selection(screen);
    refresh_selection(screen);
    on_enter(screen);
    ESP_LOGD(TAG, "-> %s", s_policy[screen].name);
}

void ui_nav_back(void)
{
    /* Leaving the alert by any route acknowledges it, so it snoozes rather
     * than vanishing with nothing behind it. */
    if (s_current == TIERCEL_SCREEN_ALERT) {
        ui_alerts_acknowledge();
        return;
    }
    if (s_policy[s_current].back != s_current) {
        tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
        ui_nav_goto(s_policy[s_current].back);
    }
}

/* ---------------------------------------------------------------------- *
 * The knob.
 * ---------------------------------------------------------------------- */
void ui_nav_rotate(int diff)
{
    if (diff == 0) {
        return;
    }
    /* Any input wakes the clock, and the waking detent is CONSUMED -- it
     * should not also move a selection the user cannot see yet. */
    if (s_current == TIERCEL_SCREEN_IDLE) {
        ui_nav_goto(TIERCEL_SCREEN_MENU);
        return;
    }

    switch (s_current) {
    case TIERCEL_SCREEN_CLIMATE:    ui_climate_rotate(diff);            return;
    case TIERCEL_SCREEN_CLOCKFACE:  ui_clock_pick_rotate(diff);         return;
    case TIERCEL_SCREEN_PEREGRINE:  ui_peregrine_rotate(diff);          return;
    case TIERCEL_SCREEN_PLAYBILL:   ui_playbill_rotate(diff);           return;
    case TIERCEL_SCREEN_BRIGHTNESS: ui_settings_brightness_step(diff);  return;
    case TIERCEL_SCREEN_BLUETOOTH:  ui_bluetooth_rotate(diff);          return;
    default: break;
    }

    const int n = selectable_count(s_current);
    if (n <= 0) {
        return;     /* read-only screen: the knob is simply inert here */
    }

    /*
     * NEVER STORE THE OVERSHOOT. `diff` is this event's detents applied to
     * the DISPLAYED index; nothing remembers how far past the end the knob
     * went, so two turns past the last item and one back moves by one.
     *
     * Carousels wrap (no ends, so no bump). Lists and Energy's pages clamp,
     * and a detent that cannot move them bumps the haptic motor -- the
     * hand learns the end of the list without looking.
     */
    int sel = s_sel[s_current] + diff;
    if (wraps(s_current)) {
        sel = wrap(sel, n);
    } else if (sel < 0 || sel > n - 1) {
        sel = sel < 0 ? 0 : n - 1;
        if (sel == s_sel[s_current]) {
            tiercel_board_haptic(TIERCEL_HAPTIC_LIMIT);
            return;
        }
    }
    if (sel != s_sel[s_current]) {
        if (s_current == TIERCEL_SCREEN_SETTINGS) {
            ui_settings_disarm_reset();
        }
        s_sel[s_current] = sel;
        refresh_selection(s_current);
    }
}

/* ---------------------------------------------------------------------- *
 * Taps.
 * ---------------------------------------------------------------------- */
void ui_nav_wake(void)
{
    if (s_current == TIERCEL_SCREEN_IDLE) {
        tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
        ui_nav_goto(TIERCEL_SCREEN_MENU);
    }
}

void ui_nav_open(void)
{
    if (s_current != TIERCEL_SCREEN_MENU) {
        return;
    }
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    ui_nav_goto(s_menu[ui_nav_menu_item()].dest);
}

void ui_nav_step(int dir)
{
    if (!wraps(s_current)) {
        return;
    }
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    ui_nav_rotate(dir < 0 ? -1 : 1);
}

void ui_nav_select_row(int row)
{
    const int n = selectable_count(s_current);
    if (row < 0 || row >= n) {
        return;
    }
    s_sel[s_current] = row;
    refresh_selection(s_current);
}

/* ---------------------------------------------------------------------- *
 * The back gesture.
 *
 * Read straight from the touch input on a 20 ms timer rather than from widget
 * events, because both gestures must work ANYWHERE on the glass -- over a
 * button, over a label, over the rim -- and LVGL delivers events to whatever
 * is under the finger, not to the screen.
 *
 * Once a gesture is recognised the touch is taken away from the widgets
 * (lv_indev_reset + wait_release): a swipe that starts on the play button
 * must not also press play when the finger lifts.
 *
 *   swipe  a drag, left or right, that is more horizontal than vertical; the
 *          left-edge arc lights as it grows; released past 70 px in the
 *          direction it started it goes back, released short it does nothing
 *          (the design's rule). Either direction counts: on a round face
 *          there is no edge to swipe in from, and a reach across the glass
 *          comes as naturally one way as the other
 *   hold   a still finger for 650 ms; the rim fills as it goes, and lifting
 *          or moving early cancels it
 *
 * Any other drag past SLOP_PX -- on any screen, in any back mode, including
 * the menu where there is no back gesture at all -- is taken away from the
 * widgets too, and does nothing. A drag is never a tap: without this a swipe
 * that starts on a carousel's neighbour icon clicks it on release and steps
 * the carousel, and swipes must NEVER move between apps -- that is the knob's
 * job alone. Nothing on the glass needs a drag (every arc is display-only).
 * ---------------------------------------------------------------------- */
#define GESTURE_TICK_MS  20
#define SLOP_PX          10
#define SWIPE_BACK_PX    70
#define HOLD_MS          650
#define HOLD_SHOW_MS     120     /* a tap never flashes the ring */

/* G_DONE: the touch has been dealt with (or ignored); wait for the lift. */
typedef enum { G_IDLE, G_PRESSED, G_SWIPE, G_DONE } g_state_t;

static g_state_t  s_g;
static lv_point_t s_g_start;
static int64_t    s_g_at;
static int        s_g_dir;      /* +1 a right swipe, -1 a left one */

static void gesture_end(const overlay_t *o)
{
    if (o->swipe) {
        ui_lv_set_hidden(*o->swipe, true);
        ui_lv_set_hidden(*o->hold, true);
    }
    s_g = G_IDLE;
}

static void gesture_steal(lv_indev_t *in)
{
    lv_indev_reset(in, NULL);
    lv_indev_wait_release(in);
}

static void gesture_timer_cb(lv_timer_t *t)
{
    (void)t;
    lv_indev_t *in = tiercel_board_touch_indev();
    if (!in) {
        return;
    }
    const tiercel_back_gesture_t mode = back_gesture();
    const overlay_t o = overlay_of(s_current);
    const bool armed = s_policy[s_current].app && o.swipe &&
                       mode != TIERCEL_BACK_CHIP;
    const bool down = lv_indev_get_state(in) == LV_INDEV_STATE_PRESSED;

    if (!down) {
        if (s_g == G_SWIPE && armed) {
            lv_point_t p;
            lv_indev_get_point(in, &p);
            if (s_g_dir * (p.x - s_g_start.x) >= SWIPE_BACK_PX) {
                gesture_end(&o);
                ui_nav_back();
                return;
            }
        }
        gesture_end(&o);
        return;
    }

    lv_point_t p;
    lv_indev_get_point(in, &p);
    const int64_t now = esp_timer_get_time();

    if (s_g == G_IDLE) {
        s_g = G_PRESSED;
        s_g_start = p;
        s_g_at = now;
        return;
    }
    if (s_g == G_DONE) {
        return;                 /* waiting for the finger to lift */
    }

    const int dx = p.x - s_g_start.x, dy = p.y - s_g_start.y;
    const int adx = dx < 0 ? -dx : dx, ady = dy < 0 ? -dy : dy;
    const bool moved = adx > SLOP_PX || ady > SLOP_PX;

    if (armed && mode == TIERCEL_BACK_SWIPE) {
        if (s_g == G_PRESSED && adx > SLOP_PX && adx > ady) {
            s_g = G_SWIPE;
            s_g_dir = dx > 0 ? 1 : -1;
            gesture_steal(in);
        } else if (s_g == G_PRESSED && moved) {
            gesture_steal(in);  /* a vertical drag: not a back, not a tap */
            s_g = G_DONE;
            return;
        }
        if (s_g == G_SWIPE) {
            /* The arc grows with the drag and is full at the threshold. */
            const int d = s_g_dir * dx;
            const int pct = d <= 0 ? 0 : d >= SWIPE_BACK_PX ? 100
                          : d * 100 / SWIPE_BACK_PX;
            lv_arc_set_value(*o.swipe, pct);
            ui_lv_set_hidden(*o.swipe, false);
        }
        return;
    }

    /* No back gesture here (the menu, the clock, setup, an alert, or Chip
     * mode): a drag is still not a tap. It wakes the clock, as any input
     * does, and otherwise does nothing. */
    if (!armed || mode != TIERCEL_BACK_HOLD) {
        if (moved) {
            gesture_steal(in);
            s_g = G_DONE;
            ui_nav_wake();
        }
        return;
    }

    /* Hold. */
    if (moved) {
        gesture_steal(in);
        ui_lv_set_hidden(*o.hold, true);
        s_g = G_DONE;           /* moved: not a hold, ignore until lifted */
        return;
    }
    const int ms = (int)((now - s_g_at) / 1000);
    if (ms >= HOLD_SHOW_MS) {
        lv_arc_set_value(*o.hold, ms >= HOLD_MS ? 100 : ms * 100 / HOLD_MS);
        ui_lv_set_hidden(*o.hold, false);
    }
    if (ms >= HOLD_MS) {
        gesture_steal(in);
        ui_lv_set_hidden(*o.hold, true);
        s_g = G_DONE;
        ui_nav_back();
    }
}

/* ---------------------------------------------------------------------- *
 * Back to the clock on its own, after Settings > Clock Timeout.
 * ---------------------------------------------------------------------- */
static void idle_timer_cb(lv_timer_t *t)
{
    (void)t;
    if (s_current == TIERCEL_SCREEN_IDLE || s_current == TIERCEL_SCREEN_SETUP ||
        s_current == TIERCEL_SCREEN_ALERT) {
        return;
    }
    /* A conversation in progress is not idleness. */
    if (s_current == TIERCEL_SCREEN_PEREGRINE && ui_peregrine_busy()) {
        return;
    }
    tiercel_display_cfg_t d;
    tiercel_config_get_display(&d);
    if (d.idle_timeout_s == 0 ||
        tiercel_board_ms_since_input() < (uint32_t)d.idle_timeout_s * 1000u) {
        return;
    }
    ui_nav_goto(TIERCEL_SCREEN_IDLE);
}

static void on_knob(int diff, void *ctx)
{
    (void)ctx;
    ui_nav_rotate(diff);
}

void ui_nav_init(void)
{
    tiercel_board_set_rotate_callback(on_knob, NULL);
    ui_nav_goto(TIERCEL_SCREEN_IDLE);
    lv_timer_create(gesture_timer_cb, GESTURE_TICK_MS, NULL);
    lv_timer_create(idle_timer_cb, 1000, NULL);
}
