# MQTT contract

Tiercel speaks the same contract as Capstan, plus Playbill and a services
topic. The broker is Headwaters' Mosquitto: **TLS only on 8883**, a
self-signed certificate (not verified on the device), username and password
required. All parsing is in `components/tiercel_mqtt/src/tiercel_mqtt.c`.

## Connection

| | |
|---|---|
| URI | `mqtts://<host>:<port>` from NVS (set in the setup portal) |
| Client id | `tc-tiercel-<12 hex MAC>` |
| Keepalive | 60 s |
| LWT | `local/tiercel/<12 hex MAC>/status` = `offline`, QoS 1, retained; `online` is published retained on connect |
| Hostname | `esp32-XXXXXX` (last three MAC bytes): the key for per-device config |

## Subscriptions

| Topic | Payload fields used |
|---|---|
| `local/energy/status` | `battery_voltage`, `battery_percent`, `battery_watts` (+ charging), `consumption_watts`, `solar_watts`, `time_remaining_minutes`, `charge_type` |
| `local/airquality/temphumid` | `tempInF`, `tempInC`, `humidity` |
| `local/airquality/status` | `tvoc_ppb`, `eco2_ppm` |
| `local/airquality/safety` | `co_ppm`, `co_warn`, `co_alarm`, `lpg_warn`, `lpg_alarm`, `co2_warn`, `co2_alarm`, `voc_alarm` |
| `local/water/status` | `fresh`, `grey`, `black` (percent) |
| `local/level/tilt` | `front_back`, `side_to_side` (deg); optional `front_back_diff_mm`, `left_right_diff_mm` |
| `local/lights/+/status` | `state`, `brightness` (PDM channels; `+` is the unified light id) |
| `local/relays/+/status` | `state` (Switchback relays; id = `+` + 100) |
| `local/picket/+/inputs`, `local/spoor/+/inputs` | `addr`, `inputs` bitmask, for alarms |
| `local/mode/current` | `mode`: `camping` / `driving` / `storage` (retained) |
| `local/gps/time` | `year`…`second`, UTC: the only clock source on the rig |
| `os/timezone/current` | `tz` (IANA name, retained) |
| `local/discovery/trigger`, `local/ota/trigger` | `*` or this unit's hostname |
| `local/playbill/+/{system,transport,volume,radio,source}/status` | see [playbill.md](playbill.md) |

Nothing on the sensor topics is retained, so every value is timestamped and
reads as `--` once its module goes quiet (energy and water 10 s, air 20 s,
level 5 s).

## Per-device configuration

Three retained topics, QoS 1, keyed by hostname. Each replaces its list or
block wholesale, and an empty one clears it. All three are written through to
NVS, and a redelivery that changes nothing does not rewrite flash.

### `local/config/tiercel/<hostname>/services` (new for Tiercel)

```json
{
  "peregrine": { "url": "https://peregrine.local", "ca": "-----BEGIN CERTIFICATE-----\n..." },
  "playbill":  { "device": "living-room" }
}
```

- `peregrine.url`: Peregrine's HTTPS listener, the URL Headwaters itself uses
  for Overlook's chat. No trailing slash (one is stripped if sent).
- `peregrine.ca`: Peregrine's CA certificate, PEM, as uploaded to Headwaters.
  At most 2047 bytes (`TIERCEL_CA_MAX_LEN`); a 2048-bit RSA CA is about 1.4 KB.
- `playbill.device`: the Playbill `deviceId` slug. Omit it to follow the
  first Playbill online.

A missing block clears that service, so removing Peregrine in the PWA really
does remove it from the dial.

The whole payload has to fit `MSG_DATA_MAX` (2048 bytes) in
`tiercel_mqtt.c`; with a 2048-bit RSA CA it is about 1.5 KB. Nothing in it is
a secret -- a CA certificate is public -- so it can sit retained on the broker.

### `local/config/tiercel/<hostname>/controls`

The Devices carousel. Identical to Capstan's payload, so Headwaters'
`capstan-config-sync.js` can publish it unchanged:

```json
{ "controls": [ { "id": 3, "name": "Kitchen", "icon": "lightbulb" },
                { "id": 104, "name": "Water Pump", "icon": "power-outlet" } ] }
```

At most 8. An `id` at or above 100 is a Switchback relay.

### `local/config/panel/<hostname>/alarms`

Shared with every panel (Capstan, and Milepost and Fireside when they adopt
it):

```json
{ "alarms": [ { "key": "picket:0:3", "name": "Entry Door", "icon": "door-open",
                "modes": { "camping": "high", "driving": "high", "storage": "high" } } ] }
```

At most 6. `key` is `picket|switchback:<addr>:<sensor>`, with sensor
numbered from 1. A verdict per mode: `high` alarms while the input is on,
`low` while it is off, `none` ignores it.

## Published

| Topic | Payload | When |
|---|---|---|
| `local/lights/<id>/command` | `{"state":0\|1}` | a tap on the Devices tile |
| `local/playbill/<id>/<feature>/command` | see [playbill.md](playbill.md) | Playbill controls, QoS 1 |
| `local/tiercel/<mac>/status` | `online` / `offline` (LWT) | connect / drop |

The dial changes a device's on/off look only when the module's status
confirms it; until then the tile reads "Switching".

## Discovery

On a trigger addressed to `*` or its hostname, Tiercel stops MQTT, advertises
`_trailcurrent._tcp` on port 80 (TXT `type=tiercel`, `fw=<version>`), serves
`GET /discovery/confirm`, and reconnects after the confirmation or three
minutes. See [provisioning.md](provisioning.md).

## What Headwaters sends

Headwaters treats `tiercel` as a module type alongside Capstan
(`routes/modules.js`) and publishes all three per-device topics from
`services/capstan-config-sync.js`:

1. `controls` and `alarms` from the Tiercel's module record, edited in
   **Settings → Network & Modules** with the same editor Capstan uses;
2. `services`, built from **Settings → Voice Assistant → Peregrine**: the
   Peregrine URL and the CA uploaded there, unchanged. Headwaters only
   brokers the configuration; the dial talks to Peregrine directly. The
   `playbill` block is not sent yet, so the dial follows the first Playbill
   online.

All three are republished whenever the Tiercel is confirmed, module config
or Peregrine settings are saved, Headwaters restarts, or a config request
arrives, so a changed Peregrine address or CA reaches every dial without
touching it.
