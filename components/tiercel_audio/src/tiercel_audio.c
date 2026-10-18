/*
 * Microphone and DAC. See tiercel_audio.h.
 */

#include <math.h>
#include <string.h>

#include "driver/gpio.h"
#include "driver/i2s_pdm.h"
#include "driver/i2s_std.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "tiercel_audio.h"

static const char *TAG = "audio";

/* Board wiring -- the citations are in components/tiercel_board/src/
 * board_pins.h. Repeated here rather than shared because this component must
 * not depend on the board component's private header. */
#define PIN_ROUTE   GPIO_NUM_0     /* CH445P select: 1 = S3 owns the DAC */
#define PIN_BCLK    GPIO_NUM_39
#define PIN_WS      GPIO_NUM_40
#define PIN_DOUT    GPIO_NUM_41
#define PIN_PDM_CLK GPIO_NUM_45
#define PIN_PDM_DIN GPIO_NUM_46

#define SPK_PORT    I2S_NUM_1
#define MIC_PORT    I2S_NUM_0       /* PDM RX exists on I2S0 only */

/* Fireside found the PDM mic quiet at unity; 4x put speech at a level
 * Whisper transcribes reliably without clipping a raised voice. */
#define MIC_SW_GAIN      4
#define MIC_MAX_SECONDS  12
#define MIC_MAX_BYTES    ((size_t)TIERCEL_AUDIO_MIC_RATE * 2 * MIC_MAX_SECONDS)
#define MIC_CHUNK_BYTES  1024      /* 32 ms -- the level meter's frame */

/* I2S DMA cannot read PSRAM, so a reply is scaled into internal RAM in
 * slices on its way to the DAC. */
#define PLAY_SCRATCH_SAMPLES 1024

static i2s_chan_handle_t s_tx;
static i2s_chan_handle_t s_rx;
static SemaphoreHandle_t s_lock;

static int16_t      *s_mic_buf;
static size_t        s_mic_len;
static TaskHandle_t  s_mic_task;
static volatile bool s_mic_stop;
static volatile uint8_t s_mic_level;

static volatile bool    s_playing;
static volatile uint8_t s_out_level;
static uint8_t          s_volume = 60;
static int16_t          s_scratch[PLAY_SCRATCH_SAMPLES];

/* RMS of a block of samples, mapped onto 0-100 on a log scale: -50 dBFS is
 * silence on the meter, 0 dBFS is full. Speech sits around 40-75. */
static uint8_t level_of(const int16_t *s, size_t n)
{
    if (!n) {
        return 0;
    }
    double acc = 0;
    for (size_t i = 0; i < n; i++) {
        acc += (double)s[i] * s[i];
    }
    const double rms = sqrt(acc / n) / 32768.0;
    if (rms <= 0.00316) {           /* -50 dBFS */
        return 0;
    }
    double db = 20.0 * log10(rms);  /* -50..0 */
    int v = (int)lround((db + 50.0) * 2.0);
    return (uint8_t)(v < 0 ? 0 : v > 100 ? 100 : v);
}

/* Fast attack, slow release -- what makes a meter read as a voice rather
 * than as noise. */
static uint8_t smooth(uint8_t prev, uint8_t now)
{
    return now > prev ? now : (uint8_t)((prev * 3 + now) / 4);
}

/* ---------------------------------------------------------------------- */

static esp_err_t speaker_init(void)
{
    i2s_chan_config_t chan = I2S_CHANNEL_DEFAULT_CONFIG(SPK_PORT, I2S_ROLE_MASTER);
    chan.auto_clear_after_cb = true;     /* underrun plays silence, not loops */
    ESP_RETURN_ON_ERROR(i2s_new_channel(&chan, &s_tx, NULL), TAG, "tx chan");

    const i2s_std_config_t std = {
        .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(TIERCEL_AUDIO_MIC_RATE),
        /* Philips framing: the PCM5100A's FMT pin is tied to GND. Mono writes
         * the same sample to both slots, so both ears hear it. */
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT,
                                                        I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,     /* DAC SCK tied low: PLL from BCK */
            .bclk = PIN_BCLK,
            .ws   = PIN_WS,
            .dout = PIN_DOUT,
            .din  = I2S_GPIO_UNUSED,
        },
    };
    ESP_RETURN_ON_ERROR(i2s_channel_init_std_mode(s_tx, &std), TAG, "tx std");
    return ESP_OK;
}

static esp_err_t mic_init(void)
{
    i2s_chan_config_t chan = I2S_CHANNEL_DEFAULT_CONFIG(MIC_PORT, I2S_ROLE_MASTER);
    chan.dma_desc_num  = 6;
    chan.dma_frame_num = 256;
    ESP_RETURN_ON_ERROR(i2s_new_channel(&chan, NULL, &s_rx), TAG, "rx chan");

    i2s_pdm_rx_config_t pdm = {
        .clk_cfg  = I2S_PDM_RX_CLK_DEFAULT_CONFIG(TIERCEL_AUDIO_MIC_RATE),
        .slot_cfg = I2S_PDM_RX_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT,
                                                   I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .clk = PIN_PDM_CLK,
            .din = PIN_PDM_DIN,
        },
    };
    pdm.slot_cfg.slot_mask = I2S_PDM_SLOT_LEFT;   /* mic L/R tied to GND */
    ESP_RETURN_ON_ERROR(i2s_channel_init_pdm_rx_mode(s_rx, &pdm), TAG, "pdm rx");
    return ESP_OK;
}

esp_err_t tiercel_audio_init(void)
{
    /*
     * Claim the DAC. GPIO0 is high at reset through its pull-up, so this only
     * makes it stick: left as an input, anything that later touched the BOOT
     * strap's net would hand the DAC to the other MCU mid-reply.
     */
    const gpio_config_t route = {
        .pin_bit_mask = 1ULL << PIN_ROUTE,
        .mode = GPIO_MODE_OUTPUT,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&route), TAG, "route gpio");
    gpio_set_level(PIN_ROUTE, 1);

    s_lock = xSemaphoreCreateMutex();
    ESP_RETURN_ON_FALSE(s_lock, ESP_ERR_NO_MEM, TAG, "mutex");

    s_mic_buf = heap_caps_malloc(MIC_MAX_BYTES, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    ESP_RETURN_ON_FALSE(s_mic_buf, ESP_ERR_NO_MEM, TAG, "mic buffer (%u B PSRAM)",
                        (unsigned)MIC_MAX_BYTES);

    ESP_RETURN_ON_ERROR(speaker_init(), TAG, "speaker");
    ESP_RETURN_ON_ERROR(mic_init(), TAG, "mic");
    ESP_LOGI(TAG, "PCM5100A on BCK %d WS %d DOUT %d; PDM mic on CLK %d DIN %d "
                  "@ %d Hz", PIN_BCLK, PIN_WS, PIN_DOUT, PIN_PDM_CLK,
             PIN_PDM_DIN, TIERCEL_AUDIO_MIC_RATE);
    return ESP_OK;
}

/* ---- capture ---------------------------------------------------------- */

static void mic_task(void *arg)
{
    (void)arg;
    static int16_t chunk[MIC_CHUNK_BYTES / 2];
    while (!s_mic_stop) {
        size_t got = 0;
        if (i2s_channel_read(s_rx, chunk, sizeof(chunk), &got,
                             pdMS_TO_TICKS(200)) != ESP_OK || got == 0) {
            continue;
        }
        const size_t n = got / 2;
        for (size_t i = 0; i < n; i++) {
            int32_t v = (int32_t)chunk[i] * MIC_SW_GAIN;
            chunk[i] = (int16_t)(v > 32767 ? 32767 : v < -32768 ? -32768 : v);
        }
        s_mic_level = smooth(s_mic_level, level_of(chunk, n));

        xSemaphoreTake(s_lock, portMAX_DELAY);
        const size_t room = MIC_MAX_BYTES - s_mic_len;
        const size_t take = got < room ? got : room;
        memcpy((uint8_t *)s_mic_buf + s_mic_len, chunk, take);
        s_mic_len += take;
        xSemaphoreGive(s_lock);
        if (take < got) {
            ESP_LOGW(TAG, "capture hit its %d s cap", MIC_MAX_SECONDS);
            break;
        }
    }
    s_mic_level = 0;
    s_mic_task = NULL;
    vTaskDelete(NULL);
}

bool tiercel_audio_mic_start(void)
{
    if (!s_rx || s_mic_task) {
        return false;
    }
    xSemaphoreTake(s_lock, portMAX_DELAY);
    s_mic_len = 0;
    xSemaphoreGive(s_lock);
    s_mic_level = 0;
    s_mic_stop = false;
    if (i2s_channel_enable(s_rx) != ESP_OK) {
        ESP_LOGE(TAG, "mic enable failed");
        return false;
    }
    if (xTaskCreatePinnedToCore(mic_task, "mic", 4096, NULL, 6, &s_mic_task, 1)
            != pdPASS) {
        i2s_channel_disable(s_rx);
        s_mic_task = NULL;
        return false;
    }
    return true;
}

void tiercel_audio_mic_stop(const int16_t **pcm, size_t *bytes)
{
    s_mic_stop = true;
    for (int i = 0; i < 100 && s_mic_task; i++) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    if (s_rx) {
        i2s_channel_disable(s_rx);
    }
    xSemaphoreTake(s_lock, portMAX_DELAY);
    if (pcm) {
        *pcm = s_mic_buf;
    }
    if (bytes) {
        *bytes = s_mic_len;
    }
    xSemaphoreGive(s_lock);
}

bool    tiercel_audio_mic_active(void) { return s_mic_task != NULL; }
uint8_t tiercel_audio_mic_level(void)  { return s_mic_level; }

size_t tiercel_audio_mic_bytes(void)
{
    xSemaphoreTake(s_lock, portMAX_DELAY);
    const size_t n = s_mic_len;
    xSemaphoreGive(s_lock);
    return n;
}

/* ---- playback --------------------------------------------------------- */

static uint32_t            s_rate;
static tiercel_audio_tap_t s_tap;
static void               *s_tap_ctx;

void tiercel_audio_set_tap(tiercel_audio_tap_t tap, void *ctx)
{
    s_tap_ctx = ctx;
    s_tap = tap;
}

static void set_rate(uint32_t hz)
{
    const i2s_std_clk_config_t clk = I2S_STD_CLK_DEFAULT_CONFIG(hz);
    if (i2s_channel_reconfig_std_clock(s_tx, &clk) != ESP_OK) {
        ESP_LOGE(TAG, "clock %lu Hz rejected", (unsigned long)hz);
    }
}

bool tiercel_audio_play_start(uint32_t rate_hz)
{
    if (!s_tx || s_playing) {
        return false;
    }
    if (rate_hz < 8000 || rate_hz > 48000) {
        ESP_LOGW(TAG, "reply rate %lu Hz implausible -- using %d",
                 (unsigned long)rate_hz, TIERCEL_AUDIO_MIC_RATE);
        rate_hz = TIERCEL_AUDIO_MIC_RATE;
    }
    set_rate(rate_hz);
    s_rate = rate_hz;
    if (i2s_channel_enable(s_tx) != ESP_OK) {
        return false;
    }
    s_playing = true;
    ESP_LOGI(TAG, "playback @ %lu Hz, volume %u", (unsigned long)rate_hz,
             (unsigned)s_volume);
    return true;
}

bool tiercel_audio_play_write(const void *pcm, size_t bytes)
{
    if (!s_playing || !pcm) {
        return false;
    }
    /* Squared curve: 50 on the knob is a quarter of full scale, which is
     * about where "half as loud" sits to the ear. */
    const int32_t gain = (int32_t)s_volume * s_volume;     /* 0..10000 */
    const int16_t *src = pcm;
    size_t left = bytes / 2;
    while (left) {
        const size_t n = left < PLAY_SCRATCH_SAMPLES ? left : PLAY_SCRATCH_SAMPLES;
        for (size_t i = 0; i < n; i++) {
            s_scratch[i] = (int16_t)(((int32_t)src[i] * gain) / 10000);
        }
        s_out_level = smooth(s_out_level, level_of(src, n));
        if (s_tap) {
            s_tap(s_scratch, n, s_rate, s_tap_ctx);
        }
        size_t written = 0;
        if (i2s_channel_write(s_tx, s_scratch, n * 2, &written,
                              pdMS_TO_TICKS(2000)) != ESP_OK) {
            return false;
        }
        src  += n;
        left -= n;
    }
    return true;
}

void tiercel_audio_play_end(void)
{
    if (!s_playing) {
        return;
    }
    /* 100 ms of silence so the last DMA buffer is not replayed as a click
     * when the channel stops. */
    memset(s_scratch, 0, sizeof(s_scratch));
    size_t written = 0;
    for (int i = 0; i < 2; i++) {
        i2s_channel_write(s_tx, s_scratch, sizeof(s_scratch), &written,
                          pdMS_TO_TICKS(300));
    }
    i2s_channel_disable(s_tx);
    s_playing = false;
    s_out_level = 0;
}

uint8_t tiercel_audio_out_level(void) { return s_playing ? s_out_level : 0; }

void tiercel_audio_set_volume(uint8_t pct) { s_volume = pct > 100 ? 100 : pct; }
uint8_t tiercel_audio_get_volume(void)      { return s_volume; }
