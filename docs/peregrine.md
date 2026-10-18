# Peregrine (voice)

Peregrine is TrailCurrent's on-rig voice assistant. Tiercel is a voice
**terminal** for it: it records, Peregrine transcribes, thinks and speaks, and
Tiercel plays the reply. The client is `components/tiercel_peregrine`, ported
from Fireside's `peregrine_voice` component; the screen is `main/ui_peregrine.c`.

## Protocol

One utterance is one HTTPS request to Peregrine's main listener, the same
one Overlook's chat uses. This is Peregrine's protocol (`src/web_chat.py`,
`VoiceMixin`; `docs/voice-terminal.md` in TrailCurrentPeregrine), not
something Tiercel defines. Trust is Peregrine's CA, exactly as for the chat:
there is no separate voice credential.

```
POST <url>/api/voice
Content-Type:  audio/wav
Content-Length: <44 + PCM bytes>       -- required; chunked uploads get 411

<44-byte WAV header, 16 kHz mono S16LE><PCM>
```

| Response | |
|---|---|
| `200 audio/wav` | the reply. A 44-byte WAV header, then PCM until the socket closes. |
| `X-Peregrine-Transcript` | what it heard, URL-encoded |
| `X-Peregrine-Response` | what it said, URL-encoded, or the literal `streaming` when an LLM answer is spoken sentence by sentence (no text to show) |
| `411 / 413 / 400 / 503 / 500` | `{"error": "..."}`, shown as a one-line error under the orb |
| `404` | a Peregrine too old to serve voice on HTTPS; shown as **Peregrine needs updating** |

The reply's sample rate is **read from its header**, never assumed. The
server resamples to its `PEREGRINE_VOICE_TARGET_SR` (16 kHz by default), not
Piper's native 22.05 kHz, and playing at the wrong rate shifts the voice by
several semitones.

There is no session, no control channel, and no wake word on the wire.

## Configuration

The URL and Peregrine's CA come from Headwaters, on the retained
`local/config/tiercel/<hostname>/services` topic (see [mqtt.md](mqtt.md)).
They are the same two things Headwaters uses to reach Peregrine itself, so
setting up Peregrine in Headwaters is all a dial needs. Both are stored in
NVS (the CA in PSRAM at run time) and never entered on the device. With no
URL, or an `https://` URL and no CA, the screen says **Peregrine is not set
up**.

The dial verifies Peregrine's certificate against the CA, name included, so
the URL keeps the hostname (`peregrine.local`) rather than an address. A
verification failure is logged with its reason and shown as **Can't reach
Peregrine**.

## The conversation

| Phase | The orb | Hint | A tap |
|---|---|---|---|
| Idle | outlined | Turn for voice volume | starts listening |
| Listening | pale green, ring of ticks follows your voice | Tap to send / Tap to stop | sends now (or cancels if nothing was heard) |
| Thinking | spinner around it | Tap to cancel | cancels |
| Speaking | solid green, ring follows the reply | Tap to stop | stops |
| Done | outlined; transcript and answer below | Tap to ask again | starts listening |

**End of speech** is detected by level, because there is nothing to hold.
After the microphone has heard speech, 0.9 s of quiet ends the utterance; with
no speech at all it gives up after 6 s, and a capture never runs past 11 s.
Every capture logs its peak level so the thresholds in `tiercel_peregrine.c`
(`SPEECH_LEVEL`, `TRAILING_QUIET_MS`) can be tuned on the bench.

**Leaving the screen** abandons anything in progress. The idle-clock timeout
waits while a conversation is running.

**Volume** is the knob: 5 % a detent, a haptic bump at 0 and 100, shown on
the rim. It is applied in software on a squared curve and saved to NVS when
you leave the screen.

## Audio path

16 kHz PDM microphone → 4× software gain → PSRAM capture buffer → HTTP upload.
Reply → software volume → I2S (Philips framing) → CH445P switch → PCM5100A →
3.5 mm jack. Two hardware dependencies sit outside this firmware; see
[hardware.md](hardware.md):

- the S3 claims the DAC by driving GPIO0 high;
- the DAC's soft-mute is held by the **ESP32 co-processor's** factory
  firmware.

## No wake word, yet

The design's "Hey Peregrine" exists only on Peregrine's own server loop, an
openWakeWord ONNX model the ESP32-S3 cannot run. Espressif's on-device WakeNet
(ESP-SR) runs on the S3 but ships no "Hey Peregrine" model; adding one means
training a WakeNet model (Espressif offers this as a service) and adding a
model partition. Until then, Tiercel is tap-to-talk.
