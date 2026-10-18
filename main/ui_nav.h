/*
 * Screen navigation, input routing and the back gesture.
 *
 * Tiercel's input model, from the design (DOCS/Tiercel.dc.html):
 *
 *   TURN  moves through items or adjusts the value drawn on the rim. One
 *         detent, one step. A value already at its end bumps the haptic
 *         motor instead of moving -- every bounded knob value, on every
 *         screen. Carousels wrap and so never bump.
 *   TAP   replaces press: open, toggle, confirm. Only the centred item and
 *         labelled buttons respond.
 *   BACK  one of three gestures, chosen in Settings > Back Gesture: swipe
 *         right anywhere, hold anywhere for 0.65 s, or a chip at 12 o'clock.
 *
 * C here sets text, states and flags and loads screens. It never moves,
 * resizes or restyles an EEZ-authored widget -- see docs/gui.md.
 */
#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Mirrors PAGES in GUI/tmp/gen_eez_project.py, in order. ui_nav.c maps each
 * onto the generated ScreensEnum explicitly and a _Static_assert there fails
 * the build if the two lists differ in length.
 */
typedef enum {
    TIERCEL_SCREEN_IDLE = 0,       /* the clock */
    TIERCEL_SCREEN_MENU,           /* the app carousel */
    TIERCEL_SCREEN_CLIMATE,
    TIERCEL_SCREEN_CLIMATE_MODE,
    TIERCEL_SCREEN_DEVICES,        /* the devices carousel */
    TIERCEL_SCREEN_PEREGRINE,      /* voice */
    TIERCEL_SCREEN_PLAYBILL,       /* media */
    TIERCEL_SCREEN_ENERGY,
    TIERCEL_SCREEN_WATER,
    TIERCEL_SCREEN_AIR,
    TIERCEL_SCREEN_LEVEL,
    TIERCEL_SCREEN_SETTINGS,
    TIERCEL_SCREEN_ALERT,          /* full-screen overlay */
    TIERCEL_SCREEN_SETUP,          /* soft-AP provisioning instructions */
    TIERCEL_SCREEN_LOCALE,         /* units, opened from Settings */
    TIERCEL_SCREEN_CLOCKFACE,      /* Settings > Clock Face picker */
    TIERCEL_SCREEN_BRIGHTNESS,     /* Settings > Brightness */
    TIERCEL_SCREEN_BLUETOOTH,      /* Settings > Bluetooth headphones */
    TIERCEL_SCREEN_COUNT
} tiercel_screen_t;

void ui_nav_init(void);
void ui_nav_goto(tiercel_screen_t screen);
void ui_nav_back(void);
tiercel_screen_t ui_nav_current(void);

/** The knob turned `diff` detents (+ clockwise). LVGL task, lock held. */
void ui_nav_rotate(int diff);

/* ---- taps, from main/actions.c --------------------------------------- */

/** Any tap on the idle clock. */
void ui_nav_wake(void);
/** The centre tile of a carousel (menu: open the app). */
void ui_nav_open(void);
/** A carousel neighbour: dir -1 previous, +1 next. */
void ui_nav_step(int dir);
/** A list row was tapped: select it (Climate Mode, Locale). */
void ui_nav_select_row(int row);

/* ---- selection -------------------------------------------------------- */

/** The highlighted item on the current screen, or -1 if none. */
int ui_nav_selection(void);
int ui_nav_selection_of(tiercel_screen_t s);
/** The app carousel's selected item, as an index into the menu table. */
int ui_nav_menu_item(void);
/** Re-clamp and repaint after a list changed length underneath. */
void ui_nav_selection_changed(void);

#ifdef __cplusplus
}
#endif
