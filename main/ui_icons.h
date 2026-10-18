/*
 * The app carousel's glyphs, as UTF-8.
 *
 * These are drawn in the reduced `fh` face, so every one must be in FH_RANGE
 * in GUI/tmp/fonts.py -- one outside it renders as an empty box with no
 * warning. gen_eez_project.py diffs this header against FA in layout.py and
 * against FH_RANGE on every run (check_icon_header), so a mismatch fails the
 * generator rather than the panel.
 */
#pragma once

#define UI_ICON_CLIMATE   "\xEF\x8B\x87"   /* 0xF2C7 thermometer-half */
#define UI_ICON_DEVICES   "\xEF\x83\xAB"   /* 0xF0EB lightbulb        */
#define UI_ICON_MIC       "\xEF\x84\xB0"   /* 0xF130 microphone       */
#define UI_ICON_MUSIC     "\xEF\x80\x81"   /* 0xF001 music            */
#define UI_ICON_ENERGY    "\xEF\x83\xA7"   /* 0xF0E7 bolt             */
#define UI_ICON_WATER     "\xEF\x81\x83"   /* 0xF043 droplet          */
#define UI_ICON_AIR       "\xEF\x83\x82"   /* 0xF0C2 cloud            */
#define UI_ICON_LEVEL     "\xEF\x8F\xBD"   /* 0xF3FD gauge            */
#define UI_ICON_SETTINGS  "\xEF\x80\x93"   /* 0xF013 gear             */
#define UI_ICON_CLOCK     "\xEF\x80\x97"   /* 0xF017 clock            */

/* Peregrine and Playbill by name, for the menu table. */
#define UI_ICON_PEREGRINE UI_ICON_MIC
#define UI_ICON_PLAYBILL  UI_ICON_MUSIC
