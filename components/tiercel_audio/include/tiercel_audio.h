/*
 * tiercel_audio -- the PDM microphone and the PCM5100A headphone DAC.
 *
 * Ported from Fireside's raw-I2S audio path (the elecrow-10-inch-attempt
 * branch, main/audio.c), which is the one written against a bare DAC and a
 * PDM mic rather than a codec chip. What changed for this board:
 *
 *   - The DAC is reached through a CH445P switch whose select line is GPIO0.
 *     It is driven high (S3 owns the DAC) for as long as this firmware runs.
 *   - Standard I2S (Philips) framing, because the PCM5100A's FMT pin is tied
 *     low. Fireside's MSB-aligned slot config is right for its amp and wrong
 *     here.
 *   - No amp-enable GPIO and no squeal to pad against -- this is a line-level
 *     headphone output. The silence pad at the end of a stream stays anyway:
 *     it is what stops the last DMA buffer replaying as a click.
 *   - Live input and output levels, for the Peregrine screen's animations and
 *     its end-of-speech detection.
 *
 * The DAC's soft-mute line belongs to the board's OTHER MCU (ESP32 IO32). If
 * nothing is heard, check that the ESP32 runs its factory firmware or
 * Tiercel's coprocessor/ firmware, and that no Bluetooth headphones are
 * connected (they mute the jack), before looking here -- see
 * docs/bluetooth.md.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Microphone sample rate. Peregrine's Whisper reads any rate from the WAV
 *  header; 16 kHz is what it is tuned for and what keeps an upload small. */
#define TIERCEL_AUDIO_MIC_RATE 16000

esp_err_t tiercel_audio_init(void);

/* ---- capture ---------------------------------------------------------- */

/** Start recording into the PSRAM capture buffer (up to 12 s). */
bool tiercel_audio_mic_start(void);

/**
 * Stop recording. The buffer stays owned by this module and is valid until
 * the next tiercel_audio_mic_start().
 */
void tiercel_audio_mic_stop(const int16_t **pcm, size_t *bytes);

bool tiercel_audio_mic_active(void);

/** Bytes captured so far. */
size_t tiercel_audio_mic_bytes(void);

/** Smoothed input level, 0-100, roughly perceptual. 0 while not recording. */
uint8_t tiercel_audio_mic_level(void);

/* ---- playback --------------------------------------------------------- */

/** Open the DAC for a streamed reply at `rate_hz` (8-48 kHz). */
bool tiercel_audio_play_start(uint32_t rate_hz);
/** S16LE mono. Blocks while the DMA drains. */
bool tiercel_audio_play_write(const void *pcm, size_t bytes);
void tiercel_audio_play_end(void);

/** Smoothed output level, 0-100. 0 when nothing is playing. */
uint8_t tiercel_audio_out_level(void);

/** Output volume 0-100. Applied in software with a squared curve, so the
 *  knob's steps sound even rather than all crowding into the top fifth. */
void    tiercel_audio_set_volume(uint8_t pct);
uint8_t tiercel_audio_get_volume(void);

/**
 * A copy of every block played, after the volume is applied: S16 mono at
 * `rate_hz`, on the playback caller's task. Bluetooth (tiercel_bt) streams
 * these to connected headphones while the co-processor mutes the DAC. The
 * tap must not block for long -- the DAC is waiting behind it.
 */
typedef void (*tiercel_audio_tap_t)(const int16_t *pcm, size_t samples,
                                    uint32_t rate_hz, void *ctx);
void tiercel_audio_set_tap(tiercel_audio_tap_t tap, void *ctx);

#ifdef __cplusplus
}
#endif
