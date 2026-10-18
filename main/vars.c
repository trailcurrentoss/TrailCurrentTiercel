/*
 * The variables EEZ Studio declares in ui/vars.h.
 *
 * Expression-bound properties in GUI/Tiercel.eez-project read these through
 * get_var_<name>() on every ui_tick() (20 ms, main.c). They are computed, not
 * stored: each getter asks the module that owns the value, so there is no
 * second copy to keep in step. The setters exist because EEZ declares them;
 * nothing in the UI writes back.
 */

#include <stdint.h>

#include "vars.h"

#include "ui_climate.h"
#include "ui_clock.h"
#include "ui_peregrine.h"

/* Climate: the active range of ticks in each mode colour. -1 parks a
 * section off the 0..80 scale, which is how the unused two disappear. */
#define CLIMATE_SECTION_VAR(name, sec, which)                             \
    int32_t get_var_##name(void)                                          \
    {                                                                     \
        int32_t lo, hi;                                                   \
        ui_climate_section(sec, &lo, &hi);                                \
        return which;                                                     \
    }                                                                     \
    void set_var_##name(int32_t value) { (void)value; }

CLIMATE_SECTION_VAR(climate_heat_min, UI_CLIMATE_SEC_HEAT, lo)
CLIMATE_SECTION_VAR(climate_heat_max, UI_CLIMATE_SEC_HEAT, hi)
CLIMATE_SECTION_VAR(climate_cool_min, UI_CLIMATE_SEC_COOL, lo)
CLIMATE_SECTION_VAR(climate_cool_max, UI_CLIMATE_SEC_COOL, hi)
CLIMATE_SECTION_VAR(climate_hold_min, UI_CLIMATE_SEC_HOLD, lo)
CLIMATE_SECTION_VAR(climate_hold_max, UI_CLIMATE_SEC_HOLD, hi)

/* The Digital face's elapsed-seconds ring. */
int32_t get_var_clock_sec_min(void) { return 0; }
void set_var_clock_sec_min(int32_t value) { (void)value; }
int32_t get_var_clock_sec_max(void) { return ui_clock_second(); }
void set_var_clock_sec_max(int32_t value) { (void)value; }

/* Peregrine's level ring: section a lights 0..n clockwise from 12, section b
 * lights 60-n..59 anticlockwise, so the lit arc grows out both ways. */
int32_t get_var_pg_ring_a_min(void) { return 0; }
void set_var_pg_ring_a_min(int32_t value) { (void)value; }
int32_t get_var_pg_ring_a_max(void) { return ui_peregrine_ring_a_max(); }
void set_var_pg_ring_a_max(int32_t value) { (void)value; }
int32_t get_var_pg_ring_b_min(void) { return ui_peregrine_ring_b_min(); }
void set_var_pg_ring_b_min(int32_t value) { (void)value; }
int32_t get_var_pg_ring_b_max(void) { return 59; }
void set_var_pg_ring_b_max(int32_t value) { (void)value; }

/* The thinking spinner's rotation, degrees. */
int32_t get_var_pg_spin(void) { return ui_peregrine_spin(); }
void set_var_pg_spin(int32_t value) { (void)value; }
