/*
 * Bluetooth headphones, through the ESP32 co-processor. See tiercel_bt.h,
 * and coprocessor/main/main.c for the other end of every line.
 *
 * Ported from the Crack the Safe sample for this board (main/bt_link.c),
 * without the game's uploaded sound clips and synthesised siren: Tiercel's
 * only audio is what tiercel_audio plays, so everything reaches the
 * headphones as the live stream.
 */

#include "tiercel_bt.h"

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_rom_crc.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "mbedtls/base64.h"

#include "tiercel_audio.h"
#include "tiercel_board.h"

static const char *TAG = "bt";

#define HELLO_US   (2 * 1000 * 1000)    /* ask again until the ESP32 answers */

static SemaphoreHandle_t s_lock;
static tiercel_bt_state_t s_st;

static void send(const char *line)
{
    tiercel_board_coproc_send(line, strlen(line));
}

/* Copy a device name (UTF-8) into what the screen's Roboto subset can draw:
 * printable ASCII plus the few extras in GUI/tmp/fonts.py's ROBOTO_RANGE.
 * Curly quotes become straight ones -- Apple names AirPods "Name’s AirPods"
 * with U+2019 -- and anything else becomes '?', rather than a blank box. */
static void name_copy(char *dst, size_t size, const char *src)
{
    size_t n = 0;
    const unsigned char *p = (const unsigned char *)src;
    while (*p && n + 4 < size) {
        uint32_t cp;
        int len;
        if (*p < 0x80) {
            cp = *p;
            len = 1;
        } else if ((*p & 0xE0) == 0xC0 && p[1]) {
            cp = ((*p & 0x1Fu) << 6) | (p[1] & 0x3Fu);
            len = 2;
        } else if ((*p & 0xF0) == 0xE0 && p[1] && p[2]) {
            cp = ((*p & 0x0Fu) << 12) | ((p[1] & 0x3Fu) << 6) | (p[2] & 0x3Fu);
            len = 3;
        } else {
            cp = '?';
            len = 1;
            while (p[len] && (p[len] & 0xC0) == 0x80) {
                len++;             /* skip the rest of a 4-byte sequence */
            }
        }
        p += len;
        if (cp == 0x2018 || cp == 0x2019 || cp == 0x201B || cp == 0x2032) {
            cp = '\'';
        } else if (cp == 0x201C || cp == 0x201D) {
            cp = '"';
        }
        if (cp >= 0x20 && cp < 0x7F) {
            dst[n++] = (char)cp;
        } else if (cp == 0xB0 || cp == 0xB7) {
            dst[n++] = (char)(0xC0 | (cp >> 6));
            dst[n++] = (char)(0x80 | (cp & 0x3F));
        } else if (cp == 0x2013 || cp == 0x2014 || cp == 0x2026) {
            dst[n++] = (char)(0xE0 | (cp >> 12));
            dst[n++] = (char)(0x80 | ((cp >> 6) & 0x3F));
            dst[n++] = (char)(0x80 | (cp & 0x3F));
        } else {
            dst[n++] = '?';
        }
    }
    dst[n] = '\0';
}

static void dev_from(tiercel_bt_dev_t *d, const char *addr, int rssi,
                     const char *name)
{
    strlcpy(d->addr, addr, sizeof(d->addr));
    d->rssi = (int8_t)rssi;
    name_copy(d->name, sizeof(d->name), name && *name ? name : addr);
}

/* One line from the ESP32, on the board's reader task. */
static void on_line(const char *line, void *ctx)
{
    (void)ctx;
    ESP_LOGD(TAG, "<- %s", line);
    bool hello = false;

    if (strncmp(line, "LOG ", 4) == 0) {
        /* The ESP32's own log, so it can be read on the S3's USB port --
         * the ESP32's USB is on the other side of the plug. */
        ESP_LOGI(TAG, "esp32| %s", line + 4);
        return;
    }

    xSemaphoreTake(s_lock, portMAX_DELAY);
    if (strncmp(line, "READY", 5) == 0) {
        /* Just started (or answering HELLO): whatever it was doing before --
         * a search, a connection -- is gone. Its SAVED and STATE lines
         * follow at once. */
        hello = !s_st.present;
        s_st.present = true;
        s_st.searching = false;
        s_st.failed = false;
        s_st.conn = TIERCEL_BT_IDLE;
    } else if (strcmp(line, "SCAN start") == 0) {
        s_st.searching = true;
        s_st.ndev = 0;
        s_st.failed = false;
    } else if (strcmp(line, "SCAN done") == 0) {
        s_st.searching = false;
    } else if (strncmp(line, "DEV ", 4) == 0) {
        char addr[18];
        int rssi, used = 0;
        if (sscanf(line + 4, "%17s %d %n", addr, &rssi, &used) >= 2) {
            const char *name = used ? line + 4 + used : "";
            int i;
            for (i = 0; i < s_st.ndev && strcmp(s_st.dev[i].addr, addr) != 0;
                 i++) {
            }
            if (i < TIERCEL_BT_ROWS) {
                dev_from(&s_st.dev[i], addr, rssi, name);
                if (i == s_st.ndev) {
                    s_st.ndev++;
                }
            }
        }
    } else if (strncmp(line, "STATE ", 6) == 0) {
        char st[16], addr[18];
        int used = 0;
        if (sscanf(line + 6, "%15s %17s %n", st, addr, &used) >= 2) {
            const char *name = used ? line + 6 + used : "";
            s_st.conn = strcmp(st, "connected") == 0  ? TIERCEL_BT_CONNECTED
                      : strcmp(st, "connecting") == 0 ? TIERCEL_BT_CONNECTING
                                                      : TIERCEL_BT_IDLE;
            if (s_st.conn != TIERCEL_BT_IDLE) {
                dev_from(&s_st.peer, addr, 0, name);
            }
            if (s_st.conn == TIERCEL_BT_CONNECTED) {
                s_st.failed = false;
            }
        }
    } else if (strncmp(line, "SAVED ", 6) == 0) {
        char addr[18];
        int used = 0;
        if (sscanf(line + 6, "%17s %n", addr, &used) >= 1) {
            s_st.has_saved = strcmp(addr, "-") != 0;
            if (s_st.has_saved) {
                dev_from(&s_st.saved, addr, 0, used ? line + 6 + used : "");
            }
        }
    } else if (strcmp(line, "FAIL") == 0) {
        s_st.failed = true;
    } else {
        xSemaphoreGive(s_lock);
        return;                          /* not ours -- leave gen alone */
    }
    s_st.gen++;
    xSemaphoreGive(s_lock);

    if (hello) {
        ESP_LOGI(TAG, "co-processor answered");
    }
}

static void hello_cb(void *arg)
{
    (void)arg;
    xSemaphoreTake(s_lock, portMAX_DELAY);
    const bool present = s_st.present;
    xSemaphoreGive(s_lock);
    if (!present) {
        /* Still on the factory firmware, or not up yet. */
        send("HELLO");
    }
}

/* ---- Streaming -------------------------------------------------------------
 *
 * While headphones are connected each block the DAC gets goes to the ESP32
 * as "PCM <crc> <base64>": STREAM_BLOCK samples at the link's fixed
 * STREAM_RATE, ~700 bytes every 11.6 ms -- about two thirds of the link at
 * 921600 baud. The CRC lets the ESP32 drop a line damaged on the wire
 * instead of playing it as noise. The wired output is muted meanwhile, so
 * the same blocks going to the DAC are harmless.
 *
 * Peregrine's replies arrive at whatever rate its TTS produced, so they are
 * resampled here, linearly -- speech, through headphones, at 22 kHz. The
 * playback task is the only caller, so this state needs no lock. */
#define STREAM_RATE    22050      /* fixed by the co-processor (STREAM_RATE) */
#define STREAM_BLOCK   256
#define STREAM_GAP_US  (200 * 1000)   /* longer than this: a new sound */

static int16_t  s_blk[STREAM_BLOCK];
static size_t   s_blk_n;
static uint32_t s_rs_rate;        /* input rate of the current sound */
static uint32_t s_rs_pos;         /* Q16, next output's input position */
static int16_t  s_rs_prev;        /* the last input sample, as index -1 */
static int64_t  s_rs_last_us;

static void stream_flush(void)
{
    /* base64 of 512 bytes is 684 characters. */
    static char line[4 + 5 + 4 * ((STREAM_BLOCK * 2 + 2) / 3) + 1] = "PCM ";
    if (!s_blk_n) {
        return;
    }
    const size_t bytes = s_blk_n * sizeof(int16_t);
    snprintf(line + 4, 6, "%04x ",
             esp_rom_crc16_le(0, (const uint8_t *)s_blk, bytes));
    size_t out = 0;
    if (mbedtls_base64_encode((unsigned char *)line + 9, sizeof(line) - 9,
                              &out, (const unsigned char *)s_blk, bytes) == 0) {
        tiercel_board_coproc_send(line, 9 + out);
    }
    s_blk_n = 0;
}

static void stream_put(int16_t v)
{
    s_blk[s_blk_n++] = v;
    if (s_blk_n == STREAM_BLOCK) {
        stream_flush();
    }
}

static void stream_tap(const int16_t *pcm, size_t n, uint32_t rate, void *ctx)
{
    (void)ctx;
    if (!n || !tiercel_bt_connected()) {
        s_blk_n = 0;
        return;
    }
    const int64_t now = esp_timer_get_time();
    if (rate != s_rs_rate || now - s_rs_last_us > STREAM_GAP_US) {
        /* A new sound: send what is left of the last, start clean. */
        stream_flush();
        s_rs_rate = rate;
        s_rs_pos = 0;
        s_rs_prev = pcm[0];
    }
    s_rs_last_us = now;

    /* Position p (Q16) in input samples, where integer k is pcm[k] and -1
     * is the previous block's last sample. Interpolate between floor(p) and
     * the next, while both exist in this block. */
    const uint32_t step = (uint32_t)(((uint64_t)rate << 16) / STREAM_RATE);
    const uint32_t end = (uint32_t)(n - 1) << 16;
    uint32_t p = s_rs_pos;
    while (p < end + 65536u) {
        const int32_t i = (int32_t)(p >> 16) - 1;          /* -1 .. n-2 */
        const int32_t a = i < 0 ? s_rs_prev : pcm[i];
        const int32_t b = pcm[i + 1];
        const int32_t f = (int32_t)(p & 0xFFFF);
        stream_put((int16_t)(a + (((b - a) * f) >> 16)));
        p += step;
    }
    s_rs_pos = p - ((uint32_t)n << 16);
    s_rs_prev = pcm[n - 1];
}

/* ---- Public ----------------------------------------------------------------- */

void tiercel_bt_init(void)
{
    s_lock = xSemaphoreCreateMutex();
    if (tiercel_board_coproc_start(on_line, NULL) != ESP_OK) {
        return;
    }
    send("HELLO");
    tiercel_audio_set_tap(stream_tap, NULL);
    const esp_timer_create_args_t args = { .callback = hello_cb,
                                           .name = "bt_hello" };
    esp_timer_handle_t t;
    if (esp_timer_create(&args, &t) == ESP_OK) {
        esp_timer_start_periodic(t, HELLO_US);
    }
}

void tiercel_bt_get(tiercel_bt_state_t *out)
{
    if (!s_lock) {
        memset(out, 0, sizeof(*out));
        return;
    }
    xSemaphoreTake(s_lock, portMAX_DELAY);
    *out = s_st;
    xSemaphoreGive(s_lock);
}

bool tiercel_bt_home(const tiercel_bt_state_t *s, tiercel_bt_dev_t *out)
{
    if (s->conn != TIERCEL_BT_IDLE) {
        *out = s->peer;
        return true;
    }
    if (s->has_saved) {
        *out = s->saved;
        return true;
    }
    return false;
}

bool tiercel_bt_row(const tiercel_bt_state_t *s, int i, tiercel_bt_dev_t *out)
{
    tiercel_bt_dev_t home;
    const bool has_home = tiercel_bt_home(s, &home);
    if (has_home) {
        if (i == 0) {
            *out = home;
            return true;
        }
        i--;
    }
    for (int k = 0; k < s->ndev; k++) {
        if (has_home && strcmp(s->dev[k].addr, home.addr) == 0) {
            continue;                    /* already row 0 */
        }
        if (i-- == 0) {
            *out = s->dev[k];
            return true;
        }
    }
    return false;
}

void tiercel_bt_search(void)
{
    send("SCAN");
}

void tiercel_bt_stop_search(void)
{
    send("STOP");
}

void tiercel_bt_connect(const char *addr)
{
    char line[32];
    snprintf(line, sizeof(line), "CONNECT %s", addr);
    xSemaphoreTake(s_lock, portMAX_DELAY);
    s_st.failed = false;
    s_st.gen++;
    xSemaphoreGive(s_lock);
    send(line);
}

void tiercel_bt_forget(void)
{
    send("FORGET");
}

bool tiercel_bt_connected(void)
{
    if (!s_lock) {
        return false;
    }
    xSemaphoreTake(s_lock, portMAX_DELAY);
    const bool c = s_st.present && s_st.conn == TIERCEL_BT_CONNECTED;
    xSemaphoreGive(s_lock);
    return c;
}
