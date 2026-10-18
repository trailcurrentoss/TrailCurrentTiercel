/*
 * Settings > Bluetooth: pair headphones through the ESP32 co-processor
 * (components/tiercel_bt). Opening the screen searches unless headphones are
 * already connected; a tapped row connects; Forget drops the remembered
 * pair. Nothing about Bluetooth is shown anywhere else -- at boot the
 * co-processor quietly reconnects whatever it remembers.
 *
 * All of these run in the LVGL task with the display lock held.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

void ui_bluetooth_enter(void);
void ui_bluetooth_leave(void);
/** The knob scrolls the device list. */
void ui_bluetooth_rotate(int diff);

void ui_bluetooth_row_pressed(int row);
void ui_bluetooth_search_pressed(void);
void ui_bluetooth_forget_pressed(void);

/** Settings carousel value: the connected headphones' name, or the state.
 *  `*ok` true while connected (green). */
const char *ui_bluetooth_value(bool *ok);
