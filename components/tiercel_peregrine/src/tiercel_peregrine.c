/*
 * Peregrine voice client. See tiercel_peregrine.h for the protocol.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "esp_heap_caps.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "tiercel_audio.h"
#include "tiercel_peregrine.h"

static const char *TAG = "peregrine";

#define PATH_VOICE       "/api/voice"
#define WAV_HDR_LEN      44
#define RESP_CHUNK       4096
#define HTTP_TIMEOUT_MS  60000

/*
 * End-of-speech detection, by the mic's smoothed level (tiercel_audio).
 *
 * SPEECH_LEVEL is the floor a voice at arm's length clears and room noise in
 * a quiet coach does not; a fan or a running generator can sit above it, in
 * which case the utterance simply runs until MAX_LISTEN or a tap -- slower,
 * never wrong. Measured levels are logged at the end of every capture so the
 * thresholds can be tuned from the bench.
 */
#define SPEECH_LEVEL      38
#define TRAILING_QUIET_MS 900      /* silence after speech that ends it */
#define NO_SPEECH_MS      6000     /* nothing heard at all: give up */
#define MAX_LISTEN_MS     11000    /* under the 12 s capture buffer */
#define MIN_SPEECH_BYTES  (TIERCEL_AUDIO_MIC_RATE * 2 / 3)   /* ~0.33 s */

typedef enum { JOB_NONE = 0, JOB_SEND } job_t;

static SemaphoreHandle_t   s_lock;
static TaskHandle_t        s_task;
static tiercel_pg_status_t s_st;
static int64_t             s_phase_at;
static int64_t             s_last_loud;
static uint8_t             s_peak;
static volatile bool       s_cancel;
static volatile bool       s_stop_listen;
static char                s_url[129];
static char               *s_ca;          /* PSRAM, NULL when none */

#define LOCK()   xSemaphoreTake(s_lock, portMAX_DELAY)
#define UNLOCK() xSemaphoreGive(s_lock)

static void set_phase_locked(tiercel_pg_phase_t p)
{
    s_st.phase = p;
    s_phase_at = esp_timer_get_time();
}

static void set_phase(tiercel_pg_phase_t p)
{
    LOCK();
    set_phase_locked(p);
    UNLOCK();
}

static void fail(const char *why)
{
    LOCK();
    snprintf(s_st.error, sizeof(s_st.error), "%s", why);
    set_phase_locked(TIERCEL_PG_ERROR);
    UNLOCK();
    ESP_LOGW(TAG, "%s", why);
}

/* In-place %XX and '+' decoding for the X-Peregrine-* headers. */
static void url_decode(char *s)
{
    char *o = s;
    for (char *p = s; *p; p++) {
        if (*p == '%' && isxdigit((unsigned char)p[1]) && isxdigit((unsigned char)p[2])) {
            char hex[3] = { p[1], p[2], 0 };
            *o++ = (char)strtol(hex, NULL, 16);
            p += 2;
        } else if (*p == '+') {
            *o++ = ' ';
        } else {
            *o++ = *p;
        }
    }
    *o = '\0';
}

/*
 * LVGL's Roboto subset is Latin-1 plus a few dashes; anything else renders as
 * an empty box. Whisper and the LLM both like curly quotes and em dashes, so
 * they are folded to ASCII here, before the text ever reaches a label.
 */
static void fold_to_ascii(char *s)
{
    char *o = s;
    for (unsigned char *p = (unsigned char *)s; *p; ) {
        if (p[0] == 0xE2 && p[1] == 0x80) {
            switch (p[2]) {
            case 0x98: case 0x99: *o++ = '\''; p += 3; continue;
            case 0x9C: case 0x9D: *o++ = '"';  p += 3; continue;
            case 0x93: case 0x94: *o++ = '-';  p += 3; continue;
            case 0xA6: *o++ = '.'; *o++ = '.'; *o++ = '.'; p += 3; continue;
            default: break;
            }
        }
        *o++ = (char)*p++;
    }
    *o = '\0';
}

static esp_err_t http_event(esp_http_client_event_t *e)
{
    if (e->event_id != HTTP_EVENT_ON_HEADER || !e->header_key || !e->header_value) {
        return ESP_OK;
    }
    char *dst = NULL;
    if (strcasecmp(e->header_key, "X-Peregrine-Transcript") == 0) {
        dst = s_st.transcript;
    } else if (strcasecmp(e->header_key, "X-Peregrine-Response") == 0) {
        /* "streaming" is a placeholder, not an answer: the reply is being
         * spoken as the LLM writes it and there is no text to show. */
        if (strcmp(e->header_value, "streaming") == 0) {
            return ESP_OK;
        }
        dst = s_st.answer;
    }
    if (dst) {
        LOCK();
        snprintf(dst, TIERCEL_PG_TEXT_MAX, "%s", e->header_value);
        url_decode(dst);
        fold_to_ascii(dst);
        UNLOCK();
    }
    return ESP_OK;
}

static void wav_header(uint8_t *o, uint32_t pcm_bytes, uint32_t rate)
{
    const uint32_t riff = 36 + pcm_bytes, byte_rate = rate * 2;
    memcpy(o, "RIFF", 4);
    o[4] = riff; o[5] = riff >> 8; o[6] = riff >> 16; o[7] = riff >> 24;
    memcpy(o + 8, "WAVEfmt ", 8);
    o[16] = 16; o[17] = o[18] = o[19] = 0;     /* fmt chunk size */
    o[20] = 1;  o[21] = 0;                      /* PCM */
    o[22] = 1;  o[23] = 0;                      /* mono */
    o[24] = rate; o[25] = rate >> 8; o[26] = rate >> 16; o[27] = rate >> 24;
    o[28] = byte_rate; o[29] = byte_rate >> 8;
    o[30] = byte_rate >> 16; o[31] = byte_rate >> 24;
    o[32] = 2;  o[33] = 0;                      /* block align */
    o[34] = 16; o[35] = 0;                      /* bits */
    memcpy(o + 36, "data", 4);
    o[40] = pcm_bytes; o[41] = pcm_bytes >> 8;
    o[42] = pcm_bytes >> 16; o[43] = pcm_bytes >> 24;
}

/* One request: upload the capture, play the reply. Runs on the worker. */
static void send_and_play(const int16_t *pcm, size_t bytes)
{
    char url[160];
    /* A private copy of the CA for the life of this request: Headwaters can
     * republish (and configure() free the old one) mid-request. */
    char *ca = NULL;
    bool have_ca;
    LOCK();
    snprintf(url, sizeof(url), "%s%s", s_url, PATH_VOICE);
    have_ca = s_ca != NULL;
    if (have_ca) {
        const size_t n = strlen(s_ca) + 1;
        ca = heap_caps_malloc(n, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (ca) {
            memcpy(ca, s_ca, n);
        }
    }
    UNLOCK();
    if (have_ca && !ca) {
        fail("Out of memory");
        return;
    }
    /* sdkconfig has CONFIG_ESP_TLS_SKIP_SERVER_CERT_VERIFY for the broker's
     * self-signed certificate, and with it esp-tls connects to an https://
     * URL given no CA WITHOUT verifying anything, and says nothing. So an
     * https:// request never goes out without one, whatever `configured`
     * said when the tap was taken -- the CA can be cleared mid-utterance. */
    if (strncmp(url, "https://", 8) == 0 && !ca) {
        fail("Peregrine is not set up");
        return;
    }

    /* For https:// the server certificate is verified against Peregrine's
     * CA, including its name, which is why the URL keeps the hostname
     * (peregrine.local) rather than an address. mbedTLS allocates in PSRAM
     * (CONFIG_MBEDTLS_EXTERNAL_MEM_ALLOC), so this session sits beside the
     * broker's without crowding internal RAM. */
    const esp_http_client_config_t cfg = {
        .url            = url,
        .method         = HTTP_METHOD_POST,
        .timeout_ms     = HTTP_TIMEOUT_MS,
        .buffer_size    = RESP_CHUNK,
        .buffer_size_tx = 1024,
        .event_handler  = http_event,
        .cert_pem       = ca,
    };
    esp_http_client_handle_t c = esp_http_client_init(&cfg);
    if (!c) {
        heap_caps_free(ca);
        fail("Out of memory");
        return;
    }
    esp_http_client_set_header(c, "Content-Type", "audio/wav");

    const int64_t t0 = esp_timer_get_time();
    const esp_err_t oerr = esp_http_client_open(c, (int)(WAV_HDR_LEN + bytes));
    if (oerr != ESP_OK) {
        esp_http_client_cleanup(c);
        heap_caps_free(ca);
        /* A TLS verification failure lands here too: logged with the reason,
         * shown as unreachable. */
        ESP_LOGW(TAG, "open %s: %s (%u B stack free)", url, esp_err_to_name(oerr),
                 (unsigned)uxTaskGetStackHighWaterMark(NULL));
        fail("Can't reach Peregrine");
        return;
    }

    uint8_t hdr[WAV_HDR_LEN];
    wav_header(hdr, (uint32_t)bytes, TIERCEL_AUDIO_MIC_RATE);
    bool ok = esp_http_client_write(c, (const char *)hdr, WAV_HDR_LEN) == WAV_HDR_LEN;
    for (size_t off = 0; ok && off < bytes && !s_cancel; ) {
        const size_t take = bytes - off > 8192 ? 8192 : bytes - off;
        const int w = esp_http_client_write(c, (const char *)pcm + off, take);
        ok = w > 0;
        off += w > 0 ? (size_t)w : 0;
    }
    if (!ok || s_cancel) {
        esp_http_client_close(c);
        esp_http_client_cleanup(c);
        heap_caps_free(ca);
        if (!s_cancel) {
            fail("Upload failed");
        }
        return;
    }

    esp_http_client_fetch_headers(c);
    const int status = esp_http_client_get_status_code(c);
    if (status != 200) {
        esp_http_client_close(c);
        esp_http_client_cleanup(c);
        heap_caps_free(ca);
        ESP_LOGW(TAG, "HTTP %d", status);
        /* 404: a Peregrine from before /api/voice moved onto its HTTPS
         * listener -- it needs a deploy, not a setting. */
        fail(status == 404 ? "Peregrine needs updating"
           : status == 503 ? "Peregrine is busy"
                           : "Peregrine had a problem");
        return;
    }

    /*
     * Stream the reply. The first 44 bytes are its WAV header, and the rate
     * in it drives the DAC clock: the server resamples to its own target
     * rate, and playing it at our mic rate pitched the voice down four
     * semitones on Fireside before this was read instead of assumed.
     *
     * The body ends when the server closes the socket. esp_http_client reports
     * that as a short or negative read, and for a streamed reply with no
     * Content-Length that IS the normal end, not an error.
     */
    static uint8_t chunk[RESP_CHUNK];
    uint8_t rhdr[WAV_HDR_LEN];
    size_t  have_hdr = 0, body = 0;
    bool    playing = false;
    while (!s_cancel) {
        const int n = esp_http_client_read(c, (char *)chunk, sizeof(chunk));
        if (n <= 0) {
            break;
        }
        body += (size_t)n;
        size_t off = 0;
        if (have_hdr < WAV_HDR_LEN) {
            const size_t take = WAV_HDR_LEN - have_hdr < (size_t)n
                              ? WAV_HDR_LEN - have_hdr : (size_t)n;
            memcpy(rhdr + have_hdr, chunk, take);
            have_hdr += take;
            off = take;
            if (have_hdr == WAV_HDR_LEN) {
                const uint32_t rate = rhdr[24] | rhdr[25] << 8 |
                                      rhdr[26] << 16 | (uint32_t)rhdr[27] << 24;
                playing = tiercel_audio_play_start(rate);
                set_phase(TIERCEL_PG_SPEAKING);
            }
        }
        if (playing && off < (size_t)n) {
            tiercel_audio_play_write(chunk + off, (size_t)n - off);
        }
    }
    if (playing) {
        tiercel_audio_play_end();
    }
    esp_http_client_close(c);
    esp_http_client_cleanup(c);
    heap_caps_free(ca);

    /* Stack margin too: this task's 8 KB now carries a TLS handshake. */
    ESP_LOGI(TAG, "round trip %lld ms, %u B up, %u B reply%s, %u B stack free",
             (esp_timer_get_time() - t0) / 1000, (unsigned)bytes,
             (unsigned)body, s_cancel ? " (cancelled)" : "",
             (unsigned)uxTaskGetStackHighWaterMark(NULL));
    if (!s_cancel) {
        set_phase(TIERCEL_PG_DONE);
    }
}

/*
 * The worker owns the whole utterance: it listens, decides when speech has
 * ended, sends, and plays. Doing the end-of-speech check here rather than on
 * the UI timer keeps it running at the mic's own pace and off the LVGL task.
 */
static void worker(void *arg)
{
    (void)arg;
    while (true) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        s_cancel = false;
        s_stop_listen = false;

        if (!tiercel_audio_mic_start()) {
            fail("Microphone unavailable");
            continue;
        }
        const int64_t start = esp_timer_get_time();
        s_last_loud = start;
        s_peak = 0;
        bool speech = false;
        while (!s_cancel && !s_stop_listen) {
            vTaskDelay(pdMS_TO_TICKS(40));
            const int64_t now = esp_timer_get_time();
            const uint8_t lvl = tiercel_audio_mic_level();
            if (lvl > s_peak) {
                s_peak = lvl;
            }
            if (lvl >= SPEECH_LEVEL) {
                s_last_loud = now;
                if (!speech) {
                    speech = true;
                    LOCK();
                    s_st.heard_speech = true;
                    UNLOCK();
                }
            }
            const int64_t since = (now - start) / 1000;
            if ((speech && (now - s_last_loud) / 1000 > TRAILING_QUIET_MS) ||
                (!speech && since > NO_SPEECH_MS) ||
                since > MAX_LISTEN_MS || !tiercel_audio_mic_active()) {
                break;
            }
        }
        const int16_t *pcm = NULL;
        size_t bytes = 0;
        tiercel_audio_mic_stop(&pcm, &bytes);
        ESP_LOGI(TAG, "captured %u B (%.1f s), peak level %u, speech %s",
                 (unsigned)bytes, bytes / (TIERCEL_AUDIO_MIC_RATE * 2.0),
                 (unsigned)s_peak, speech ? "yes" : "no");

        if (s_cancel) {
            continue;                   /* phase already set by cancel() */
        }
        if (!speech || bytes < MIN_SPEECH_BYTES) {
            fail("Didn't hear anything");
            continue;
        }
        set_phase(TIERCEL_PG_THINKING);
        send_and_play(pcm, bytes);
    }
}

esp_err_t tiercel_peregrine_init(void)
{
    s_lock = xSemaphoreCreateMutex();
    if (!s_lock) {
        return ESP_ERR_NO_MEM;
    }
    set_phase(TIERCEL_PG_IDLE);
    /* Core 0, with Wi-Fi: the request is network-bound, and keeping it off
     * core 1 leaves LVGL a whole core to animate the orb while it waits. */
    if (xTaskCreatePinnedToCore(worker, "peregrine", 8192, NULL, 5, &s_task, 0)
            != pdPASS) {
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

void tiercel_peregrine_configure(const char *url, const char *ca_pem)
{
    char *ca = NULL;
    if (ca_pem && ca_pem[0]) {
        const size_t n = strlen(ca_pem) + 1;
        ca = heap_caps_malloc(n, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        if (ca) {
            memcpy(ca, ca_pem, n);
        } else {
            ESP_LOGE(TAG, "no memory for the Peregrine CA");
        }
    }
    LOCK();
    snprintf(s_url, sizeof(s_url), "%s", url ? url : "");
    char *old = s_ca;
    s_ca = ca;
    const bool https = strncmp(s_url, "https://", 8) == 0;
    s_st.configured = s_url[0] && (!https || s_ca);
    const bool configured = s_st.configured;
    UNLOCK();
    heap_caps_free(old);
    ESP_LOGI(TAG, "configured: %s%s", s_url[0] ? s_url : "<unset>",
             configured ? (https ? " (CA)" : "") : " -- not set up");
}

void tiercel_peregrine_tap(void)
{
    LOCK();
    const tiercel_pg_phase_t p = s_st.phase;
    const bool configured = s_st.configured;
    const bool heard = s_st.heard_speech;
    UNLOCK();

    switch (p) {
    case TIERCEL_PG_IDLE:
    case TIERCEL_PG_DONE:
    case TIERCEL_PG_ERROR:
        if (!configured) {
            fail("Peregrine is not set up");
            return;
        }
        LOCK();
        s_st.transcript[0] = s_st.answer[0] = s_st.error[0] = '\0';
        s_st.heard_speech = false;
        set_phase_locked(TIERCEL_PG_LISTENING);
        UNLOCK();
        xTaskNotifyGive(s_task);
        return;
    case TIERCEL_PG_LISTENING:
        if (heard) {
            s_stop_listen = true;       /* send what we have now */
        } else {
            tiercel_peregrine_cancel();
        }
        return;
    case TIERCEL_PG_THINKING:
    case TIERCEL_PG_SPEAKING:
        tiercel_peregrine_cancel();
        return;
    }
}

void tiercel_peregrine_cancel(void)
{
    LOCK();
    const tiercel_pg_phase_t p = s_st.phase;
    if (p == TIERCEL_PG_LISTENING) {
        set_phase_locked(TIERCEL_PG_IDLE);
    } else if (p == TIERCEL_PG_THINKING || p == TIERCEL_PG_SPEAKING) {
        set_phase_locked(TIERCEL_PG_DONE);
    }
    UNLOCK();
    s_cancel = true;
}

void tiercel_peregrine_reset(void)
{
    LOCK();
    const tiercel_pg_phase_t p = s_st.phase;
    if (p == TIERCEL_PG_DONE || p == TIERCEL_PG_ERROR) {
        s_st.transcript[0] = s_st.answer[0] = s_st.error[0] = '\0';
        set_phase_locked(TIERCEL_PG_IDLE);
    }
    UNLOCK();
}

void tiercel_peregrine_status(tiercel_pg_status_t *out)
{
    if (!out) {
        return;
    }
    LOCK();
    *out = s_st;
    out->phase_ms = (uint32_t)((esp_timer_get_time() - s_phase_at) / 1000);
    UNLOCK();
}

uint8_t tiercel_peregrine_level(void)
{
    switch (s_st.phase) {
    case TIERCEL_PG_LISTENING: return tiercel_audio_mic_level();
    case TIERCEL_PG_SPEAKING:  return tiercel_audio_out_level();
    default:                   return 0;
    }
}
