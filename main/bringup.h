#pragma once

/** Build the bring-up screen on the active display. Call under the LVGL lock.
 *  Exists only while no EEZ Studio export does -- see bringup.c. */
void bringup_screen(void);
