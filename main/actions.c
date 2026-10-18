/*
 * The actions EEZ Studio declares in ui/actions.h.
 *
 * With flowSupport off, every tap target in GUI/Tiercel.eez-project names an
 * action and EEZ emits `void action_<name>(lv_event_t *e)` for it. They are
 * implemented here -- NOT in main/ui/, which the export owns -- and each one
 * is a one-line hand-off to the module that owns the behaviour. Variants of
 * one action (a carousel's previous/next, a list's rows) arrive as the event
 * handler's `userData`.
 *
 * All of these run in the LVGL task with the display lock held.
 */

#include <stdint.h>

#include "actions.h"

#include "tiercel_board.h"
#include "ui_alerts.h"
#include "ui_bluetooth.h"
#include "ui_climate.h"
#include "ui_clock.h"
#include "ui_devices.h"
#include "ui_nav.h"
#include "ui_peregrine.h"
#include "ui_playbill.h"
#include "ui_settings.h"

static int user_data(lv_event_t *e)
{
    return (int)(intptr_t)lv_event_get_user_data(e);
}

void action_wake(lv_event_t *e)           { (void)e; ui_nav_wake(); }
void action_nav_back(lv_event_t *e)       { (void)e; ui_nav_back(); }
void action_nav_open(lv_event_t *e)       { (void)e; ui_nav_open(); }

/* userData 0 = the previous neighbour, 1 = the next. */
void action_nav_step(lv_event_t *e)       { ui_nav_step(user_data(e) ? 1 : -1); }

void action_climate_open_mode(lv_event_t *e)
{
    (void)e;
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    ui_nav_goto(TIERCEL_SCREEN_CLIMATE_MODE);
    ui_nav_select_row((int)ui_climate_mode());
}

/* userData = the row: Heat, Cool, Auto, Off. Sets it and returns, as the
 * design's Mode list does. */
void action_climate_mode_select(lv_event_t *e)
{
    const int row = user_data(e);
    ui_nav_select_row(row);
    ui_climate_set_mode((ui_climate_mode_t)row);
    ui_climate_refresh();
    tiercel_board_haptic(TIERCEL_HAPTIC_CONFIRM);
    ui_nav_goto(TIERCEL_SCREEN_CLIMATE);
}

void action_device_toggle(lv_event_t *e)  { (void)e; ui_devices_press(); }

/* Battery -> Solar -> Loads -> Battery: a tap wraps, where the knob stops
 * at the ends. */
void action_energy_next(lv_event_t *e)
{
    (void)e;
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    const int next = (ui_nav_selection_of(TIERCEL_SCREEN_ENERGY) + 1) % 3;
    ui_nav_rotate(next - ui_nav_selection_of(TIERCEL_SCREEN_ENERGY));
}

void action_settings_activate(lv_event_t *e)
{
    (void)e;
    switch (ui_nav_selection_of(TIERCEL_SCREEN_SETTINGS)) {
    case UI_SETTINGS_WIFI:
    case UI_SETTINGS_MQTT:
        /* Status only: both are set through the setup portal, and the way
         * back into it is a factory reset. */
        return;
    case UI_SETTINGS_THEME:   ui_settings_theme_pressed();   break;
    case UI_SETTINGS_BACK:    ui_settings_back_pressed();    break;
    case UI_SETTINGS_HAPTICS: ui_settings_haptics_pressed(); break;
    case UI_SETTINGS_KNOB:    ui_settings_knob_pressed();    break;
    case UI_SETTINGS_SNOOZE:  ui_settings_snooze_pressed();  break;
    case UI_SETTINGS_TIMEOUT: ui_settings_timeout_pressed(); break;
    case UI_SETTINGS_RESET:   ui_settings_factory_reset_pressed(); break;
    case UI_SETTINGS_BRIGHTNESS:
        tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
        ui_nav_goto(TIERCEL_SCREEN_BRIGHTNESS);
        return;
    case UI_SETTINGS_BLUETOOTH:
        tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
        ui_nav_goto(TIERCEL_SCREEN_BLUETOOTH);
        return;
    case UI_SETTINGS_LOCALE:
        tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
        ui_nav_goto(TIERCEL_SCREEN_LOCALE);
        return;
    case UI_SETTINGS_FACE:
        tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
        ui_nav_goto(TIERCEL_SCREEN_CLOCKFACE);
        return;
    default:
        return;
    }
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    ui_settings_refresh();
}

/* userData = the row: Temperature, Leveling, Clock. */
void action_locale_toggle(lv_event_t *e)
{
    const int row = user_data(e);
    ui_nav_select_row(row);
    ui_settings_locale_pressed(row);
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
}

void action_clock_face_set(lv_event_t *e)
{
    (void)e;
    ui_clock_pick_press();
    tiercel_board_haptic(TIERCEL_HAPTIC_CONFIRM);
    ui_nav_goto(TIERCEL_SCREEN_SETTINGS);
}

void action_alert_dismiss(lv_event_t *e)
{
    (void)e;
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    ui_alerts_acknowledge();
}

void action_peregrine_tap(lv_event_t *e)  { (void)e; ui_peregrine_tap(); }

void action_playbill_source(lv_event_t *e) { ui_playbill_source(user_data(e)); }
void action_playbill_prev(lv_event_t *e)   { (void)e; ui_playbill_prev(); }
void action_playbill_play(lv_event_t *e)   { (void)e; ui_playbill_play(); }
void action_playbill_next(lv_event_t *e)   { (void)e; ui_playbill_next(); }
void action_playbill_knob_toggle(lv_event_t *e) { (void)e; ui_playbill_knob_toggle(); }

void action_bluetooth_row(lv_event_t *e)    { ui_bluetooth_row_pressed(user_data(e)); }
void action_bluetooth_search(lv_event_t *e) { (void)e; ui_bluetooth_search_pressed(); }
void action_bluetooth_forget(lv_event_t *e) { (void)e; ui_bluetooth_forget_pressed(); }
