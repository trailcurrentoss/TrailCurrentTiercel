#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_PG_RING_A_MIN = 0,
    FLOW_GLOBAL_VARIABLE_PG_RING_A_MAX = 1,
    FLOW_GLOBAL_VARIABLE_PG_RING_B_MIN = 2,
    FLOW_GLOBAL_VARIABLE_PG_RING_B_MAX = 3,
    FLOW_GLOBAL_VARIABLE_PG_SPIN = 4,
    FLOW_GLOBAL_VARIABLE_CLIMATE_HEAT_MIN = 5,
    FLOW_GLOBAL_VARIABLE_CLIMATE_HEAT_MAX = 6,
    FLOW_GLOBAL_VARIABLE_CLIMATE_COOL_MIN = 7,
    FLOW_GLOBAL_VARIABLE_CLIMATE_COOL_MAX = 8,
    FLOW_GLOBAL_VARIABLE_CLIMATE_HOLD_MIN = 9,
    FLOW_GLOBAL_VARIABLE_CLIMATE_HOLD_MAX = 10,
    FLOW_GLOBAL_VARIABLE_CLOCK_SEC_MIN = 11,
    FLOW_GLOBAL_VARIABLE_CLOCK_SEC_MAX = 12
};

// Native global variables

extern int32_t get_var_pg_ring_a_min();
extern void set_var_pg_ring_a_min(int32_t value);
extern int32_t get_var_pg_ring_a_max();
extern void set_var_pg_ring_a_max(int32_t value);
extern int32_t get_var_pg_ring_b_min();
extern void set_var_pg_ring_b_min(int32_t value);
extern int32_t get_var_pg_ring_b_max();
extern void set_var_pg_ring_b_max(int32_t value);
extern int32_t get_var_pg_spin();
extern void set_var_pg_spin(int32_t value);
extern int32_t get_var_climate_heat_min();
extern void set_var_climate_heat_min(int32_t value);
extern int32_t get_var_climate_heat_max();
extern void set_var_climate_heat_max(int32_t value);
extern int32_t get_var_climate_cool_min();
extern void set_var_climate_cool_min(int32_t value);
extern int32_t get_var_climate_cool_max();
extern void set_var_climate_cool_max(int32_t value);
extern int32_t get_var_climate_hold_min();
extern void set_var_climate_hold_min(int32_t value);
extern int32_t get_var_climate_hold_max();
extern void set_var_climate_hold_max(int32_t value);
extern int32_t get_var_clock_sec_min();
extern void set_var_clock_sec_min(int32_t value);
extern int32_t get_var_clock_sec_max();
extern void set_var_clock_sec_max(int32_t value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/