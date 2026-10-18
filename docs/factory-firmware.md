# Factory firmware

Waveshare's factory images are worth keeping. Tiercel replaces the **ESP32
co-processor's** image with its own (`coprocessor/`) to support Bluetooth
headphones -- see [bluetooth.md](bluetooth.md) -- so back that chip up before
flashing it. Tiercel works on either image: both hold the DAC's soft-mute line
unmuted.

Back up **before the first flash**. The dumps are git-ignored
(`firmware_backup/`): each is a full image of one particular unit, including
its NVS and RF calibration.

Waveshare also publishes images in its demo package
(`ESP32-S3-Knob-Touch-LCD-1.8-Demo/Firmware/`), `WX-ESP32S3-KNOB_V1.2.bin`
for the S3 and `ESP32-KNOB_ESP32_0.bin` for the ESP32. Those are a fallback
if a unit's own dump is lost.

## What the factory images are

| Chip | Flash | Partitions | What runs |
|---|---|---|---|
| ESP32-S3 | 16 MB, DIO | `nvs`, `otadata`, `app0`/`app1` (3 MB each), `spiffs` (9.9 MB), `coredump` | Waveshare's demo UI (Arduino core 3.0.7 / IDF 5.1.4) |
| ESP32-U4WDH | 4 MB | `nvs`, `phy_init`, `factory` (1.6 MB), `storage` | Bluetooth A2DP sink, BLE HID knob, knob decoding, holds the DAC unmuted (IDF 5.4) |

## Back up

The USB-C plug orientation chooses the chip. The S3 enumerates as
`/dev/ttyACM*`, the ESP32 as `/dev/ttyUSB*` (CH340).

```sh
mkdir -p firmware_backup && cd firmware_backup

# ESP32-S3 -- 16 MB, about 4 minutes
esptool --port /dev/ttyACM0 -b 921600 read-flash 0 ALL esp32s3_factory_16MB.bin

# flip the USB-C plug, then the ESP32 -- 4 MB, about 1 minute
esptool --port /dev/ttyUSB0 -b 921600 read-flash 0 ALL esp32_u4wdh_factory_4MB.bin

sha256sum *.bin > SHA256SUMS
```

Check the partition table parses before trusting a dump:

```sh
python3 -c "import struct,sys; d=open(sys.argv[1],'rb').read(0x9000)
for o in range(0x8000,0x8c00,32):
    e=d[o:o+32]
    if e[:2]!=b'\xaa\x50': break
    print(e[12:28].rstrip(b'\0').decode(), hex(struct.unpack('<I',e[4:8])[0]))" esp32s3_factory_16MB.bin
```

## Restore

```sh
# ESP32-S3
esptool --port /dev/ttyACM0 -b 921600 write-flash 0 esp32s3_factory_16MB.bin

# ESP32 (flip the plug first)
esptool --port /dev/ttyUSB0 -b 921600 write-flash 0 esp32_u4wdh_factory_4MB.bin
```

A full image includes the partition table and bootloader, so it restores the
chip exactly as it shipped. If the S3 will not enter download mode, hold
**BOOT** while plugging it in.
