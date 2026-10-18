/*
 * tiercel_board -- the only board-aware code in the firmware.
 *
 * One board sits behind this header: the Waveshare ESP32-S3-Knob-Touch-LCD-1.8
 * (360x360 ST77916 on QSPI, CST816 touch, a detent knob with no push, and a
 * DRV2605 haptic driver). Everything above it -- the UI, the data model,
 * MQTT, Peregrine -- is board-agnostic and should stay that way.
 *
 * The shape follows Capstan's capstan_board.h so the navigation layer reads
 * the same on both products. What differs is the input model: Capstan is
 * ring-only with touch off; Tiercel is touch-first, and the knob only ever
 * turns -- there is no press, so "press" is a tap on the glass.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "driver/i2c_master.h"
#include "esp_err.h"
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *name;      /**< "knob18" */
    uint16_t    h_res;
    uint16_t    v_res;
    bool        round;
    bool        has_haptics;   /**< false if the DRV2605 did not answer */
} tiercel_board_info_t;

const tiercel_board_info_t *tiercel_board_info(void);

/**
 * Bring up the panel, touch, knob, haptics and LVGL.
 *
 * On return LVGL runs on its own task and the panel is cleared but dark --
 * call tiercel_board_backlight_set() once the first screen is drawn.
 */
esp_err_t tiercel_board_init(void);

/**
 * The LVGL mutex. Wrap every lv_* call made from a task other than LVGL's.
 * LVGL's own callbacks (timers, events, the rotate callback below) already
 * hold it -- taking it again there deadlocks.
 *
 * @param timeout_ms  0 waits forever.
 */
bool tiercel_board_lock(uint32_t timeout_ms);
void tiercel_board_unlock(void);

/** Backlight 0-100, LEDC PWM. */
esp_err_t tiercel_board_backlight_set(uint8_t percent);

/** The shared I2C0 bus (touch + haptics), for anything else that lands on it. */
i2c_master_bus_handle_t tiercel_board_i2c_bus(void);

/** The touch input device. Always enabled -- Tiercel is touch-first. */
lv_indev_t *tiercel_board_touch_indev(void);

/**
 * Knob rotation, as a signed number of detents (+ is clockwise).
 *
 * Delivered from an lv_timer, so it runs in the LVGL task with the display
 * lock ALREADY HELD: call lv_* freely, never tiercel_board_lock() inside.
 * Detents are relative and the board keeps no running total -- see the "never
 * store the overshoot" note in main/ui_nav.c.
 */
typedef void (*tiercel_board_rotate_cb_t)(int diff, void *ctx);
void tiercel_board_set_rotate_callback(tiercel_board_rotate_cb_t cb, void *ctx);

/**
 * Milliseconds since the last touch or detent. The idle-clock timeout runs
 * from this rather than LVGL's inactivity timer, which any redraw resets --
 * including the clock's own second hand.
 */
uint32_t tiercel_board_ms_since_input(void);

/* ----------------------------------------------------------------------
 * Haptics (DRV2605, internal trigger, ROM waveform library).
 *
 * Fire-and-forget: each call queues one effect and returns. Calls closer
 * together than the effect's own length are coalesced rather than queued,
 * so spinning the knob against an end stop gives a steady train of bumps
 * instead of a backlog that keeps buzzing after the hand has let go.
 *
 * All of them are no-ops when the user has turned haptics off
 * (tiercel_board_haptics_enable(false)) or the driver did not answer.
 * ---------------------------------------------------------------------- */
typedef enum {
    TIERCEL_HAPTIC_LIMIT = 0,  /**< a knob value is already at its min/max */
    TIERCEL_HAPTIC_TAP,        /**< a tap was accepted */
    TIERCEL_HAPTIC_CONFIRM,    /**< something committed: mode set, toggle */
    TIERCEL_HAPTIC_ALERT,      /**< an alarm overlay was raised */
} tiercel_haptic_t;

void tiercel_board_haptic(tiercel_haptic_t kind);
void tiercel_board_haptics_enable(bool enabled);

/* ----------------------------------------------------------------------
 * The UART to the ESP32 co-processor (Bluetooth headphones).
 *
 * Moves text lines both ways and knows nothing about what they say -- the
 * protocol lives in components/tiercel_bt and coprocessor/main/main.c.
 * ---------------------------------------------------------------------- */
typedef void (*tiercel_board_coproc_line_cb_t)(const char *line, void *ctx);

/** Open the link. Call once; `cb` receives every line the ESP32 sends, on
 *  the link's own reader task. */
esp_err_t tiercel_board_coproc_start(tiercel_board_coproc_line_cb_t cb,
                                     void *ctx);

/** Send `len` bytes as one line (a newline is appended). Thread-safe; blocks
 *  only while the TX buffer is full. A no-op before the link is started. */
void tiercel_board_coproc_send(const char *line, size_t len);

#ifdef __cplusplus
}
#endif
