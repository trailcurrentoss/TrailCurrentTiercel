# Playbill (media)

Playbill is the rig's media box: local media, streaming sources and an
RTL-SDR FM radio. Its only remote interface is MQTT on the Headwaters broker,
and Tiercel uses exactly the contract Headwaters' PWA and Peregrine use
(`controller/src/schema/commands.schema.json` and `controller/src/topics.js`
in TrailCurrentPlaybill). Code: `tiercel_mqtt.c` (`apply_playbill`),
`tiercel_model/src/tiercel_playbill.c` (state), `main/ui_playbill.c` (screen).

## Which Playbill

A rig can have several. Headwaters names the one this dial controls in the
services config (`"playbill": {"device": "<deviceId>"}`). With none named,
Tiercel adopts the **first Playbill that reports itself online** on
`local/playbill/+/system/status` and logs that it did. That is right for the
common single-Playbill rig.

## State (subscribed, retained)

| Topic | Used for |
|---|---|
| `local/playbill/<id>/system/status` | presence, and `name` ("Living Room") for the eyebrow |
| `local/playbill/<id>/source/status` | which view to show (`"radio"` → Radio) |
| `local/playbill/<id>/transport/status` | `title`, `subtitle`, `paused`, `positionMs`, `durationMs` |
| `local/playbill/<id>/volume/status` | `volumePct`, `muted` |
| `local/playbill/<id>/radio/status` | `running`, `band`, `frequencyHz`, `lastScan.stations` |

Transport status is throttled to one publish a second with no trailing
publish, so Tiercel **interpolates the position** while playing.

## Commands (published, QoS 1)

Playbill validates commands strictly: extra or misspelled fields are dropped
and only logged on the Playbill. These are the exact payloads sent.

| Control | Topic feature | Payload |
|---|---|---|
| Play / pause | `transport` | `{"action":"transport.toggle"}` |
| Previous / next (media) | `transport` | `{"action":"transport.previous"}` / `{"action":"transport.next"}` |
| Volume | `volume` | `{"action":"transport.volumeSet","percent":N}`, then `{"action":"transport.muteOff"}` if it was muted |
| Tune | `radio` | `{"action":"radio.tune","value":{"band":"fm","frequencyHz":N}}` |
| Radio off | `radio` | `{"action":"radio.stop"}` |
| Media tab | `radio`, `transport` | `radio.stop`, then `transport.play` (resumes whatever mpv had) |

Topic: `local/playbill/<id>/<feature>/command`.

## What Playbill does not offer, and what Tiercel does instead

- **No track queue.** `transport.next` / `previous` skip +30 s / −10 s. The
  buttons do what the PWA's do.
- **No "switch to media" command.** Media needs an item that only Playbill's
  own GUI can pick, so the Media tab stops the radio and resumes mpv. If
  nothing was loaded, it says "Nothing playing".
- **No artist or kind field.** The second line is Playbill's `subtitle`
  ("artist · album", a channel, a year), and the eyebrow is the source and
  room.
- **No station names, RDS or seek, and presets cannot be read.** The radio
  eyebrow shows the room, not a call sign. Previous/next jump between the
  stations found by Playbill's last band scan (`lastScan.stations`); with no
  scan, they step 1 MHz.

## Knob and rate limiting

On media the knob is volume, 2 % a detent. On the radio it tunes in 0.2 MHz
steps (the US FM grid, as Playbill's scan uses), and the chip swaps it to
volume. Both bump the haptic motor at their limits.

Each volume command spawns `wpctl`, and each tune restarts `rtl_fm`
(100–500 ms), on the Playbill. So Tiercel coalesces: volume goes out at most
every 100 ms with the latest value, and a tune is sent once the knob has been
still for 350 ms. The rim moves at once regardless, because the dial shows the value it
just set until Playbill's own report catches up (a 1.5 s hold-off stops a
late report snapping the rim back mid-turn).
