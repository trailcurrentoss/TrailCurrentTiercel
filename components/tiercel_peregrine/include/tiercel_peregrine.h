/*
 * tiercel_peregrine -- the voice client for Peregrine, TrailCurrent's
 * on-rig assistant.
 *
 * THE PROTOCOL IS PEREGRINE'S, NOT OURS
 *
 * One utterance is one HTTPS request to Peregrine's main listener, the same
 * one Overlook's chat uses (TrailCurrentPeregrine src/web_chat.py,
 * VoiceMixin; docs/voice-terminal.md). Trust is Peregrine's CA, exactly as
 * for the chat: there is no separate voice credential.
 *
 *   POST <url>/api/voice
 *   Content-Type:  audio/wav          Content-Length is REQUIRED (411 without)
 *   body:          a 44-byte WAV header + S16LE mono PCM
 *
 *   200  audio/wav, the reply -- a 44-byte header whose sample rate is the
 *        server's PEREGRINE_VOICE_TARGET_SR (16 kHz by default, NOT the
 *        22.05 kHz Piper speaks at), then PCM until the socket closes.
 *        X-Peregrine-Transcript: what it heard, URL-encoded.
 *        X-Peregrine-Response:   what it said, URL-encoded -- or the literal
 *                                "streaming" when the reply is an LLM
 *                                answer spoken sentence by sentence.
 *   411/413/400/503/500  {"error": "..."}
 *
 * There is no session, no wake word on the device and no control channel.
 * The client is a state machine around that one request; it is ported from
 * Fireside's peregrine_voice component and adds what a hands-free, buttonless
 * device needs:
 *
 *   - END OF SPEECH by level. Fireside is push-to-talk: the release of a
 *     finger ends the utterance. Tiercel has a tap to start and nothing to
 *     hold, so a short silence after speech ends it, and a tap ends it early.
 *   - CANCEL. A tap while thinking or speaking abandons the request.
 *   - TEXT. The transcript and (when the server sends it) the answer are
 *     kept for the screen, which the design shows under the orb.
 *
 * URL and CA come from Headwaters (tiercel_config services) -- the same pair
 * Headwaters uses to reach Peregrine itself; nothing is entered on the
 * device. An empty URL, or an https:// URL with no CA, leaves the client
 * NOT_SET_UP.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TIERCEL_PG_IDLE = 0,     /**< ready; nothing said yet this visit */
    TIERCEL_PG_LISTENING,    /**< recording */
    TIERCEL_PG_THINKING,     /**< uploaded, waiting for the reply */
    TIERCEL_PG_SPEAKING,     /**< reply playing */
    TIERCEL_PG_DONE,         /**< finished; transcript/answer on screen */
    TIERCEL_PG_ERROR,        /**< see tiercel_peregrine_status_t.error */
} tiercel_pg_phase_t;

#define TIERCEL_PG_TEXT_MAX 192

typedef struct {
    tiercel_pg_phase_t phase;
    bool     configured;                    /**< URL (and CA, for https) present */
    bool     heard_speech;                  /**< level crossed the speech floor */
    char     transcript[TIERCEL_PG_TEXT_MAX];
    char     answer[TIERCEL_PG_TEXT_MAX];   /**< "" when the reply streamed */
    char     error[64];                     /**< human-readable, for the hint */
    uint32_t phase_ms;                      /**< time in the current phase */
} tiercel_pg_status_t;

esp_err_t tiercel_peregrine_init(void);

/** From Headwaters' services config. The CA (PEM) is copied, so the caller's
 *  buffer can go once this returns. */
void tiercel_peregrine_configure(const char *url, const char *ca_pem);

/**
 * The orb was tapped. What it means depends on the phase, exactly as the
 * design's hint line says:
 *
 *   idle / done / error  start listening
 *   listening            stop and send (or cancel, if nothing was heard)
 *   thinking / speaking  cancel
 */
void tiercel_peregrine_tap(void);

/** Abandon whatever is in progress -- leaving the screen calls this. */
void tiercel_peregrine_cancel(void);

/** Back to IDLE and clear the text, e.g. on entering the screen. */
void tiercel_peregrine_reset(void);

void tiercel_peregrine_status(tiercel_pg_status_t *out);

/** Animation inputs, 0-100: the mic while listening, the reply while
 *  speaking, 0 otherwise. */
uint8_t tiercel_peregrine_level(void);

#ifdef __cplusplus
}
#endif
