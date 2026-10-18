/*
 * DRV2605 haptic driver, internal trigger, ROM effect library.
 *
 * Bring-up mirrors what Waveshare's Demo 03 does through SensorLib's
 * SensorDRV2605::begin(): out of standby, clear the waveform slots and the
 * overdrive/sustain/brake offsets, ERM open-loop, then select a library and
 * internal-trigger mode. The motor nets are labelled LRA_P/LRA_N on the
 * schematic, but the demo drives it as an ERM and it works; LRA mode is a
 * Kconfig option (TIERCEL_HAPTIC_LRA) for a unit that feels weak as ERM.
 *
 * An effect is "write its id into waveform slot 1, zero slot 2, set GO". The
 * driver plays it from its own ROM and the I2C bus is free again in well
 * under a millisecond, so this runs inline on the caller's task.
 */

#include "esp_check.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "board_internal.h"
#include "board_pins.h"
#include "tiercel_board.h"

static const char *TAG = "board.haptic";

#define REG_STATUS     0x00
#define REG_MODE       0x01
#define REG_RTP        0x02
#define REG_LIBRARY    0x03
#define REG_WAVESEQ1   0x04
#define REG_WAVESEQ2   0x05
#define REG_GO         0x0C
#define REG_OVERDRIVE  0x0D
#define REG_SUSTAINPOS 0x0E
#define REG_SUSTAINNEG 0x0F
#define REG_BREAK      0x10
#define REG_AUDIOMAX   0x13
#define REG_FEEDBACK   0x1A
#define REG_CONTROL3   0x1D

static i2c_master_dev_handle_t s_dev;
static bool    s_present;
static bool    s_enabled = true;
static int64_t s_last_us;

/*
 * Effect ids from the DRV2605 ROM library table (datasheet section 11.2),
 * which is the same across libraries 1-7. Kconfig overrides let a unit with a
 * different motor be tuned without touching code.
 */
static const struct {
    uint8_t  effect;
    uint32_t min_gap_ms;   /* coalescing window -- see tiercel_board.h */
} s_kinds[] = {
    [TIERCEL_HAPTIC_LIMIT]   = { CONFIG_TIERCEL_HAPTIC_LIMIT_EFFECT,   90 },
    [TIERCEL_HAPTIC_TAP]     = { CONFIG_TIERCEL_HAPTIC_TAP_EFFECT,     60 },
    [TIERCEL_HAPTIC_CONFIRM] = { CONFIG_TIERCEL_HAPTIC_CONFIRM_EFFECT, 120 },
    [TIERCEL_HAPTIC_ALERT]   = { CONFIG_TIERCEL_HAPTIC_ALERT_EFFECT,   400 },
};

static esp_err_t wr(uint8_t reg, uint8_t val)
{
    const uint8_t buf[2] = { reg, val };
    return i2c_master_transmit(s_dev, buf, sizeof(buf), 50);
}

static esp_err_t rd(uint8_t reg, uint8_t *val)
{
    return i2c_master_transmit_receive(s_dev, &reg, 1, val, 1, 50);
}

esp_err_t board_haptic_init(i2c_master_bus_handle_t bus, bool *present)
{
    *present = false;
    const i2c_device_config_t cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address  = BOARD_HAPTIC_ADDR,
        .scl_speed_hz    = BOARD_I2C_HZ,
    };
    ESP_RETURN_ON_ERROR(i2c_master_bus_add_device(bus, &cfg, &s_dev), TAG,
                        "add device");

    uint8_t status = 0;
    if (rd(REG_STATUS, &status) != ESP_OK) {
        /* Not fatal: everything else on the board works without it, and the
         * haptic calls become no-ops. */
        ESP_LOGW(TAG, "DRV2605 did not answer at 0x%02X -- haptics off",
                 BOARD_HAPTIC_ADDR);
        return ESP_OK;
    }
    const uint8_t id = status >> 5;   /* 3 = DRV2605, 7 = DRV2605L */

    esp_err_t err = ESP_OK;
    err |= wr(REG_MODE, 0x00);          /* out of standby, internal trigger */
    err |= wr(REG_RTP, 0x00);
    err |= wr(REG_WAVESEQ1, 1);
    err |= wr(REG_WAVESEQ2, 0);
    err |= wr(REG_OVERDRIVE, 0);
    err |= wr(REG_SUSTAINPOS, 0);
    err |= wr(REG_SUSTAINNEG, 0);
    err |= wr(REG_BREAK, 0);
    err |= wr(REG_AUDIOMAX, 0x64);

    uint8_t fb = 0, c3 = 0;
    err |= rd(REG_FEEDBACK, &fb);
    err |= rd(REG_CONTROL3, &c3);
#ifdef CONFIG_TIERCEL_HAPTIC_LRA
    err |= wr(REG_FEEDBACK, fb | 0x80);           /* N_ERM_LRA = LRA */
    /* Open loop: closed-loop LRA needs an auto-calibration run per motor,
     * which this firmware does not do. */
    err |= wr(REG_CONTROL3, c3 | 0x01);           /* LRA_OPEN_LOOP */
#else
    err |= wr(REG_FEEDBACK, fb & 0x7F);           /* N_ERM_LRA = ERM */
    err |= wr(REG_CONTROL3, c3 | 0x20);           /* ERM open loop */
#endif
    err |= wr(REG_LIBRARY, CONFIG_TIERCEL_HAPTIC_LIBRARY);
    err |= wr(REG_MODE, 0x00);
    ESP_RETURN_ON_ERROR(err, TAG, "DRV2605 setup failed");

    s_present = true;
    *present = true;
    ESP_LOGI(TAG, "DRV2605%s (id %u) ready, library %d, %s",
             id == 7 ? "L" : "", (unsigned)id, CONFIG_TIERCEL_HAPTIC_LIBRARY,
#ifdef CONFIG_TIERCEL_HAPTIC_LRA
             "LRA"
#else
             "ERM"
#endif
             );
    return ESP_OK;
}

void tiercel_board_haptics_enable(bool enabled)
{
    s_enabled = enabled;
}

void tiercel_board_haptic(tiercel_haptic_t kind)
{
    if (!s_present || !s_enabled ||
        kind < 0 || kind >= (int)(sizeof(s_kinds) / sizeof(s_kinds[0]))) {
        return;
    }
    const int64_t now = esp_timer_get_time();
    if (now - s_last_us < (int64_t)s_kinds[kind].min_gap_ms * 1000) {
        return;
    }
    s_last_us = now;

    if (wr(REG_WAVESEQ1, s_kinds[kind].effect) != ESP_OK ||
        wr(REG_WAVESEQ2, 0) != ESP_OK ||
        wr(REG_GO, 1) != ESP_OK) {
        ESP_LOGD(TAG, "effect %u not sent", (unsigned)s_kinds[kind].effect);
    }
}
