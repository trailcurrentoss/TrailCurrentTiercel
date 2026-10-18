/*
 * The Playbill screen: now playing or the FM tuner, transport, and the knob
 * as volume or tuning. State comes from tiercel_playbill (Playbill's own
 * retained status topics); commands go out through tiercel_mqtt.
 */
#pragma once

#include <stddef.h>

void ui_playbill_init(void);
void ui_playbill_enter(void);
void ui_playbill_leave(void);
/** Knob: volume (2% a detent) or, on the radio in tune mode, frequency
 *  (0.2 MHz a detent). Both bump at their ends. */
void ui_playbill_rotate(int diff);

/** Taps, from main/actions.c. `src` 0 = Media, 1 = Radio. */
void ui_playbill_source(int src);
void ui_playbill_prev(void);
void ui_playbill_play(void);
void ui_playbill_next(void);
void ui_playbill_knob_toggle(void);

void ui_playbill_refresh(void);
void ui_playbill_summary(char *out, size_t len);
