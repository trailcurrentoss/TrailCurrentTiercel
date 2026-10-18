# GUI

The UI is an **EEZ Studio** LVGL project: `GUI/Tiercel.eez-project`, one
360×360 round canvas, LVGL 9.2.2, flow support off.

## Where things live

| Path | What | Who writes it |
|---|---|---|
| `GUI/Tiercel.eez-project` | screens, styles, colours, fonts, actions | EEZ Studio, or the generator |
| `GUI/ASSETS/` | Roboto and Font Awesome source files | — |
| `main/ui/` | the C export | **EEZ Studio's Ctrl+B only** |
| `main/actions.c`, `main/vars.c` | the functions the export declares | hand-written |
| `main/ui_*.c` | what each screen does | hand-written |

`main/ui/` contains nothing hand-written. Empty it and re-export whenever you
like. `main/CMakeLists.txt` globs whatever is there and fails the build
loudly if `ui.h` exists without its sources, so a stale export can't produce
a UI-less firmware.

## The rule: the canvas must match the device

EEZ Studio's canvas renders the JSON. It never runs our C. So C may change
only what the canvas has no opinion about:

- text (`lv_label_set_text`), values (`lv_arc_set_value`, `lv_bar_set_value`),
  line points (clock hands, climate needles, the FM marker)
- states (`lv_obj_add_state`) and flags (`LV_OBJ_FLAG_HIDDEN`)
- which screen is loaded (`loadScreen`)

C never sets a widget's position, size, alignment, font or colour. Every
look a widget can take (a selected row, a heating pill, a speaking orb) is
authored as a **state** in its style, and C picks the state. Several styles
borrow `CHECKED` / `PRESSED` / `DISABLED` as plain selectors. That is safe
only on widgets nobody can press, which is why the climate pill and the
Peregrine orb are inert panels with a transparent tap target laid on top.

Before a change is called done, grep `main/*.c` for `lv_obj_set_pos`,
`_size`, `_align`, `lv_obj_center` and `lv_obj_set_style_*` applied to an
`objects.` widget. There should be none. The one deliberate exception is the
levelling bubble, offset by a style **translate** that carries the live tilt,
like a clock hand (`refresh_level()` in `ui_data.c`). Its authored position is
level, and that is what the canvas shows.

## Touch targets

Tiercel is touch-first, so the project follows three rules that a ring-only
UI never needed:

1. **Decorative widgets are not clickable.** LVGL hit-tests the topmost
   CLICKABLE object under the finger and does not bubble, so a label or panel
   left clickable over a button silently eats the tap. Panels, scales, bars,
   dots and labels are authored inert; only buttons and panels with an event
   handler are clickable.
2. **Tap targets are the design's size**, 64–116 px, not shrunk to fit.
3. **A tap target laid over drawn content is its last child** (top of the
   z-order) with a fully transparent style.

## The generator

The project was generated from Python in `GUI/tmp/` (forked from Capstan's
generator): `gen_eez_project.py`, `screens_layout.py`, `layout.py`,
`fonts.py`, `palette.py`. It validates as it writes: unknown widget keys,
styles applied to the wrong widget type, hex colours, widgets outside the
round mask, and the C tables that must agree with it (menu icons, device dot
count, air range).

`GUI/tmp/` is **git-ignored**, the house convention for working scripts, so
`Tiercel.eez-project` is the tracked source of truth. Changes made in EEZ
Studio are the normal path from here; regenerating overwrites them.

## Fonts and colours

- **Fonts** are the house set, shared with Milepost, Fireside and Capstan:
  Roboto Light/Regular/Medium and Font Awesome 6 Solid, embedded in the
  project. Tiercel adds icons for Playbill (pause, skip, volume, radio) and
  Settings (back gesture, haptics), plus a 64 px digits-only numeral for the
  FM frequency and brightness.
- **Colours** are the house palette, 28 tokens in light and dark themes, plus
  four readable text variants of the accents. Styles reference tokens only;
  there are no hex values in any style.

## After editing the project outside EEZ Studio

EEZ Studio exports what it has **in memory**. Close and reopen the project
before Ctrl+B, or the export silently uses the old version.
