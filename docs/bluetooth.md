# Bluetooth headphones

Tiercel can play its audio (Peregrine's replies) through Bluetooth
headphones as well as the 3.5 mm jack. Pairing is in **Settings →
Bluetooth**. Nothing about Bluetooth appears at start-up: once a pair has been
connected, the board reconnects it by itself whenever it is switched on.

## How it works

The ESP32-S3 has no Classic Bluetooth, and headphones need it (A2DP). The
board's second chip, the ESP32-U4WDH co-processor, does the radio work. It
runs Tiercel's own firmware from [`coprocessor/`](../coprocessor) and talks
to the S3 over a UART (S3 GPIO38 → ESP32 IO18, ESP32 IO23 → S3 GPIO48) at
921600 baud, one text line per message. The protocol is documented at the
top of `coprocessor/main/main.c`; the S3 side is `components/tiercel_bt`.

| Piece | Where |
|---|---|
| Radio, pairing, reconnecting, A2DP stream | `coprocessor/main/main.c` (ESP32) |
| The UART | `components/tiercel_board/src/board_coproc.c` |
| Protocol, state, audio streaming | `components/tiercel_bt` |
| The screen | `main/ui_bluetooth.c`, `page_bluetooth()` in `GUI/tmp/screens_layout.py` |

While headphones are connected:

- Every block the board plays is also streamed to the ESP32, after Tiercel's
  volume is applied and resampled to the link's fixed 22050 Hz.
- The ESP32 pulls the DAC's soft-mute (`XSMT`, its IO32) low, so the 3.5 mm
  jack goes quiet. The board cannot tell whether anything is plugged into the
  jack (there is no plug-detect line), so connecting is what moves the sound.
- Expect roughly 150–300 ms of Bluetooth latency.

With no headphones connected, the ESP32 holds `XSMT` high exactly as the
factory firmware did, and the jack works as before.

## Using it

Open **Settings → Bluetooth**. If no headphones are connected it starts
searching straight away; put the headphones in pairing mode and they appear
in the list (turn the knob or drag to scroll). Tap one to connect. The
connected pair is outlined in green, and the Settings carousel shows its name.

- **Search / Stop** starts or stops a search. A search briefly drops connected
  headphones (the radio cannot search with a link up) and reconnects them
  afterwards if nothing new is picked.
- **Forget** disconnects and forgets the remembered pair.
- Leaving the screen stops a search in progress.

**Bluetooth is not available** on the screen means the co-processor is still
on its factory firmware, or not answering. Flash it as below.

AirPods: put both in the case, open the lid, and hold the button on the back
of the case until the light flashes white. If they still do not show up,
another Apple device has probably claimed them.

## Building and flashing the co-processor

The co-processor is a separate ESP-IDF project, target `esp32`:

```sh
cd coprocessor
idf.py set-target esp32      # once
idf.py build
```

**Back up the factory image first** if you have not already -- see
[factory-firmware.md](factory-firmware.md). This replaces it.

The USB-C plug's orientation chooses the chip. Flip the plug so the board
enumerates as a CH340 (`/dev/ttyUSB*`), then:

```sh
idf.py -p /dev/ttyUSB0 flash
```

Flip the plug back to reach the S3 (`/dev/ttyACM*`). The S3 logs the ESP32's
own log lines as `bt: esp32| ...`, since the ESP32's USB is on the other side
of the plug.

The headphones' loudness relative to the jack is `HEADPHONE_BOOST` in
`coprocessor/main/main.c`; it applies a soft limiter above 60% of full scale.

Restoring the factory image (see [factory-firmware.md](factory-firmware.md))
removes Bluetooth and nothing else: the jack keeps working either way.
