/*
 * The Peregrine screen: the orb, its level ring and spinner, what was heard
 * and said, and the voice volume on the knob. The voice client itself is
 * components/tiercel_peregrine; this file only paints it.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void ui_peregrine_init(void);
void ui_peregrine_enter(void);
/** Leaving abandons a conversation in progress and saves the volume. */
void ui_peregrine_leave(void);
/** Knob: voice volume, 5% a detent, bumping at 0 and 100. */
void ui_peregrine_rotate(int diff);
void ui_peregrine_tap(void);
/** True while listening, thinking or speaking -- the idle timeout waits. */
bool ui_peregrine_busy(void);
/** The menu carousel's summary line. */
void ui_peregrine_summary(char *out, size_t len);

/* Bound to the ring sections and the spinner (main/vars.c). */
int32_t ui_peregrine_ring_a_max(void);
int32_t ui_peregrine_ring_b_min(void);
int32_t ui_peregrine_spin(void);
