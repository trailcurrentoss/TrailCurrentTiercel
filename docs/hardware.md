# Hardware

Tiercel runs on the **Waveshare ESP32-S3-Knob-Touch-LCD-1.8**
([wiki](https://www.waveshare.com/wiki/ESP32-S3-Knob-Touch-LCD-1.8)). Every pin
below was read from Waveshare's ESP-IDF demo package and checked against the
board schematic; [`components/tiercel_board/src/board_pins.h`](../components/tiercel_board/src/board_pins.h)
cites the source for each one.

## Two MCUs, one USB-C port

The board carries **two** microcontrollers:

| Chip | Role in Tiercel |
|---|---|
| **ESP32-S3R8** (8 MB octal PSRAM, 16 MB flash) | Runs this firmware: display, touch, knob, haptics, Wi-Fi, microphone, audio out |
| **ESP32-U4WDH** (4 MB flash) | Runs Tiercel's **co-processor firmware** (`coprocessor/`) for Bluetooth headphones, or its factory firmware. Either way it holds the DAC's soft-mute line and owns the second knob contact pair. See [bluetooth.md](bluetooth.md) |

The USB-C port is switched by **plug orientation**: one way round it reaches
the S3's native USB (`/dev/ttyACM*`, Espressif VID `303a`), the other way
round a CH340 bridge to the ESP32 (`/dev/ttyUSB*`, VID `1a86`). If the wrong
chip answers, unplug, rotate the plug 180° and plug it back in.

## Pin map (ESP32-S3)

| Function | GPIO | Notes |
|---|---|---|
| LCD CS / SCLK | 14 / 13 | ST77916, QSPI on SPI2, 40 MHz |
| LCD D0–D3 | 15, 16, 17, 18 | |
| LCD reset | 21 | |
| LCD backlight | 47 | AO3400 low-side, active high, LEDC PWM 50 kHz |
| LCD TE | — | on the panel connector, not wired to the S3 |
| I2C SDA / SCL | 11 / 12 | shared by touch and haptics, 300 kHz |
| Touch INT / RST | 9 / 10 | CST816 at 0x15 |
| Haptics | I2C | DRV2605(L) at 0x5A |
| Knob A (clockwise) | 8 | detent switch to GND, 10k pull-up |
| Knob B (anticlockwise) | 7 | detent switch to GND, 10k pull-up |
| I2S BCLK / WS / DOUT | 39 / 40 / 41 | to the PCM5100A through the CH445P switch |
| Audio route select | 0 | CH445P `IN`; high = S3 owns the DAC. Also the BOOT strap |
| PDM mic CLK / DATA | 45 / 46 | MSM261D4030H1CPM, left slot |
| UART to the ESP32 | 38 (TX) / 48 (RX) | Bluetooth link to the co-processor, 921600 baud ([bluetooth.md](bluetooth.md)) |
| SD card (SDMMC 4-bit) | CLK 4, CMD 3, D0 5, D1 6, D2 42, D3 2 | unused by this firmware |

## Things that differ from what you would assume

**The panel is an ST77916, not an SH8601.** Waveshare's demo drives it with
the `esp_lcd_sh8601` driver and an init table that is plainly an ST77916
sequence; the factory firmware links the ST77916 driver. Tiercel uses
Espressif's `esp_lcd_st77916` with Waveshare's init table, which is tuned to
this glass. The controller only accepts **even-aligned** draw windows, so the
display layer widens every invalidated area to even edges (see
`round_area_cb` in `board_display.c`). An odd window shows as a one-pixel
seam that looks like tearing.

**The knob is not a quadrature encoder.** It is an Alps SSCM110100
bidirectional detector switch: two independent contacts, one closing per
detent each way. It is decoded by polling every 3 ms and counting a debounced
release, the same rule as Waveshare's demo. There is **no push**.

**The DAC is shared and its mute belongs to the other chip.** The PCM5100A's
inputs sit behind a CH445P 4PDT switch selected by S3 GPIO0. Tiercel drives
GPIO0 high at boot to claim the DAC. The DAC's soft-mute (`XSMT`) is driven
by the **ESP32's** IO32, so the S3 cannot unmute it. Both the factory ESP32
firmware and Tiercel's `coprocessor/` firmware hold it unmuted; Tiercel's
mutes it only while Bluetooth headphones are connected. If you put any other
firmware on the ESP32 and Peregrine goes silent, this is why.

**The PCM5100A expects standard I2S.** Its `FMT` pin is tied low, which
selects Philips framing. Waveshare's audio demo configures MSB-aligned slots,
which put every sample a bit late. Tiercel uses Philips framing. There is no
MCLK; the DAC derives its clock from BCK.

**The battery ADC reads the 5 V rail.** GPIO1 sits on a divider from the
system 5 V net, not the cell, so it cannot report battery charge. Tiercel
does not use it.

**Haptics.** The motor nets are labelled `LRA_P/LRA_N`, but Waveshare's demo
drives the DRV2605 as an ERM in open loop with ROM library 5, and that is
what Tiercel does by default. `menuconfig → Tiercel board` has an LRA option
and per-effect overrides if a unit feels weak.

**Flash mode is DIO.** The factory image and Waveshare's demos run DIO; this
unit's flash has not been proven in QIO, so `sdkconfig.defaults` sets DIO.
