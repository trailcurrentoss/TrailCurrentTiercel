# Screens and input

Derived from the design prototype, `DOCS/Capstan Knob touch UI adaptation.zip`
(`Tiercel.dc.html`). It is a touch adaptation of Capstan's rotary design, with
Peregrine and Playbill added.

## The input model

| Input | Meaning |
|---|---|
| **Turn** | Moves through items, or adjusts the value drawn on the rim. One step per click by default; **Settings → Knob Ratio** makes it two or three clicks per step. |
| **Tap** | Replaces press: open, toggle, confirm. Only the centred item and labelled buttons respond. |
| **Back** | One of three gestures, set in **Settings → Back Gesture** (below). |

Any tap or turn wakes the idle clock, and that first input is consumed. It
does not also act on a screen you cannot see yet.

### Back gestures

| Setting | How | Feedback |
|---|---|---|
| **Swipe** (default) | drag left or right anywhere on the glass; release past 70 px. Swipes only ever go back -- moving between apps is the knob alone | the left edge lights and grows as you drag |
| **Hold** | keep a finger still anywhere for 0.65 s | the rim fills green; lifting or moving early cancels |
| **Back chip** | tap the chip at 12 o'clock on every app screen | always visible, never accidental |

A recognised swipe or hold takes the touch away from whatever it started on,
so a swipe that begins on the play button does not also press play.

### Haptics: graded so you can tell them apart

Each kind of input has its own feel, from lightest to heaviest:

| Event | Feel |
|---|---|
| A knob step, timed to the click that completes it (any screen) | one click |
| A tap that selects or opens something | double click |
| A committed change (a device toggled, a mode set) | triple click |
| Back taken (swipe, hold or the back chip) | strong buzz |
| A detent past a limit | strong pulse |
| An alarm | buzz |

A swipe also gives a single click when it crosses the back threshold, so you
know letting go will go back. A heavier effect cuts off a lighter one still
playing; a lighter one never cuts off a heavier one.

Every value the knob adjusts that has a **minimum or maximum** answers a
detent past its limit with the pulse, and the value does not move:

| Screen | Knob adjusts | Limits |
|---|---|---|
| Climate | setpoint | 50–90 °F (10–32 °C) |
| Climate Mode | highlighted row | first / last row |
| Energy | page | Battery / Loads |
| Locale | highlighted row | first / last row |
| Peregrine | voice volume | 0 / 100 % |
| Playbill | volume, or FM frequency | 0 / 100 %; 87.9 / 107.9 MHz |
| Brightness | backlight | 5 / 100 % |

The three **carousels** (the app menu, Devices, Settings) and the clock-face
picker wrap around, so they have no end to bump against: every detent clicks.
**Settings → Haptics** turns all of it off. Effects, and the motor's drive
clamp, are in `menuconfig → Tiercel board`.

## Screens

**Idle clock.** One of four faces (Classic, Digital, TrailCurrent, Climate
Ring), chosen in Settings. Tap or turn to wake. Returns on its own after the
**Clock Timeout**, unless an alarm or setup is showing or Peregrine is mid-conversation.

**Menu.** A carousel of ten apps: Climate, Devices, Peregrine, Playbill,
Energy, Water Tanks, Air Quality, Leveling, Settings, Clock. Turn to browse,
tap the centre tile to open, tap a side icon to step. Each item shows a
one-line live summary.

**Climate.** The ring of ticks is the range; the active span between inside
and target is in the mode colour. Turn to set the target. Tap the **mode
pill** to open **Mode** (Heat, Cool, Auto, Off), where you tap a row to set it.
There is no thermostat topic on the bus yet, so as on Capstan, the setpoint
and mode are local to the dial.

**Devices.** The lights and relays Headwaters assigned to this unit (up to
eight). Turn to choose, tap the tile to switch it. The tile reads
**Switching** until the module confirms; its on/off look changes only on that
confirmation.

**Peregrine.** See [peregrine.md](peregrine.md). Tap the orb to talk; it
listens, thinks and speaks, and what it heard and said appears under it.
Turn for voice volume, shown on the rim.

**Playbill.** See [playbill.md](playbill.md). **Media | Radio** at the top;
now-playing or the FM frequency; previous, play/pause, next. On media the
knob is volume. On the radio the knob tunes, and the chip at the bottom
swaps it to volume.

**Energy.** Battery, Solar Input and Loads, one at a time on a 270° arc. Turn
or tap to change page.

**Water Tanks, Air Quality, Leveling.** Read-only.

**Settings.** A carousel: Wi-Fi and MQTT (status only), Bluetooth, Theme,
Brightness, Back Gesture, Haptics, Knob Ratio (1:1, 2:1 or 3:1 clicks per step), Locale, Clock Face, Alarm Snooze, Clock
Timeout, Factory Reset. Tap the tile to change or open the setting. Factory Reset
takes two taps within five seconds.

**Brightness.** Turn to set the backlight, 5–100 % in 5 % steps. It changes
as you turn and is saved to NVS when you leave the screen, which means one
flash write per visit rather than one per detent.

**Bluetooth.** Pair headphones. Opening it searches (unless headphones are
already connected); tap a row to connect, Forget to drop the remembered pair.
The knob scrolls the list. See [bluetooth.md](bluetooth.md).

**Alert.** A door or safety alarm takes over the whole face with a red ring
and a buzz. Tap to snooze it for the Alarm Snooze interval. It comes back if
the alarm is still active when the snooze runs out.

**Setup.** The provisioning instructions, shown until the unit is set up.
See [provisioning.md](provisioning.md).

## What is not here

- **Wake word.** The design shows "Hey Peregrine". Peregrine's own wake word
  is an openWakeWord model on the server, which the ESP32-S3 cannot run, and
  Espressif's on-device WakeNet has no "Hey Peregrine" model. Tiercel is
  tap-to-talk until a WakeNet model is trained. See [peregrine.md](peregrine.md).
- **Getting Started**, Capstan's ring tutorial. Its copy teaches press-and-hold
  on a ring, and the Tiercel design has no equivalent.
- **The LED ring.** This board has none.
