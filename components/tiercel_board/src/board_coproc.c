/*
 * The UART to the ESP32 co-processor. Moves text lines both ways and knows
 * nothing about what they say, beyond that each starts with a capital -- the
 * protocol lives in components/tiercel_bt and coprocessor/main/main.c.
 *
 * Ported from the Crack the Safe sample for this board, with smaller ring
 * buffers: internal RAM is the scarce resource on Tiercel (see the MQTT task
 * stack note in tiercel_mqtt.c), and the only large thing ever sent is
 * streamed audio, whose writer can afford to wait for the wire.
 */

#include <string.h>

#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "board_pins.h"
#include "tiercel_board.h"

static const char *TAG = "board.coproc";

#define LINE_MAX_LEN   160          /* the ESP32 sends short lines only */
#define RX_BUF         512
/* 2 KB, the sample's 8 KB cut down. A streamed "PCM" line is ~700 bytes
 * every 11.6 ms; when the buffer is full uart_write_bytes() blocks the
 * playback task until the wire drains, which only paces it. */
#define TX_BUF         2048
#define RX_TASK_STACK  3072

static tiercel_board_coproc_line_cb_t s_cb;
static void *s_ctx;
static SemaphoreHandle_t s_tx_lock;

static void rx_task(void *arg)
{
    (void)arg;
    char line[LINE_MAX_LEN];
    size_t n = 0;
    bool overflow = false;
    uint8_t buf[64];
    for (;;) {
        const int got = uart_read_bytes(BOARD_COPROC_UART, buf, sizeof(buf),
                                        pdMS_TO_TICKS(100));
        for (int i = 0; i < got; i++) {
            const char c = (char)buf[i];
            if (c == '\r') {
                continue;
            }
            if (c != '\n') {
                /* Every line starts with an upper-case keyword. Anything
                 * before it is junk -- typically the byte the ESP32's TX pin
                 * glitches out while it is being configured at boot. */
                if (n == 0 && !(c >= 'A' && c <= 'Z')) {
                    continue;
                }
                if (n < sizeof(line) - 1) {
                    line[n++] = c;
                } else {
                    overflow = true;
                }
                continue;
            }
            line[n] = '\0';
            if (n && !overflow && s_cb) {
                s_cb(line, s_ctx);
            }
            n = 0;
            overflow = false;
        }
    }
}

esp_err_t tiercel_board_coproc_start(tiercel_board_coproc_line_cb_t cb,
                                     void *ctx)
{
    if (s_tx_lock) {
        return ESP_ERR_INVALID_STATE;
    }
    const uart_config_t uc = {
        .baud_rate = BOARD_COPROC_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    esp_err_t err = uart_driver_install(BOARD_COPROC_UART, RX_BUF, TX_BUF, 0,
                                        NULL, 0);
    if (err == ESP_OK) {
        err = uart_param_config(BOARD_COPROC_UART, &uc);
    }
    if (err == ESP_OK) {
        err = uart_set_pin(BOARD_COPROC_UART, BOARD_COPROC_TX, BOARD_COPROC_RX,
                           UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    }
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "UART setup failed: %s", esp_err_to_name(err));
        return err;
    }
    s_cb = cb;
    s_ctx = ctx;
    s_tx_lock = xSemaphoreCreateMutex();
    if (xTaskCreate(rx_task, "coproc_rx", RX_TASK_STACK, NULL, 4, NULL)
            != pdPASS) {
        ESP_LOGE(TAG, "no memory for the reader task");
        return ESP_ERR_NO_MEM;
    }
    ESP_LOGI(TAG, "link on UART%d (TX %d, RX %d) at %d", BOARD_COPROC_UART,
             BOARD_COPROC_TX, BOARD_COPROC_RX, BOARD_COPROC_BAUD);
    return ESP_OK;
}

void tiercel_board_coproc_send(const char *line, size_t len)
{
    if (!s_tx_lock) {
        return;
    }
    xSemaphoreTake(s_tx_lock, portMAX_DELAY);
    uart_write_bytes(BOARD_COPROC_UART, line, len);
    uart_write_bytes(BOARD_COPROC_UART, "\n", 1);
    xSemaphoreGive(s_tx_lock);
}
