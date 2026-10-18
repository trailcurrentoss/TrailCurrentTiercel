# Building and flashing

## Toolchain

- **ESP-IDF 5.5** (developed on 5.5.2). Earlier releases lack the
  `esp_lcd_st77916` dependency floor (`idf >= 5.4`).
- **EEZ Studio** 0.26 or later, for the GUI. See [gui.md](gui.md).

Managed components are fetched on the first build and pinned there:
`lvgl/lvgl ~9.2.2`, `espressif/esp_lvgl_port ~2.4`, `espressif/esp_lcd_st77916`,
`espressif/esp_lcd_touch_cst816s`, `espressif/mdns`. **LVGL and esp_lvgl_port
move together with `lvglVersion` in the EEZ project or not at all**; see
`components/tiercel_board/idf_component.yml`.

## Build

```sh
. <path-to-esp-idf-5.5>/export.sh
idf.py set-target esp32s3        # once
idf.py build
```

Configuration lives in `sdkconfig.defaults` (and `menuconfig → Tiercel`,
`→ Tiercel board`). **Never edit `sdkconfig`**: it is generated and ignored by
git. `sdkconfig.defaults` only seeds a new `sdkconfig`, so after changing it:

```sh
rm sdkconfig && idf.py build
```

## Flash

Plug the USB-C cable in the orientation that reaches the **ESP32-S3**. It
enumerates as `/dev/ttyACM*` (USB-Serial/JTAG). If you get `/dev/ttyUSB*`
(a CH340), that is the ESP32 co-processor: flip the plug.

```sh
idf.py -p /dev/ttyACM0 flash monitor
```

If the S3 will not enter download mode, hold **BOOT** while powering it on.

## The bring-up screen

Until EEZ Studio has exported the UI into `main/ui/`, the build produces a
**bring-up screen** instead (`main/bringup.c`). It is a test jig, not
product UI:

| Check | What you should see |
|---|---|
| Display | a green ring at the glass edge, four colour swatches (red, green, blue, white) |
| Knob | the counter follows the knob with a light tick per detent, and a stronger **end-stop bump** past ±10 |
| Touch | a red dot follows your finger, with coordinates |
| Audio | hold the glass: it records 3 s from the microphone and plays it back on the 3.5 mm jack |

The free internal and PSRAM heap is shown at the bottom.

## Exporting the UI

Open `GUI/Tiercel.eez-project` in EEZ Studio and press **Ctrl+B**. The export
writes `main/ui/`; the next `idf.py build` picks it up (`main/CMakeLists.txt`
globs it) and the bring-up screen disappears from the binary. If you edited
the `.eez-project` outside EEZ Studio, **close and reopen it first**, because
EEZ Studio exports the copy it has in memory.

Everything in `main/ui/` is generated. Delete it and re-export at any time;
no hand-written code lives there.
