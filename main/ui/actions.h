#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_wake(lv_event_t * e);
extern void action_nav_back(lv_event_t * e);
extern void action_nav_open(lv_event_t * e);
extern void action_nav_step(lv_event_t * e);
extern void action_climate_open_mode(lv_event_t * e);
extern void action_climate_mode_select(lv_event_t * e);
extern void action_device_toggle(lv_event_t * e);
extern void action_energy_next(lv_event_t * e);
extern void action_settings_activate(lv_event_t * e);
extern void action_locale_toggle(lv_event_t * e);
extern void action_clock_face_set(lv_event_t * e);
extern void action_alert_dismiss(lv_event_t * e);
extern void action_peregrine_tap(lv_event_t * e);
extern void action_playbill_source(lv_event_t * e);
extern void action_playbill_prev(lv_event_t * e);
extern void action_playbill_play(lv_event_t * e);
extern void action_playbill_next(lv_event_t * e);
extern void action_playbill_knob_toggle(lv_event_t * e);
extern void action_bluetooth_row(lv_event_t * e);
extern void action_bluetooth_search(lv_event_t * e);
extern void action_bluetooth_forget(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/