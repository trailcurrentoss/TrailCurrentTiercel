#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

screen_page_idle_state_t screen_page_idle_state;
screen_page_climate_state_t screen_page_climate_state;
screen_page_peregrine_state_t screen_page_peregrine_state;
screen_page_clock_face_state_t screen_page_clock_face_state;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

//
// Screens
//

void create_screen_page_idle() {
    screen_page_idle_state_t *state = &screen_page_idle_state;
    (void)state;
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_idle = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_add_event_cb(obj, action_wake, LV_EVENT_CLICKED, (void *)0);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // idle_cl
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.idle_cl = obj;
            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // idle_cl_ticks
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    objects.idle_cl_ticks = obj;
                    lv_obj_set_pos(obj, 11, 11);
                    lv_obj_set_size(obj, 338, 338);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 59);
                    lv_scale_set_angle_range(obj, 354);
                    lv_scale_set_rotation(obj, 270);
                    lv_scale_set_total_tick_count(obj, 60);
                    lv_scale_set_major_tick_every(obj, 5);
                    lv_scale_set_label_show(obj, false);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_face_ticks(obj);
                }
                {
                    // idle_cl_day
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_cl_day = obj;
                    lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(16));
                    lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_face_text_lg(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Tuesday");
                }
                {
                    // idle_cl_date
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_cl_date = obj;
                    lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(22));
                    lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_face_text_md(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "September 29, 2026");
                }
                {
                    // idle_cl_inside
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_cl_inside = obj;
                    lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(59));
                    lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_face_text_lg(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Inside 67°");
                }
                {
                    // idle_cl_moderow
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_cl_moderow = obj;
                    lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(66));
                    lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // idle_cl_mode
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.idle_cl_mode = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // idle_cl_mode_icon
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_cl_mode_icon = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    add_style_face_mode_icon(obj);
                                    lv_label_set_text_static(obj, "");
                                }
                                {
                                    // idle_cl_mode_text
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_cl_mode_text = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_mode(obj);
                                    lv_label_set_text(obj, "Heating to 72°");
                                }
                            }
                        }
                    }
                }
                {
                    // idle_cl_stats
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_cl_stats = obj;
                    lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(73));
                    lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // idle_cl_batt
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.idle_cl_batt = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // idle_cl_batt_icon
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_cl_batt_icon = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    add_style_face_stat_icon(obj);
                                    lv_label_set_text_static(obj, "");
                                }
                                {
                                    // idle_cl_batt_value
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_cl_batt_value = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_stat(obj);
                                    lv_label_set_text(obj, "82%");
                                }
                            }
                        }
                        {
                            // idle_cl_water
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.idle_cl_water = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // idle_cl_water_icon
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_cl_water_icon = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    add_style_face_stat_icon(obj);
                                    lv_label_set_text_static(obj, "");
                                }
                                {
                                    // idle_cl_water_value
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_cl_water_value = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_stat(obj);
                                    lv_label_set_text(obj, "72%");
                                }
                            }
                        }
                    }
                }
                {
                    // idle_cl_hand_h
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.idle_cl_hand_h = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                    static lv_point_precise_t line_points[] = {
                        { 180, 180 },
                        { 180, 97 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    add_style_face_hand_h(obj);
                }
                {
                    // idle_cl_hand_m
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.idle_cl_hand_m = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                    static lv_point_precise_t line_points[] = {
                        { 180, 180 },
                        { 180, 52 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    add_style_face_hand_m(obj);
                }
                {
                    // idle_cl_hand_s
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.idle_cl_hand_s = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                    static lv_point_precise_t line_points[] = {
                        { 180, 202 },
                        { 180, 37 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    add_style_face_hand_s(obj);
                }
                {
                    // idle_cl_cap
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_cl_cap = obj;
                    lv_obj_set_pos(obj, LV_PCT(48), LV_PCT(48));
                    lv_obj_set_size(obj, LV_PCT(4), LV_PCT(4));
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_face_cap(obj);
                }
                {
                    // idle_cl_cap_in
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_cl_cap_in = obj;
                    lv_obj_set_pos(obj, LV_PCT(49), LV_PCT(49));
                    lv_obj_set_size(obj, LV_PCT(1), LV_PCT(1));
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_face_cap_in(obj);
                }
            }
        }
        {
            // idle_dg
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.idle_dg = obj;
            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // idle_dg_ticks
                    lv_obj_t *obj = lv_scale_create(parent_obj);
                    objects.idle_dg_ticks = obj;
                    lv_obj_set_pos(obj, 11, 11);
                    lv_obj_set_size(obj, 338, 338);
                    lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                    lv_scale_set_range(obj, 0, 59);
                    lv_scale_set_angle_range(obj, 354);
                    lv_scale_set_rotation(obj, 270);
                    lv_scale_set_total_tick_count(obj, 60);
                    lv_scale_set_major_tick_every(obj, 5);
                    lv_scale_set_label_show(obj, false);
                    {
                        state->scale_section = lv_scale_add_section(obj);
                        lv_scale_section_set_style(state->scale_section, LV_PART_MAIN, get_style_face_sec_on_MAIN_DEFAULT());
                        lv_scale_section_set_style(state->scale_section, LV_PART_INDICATOR, get_style_face_sec_on_INDICATOR_DEFAULT());
                        lv_scale_section_set_style(state->scale_section, LV_PART_ITEMS, get_style_face_sec_on_ITEMS_DEFAULT());
                    }
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_face_sec_ticks(obj);
                }
                {
                    // idle_dg_date
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_dg_date = obj;
                    lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(20));
                    lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_face_text_lg(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Tuesday · September 29");
                }
                {
                    // idle_dg_timerow
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_dg_timerow = obj;
                    lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(27));
                    lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // idle_dg_time
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.idle_dg_time = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_time(obj);
                            lv_label_set_text(obj, "7:42");
                        }
                        {
                            // idle_dg_ampm
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.idle_dg_ampm = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_am_pm(obj);
                            lv_label_set_text(obj, "PM");
                        }
                    }
                }
                {
                    // idle_dg_inside
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_dg_inside = obj;
                    lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(63));
                    lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_face_text_lg(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Inside 67°");
                }
                {
                    // idle_dg_moderow
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_dg_moderow = obj;
                    lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(69));
                    lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // idle_dg_mode
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.idle_dg_mode = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // idle_dg_mode_icon
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_dg_mode_icon = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    add_style_face_mode_icon(obj);
                                    lv_label_set_text_static(obj, "");
                                }
                                {
                                    // idle_dg_mode_text
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_dg_mode_text = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_mode(obj);
                                    lv_label_set_text(obj, "Heating to 72°");
                                }
                            }
                        }
                    }
                }
            }
        }
        {
            // idle_br
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.idle_br = obj;
            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // idle_br_arc
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    objects.idle_br_arc = obj;
                    lv_obj_set_pos(obj, 11, 11);
                    lv_obj_set_size(obj, 338, 338);
                    lv_arc_set_range(obj, 0, 3600);
                    lv_arc_set_value(obj, 2520);
                    lv_arc_set_bg_start_angle(obj, 0);
                    lv_arc_set_bg_end_angle(obj, 360);
                    lv_arc_set_rotation(obj, 270);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    add_style_face_min_arc(obj);
                }
                {
                    // idle_br_logo
                    lv_obj_t *obj = lv_image_create(parent_obj);
                    objects.idle_br_logo = obj;
                    lv_obj_set_pos(obj, LV_PCT(42), LV_PCT(14));
                    lv_obj_set_size(obj, 57, 57);
                    lv_image_set_src(obj, &img_tc_logo);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_ADV_HITTEST|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                }
                {
                    // idle_br_name
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_br_name = obj;
                    lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(32));
                    lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_face_brand_name(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "TrailCurrent");
                }
                {
                    // idle_br_timerow
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_br_timerow = obj;
                    lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(38));
                    lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // idle_br_time
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.idle_br_time = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_time_sm(obj);
                            lv_label_set_text(obj, "7:42");
                        }
                        {
                            // idle_br_ampm
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.idle_br_ampm = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_am_pm_sm(obj);
                            lv_label_set_text(obj, "PM");
                        }
                    }
                }
                {
                    // idle_br_date
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_br_date = obj;
                    lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(65));
                    lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_face_text_muted(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Tuesday · September 29");
                }
                {
                    // idle_br_bottom
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_br_bottom = obj;
                    lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(72));
                    lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // idle_br_inside
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.idle_br_inside = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_label_body(obj);
                            lv_label_set_text(obj, "Inside 67°");
                        }
                        {
                            // idle_br_mode
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.idle_br_mode = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // idle_br_mode_icon
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_br_mode_icon = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    add_style_face_mode_icon(obj);
                                    lv_label_set_text_static(obj, "");
                                }
                                {
                                    // idle_br_mode_text
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_br_mode_text = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_mode(obj);
                                    lv_label_set_text(obj, "Heating to 72°");
                                }
                            }
                        }
                    }
                }
            }
        }
        {
            // idle_rg
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.idle_rg = obj;
            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // idle_rg_arc
                    lv_obj_t *obj = lv_arc_create(parent_obj);
                    objects.idle_rg_arc = obj;
                    lv_obj_set_pos(obj, 11, 11);
                    lv_obj_set_size(obj, 338, 338);
                    lv_arc_set_range(obj, 0, 3600);
                    lv_arc_set_value(obj, 2520);
                    lv_arc_set_bg_start_angle(obj, 0);
                    lv_arc_set_bg_end_angle(obj, 360);
                    lv_arc_set_rotation(obj, 270);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    add_style_face_ring_arc(obj);
                }
                {
                    // idle_rg_h1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h1 = obj;
                    lv_obj_set_pos(obj, LV_PCT(64), LV_PCT(14));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "1");
                }
                {
                    // idle_rg_h2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h2 = obj;
                    lv_obj_set_pos(obj, LV_PCT(79), LV_PCT(28));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "2");
                }
                {
                    // idle_rg_h3
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h3 = obj;
                    lv_obj_set_pos(obj, LV_PCT(84), LV_PCT(47));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "3");
                }
                {
                    // idle_rg_h4
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h4 = obj;
                    lv_obj_set_pos(obj, LV_PCT(79), LV_PCT(67));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "4");
                }
                {
                    // idle_rg_h5
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h5 = obj;
                    lv_obj_set_pos(obj, LV_PCT(64), LV_PCT(81));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "5");
                }
                {
                    // idle_rg_h6
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h6 = obj;
                    lv_obj_set_pos(obj, LV_PCT(45), LV_PCT(86));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "6");
                }
                {
                    // idle_rg_h7
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h7 = obj;
                    lv_obj_set_pos(obj, LV_PCT(26), LV_PCT(81));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "7");
                }
                {
                    // idle_rg_h8
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h8 = obj;
                    lv_obj_set_pos(obj, LV_PCT(11), LV_PCT(67));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "8");
                }
                {
                    // idle_rg_h9
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h9 = obj;
                    lv_obj_set_pos(obj, LV_PCT(6), LV_PCT(47));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "9");
                }
                {
                    // idle_rg_h10
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h10 = obj;
                    lv_obj_set_pos(obj, LV_PCT(11), LV_PCT(28));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "10");
                }
                {
                    // idle_rg_h11
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h11 = obj;
                    lv_obj_set_pos(obj, LV_PCT(26), LV_PCT(14));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "11");
                }
                {
                    // idle_rg_h12
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.idle_rg_h12 = obj;
                    lv_obj_set_pos(obj, LV_PCT(45), LV_PCT(8));
                    lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_face_hour_num(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "12");
                }
                {
                    // idle_rg_col
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.idle_rg_col = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // idle_rg_eyebrow
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.idle_rg_eyebrow = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_label_eyebrow(obj);
                            lv_label_set_text(obj, "INSIDE");
                        }
                        {
                            // idle_rg_temp
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.idle_rg_temp = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_time_sm(obj);
                            lv_label_set_text(obj, "67°");
                        }
                        {
                            // idle_rg_time
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.idle_rg_time = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_ring_time(obj);
                            lv_label_set_text(obj, "7:42 PM");
                        }
                        {
                            // idle_rg_mode
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.idle_rg_mode = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // idle_rg_mode_icon
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_rg_mode_icon = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                    add_style_face_mode_icon(obj);
                                    lv_label_set_text_static(obj, "");
                                }
                                {
                                    // idle_rg_mode_text
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.idle_rg_mode_text = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_mode(obj);
                                    lv_label_set_text(obj, "Heating to 72°");
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    tick_screen_page_idle();
}

void tick_screen_page_idle() {
    screen_page_idle_state_t *state = &screen_page_idle_state;
    (void)state;
    {
        int32_t new_min_val = get_var_clock_sec_min();
        int32_t new_max_val = get_var_clock_sec_max();
        lv_scale_section_set_range(state->scale_section, new_min_val, new_max_val);
    }
}

void create_screen_page_menu() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_menu = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // menu_hero
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_hero = obj;
            lv_obj_set_pos(obj, LV_PCT(36), LV_PCT(26));
            lv_obj_set_size(obj, LV_PCT(29), LV_PCT(29));
            lv_obj_add_event_cb(obj, action_nav_open, LV_EVENT_CLICKED, (void *)0);
            add_style_hero_tile(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // menu_hero_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.menu_hero_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_hero_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // menu_prev
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_prev = obj;
            lv_obj_set_pos(obj, LV_PCT(3), LV_PCT(39));
            lv_obj_set_size(obj, LV_PCT(20), LV_PCT(20));
            lv_obj_add_event_cb(obj, action_nav_step, LV_EVENT_CLICKED, (void *)0);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // menu_prev_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.menu_prev_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_neighbour_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // menu_next
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_next = obj;
            lv_obj_set_pos(obj, LV_PCT(77), LV_PCT(39));
            lv_obj_set_size(obj, LV_PCT(20), LV_PCT(20));
            lv_obj_add_event_cb(obj, action_nav_step, LV_EVENT_CLICKED, (void *)1);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // menu_next_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.menu_next_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_neighbour_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // menu_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.menu_title = obj;
            lv_obj_set_pos(obj, LV_PCT(22), LV_PCT(59));
            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_menu_name(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Climate");
        }
        {
            // menu_summary
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.menu_summary = obj;
            lv_obj_set_pos(obj, LV_PCT(22), LV_PCT(69));
            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_menu_summary(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Heating · 72°");
        }
        {
            // menu_dot0
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot0 = obj;
            lv_obj_set_pos(obj, 249, 319);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot1 = obj;
            lv_obj_set_pos(obj, 241, 323);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot2 = obj;
            lv_obj_set_pos(obj, 234, 326);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot3 = obj;
            lv_obj_set_pos(obj, 226, 329);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot4 = obj;
            lv_obj_set_pos(obj, 218, 331);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot5
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot5 = obj;
            lv_obj_set_pos(obj, 209, 333);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot6
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot6 = obj;
            lv_obj_set_pos(obj, 201, 335);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot7
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot7 = obj;
            lv_obj_set_pos(obj, 193, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot8
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot8 = obj;
            lv_obj_set_pos(obj, 184, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot9
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot9 = obj;
            lv_obj_set_pos(obj, 176, 337);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot10
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot10 = obj;
            lv_obj_set_pos(obj, 168, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot11
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot11 = obj;
            lv_obj_set_pos(obj, 159, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot12
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot12 = obj;
            lv_obj_set_pos(obj, 151, 335);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot13
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot13 = obj;
            lv_obj_set_pos(obj, 143, 333);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot14
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot14 = obj;
            lv_obj_set_pos(obj, 134, 331);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot15
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot15 = obj;
            lv_obj_set_pos(obj, 126, 329);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot16
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot16 = obj;
            lv_obj_set_pos(obj, 118, 326);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot17
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot17 = obj;
            lv_obj_set_pos(obj, 111, 323);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // menu_dot18
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.menu_dot18 = obj;
            lv_obj_set_pos(obj, 103, 319);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_page_menu();
}

void tick_screen_page_menu() {
}

void create_screen_page_climate() {
    screen_page_climate_state_t *state = &screen_page_climate_state;
    (void)state;
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_climate = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // climate_ticks
            lv_obj_t *obj = lv_scale_create(parent_obj);
            objects.climate_ticks = obj;
            lv_obj_set_pos(obj, 14, 14);
            lv_obj_set_size(obj, 332, 332);
            lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
            lv_scale_set_range(obj, 0, 80);
            lv_scale_set_angle_range(obj, 270);
            lv_scale_set_rotation(obj, 135);
            lv_scale_set_total_tick_count(obj, 81);
            lv_scale_set_major_tick_every(obj, 1);
            lv_scale_set_label_show(obj, false);
            {
                state->scale_section = lv_scale_add_section(obj);
                lv_scale_section_set_style(state->scale_section, LV_PART_MAIN, get_style_climate_section_heat_MAIN_DEFAULT());
                lv_scale_section_set_style(state->scale_section, LV_PART_INDICATOR, get_style_climate_section_heat_INDICATOR_DEFAULT());
            }
            {
                state->scale_section1 = lv_scale_add_section(obj);
                lv_scale_section_set_style(state->scale_section1, LV_PART_MAIN, get_style_climate_section_cool_MAIN_DEFAULT());
                lv_scale_section_set_style(state->scale_section1, LV_PART_INDICATOR, get_style_climate_section_cool_INDICATOR_DEFAULT());
            }
            {
                state->scale_section2 = lv_scale_add_section(obj);
                lv_scale_section_set_style(state->scale_section2, LV_PART_MAIN, get_style_climate_section_hold_MAIN_DEFAULT());
                lv_scale_section_set_style(state->scale_section2, LV_PART_INDICATOR, get_style_climate_section_hold_INDICATOR_DEFAULT());
            }
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_climate_ticks(obj);
        }
        {
            // climate_needle_inside
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.climate_needle_inside = obj;
            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            static lv_point_precise_t line_points[] = {
                { 137, 44 },
                { 129, 21 }
            };
            lv_line_set_points(obj, line_points, 2);
            add_style_climate_needle_inside(obj);
        }
        {
            // climate_needle_target
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.climate_needle_target = obj;
            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            static lv_point_precise_t line_points[] = {
                { 210, 51 },
                { 219, 15 }
            };
            lv_line_set_points(obj, line_points, 2);
            add_style_climate_needle_target(obj);
        }
        {
            // climate_pill
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.climate_pill = obj;
            lv_obj_set_pos(obj, 90, 84);
            lv_obj_set_size(obj, 180, 44);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_climate_pill(obj);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(obj, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // climate_mode_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.climate_mode_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_climate_mode_icon(obj);
                    lv_label_set_text_static(obj, "");
                }
                {
                    // climate_mode
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.climate_mode = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_climate_mode_text(obj);
                    lv_label_set_text(obj, "HEATING");
                }
                {
                    // climate_mode_chev
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.climate_mode_chev = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_climate_mode_icon(obj);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // climate_pill_tap
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.climate_pill_tap = obj;
            lv_obj_set_pos(obj, 90, 84);
            lv_obj_set_size(obj, 180, 44);
            lv_obj_add_event_cb(obj, action_climate_open_mode, LV_EVENT_CLICKED, (void *)0);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_tap_target(obj);
        }
        {
            // climate_setpoint
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.climate_setpoint = obj;
            lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(37));
            lv_obj_set_size(obj, LV_PCT(80), LV_PCT(24));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            add_style_climate_num(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "72");
        }
        {
            // climate_info_row
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.climate_info_row = obj;
            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(62));
            lv_obj_set_size(obj, LV_PCT(70), LV_PCT(5));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(obj, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // climate_inside
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.climate_inside = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_label_body(obj);
                    lv_label_set_text(obj, "Inside 67°");
                }
                {
                    // climate_eta_group
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.climate_eta_group = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // climate_eta_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.climate_eta_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_label_icon_sm(obj);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // climate_eta
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.climate_eta = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_label_body(obj);
                            lv_label_set_text(obj, "28 min");
                        }
                    }
                }
            }
        }
        {
            // climate_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.climate_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // climate_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.climate_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // climate_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.climate_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // climate_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.climate_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_climate();
}

void tick_screen_page_climate() {
    screen_page_climate_state_t *state = &screen_page_climate_state;
    (void)state;
    {
        int32_t new_min_val = get_var_climate_heat_min();
        int32_t new_max_val = get_var_climate_heat_max();
        lv_scale_section_set_range(state->scale_section, new_min_val, new_max_val);
    }
    {
        int32_t new_min_val = get_var_climate_cool_min();
        int32_t new_max_val = get_var_climate_cool_max();
        lv_scale_section_set_range(state->scale_section1, new_min_val, new_max_val);
    }
    {
        int32_t new_min_val = get_var_climate_hold_min();
        int32_t new_max_val = get_var_climate_hold_max();
        lv_scale_section_set_range(state->scale_section2, new_min_val, new_max_val);
    }
}

void create_screen_page_climate_mode() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_climate_mode = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // cmode_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.cmode_title = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(18));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "MODE");
        }
        {
            // cmode_list
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cmode_list = obj;
            lv_obj_set_pos(obj, LV_PCT(25), LV_PCT(25));
            lv_obj_set_size(obj, LV_PCT(50), LV_PCT(56));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // cmode_item0
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cmode_item0 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(22));
                    lv_obj_add_event_cb(obj, action_climate_mode_select, LV_EVENT_CLICKED, (void *)0);
                    add_style_mode_row_heat(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // cmode_item0_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cmode_item0_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(8), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // cmode_item0_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cmode_item0_title = obj;
                            lv_obj_set_pos(obj, LV_PCT(24), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(68), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Heat");
                        }
                    }
                }
                {
                    // cmode_item1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cmode_item1 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(26));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(22));
                    lv_obj_add_event_cb(obj, action_climate_mode_select, LV_EVENT_CLICKED, (void *)1);
                    add_style_mode_row_cool(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // cmode_item1_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cmode_item1_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(8), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // cmode_item1_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cmode_item1_title = obj;
                            lv_obj_set_pos(obj, LV_PCT(24), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(68), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Cool");
                        }
                    }
                }
                {
                    // cmode_item2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cmode_item2 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(52));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(22));
                    lv_obj_add_event_cb(obj, action_climate_mode_select, LV_EVENT_CLICKED, (void *)2);
                    add_style_mode_row_auto(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // cmode_item2_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cmode_item2_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(8), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // cmode_item2_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cmode_item2_title = obj;
                            lv_obj_set_pos(obj, LV_PCT(24), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(68), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Auto");
                        }
                    }
                }
                {
                    // cmode_item3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cmode_item3 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(78));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(22));
                    lv_obj_add_event_cb(obj, action_climate_mode_select, LV_EVENT_CLICKED, (void *)3);
                    add_style_mode_row_off(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // cmode_item3_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cmode_item3_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(8), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // cmode_item3_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cmode_item3_title = obj;
                            lv_obj_set_pos(obj, LV_PCT(24), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(68), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Off");
                        }
                    }
                }
            }
        }
        {
            // cmode_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.cmode_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // cmode_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.cmode_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // cmode_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.cmode_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // cmode_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.cmode_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_climate_mode();
}

void tick_screen_page_climate_mode() {
}

void create_screen_page_devices() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_devices = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // devices_hero
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_hero = obj;
            lv_obj_set_pos(obj, LV_PCT(34), LV_PCT(25));
            lv_obj_set_size(obj, LV_PCT(31), LV_PCT(31));
            lv_obj_add_event_cb(obj, action_device_toggle, LV_EVENT_CLICKED, (void *)0);
            add_style_device_tile(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // devices_hero_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.devices_hero_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_device_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // devices_prev
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_prev = obj;
            lv_obj_set_pos(obj, LV_PCT(3), LV_PCT(39));
            lv_obj_set_size(obj, LV_PCT(20), LV_PCT(20));
            lv_obj_add_event_cb(obj, action_nav_step, LV_EVENT_CLICKED, (void *)0);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // devices_prev_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.devices_prev_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_device_neighbour_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // devices_next
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_next = obj;
            lv_obj_set_pos(obj, LV_PCT(77), LV_PCT(39));
            lv_obj_set_size(obj, LV_PCT(20), LV_PCT(20));
            lv_obj_add_event_cb(obj, action_nav_step, LV_EVENT_CLICKED, (void *)1);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // devices_next_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.devices_next_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_device_neighbour_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // devices_name
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.devices_name = obj;
            lv_obj_set_pos(obj, LV_PCT(22), LV_PCT(60));
            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_title(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Kitchen");
        }
        {
            // devices_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.devices_value = obj;
            lv_obj_set_pos(obj, LV_PCT(22), LV_PCT(70));
            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_body(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "On");
        }
        {
            // devices_dot0
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot0 = obj;
            lv_obj_set_pos(obj, 234, 326);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot1 = obj;
            lv_obj_set_pos(obj, 226, 329);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot2 = obj;
            lv_obj_set_pos(obj, 218, 331);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot3 = obj;
            lv_obj_set_pos(obj, 209, 333);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot4 = obj;
            lv_obj_set_pos(obj, 201, 335);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot5
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot5 = obj;
            lv_obj_set_pos(obj, 193, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot6
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot6 = obj;
            lv_obj_set_pos(obj, 184, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot7
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot7 = obj;
            lv_obj_set_pos(obj, 176, 337);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot8
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot8 = obj;
            lv_obj_set_pos(obj, 168, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot9
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot9 = obj;
            lv_obj_set_pos(obj, 159, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot10
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot10 = obj;
            lv_obj_set_pos(obj, 151, 335);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot11
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot11 = obj;
            lv_obj_set_pos(obj, 143, 333);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot12
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot12 = obj;
            lv_obj_set_pos(obj, 134, 331);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot13
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot13 = obj;
            lv_obj_set_pos(obj, 126, 329);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_dot14
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.devices_dot14 = obj;
            lv_obj_set_pos(obj, 118, 326);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // devices_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.devices_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // devices_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.devices_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // devices_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.devices_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // devices_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.devices_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_devices();
}

void tick_screen_page_devices() {
}

void create_screen_page_peregrine() {
    screen_page_peregrine_state_t *state = &screen_page_peregrine_state;
    (void)state;
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_peregrine = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // pg_eyebrow
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pg_eyebrow = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(14));
            lv_obj_set_size(obj, LV_PCT(60), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_pg_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "PEREGRINE");
        }
        {
            // pg_ring
            lv_obj_t *obj = lv_scale_create(parent_obj);
            objects.pg_ring = obj;
            lv_obj_set_pos(obj, 86, 68);
            lv_obj_set_size(obj, 188, 188);
            lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
            lv_scale_set_range(obj, 0, 59);
            lv_scale_set_angle_range(obj, 354);
            lv_scale_set_rotation(obj, 270);
            lv_scale_set_total_tick_count(obj, 60);
            lv_scale_set_major_tick_every(obj, 1);
            lv_scale_set_label_show(obj, false);
            {
                state->scale_section = lv_scale_add_section(obj);
                lv_scale_section_set_style(state->scale_section, LV_PART_MAIN, get_style_pg_ring_on_MAIN_DEFAULT());
                lv_scale_section_set_style(state->scale_section, LV_PART_INDICATOR, get_style_pg_ring_on_INDICATOR_DEFAULT());
            }
            {
                state->scale_section1 = lv_scale_add_section(obj);
                lv_scale_section_set_style(state->scale_section1, LV_PART_MAIN, get_style_pg_ring_on_MAIN_DEFAULT());
                lv_scale_section_set_style(state->scale_section1, LV_PART_INDICATOR, get_style_pg_ring_on_INDICATOR_DEFAULT());
            }
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_pg_ring(obj);
        }
        {
            // pg_spin_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.pg_spin_arc = obj;
            lv_obj_set_pos(obj, 104, 86);
            lv_obj_set_size(obj, 152, 152);
            lv_arc_set_value(obj, 17);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_pg_spin(obj);
        }
        {
            // pg_orb
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.pg_orb = obj;
            lv_obj_set_pos(obj, 124, 106);
            lv_obj_set_size(obj, 112, 112);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_pg_orb(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pg_orb_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pg_orb_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_pg_orb_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // pg_q
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pg_q = obj;
            lv_obj_set_pos(obj, 66, 258);
            lv_obj_set_size(obj, 228, 18);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_SCROLL_CIRCULAR);
            add_style_pg_q(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Tap to talk");
        }
        {
            // pg_a_win
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.pg_a_win = obj;
            lv_obj_set_pos(obj, 66, 276);
            lv_obj_set_size(obj, 228, 34);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pg_a
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pg_a = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 228, LV_SIZE_CONTENT);
                    add_style_pg_a(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // pg_hint
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.pg_hint = obj;
            lv_obj_set_pos(obj, 95, 312);
            lv_obj_set_size(obj, 170, 18);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_muted(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Turn for voice volume");
        }
        {
            // pg_vol
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.pg_vol = obj;
            lv_obj_set_pos(obj, 6, 6);
            lv_obj_set_size(obj, 348, 348);
            lv_arc_set_value(obj, 60);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_vol_arc(obj);
        }
        {
            // pg_tap
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.pg_tap = obj;
            lv_obj_set_pos(obj, 105, 87);
            lv_obj_set_size(obj, 150, 150);
            lv_obj_add_event_cb(obj, action_peregrine_tap, LV_EVENT_CLICKED, (void *)0);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_tap_target(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // pg_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.pg_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // pg_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.pg_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // pg_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.pg_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pg_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pg_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_peregrine();
}

void tick_screen_page_peregrine() {
    screen_page_peregrine_state_t *state = &screen_page_peregrine_state;
    (void)state;
    {
        int32_t new_min_val = get_var_pg_ring_a_min();
        int32_t new_max_val = get_var_pg_ring_a_max();
        lv_scale_section_set_range(state->scale_section, new_min_val, new_max_val);
    }
    {
        int32_t new_min_val = get_var_pg_ring_b_min();
        int32_t new_max_val = get_var_pg_ring_b_max();
        lv_scale_section_set_range(state->scale_section1, new_min_val, new_max_val);
    }
    {
        int32_t new_val = get_var_pg_spin();
        int32_t cur_val = lv_arc_get_rotation(objects.pg_spin_arc);
        if (new_val != cur_val) {
            tick_value_change_obj = objects.pg_spin_arc;
            lv_arc_set_rotation(objects.pg_spin_arc, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_page_playbill() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_playbill = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // pb_vol
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.pb_vol = obj;
            lv_obj_set_pos(obj, 6, 6);
            lv_obj_set_size(obj, 348, 348);
            lv_arc_set_value(obj, 42);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_vol_arc(obj);
        }
        {
            // pb_tune
            lv_obj_t *obj = lv_scale_create(parent_obj);
            objects.pb_tune = obj;
            lv_obj_set_pos(obj, 14, 14);
            lv_obj_set_size(obj, 332, 332);
            lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
            lv_scale_set_range(obj, 875, 1080);
            lv_scale_set_angle_range(obj, 270);
            lv_scale_set_rotation(obj, 135);
            lv_scale_set_total_tick_count(obj, 42);
            lv_scale_set_major_tick_every(obj, 4);
            lv_scale_set_label_show(obj, false);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_pb_tune(obj);
        }
        {
            // pb_tune_marker
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.pb_tune_marker = obj;
            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            static lv_point_precise_t line_points[] = {
                { 54, 263 },
                { 79, 238 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            add_style_pb_tune_marker(obj);
        }
        {
            // pb_src
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.pb_src = obj;
            lv_obj_set_pos(obj, 102, 52);
            lv_obj_set_size(obj, 156, 44);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_pb_seg(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pb_src_media
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.pb_src_media = obj;
                    lv_obj_set_pos(obj, 2, 2);
                    lv_obj_set_size(obj, 74, 38);
                    lv_obj_add_event_cb(obj, action_playbill_source, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    add_style_pb_seg_btn(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // pb_src_media_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.pb_src_media_label = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_pb_seg_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Media");
                        }
                    }
                }
                {
                    // pb_src_radio
                    lv_obj_t *obj = lv_button_create(parent_obj);
                    objects.pb_src_radio = obj;
                    lv_obj_set_pos(obj, 78, 2);
                    lv_obj_set_size(obj, 74, 38);
                    lv_obj_add_event_cb(obj, action_playbill_source, LV_EVENT_CLICKED, (void *)1);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
                    add_style_pb_seg_btn(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // pb_src_radio_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.pb_src_radio_label = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_pb_seg_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Radio");
                        }
                    }
                }
            }
        }
        {
            // pb_media
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.pb_media = obj;
            lv_obj_set_pos(obj, 66, 104);
            lv_obj_set_size(obj, 228, 98);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pb_kind
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_kind = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 228, 16);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_pb_eyebrow(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "MUSIC · LIVING ROOM");
                }
                {
                    // pb_title
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_title = obj;
                    lv_obj_set_pos(obj, 0, 18);
                    lv_obj_set_size(obj, 228, 30);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_pb_title(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Canyon Sessions");
                }
                {
                    // pb_artist
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_artist = obj;
                    lv_obj_set_pos(obj, 0, 48);
                    lv_obj_set_size(obj, 228, 20);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_label_body(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Low Horizon");
                }
                {
                    // pb_progress
                    lv_obj_t *obj = lv_bar_create(parent_obj);
                    objects.pb_progress = obj;
                    lv_obj_set_pos(obj, 26, 76);
                    lv_obj_set_size(obj, 176, 4);
                    lv_bar_set_range(obj, 0, 1000);
                    lv_bar_set_value(obj, 347, LV_ANIM_OFF);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                    add_style_pb_progress(obj);
                }
                {
                    // pb_pos
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_pos = obj;
                    lv_obj_set_pos(obj, 26, 83);
                    lv_obj_set_size(obj, 60, 14);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_pb_time(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "1:24");
                }
                {
                    // pb_len
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_len = obj;
                    lv_obj_set_pos(obj, 142, 83);
                    lv_obj_set_size(obj, 60, 14);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_pb_time(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "4:02");
                }
            }
        }
        {
            // pb_radio
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.pb_radio = obj;
            lv_obj_set_pos(obj, 66, 104);
            lv_obj_set_size(obj, 228, 90);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pb_station
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_station = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 228, 16);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_pb_eyebrow(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "FM · LIVING ROOM");
                }
                {
                    // pb_freq_row
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.pb_freq_row = obj;
                    lv_obj_set_pos(obj, 0, 20);
                    lv_obj_set_size(obj, 228, 60);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_END, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // pb_freq
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.pb_freq = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_pb_freq(obj);
                            lv_label_set_text(obj, "98.7");
                        }
                        {
                            // pb_freq_unit
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.pb_freq_unit = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_pb_freq_unit(obj);
                            lv_label_set_text(obj, "FM");
                        }
                    }
                }
            }
        }
        {
            // pb_prev
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.pb_prev = obj;
            lv_obj_set_pos(obj, 62, 214);
            lv_obj_set_size(obj, 64, 64);
            lv_obj_add_event_cb(obj, action_playbill_prev, LV_EVENT_CLICKED, (void *)0);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_pb_round(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pb_prev_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_prev_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_pb_round_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // pb_play
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.pb_play = obj;
            lv_obj_set_pos(obj, 140, 206);
            lv_obj_set_size(obj, 80, 80);
            lv_obj_add_event_cb(obj, action_playbill_play, LV_EVENT_CLICKED, (void *)0);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_pb_play(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pb_play_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_play_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_pb_play_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // pb_next
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.pb_next = obj;
            lv_obj_set_pos(obj, 234, 214);
            lv_obj_set_size(obj, 64, 64);
            lv_obj_add_event_cb(obj, action_playbill_next, LV_EVENT_CLICKED, (void *)0);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_pb_round(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pb_next_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_next_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_pb_round_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // pb_knob
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.pb_knob = obj;
            lv_obj_set_pos(obj, 105, 298);
            lv_obj_set_size(obj, 150, 40);
            lv_obj_add_event_cb(obj, action_playbill_knob_toggle, LV_EVENT_CLICKED, (void *)0);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_pb_chip(obj);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pb_knob_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_knob_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_pb_chip_icon(obj);
                    lv_label_set_text_static(obj, "");
                }
                {
                    // pb_knob_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_knob_label = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_pb_chip_text(obj);
                    lv_label_set_text(obj, "Volume 42");
                }
            }
        }
        {
            // pb_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.pb_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // pb_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.pb_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // pb_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.pb_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // pb_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.pb_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_playbill();
}

void tick_screen_page_playbill() {
}

void create_screen_page_energy() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_energy = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // energy_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.energy_arc = obj;
            lv_obj_set_pos(obj, LV_PCT(4), LV_PCT(4));
            lv_obj_set_size(obj, LV_PCT(92), LV_PCT(92));
            lv_arc_set_value(obj, 82);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_energy_arc(obj);
        }
        {
            // energy_stack
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.energy_stack = obj;
            lv_obj_set_pos(obj, LV_PCT(16), LV_PCT(16));
            lv_obj_set_size(obj, LV_PCT(68), LV_PCT(68));
            lv_obj_add_event_cb(obj, action_energy_next, LV_EVENT_CLICKED, (void *)0);
            add_style_plain(obj);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_row(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // energy_head
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.energy_head = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // energy_head_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.energy_head_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_energy_head_icon(obj);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // energy_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.energy_title = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_energy_head(obj);
                            lv_label_set_text(obj, "BATTERY");
                        }
                    }
                }
                {
                    // energy_value_row
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.energy_value_row = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_END, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_column(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_row(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // energy_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.energy_value = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_label_hero_xl(obj);
                            lv_label_set_text(obj, "82");
                        }
                        {
                            // energy_unit
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.energy_unit = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_energy_unit(obj);
                            lv_label_set_text(obj, "%");
                        }
                    }
                }
                {
                    // energy_sub1
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.energy_sub1 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_label_body(obj);
                    lv_label_set_text(obj, "13.4 V · Float");
                }
                {
                    // energy_sub2
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.energy_sub2 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_label_body_muted(obj);
                    lv_label_set_text(obj, "Time Remaining 14h 20m");
                }
            }
        }
        {
            // energy_dot0
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.energy_dot0 = obj;
            lv_obj_set_pos(obj, 161, 326);
            lv_obj_set_size(obj, 6, 6);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_energy_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // energy_dot1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.energy_dot1 = obj;
            lv_obj_set_pos(obj, 177, 327);
            lv_obj_set_size(obj, 6, 6);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_energy_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // energy_dot2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.energy_dot2 = obj;
            lv_obj_set_pos(obj, 193, 326);
            lv_obj_set_size(obj, 6, 6);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_energy_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // energy_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.energy_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // energy_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.energy_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // energy_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.energy_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // energy_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.energy_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_energy();
}

void tick_screen_page_energy() {
}

void create_screen_page_water() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_water = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // water_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_title = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(15));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "WATER TANKS");
        }
        {
            // water_fresh_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_fresh_value = obj;
            lv_obj_set_pos(obj, LV_PCT(17), LV_PCT(25));
            lv_obj_set_size(obj, LV_PCT(22), LV_PCT(7));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_title(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "--");
        }
        {
            // water_fresh_bar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.water_fresh_bar = obj;
            lv_obj_set_pos(obj, LV_PCT(21), LV_PCT(33));
            lv_obj_set_size(obj, LV_PCT(13), LV_PCT(35));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_bar_fresh(obj);
        }
        {
            // water_fresh_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_fresh_label = obj;
            lv_obj_set_pos(obj, LV_PCT(17), LV_PCT(71));
            lv_obj_set_size(obj, LV_PCT(22), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_body(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Fresh");
        }
        {
            // water_grey_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_grey_value = obj;
            lv_obj_set_pos(obj, LV_PCT(39), LV_PCT(25));
            lv_obj_set_size(obj, LV_PCT(22), LV_PCT(7));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_title(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "--");
        }
        {
            // water_grey_bar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.water_grey_bar = obj;
            lv_obj_set_pos(obj, LV_PCT(43), LV_PCT(33));
            lv_obj_set_size(obj, LV_PCT(13), LV_PCT(35));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_bar_grey(obj);
        }
        {
            // water_grey_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_grey_label = obj;
            lv_obj_set_pos(obj, LV_PCT(39), LV_PCT(71));
            lv_obj_set_size(obj, LV_PCT(22), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_body(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Grey");
        }
        {
            // water_black_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_black_value = obj;
            lv_obj_set_pos(obj, LV_PCT(62), LV_PCT(25));
            lv_obj_set_size(obj, LV_PCT(22), LV_PCT(7));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_title(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "--");
        }
        {
            // water_black_bar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.water_black_bar = obj;
            lv_obj_set_pos(obj, LV_PCT(66), LV_PCT(33));
            lv_obj_set_size(obj, LV_PCT(13), LV_PCT(35));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_bar_black(obj);
        }
        {
            // water_black_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.water_black_label = obj;
            lv_obj_set_pos(obj, LV_PCT(62), LV_PCT(71));
            lv_obj_set_size(obj, LV_PCT(22), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_body(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Black");
        }
        {
            // water_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.water_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // water_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.water_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // water_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.water_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // water_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.water_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_water();
}

void tick_screen_page_water() {
}

void create_screen_page_air() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_air = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // air_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.air_arc = obj;
            lv_obj_set_pos(obj, LV_PCT(4), LV_PCT(4));
            lv_obj_set_size(obj, LV_PCT(92), LV_PCT(92));
            lv_arc_set_range(obj, 400, 2000);
            lv_arc_set_value(obj, 640);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_arc_thin(obj);
        }
        {
            // air_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.air_title = obj;
            lv_obj_set_pos(obj, LV_PCT(25), LV_PCT(21));
            lv_obj_set_size(obj, LV_PCT(50), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "AIR QUALITY");
        }
        {
            // air_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.air_value = obj;
            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(27));
            lv_obj_set_size(obj, LV_PCT(70), LV_PCT(19));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            add_style_label_hero_xl(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "640");
        }
        {
            // air_unit
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.air_unit = obj;
            lv_obj_set_pos(obj, LV_PCT(40), LV_PCT(51));
            lv_obj_set_size(obj, LV_PCT(20), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_muted(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "ppm");
        }
        {
            // air_status
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.air_status = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(57));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(7));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_status(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Good");
        }
        {
            // air_metrics
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.air_metrics = obj;
            lv_obj_set_pos(obj, LV_PCT(18), LV_PCT(69));
            lv_obj_set_size(obj, LV_PCT(64), LV_PCT(11));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // air_voc_col
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.air_voc_col = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(33), LV_PCT(100));
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // air_voc_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.air_voc_label = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(47));
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_label_muted(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "VOC");
                        }
                        {
                            // air_voc
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.air_voc = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(47));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(53));
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_label_metric(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "120 ppb");
                        }
                    }
                }
                {
                    // air_humidity_col
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.air_humidity_col = obj;
                    lv_obj_set_pos(obj, LV_PCT(33), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(33), LV_PCT(100));
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // air_humidity_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.air_humidity_label = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(47));
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_label_muted(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Humidity");
                        }
                        {
                            // air_humidity
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.air_humidity = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(47));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(53));
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_label_metric(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "44%");
                        }
                    }
                }
                {
                    // air_temp_col
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.air_temp_col = obj;
                    lv_obj_set_pos(obj, LV_PCT(67), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(33), LV_PCT(100));
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_plain(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // air_temp_label
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.air_temp_label = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(47));
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_label_muted(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "Temp");
                        }
                        {
                            // air_temp
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.air_temp = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(47));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(53));
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_label_metric(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "70°F");
                        }
                    }
                }
            }
        }
        {
            // air_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.air_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // air_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.air_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // air_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.air_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // air_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.air_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_air();
}

void tick_screen_page_air() {
}

void create_screen_page_level() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_level = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // level_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.level_title = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(13));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "LEVELING");
        }
        {
            // level_well
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.level_well = obj;
            lv_obj_set_pos(obj, LV_PCT(27), LV_PCT(23));
            lv_obj_set_size(obj, LV_PCT(46), LV_PCT(46));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_level_well(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // level_cross_v
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.level_cross_v = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                    static lv_point_precise_t line_points[] = {
                        { 80, 0 },
                        { 80, 161 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    add_style_level_line(obj);
                }
                {
                    // level_cross_h
                    lv_obj_t *obj = lv_line_create(parent_obj);
                    objects.level_cross_h = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                    static lv_point_precise_t line_points[] = {
                        { 0, 80 },
                        { 161, 80 }
                    };
                    lv_line_set_points(obj, line_points, 2);
                    add_style_level_line(obj);
                }
                {
                    // level_ring
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.level_ring = obj;
                    lv_obj_set_pos(obj, 58, 58);
                    lv_obj_set_size(obj, 45, 45);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_level_ring(obj);
                }
                {
                    // level_bubble
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.level_bubble = obj;
                    lv_obj_set_pos(obj, 64, 64);
                    lv_obj_set_size(obj, 33, 33);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_level_bubble(obj);
                }
            }
        }
        {
            // level_status
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.level_status = obj;
            lv_obj_set_pos(obj, LV_PCT(18), LV_PCT(71));
            lv_obj_set_size(obj, LV_PCT(64), LV_PCT(7));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_status(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "--");
        }
        {
            // level_detail
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.level_detail = obj;
            lv_obj_set_pos(obj, LV_PCT(18), LV_PCT(78));
            lv_obj_set_size(obj, LV_PCT(64), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_body(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "--");
        }
        {
            // level_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.level_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // level_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.level_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // level_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.level_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // level_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.level_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_level();
}

void tick_screen_page_level() {
}

void create_screen_page_settings() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_settings = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // settings_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.settings_title = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(13));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "SETTINGS");
        }
        {
            // settings_hero
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_hero = obj;
            lv_obj_set_pos(obj, LV_PCT(36), LV_PCT(26));
            lv_obj_set_size(obj, LV_PCT(29), LV_PCT(29));
            lv_obj_add_event_cb(obj, action_settings_activate, LV_EVENT_CLICKED, (void *)0);
            add_style_settings_tile(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // settings_hero_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.settings_hero_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_settings_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // settings_prev
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_prev = obj;
            lv_obj_set_pos(obj, LV_PCT(3), LV_PCT(39));
            lv_obj_set_size(obj, LV_PCT(20), LV_PCT(20));
            lv_obj_add_event_cb(obj, action_nav_step, LV_EVENT_CLICKED, (void *)0);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // settings_prev_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.settings_prev_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_device_neighbour_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // settings_next
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_next = obj;
            lv_obj_set_pos(obj, LV_PCT(77), LV_PCT(39));
            lv_obj_set_size(obj, LV_PCT(20), LV_PCT(20));
            lv_obj_add_event_cb(obj, action_nav_step, LV_EVENT_CLICKED, (void *)1);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // settings_next_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.settings_next_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_device_neighbour_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // settings_name
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.settings_name = obj;
            lv_obj_set_pos(obj, LV_PCT(22), LV_PCT(59));
            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_menu_name(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Wi-Fi");
        }
        {
            // settings_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.settings_value = obj;
            lv_obj_set_pos(obj, LV_PCT(22), LV_PCT(69));
            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_settings_value(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Headwaters");
        }
        {
            // settings_dot0
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot0 = obj;
            lv_obj_set_pos(obj, 270, 306);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot1
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot1 = obj;
            lv_obj_set_pos(obj, 256, 315);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot2
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot2 = obj;
            lv_obj_set_pos(obj, 241, 323);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot3
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot3 = obj;
            lv_obj_set_pos(obj, 226, 329);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot4
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot4 = obj;
            lv_obj_set_pos(obj, 209, 333);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot5
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot5 = obj;
            lv_obj_set_pos(obj, 193, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot6
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot6 = obj;
            lv_obj_set_pos(obj, 176, 337);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot7
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot7 = obj;
            lv_obj_set_pos(obj, 159, 336);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot8
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot8 = obj;
            lv_obj_set_pos(obj, 143, 333);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot9
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot9 = obj;
            lv_obj_set_pos(obj, 126, 329);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot10
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot10 = obj;
            lv_obj_set_pos(obj, 111, 323);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot11
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot11 = obj;
            lv_obj_set_pos(obj, 96, 315);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_dot12
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_dot12 = obj;
            lv_obj_set_pos(obj, 82, 306);
            lv_obj_set_size(obj, 8, 8);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_dot(obj);
            lv_obj_set_style_radius(obj, 1000, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // settings_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.settings_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // settings_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.settings_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // settings_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.settings_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // settings_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.settings_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_settings();
}

void tick_screen_page_settings() {
}

void create_screen_page_alert() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_alert = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // alert_bg
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.alert_bg = obj;
            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
            lv_obj_add_event_cb(obj, action_alert_dismiss, LV_EVENT_CLICKED, (void *)0);
            add_style_alert_ring(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // alert_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alert_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(30), LV_PCT(27));
                    lv_obj_set_size(obj, LV_PCT(40), LV_PCT(16));
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_alert_icon(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
                {
                    // alert_title
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alert_title = obj;
                    lv_obj_set_pos(obj, LV_PCT(12), LV_PCT(46));
                    lv_obj_set_size(obj, LV_PCT(76), LV_PCT(9));
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_label_title(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "--");
                }
                {
                    // alert_message
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alert_message = obj;
                    lv_obj_set_pos(obj, LV_PCT(12), LV_PCT(56));
                    lv_obj_set_size(obj, LV_PCT(76), LV_PCT(13));
                    add_style_label_body(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "--");
                }
                {
                    // alert_hint
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.alert_hint = obj;
                    lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(70));
                    lv_obj_set_size(obj, LV_PCT(70), LV_PCT(7));
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_label_muted(obj);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Tap to snooze");
                }
            }
        }
    }
    
    tick_screen_page_alert();
}

void tick_screen_page_alert() {
}

void create_screen_page_setup() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_setup = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // setup_logo
            lv_obj_t *obj = lv_image_create(parent_obj);
            objects.setup_logo = obj;
            lv_obj_set_pos(obj, LV_PCT(43), LV_PCT(4));
            lv_obj_set_size(obj, 52, 52);
            lv_image_set_src(obj, &img_tiercel_logo);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_ADV_HITTEST|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
        }
        {
            // setup_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.setup_title = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(20));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "WI-FI SETUP");
        }
        {
            // setup_ssid_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.setup_ssid_label = obj;
            lv_obj_set_pos(obj, LV_PCT(16), LV_PCT(27));
            lv_obj_set_size(obj, LV_PCT(68), LV_PCT(6));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_muted(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Join this network");
        }
        {
            // setup_ssid
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.setup_ssid = obj;
            lv_obj_set_pos(obj, LV_PCT(12), LV_PCT(33));
            lv_obj_set_size(obj, LV_PCT(76), LV_PCT(8));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_title(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Tiercel-------");
        }
        {
            // setup_pass_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.setup_pass_label = obj;
            lv_obj_set_pos(obj, LV_PCT(16), LV_PCT(44));
            lv_obj_set_size(obj, LV_PCT(68), LV_PCT(6));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_muted(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Password");
        }
        {
            // setup_pass
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.setup_pass = obj;
            lv_obj_set_pos(obj, LV_PCT(12), LV_PCT(50));
            lv_obj_set_size(obj, LV_PCT(76), LV_PCT(8));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_title(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "--------");
        }
        {
            // setup_url
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.setup_url = obj;
            lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(62));
            lv_obj_set_size(obj, LV_PCT(80), LV_PCT(7));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_body(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "http://192.168.4.1");
        }
        {
            // setup_status
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.setup_status = obj;
            lv_obj_set_pos(obj, LV_PCT(12), LV_PCT(71));
            lv_obj_set_size(obj, LV_PCT(76), LV_PCT(7));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_muted(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Waiting for phone...");
        }
    }
    
    tick_screen_page_setup();
}

void tick_screen_page_setup() {
}

void create_screen_page_locale() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_locale = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // locale_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.locale_title = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(25));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "LOCALE");
        }
        {
            // locale_list
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.locale_list = obj;
            lv_obj_set_pos(obj, LV_PCT(19), LV_PCT(33));
            lv_obj_set_size(obj, LV_PCT(63), LV_PCT(42));
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // locale_item0
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.locale_item0 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(30));
                    lv_obj_add_event_cb(obj, action_locale_toggle, LV_EVENT_CLICKED, (void *)0);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // locale_item0_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item0_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(7), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(8), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // locale_item0_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item0_title = obj;
                            lv_obj_set_pos(obj, LV_PCT(19), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Temperature");
                        }
                        {
                            // locale_item0_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item0_value = obj;
                            lv_obj_set_pos(obj, LV_PCT(75), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(18), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "°F");
                        }
                    }
                }
                {
                    // locale_item1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.locale_item1 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(35));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(30));
                    lv_obj_add_event_cb(obj, action_locale_toggle, LV_EVENT_CLICKED, (void *)1);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // locale_item1_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item1_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(7), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(8), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // locale_item1_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item1_title = obj;
                            lv_obj_set_pos(obj, LV_PCT(19), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Leveling");
                        }
                        {
                            // locale_item1_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item1_value = obj;
                            lv_obj_set_pos(obj, LV_PCT(75), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(18), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "in");
                        }
                    }
                }
                {
                    // locale_item2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.locale_item2 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(70));
                    lv_obj_set_size(obj, LV_PCT(100), LV_PCT(30));
                    lv_obj_add_event_cb(obj, action_locale_toggle, LV_EVENT_CLICKED, (void *)2);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // locale_item2_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item2_icon = obj;
                            lv_obj_set_pos(obj, LV_PCT(7), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(8), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // locale_item2_title
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item2_title = obj;
                            lv_obj_set_pos(obj, LV_PCT(19), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(56), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Clock");
                        }
                        {
                            // locale_item2_value
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.locale_item2_value = obj;
                            lv_obj_set_pos(obj, LV_PCT(75), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(18), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "12 h");
                        }
                    }
                }
            }
        }
        {
            // locale_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.locale_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // locale_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.locale_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // locale_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.locale_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // locale_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.locale_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_locale();
}

void tick_screen_page_locale() {
}

void create_screen_page_clock_face() {
    screen_page_clock_face_state_t *state = &screen_page_clock_face_state;
    (void)state;
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_clock_face = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // cfp_window
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cfp_window = obj;
            lv_obj_set_pos(obj, 72, 27);
            lv_obj_set_size(obj, 216, 216);
            lv_obj_add_event_cb(obj, action_clock_face_set, LV_EVENT_CLICKED, (void *)0);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            add_style_face_preview(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // cfp_cl
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cfp_cl = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 360, 360);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_face_scaled(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // cfp_cl_ticks
                            lv_obj_t *obj = lv_scale_create(parent_obj);
                            objects.cfp_cl_ticks = obj;
                            lv_obj_set_pos(obj, 11, 11);
                            lv_obj_set_size(obj, 338, 338);
                            lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                            lv_scale_set_range(obj, 0, 59);
                            lv_scale_set_angle_range(obj, 354);
                            lv_scale_set_rotation(obj, 270);
                            lv_scale_set_total_tick_count(obj, 60);
                            lv_scale_set_major_tick_every(obj, 5);
                            lv_scale_set_label_show(obj, false);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_face_ticks(obj);
                        }
                        {
                            // cfp_cl_day
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_cl_day = obj;
                            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(16));
                            lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_text_lg(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Tuesday");
                        }
                        {
                            // cfp_cl_date
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_cl_date = obj;
                            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(22));
                            lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_text_md(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "September 29, 2026");
                        }
                        {
                            // cfp_cl_inside
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_cl_inside = obj;
                            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(59));
                            lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_text_lg(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inside 67°");
                        }
                        {
                            // cfp_cl_moderow
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_cl_moderow = obj;
                            lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(66));
                            lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // cfp_cl_mode
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.cfp_cl_mode = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    add_style_plain(obj);
                                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // cfp_cl_mode_icon
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_cl_mode_icon = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            add_style_face_mode_icon(obj);
                                            lv_label_set_text_static(obj, "");
                                        }
                                        {
                                            // cfp_cl_mode_text
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_cl_mode_text = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                            add_style_face_mode(obj);
                                            lv_label_set_text(obj, "Heating to 72°");
                                        }
                                    }
                                }
                            }
                        }
                        {
                            // cfp_cl_stats
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_cl_stats = obj;
                            lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(73));
                            lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // cfp_cl_batt
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.cfp_cl_batt = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    add_style_plain(obj);
                                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // cfp_cl_batt_icon
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_cl_batt_icon = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            add_style_face_stat_icon(obj);
                                            lv_label_set_text_static(obj, "");
                                        }
                                        {
                                            // cfp_cl_batt_value
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_cl_batt_value = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                            add_style_face_stat(obj);
                                            lv_label_set_text(obj, "82%");
                                        }
                                    }
                                }
                                {
                                    // cfp_cl_water
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.cfp_cl_water = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    add_style_plain(obj);
                                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // cfp_cl_water_icon
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_cl_water_icon = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            add_style_face_stat_icon(obj);
                                            lv_label_set_text_static(obj, "");
                                        }
                                        {
                                            // cfp_cl_water_value
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_cl_water_value = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                            add_style_face_stat(obj);
                                            lv_label_set_text(obj, "72%");
                                        }
                                    }
                                }
                            }
                        }
                        {
                            // cfp_cl_hand_h
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.cfp_cl_hand_h = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            static lv_point_precise_t line_points[] = {
                                { 180, 180 },
                                { 180, 97 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            add_style_face_hand_h(obj);
                        }
                        {
                            // cfp_cl_hand_m
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.cfp_cl_hand_m = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            static lv_point_precise_t line_points[] = {
                                { 180, 180 },
                                { 180, 52 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            add_style_face_hand_m(obj);
                        }
                        {
                            // cfp_cl_hand_s
                            lv_obj_t *obj = lv_line_create(parent_obj);
                            objects.cfp_cl_hand_s = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            static lv_point_precise_t line_points[] = {
                                { 180, 202 },
                                { 180, 37 }
                            };
                            lv_line_set_points(obj, line_points, 2);
                            add_style_face_hand_s(obj);
                        }
                        {
                            // cfp_cl_cap
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_cl_cap = obj;
                            lv_obj_set_pos(obj, LV_PCT(48), LV_PCT(48));
                            lv_obj_set_size(obj, LV_PCT(4), LV_PCT(4));
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_face_cap(obj);
                        }
                        {
                            // cfp_cl_cap_in
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_cl_cap_in = obj;
                            lv_obj_set_pos(obj, LV_PCT(49), LV_PCT(49));
                            lv_obj_set_size(obj, LV_PCT(1), LV_PCT(1));
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_face_cap_in(obj);
                        }
                    }
                }
                {
                    // cfp_dg
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cfp_dg = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 360, 360);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_face_scaled(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // cfp_dg_ticks
                            lv_obj_t *obj = lv_scale_create(parent_obj);
                            objects.cfp_dg_ticks = obj;
                            lv_obj_set_pos(obj, 11, 11);
                            lv_obj_set_size(obj, 338, 338);
                            lv_scale_set_mode(obj, LV_SCALE_MODE_ROUND_INNER);
                            lv_scale_set_range(obj, 0, 59);
                            lv_scale_set_angle_range(obj, 354);
                            lv_scale_set_rotation(obj, 270);
                            lv_scale_set_total_tick_count(obj, 60);
                            lv_scale_set_major_tick_every(obj, 5);
                            lv_scale_set_label_show(obj, false);
                            {
                                state->scale_section = lv_scale_add_section(obj);
                                lv_scale_section_set_style(state->scale_section, LV_PART_MAIN, get_style_face_sec_on_MAIN_DEFAULT());
                                lv_scale_section_set_style(state->scale_section, LV_PART_INDICATOR, get_style_face_sec_on_INDICATOR_DEFAULT());
                                lv_scale_section_set_style(state->scale_section, LV_PART_ITEMS, get_style_face_sec_on_ITEMS_DEFAULT());
                            }
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_face_sec_ticks(obj);
                        }
                        {
                            // cfp_dg_date
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_dg_date = obj;
                            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(20));
                            lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_text_lg(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Tuesday · September 29");
                        }
                        {
                            // cfp_dg_timerow
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_dg_timerow = obj;
                            lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(27));
                            lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // cfp_dg_time
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.cfp_dg_time = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_time(obj);
                                    lv_label_set_text(obj, "7:42");
                                }
                                {
                                    // cfp_dg_ampm
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.cfp_dg_ampm = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_am_pm(obj);
                                    lv_label_set_text(obj, "PM");
                                }
                            }
                        }
                        {
                            // cfp_dg_inside
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_dg_inside = obj;
                            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(63));
                            lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_text_lg(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Inside 67°");
                        }
                        {
                            // cfp_dg_moderow
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_dg_moderow = obj;
                            lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(69));
                            lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // cfp_dg_mode
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.cfp_dg_mode = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    add_style_plain(obj);
                                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // cfp_dg_mode_icon
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_dg_mode_icon = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            add_style_face_mode_icon(obj);
                                            lv_label_set_text_static(obj, "");
                                        }
                                        {
                                            // cfp_dg_mode_text
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_dg_mode_text = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                            add_style_face_mode(obj);
                                            lv_label_set_text(obj, "Heating to 72°");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                {
                    // cfp_br
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cfp_br = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 360, 360);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_face_scaled(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // cfp_br_arc
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.cfp_br_arc = obj;
                            lv_obj_set_pos(obj, 11, 11);
                            lv_obj_set_size(obj, 338, 338);
                            lv_arc_set_range(obj, 0, 3600);
                            lv_arc_set_value(obj, 2520);
                            lv_arc_set_bg_start_angle(obj, 0);
                            lv_arc_set_bg_end_angle(obj, 360);
                            lv_arc_set_rotation(obj, 270);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            add_style_face_min_arc(obj);
                        }
                        {
                            // cfp_br_logo
                            lv_obj_t *obj = lv_image_create(parent_obj);
                            objects.cfp_br_logo = obj;
                            lv_obj_set_pos(obj, LV_PCT(42), LV_PCT(14));
                            lv_obj_set_size(obj, 57, 57);
                            lv_image_set_src(obj, &img_tc_logo);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_ADV_HITTEST|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
                        }
                        {
                            // cfp_br_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_br_name = obj;
                            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(32));
                            lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_brand_name(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "TrailCurrent");
                        }
                        {
                            // cfp_br_timerow
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_br_timerow = obj;
                            lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(38));
                            lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_START, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // cfp_br_time
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.cfp_br_time = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_time_sm(obj);
                                    lv_label_set_text(obj, "7:42");
                                }
                                {
                                    // cfp_br_ampm
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.cfp_br_ampm = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_am_pm_sm(obj);
                                    lv_label_set_text(obj, "PM");
                                }
                            }
                        }
                        {
                            // cfp_br_date
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_br_date = obj;
                            lv_obj_set_pos(obj, LV_PCT(15), LV_PCT(65));
                            lv_obj_set_size(obj, LV_PCT(70), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_face_text_muted(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "Tuesday · September 29");
                        }
                        {
                            // cfp_br_bottom
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_br_bottom = obj;
                            lv_obj_set_pos(obj, LV_PCT(10), LV_PCT(72));
                            lv_obj_set_size(obj, LV_PCT(80), LV_SIZE_CONTENT);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_column(obj, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // cfp_br_inside
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.cfp_br_inside = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_label_body(obj);
                                    lv_label_set_text(obj, "Inside 67°");
                                }
                                {
                                    // cfp_br_mode
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.cfp_br_mode = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    add_style_plain(obj);
                                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // cfp_br_mode_icon
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_br_mode_icon = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            add_style_face_mode_icon(obj);
                                            lv_label_set_text_static(obj, "");
                                        }
                                        {
                                            // cfp_br_mode_text
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_br_mode_text = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                            add_style_face_mode(obj);
                                            lv_label_set_text(obj, "Heating to 72°");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                {
                    // cfp_rg
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cfp_rg = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 360, 360);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_face_scaled(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // cfp_rg_arc
                            lv_obj_t *obj = lv_arc_create(parent_obj);
                            objects.cfp_rg_arc = obj;
                            lv_obj_set_pos(obj, 11, 11);
                            lv_obj_set_size(obj, 338, 338);
                            lv_arc_set_range(obj, 0, 3600);
                            lv_arc_set_value(obj, 2520);
                            lv_arc_set_bg_start_angle(obj, 0);
                            lv_arc_set_bg_end_angle(obj, 360);
                            lv_arc_set_rotation(obj, 270);
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
                            add_style_face_ring_arc(obj);
                        }
                        {
                            // cfp_rg_h1
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h1 = obj;
                            lv_obj_set_pos(obj, LV_PCT(64), LV_PCT(14));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "1");
                        }
                        {
                            // cfp_rg_h2
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h2 = obj;
                            lv_obj_set_pos(obj, LV_PCT(79), LV_PCT(28));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "2");
                        }
                        {
                            // cfp_rg_h3
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h3 = obj;
                            lv_obj_set_pos(obj, LV_PCT(84), LV_PCT(47));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "3");
                        }
                        {
                            // cfp_rg_h4
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h4 = obj;
                            lv_obj_set_pos(obj, LV_PCT(79), LV_PCT(67));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "4");
                        }
                        {
                            // cfp_rg_h5
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h5 = obj;
                            lv_obj_set_pos(obj, LV_PCT(64), LV_PCT(81));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "5");
                        }
                        {
                            // cfp_rg_h6
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h6 = obj;
                            lv_obj_set_pos(obj, LV_PCT(45), LV_PCT(86));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "6");
                        }
                        {
                            // cfp_rg_h7
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h7 = obj;
                            lv_obj_set_pos(obj, LV_PCT(26), LV_PCT(81));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "7");
                        }
                        {
                            // cfp_rg_h8
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h8 = obj;
                            lv_obj_set_pos(obj, LV_PCT(11), LV_PCT(67));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "8");
                        }
                        {
                            // cfp_rg_h9
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h9 = obj;
                            lv_obj_set_pos(obj, LV_PCT(6), LV_PCT(47));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "9");
                        }
                        {
                            // cfp_rg_h10
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h10 = obj;
                            lv_obj_set_pos(obj, LV_PCT(11), LV_PCT(28));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "10");
                        }
                        {
                            // cfp_rg_h11
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h11 = obj;
                            lv_obj_set_pos(obj, LV_PCT(26), LV_PCT(14));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "11");
                        }
                        {
                            // cfp_rg_h12
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.cfp_rg_h12 = obj;
                            lv_obj_set_pos(obj, LV_PCT(45), LV_PCT(8));
                            lv_obj_set_size(obj, LV_PCT(10), LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_face_hour_num(obj);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "12");
                        }
                        {
                            // cfp_rg_col
                            lv_obj_t *obj = lv_obj_create(parent_obj);
                            objects.cfp_rg_col = obj;
                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                            lv_obj_set_size(obj, LV_PCT(100), LV_PCT(100));
                            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                            add_style_plain(obj);
                            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_COLUMN, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_pad_row(obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
                            {
                                lv_obj_t *parent_obj = obj;
                                {
                                    // cfp_rg_eyebrow
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.cfp_rg_eyebrow = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_label_eyebrow(obj);
                                    lv_label_set_text(obj, "INSIDE");
                                }
                                {
                                    // cfp_rg_temp
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.cfp_rg_temp = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_time_sm(obj);
                                    lv_label_set_text(obj, "67°");
                                }
                                {
                                    // cfp_rg_time
                                    lv_obj_t *obj = lv_label_create(parent_obj);
                                    objects.cfp_rg_time = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                    add_style_face_ring_time(obj);
                                    lv_label_set_text(obj, "7:42 PM");
                                }
                                {
                                    // cfp_rg_mode
                                    lv_obj_t *obj = lv_obj_create(parent_obj);
                                    objects.cfp_rg_mode = obj;
                                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                                    add_style_plain(obj);
                                    lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
                                    {
                                        lv_obj_t *parent_obj = obj;
                                        {
                                            // cfp_rg_mode_icon
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_rg_mode_icon = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                                            add_style_face_mode_icon(obj);
                                            lv_label_set_text_static(obj, "");
                                        }
                                        {
                                            // cfp_rg_mode_text
                                            lv_obj_t *obj = lv_label_create(parent_obj);
                                            objects.cfp_rg_mode_text = obj;
                                            lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                                            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                                            add_style_face_mode(obj);
                                            lv_label_set_text(obj, "Heating to 72°");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        {
            // cfp_name
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.cfp_name = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(70));
            lv_obj_set_size(obj, LV_PCT(60), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_title(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Classic");
        }
        {
            // cfp_sub
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.cfp_sub = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(77));
            lv_obj_set_size(obj, LV_PCT(60), LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_face_pick_sub(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Tap to use");
        }
        {
            // cfp_dots
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.cfp_dots = obj;
            lv_obj_set_pos(obj, LV_PCT(30), LV_PCT(84));
            lv_obj_set_size(obj, LV_PCT(40), LV_SIZE_CONTENT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(obj, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // cfp_dot0
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cfp_dot0 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, 6, 6);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_guide_dot(obj);
                }
                {
                    // cfp_dot1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cfp_dot1 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, 6, 6);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_guide_dot(obj);
                }
                {
                    // cfp_dot2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cfp_dot2 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, 6, 6);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_guide_dot(obj);
                }
                {
                    // cfp_dot3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.cfp_dot3 = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, 6, 6);
                    lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
                    add_style_guide_dot(obj);
                }
            }
        }
        {
            // cfp_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.cfp_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // cfp_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.cfp_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // cfp_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.cfp_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // cfp_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.cfp_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_clock_face();
}

void tick_screen_page_clock_face() {
    screen_page_clock_face_state_t *state = &screen_page_clock_face_state;
    (void)state;
    {
        int32_t new_min_val = get_var_clock_sec_min();
        int32_t new_max_val = get_var_clock_sec_max();
        lv_scale_section_set_range(state->scale_section, new_min_val, new_max_val);
    }
}

void create_screen_page_brightness() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_brightness = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // bright_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.bright_arc = obj;
            lv_obj_set_pos(obj, 14, 14);
            lv_obj_set_size(obj, 332, 332);
            lv_arc_set_range(obj, 5, 100);
            lv_arc_set_value(obj, 80);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_energy_arc(obj);
        }
        {
            // bright_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bright_title = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(27));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "BRIGHTNESS");
        }
        {
            // bright_row
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.bright_row = obj;
            lv_obj_set_pos(obj, 66, 140);
            lv_obj_set_size(obj, 228, 70);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_SCROLLABLE);
            add_style_plain(obj);
            lv_obj_set_style_layout(obj, LV_LAYOUT_FLEX, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_flow(obj, LV_FLEX_FLOW_ROW, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_main_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_cross_place(obj, LV_FLEX_ALIGN_END, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_flex_track_place(obj, LV_FLEX_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_column(obj, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bright_value
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.bright_value = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_pb_freq(obj);
                    lv_label_set_text(obj, "80");
                }
                {
                    // bright_unit
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.bright_unit = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_energy_unit(obj);
                    lv_label_set_text(obj, "%");
                }
            }
        }
        {
            // bright_hint
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bright_hint = obj;
            lv_obj_set_pos(obj, 90, 236);
            lv_obj_set_size(obj, 180, 18);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_muted(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Turn to adjust");
        }
        {
            // bright_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.bright_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // bright_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.bright_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // bright_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.bright_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bright_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.bright_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_brightness();
}

void tick_screen_page_brightness() {
}

void create_screen_page_bluetooth() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.page_bluetooth = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 360, 360);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    add_style_screen_root(obj);
    {
        lv_obj_t *parent_obj = obj;
        {
            // bt_title
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bt_title = obj;
            lv_obj_set_pos(obj, LV_PCT(20), LV_PCT(13));
            lv_obj_set_size(obj, LV_PCT(60), LV_PCT(5));
            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
            add_style_label_eyebrow(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "BLUETOOTH");
        }
        {
            // bt_status
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.bt_status = obj;
            lv_obj_set_pos(obj, 50, 76);
            lv_obj_set_size(obj, 260, 40);
            add_style_label_body(obj);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "");
        }
        {
            // bt_list
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.bt_list = obj;
            lv_obj_set_pos(obj, 50, 124);
            lv_obj_set_size(obj, 260, 164);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_scroll_dir(obj, LV_DIR_VER);
            add_style_plain(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bt_row0
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bt_row0 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 260, 44);
                    lv_obj_add_event_cb(obj, action_bluetooth_row, LV_EVENT_CLICKED, (void *)0);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // bt_row0_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row0_icon = obj;
                            lv_obj_set_pos(obj, 12, 0);
                            lv_obj_set_size(obj, 28, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // bt_row0_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row0_name = obj;
                            lv_obj_set_pos(obj, 48, 0);
                            lv_obj_set_size(obj, 200, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                    }
                }
                {
                    // bt_row1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bt_row1 = obj;
                    lv_obj_set_pos(obj, 0, 52);
                    lv_obj_set_size(obj, 260, 44);
                    lv_obj_add_event_cb(obj, action_bluetooth_row, LV_EVENT_CLICKED, (void *)1);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // bt_row1_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row1_icon = obj;
                            lv_obj_set_pos(obj, 12, 0);
                            lv_obj_set_size(obj, 28, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // bt_row1_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row1_name = obj;
                            lv_obj_set_pos(obj, 48, 0);
                            lv_obj_set_size(obj, 200, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                    }
                }
                {
                    // bt_row2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bt_row2 = obj;
                    lv_obj_set_pos(obj, 0, 104);
                    lv_obj_set_size(obj, 260, 44);
                    lv_obj_add_event_cb(obj, action_bluetooth_row, LV_EVENT_CLICKED, (void *)2);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // bt_row2_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row2_icon = obj;
                            lv_obj_set_pos(obj, 12, 0);
                            lv_obj_set_size(obj, 28, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // bt_row2_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row2_name = obj;
                            lv_obj_set_pos(obj, 48, 0);
                            lv_obj_set_size(obj, 200, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                    }
                }
                {
                    // bt_row3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bt_row3 = obj;
                    lv_obj_set_pos(obj, 0, 156);
                    lv_obj_set_size(obj, 260, 44);
                    lv_obj_add_event_cb(obj, action_bluetooth_row, LV_EVENT_CLICKED, (void *)3);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // bt_row3_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row3_icon = obj;
                            lv_obj_set_pos(obj, 12, 0);
                            lv_obj_set_size(obj, 28, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // bt_row3_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row3_name = obj;
                            lv_obj_set_pos(obj, 48, 0);
                            lv_obj_set_size(obj, 200, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                    }
                }
                {
                    // bt_row4
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bt_row4 = obj;
                    lv_obj_set_pos(obj, 0, 208);
                    lv_obj_set_size(obj, 260, 44);
                    lv_obj_add_event_cb(obj, action_bluetooth_row, LV_EVENT_CLICKED, (void *)4);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // bt_row4_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row4_icon = obj;
                            lv_obj_set_pos(obj, 12, 0);
                            lv_obj_set_size(obj, 28, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // bt_row4_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row4_name = obj;
                            lv_obj_set_pos(obj, 48, 0);
                            lv_obj_set_size(obj, 200, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                    }
                }
                {
                    // bt_row5
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bt_row5 = obj;
                    lv_obj_set_pos(obj, 0, 260);
                    lv_obj_set_size(obj, 260, 44);
                    lv_obj_add_event_cb(obj, action_bluetooth_row, LV_EVENT_CLICKED, (void *)5);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // bt_row5_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row5_icon = obj;
                            lv_obj_set_pos(obj, 12, 0);
                            lv_obj_set_size(obj, 28, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // bt_row5_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row5_name = obj;
                            lv_obj_set_pos(obj, 48, 0);
                            lv_obj_set_size(obj, 200, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                    }
                }
                {
                    // bt_row6
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bt_row6 = obj;
                    lv_obj_set_pos(obj, 0, 312);
                    lv_obj_set_size(obj, 260, 44);
                    lv_obj_add_event_cb(obj, action_bluetooth_row, LV_EVENT_CLICKED, (void *)6);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // bt_row6_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row6_icon = obj;
                            lv_obj_set_pos(obj, 12, 0);
                            lv_obj_set_size(obj, 28, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // bt_row6_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row6_name = obj;
                            lv_obj_set_pos(obj, 48, 0);
                            lv_obj_set_size(obj, 200, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                    }
                }
                {
                    // bt_row7
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.bt_row7 = obj;
                    lv_obj_set_pos(obj, 0, 364);
                    lv_obj_set_size(obj, 260, 44);
                    lv_obj_add_event_cb(obj, action_bluetooth_row, LV_EVENT_CLICKED, (void *)7);
                    lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
                    add_style_list_row(obj);
                    {
                        lv_obj_t *parent_obj = obj;
                        {
                            // bt_row7_icon
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row7_icon = obj;
                            lv_obj_set_pos(obj, 12, 0);
                            lv_obj_set_size(obj, 28, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                            add_style_mode_row_icon(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text_static(obj, "");
                        }
                        {
                            // bt_row7_name
                            lv_obj_t *obj = lv_label_create(parent_obj);
                            objects.bt_row7_name = obj;
                            lv_obj_set_pos(obj, 48, 0);
                            lv_obj_set_size(obj, 200, 24);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            add_style_mode_row_text(obj);
                            lv_obj_set_style_align(obj, LV_ALIGN_LEFT_MID, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_label_set_text(obj, "");
                        }
                    }
                }
            }
        }
        {
            // bt_search
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.bt_search = obj;
            lv_obj_set_pos(obj, 96, 296);
            lv_obj_set_size(obj, 80, 40);
            lv_obj_add_event_cb(obj, action_bluetooth_search, LV_EVENT_CLICKED, (void *)0);
            add_style_list_row(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bt_search_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.bt_search_label = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_label_body(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Search");
                }
            }
        }
        {
            // bt_forget
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.bt_forget = obj;
            lv_obj_set_pos(obj, 184, 296);
            lv_obj_set_size(obj, 80, 40);
            lv_obj_add_event_cb(obj, action_bluetooth_forget, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            add_style_list_row(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bt_forget_label
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.bt_forget_label = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                    add_style_label_body(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Forget");
                }
            }
        }
        {
            // bt_swipe_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.bt_swipe_arc = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, 346, 346);
            lv_arc_set_value(obj, 100);
            lv_arc_set_bg_start_angle(obj, 140);
            lv_arc_set_bg_end_angle(obj, 220);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // bt_hold_arc
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.bt_hold_arc = obj;
            lv_obj_set_pos(obj, 4, 4);
            lv_obj_set_size(obj, 352, 352);
            lv_arc_set_value(obj, 0);
            lv_arc_set_bg_start_angle(obj, 0);
            lv_arc_set_bg_end_angle(obj, 360);
            lv_arc_set_rotation(obj, 270);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            add_style_gesture_arc(obj);
        }
        {
            // bt_back_chip
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.bt_back_chip = obj;
            lv_obj_set_pos(obj, 150, 6);
            lv_obj_set_size(obj, 60, 42);
            lv_obj_add_event_cb(obj, action_nav_back, LV_EVENT_CLICKED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_SCROLL_WITH_ARROW);
            add_style_back_chip(obj);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // bt_back_chip_icon
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.bt_back_chip_icon = obj;
                    lv_obj_set_pos(obj, LV_PCT(0), LV_PCT(0));
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
                    add_style_back_chip_icon(obj);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_page_bluetooth();
}

void tick_screen_page_bluetooth() {
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_page_idle,
    tick_screen_page_menu,
    tick_screen_page_climate,
    tick_screen_page_climate_mode,
    tick_screen_page_devices,
    tick_screen_page_peregrine,
    tick_screen_page_playbill,
    tick_screen_page_energy,
    tick_screen_page_water,
    tick_screen_page_air,
    tick_screen_page_level,
    tick_screen_page_settings,
    tick_screen_page_alert,
    tick_screen_page_setup,
    tick_screen_page_locale,
    tick_screen_page_clock_face,
    tick_screen_page_brightness,
    tick_screen_page_bluetooth,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 18) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
    { "rl56", &ui_font_rl56 },
    { "rn112", &ui_font_rn112 },
    { "rn90", &ui_font_rn90 },
    { "rn64", &ui_font_rn64 },
    { "rm26", &ui_font_rm26 },
    { "rm22", &ui_font_rm22 },
    { "rm20", &ui_font_rm20 },
    { "rr22", &ui_font_rr22 },
    { "rr18", &ui_font_rr18 },
    { "rr15", &ui_font_rr15 },
    { "rr14", &ui_font_rr14 },
    { "rr13", &ui_font_rr13 },
    { "rr11", &ui_font_rr11 },
    { "fa42", &ui_font_fa42 },
    { "fa28", &ui_font_fa28 },
    { "fa20", &ui_font_fa20 },
    { "fa16", &ui_font_fa16 },
    { "fh51", &ui_font_fh51 },
    { "fh33", &ui_font_fh33 },
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;
void change_color_theme(uint32_t theme_index) {
    active_theme_index = theme_index;
    
    lv_style_set_bg_color(get_style_screen_root_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][1]));
    lv_style_set_text_color(get_style_screen_root_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_card_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_card_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_card_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_card_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][10]));
    lv_style_set_border_color(get_style_card_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_card_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_card_selected_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][10]));
    lv_style_set_border_color(get_style_card_selected_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_card_selected_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_label_title_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_label_body_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_label_muted_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_label_hero_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_button_primary_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_button_primary_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][27]));
    lv_style_set_bg_color(get_style_button_neutral_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_button_neutral_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_button_neutral_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_line_color(get_style_scale_ring_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][25]));
    lv_style_set_line_color(get_style_scale_ring_ITEMS_DEFAULT(), lv_color_hex(theme_colors[theme_index][25]));
    lv_style_set_line_color(get_style_scale_ring_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][16]));
    lv_style_set_text_color(get_style_scale_ring_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][16]));
    lv_style_set_text_color(get_style_label_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_label_icon_sm_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_bg_color(get_style_hero_tile_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_hero_tile_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_shadow_color(get_style_hero_tile_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_hero_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_neighbour_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_device_tile_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_device_tile_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_device_tile_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][10]));
    lv_style_set_border_color(get_style_device_tile_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_shadow_color(get_style_device_tile_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_device_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_device_icon_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_device_neighbour_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_alert_ring_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][1]));
    lv_style_set_border_color(get_style_alert_ring_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][14]));
    lv_style_set_text_color(get_style_alert_ring_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_level_well_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][27]));
    lv_style_set_border_color(get_style_level_well_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_border_color(get_style_level_ring_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_line_color(get_style_level_line_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_level_bubble_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][11]));
    lv_style_set_shadow_color(get_style_level_bubble_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][11]));
    lv_style_set_line_color(get_style_climate_ticks_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_line_color(get_style_climate_section_heat_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][14]));
    lv_style_set_line_color(get_style_climate_section_cool_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][13]));
    lv_style_set_line_color(get_style_climate_section_hold_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_line_color(get_style_climate_needle_inside_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_line_color(get_style_climate_needle_target_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_climate_num_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_climate_mode_text_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_climate_mode_text_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][31]));
    lv_style_set_text_color(get_style_climate_mode_text_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][30]));
    lv_style_set_text_color(get_style_climate_mode_text_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_climate_mode_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_climate_mode_icon_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][31]));
    lv_style_set_text_color(get_style_climate_mode_icon_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][30]));
    lv_style_set_text_color(get_style_climate_mode_icon_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_arc_color(get_style_energy_arc_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_arc_color(get_style_energy_arc_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_arc_color(get_style_energy_arc_INDICATOR_CHECKED(), lv_color_hex(theme_colors[theme_index][11]));
    lv_style_set_arc_color(get_style_energy_arc_INDICATOR_PRESSED(), lv_color_hex(theme_colors[theme_index][13]));
    lv_style_set_arc_color(get_style_energy_arc_INDICATOR_DISABLED(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_energy_head_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_energy_head_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][29]));
    lv_style_set_text_color(get_style_energy_head_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][30]));
    lv_style_set_text_color(get_style_energy_head_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_energy_head_icon_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][29]));
    lv_style_set_text_color(get_style_energy_head_icon_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][30]));
    lv_style_set_text_color(get_style_energy_unit_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_bg_color(get_style_settings_tile_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_settings_tile_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_shadow_color(get_style_settings_tile_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_border_color(get_style_settings_tile_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][14]));
    lv_style_set_shadow_color(get_style_settings_tile_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][14]));
    lv_style_set_text_color(get_style_settings_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_settings_icon_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][14]));
    lv_style_set_text_color(get_style_settings_value_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_settings_value_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_settings_value_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][29]));
    lv_style_set_text_color(get_style_settings_value_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][31]));
    lv_style_set_border_color(get_style_list_row_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_list_row_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_bg_color(get_style_list_row_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_list_row_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_list_row_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_guide_dot_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_guide_dot_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_guide_dot_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_line_color(get_style_face_ticks_ITEMS_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_line_color(get_style_face_ticks_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_line_color(get_style_face_sec_ticks_ITEMS_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_line_color(get_style_face_sec_ticks_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_line_color(get_style_face_sec_on_ITEMS_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_line_color(get_style_face_sec_on_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_line_color(get_style_face_hand_h_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_line_color(get_style_face_hand_m_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_line_color(get_style_face_hand_s_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_face_cap_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_face_cap_in_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][1]));
    lv_style_set_arc_color(get_style_face_min_arc_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_arc_color(get_style_face_min_arc_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_face_min_arc_KNOB_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_arc_color(get_style_face_ring_arc_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_arc_color(get_style_face_ring_arc_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_face_ring_arc_KNOB_DEFAULT(), lv_color_hex(theme_colors[theme_index][1]));
    lv_style_set_border_color(get_style_face_ring_arc_KNOB_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_face_text_lg_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_face_text_md_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_face_text_muted_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_stat_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_stat_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_brand_name_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_face_time_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_face_time_sm_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_face_am_pm_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_am_pm_sm_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_ring_time_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_face_mode_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_face_mode_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][31]));
    lv_style_set_text_color(get_style_face_mode_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][30]));
    lv_style_set_text_color(get_style_face_mode_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_mode_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_face_mode_icon_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][31]));
    lv_style_set_text_color(get_style_face_mode_icon_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][30]));
    lv_style_set_text_color(get_style_face_mode_icon_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_hour_num_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_hour_num_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_face_preview_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][1]));
    lv_style_set_border_color(get_style_face_preview_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_face_pick_sub_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_face_pick_sub_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_menu_name_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_menu_summary_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_border_color(get_style_mode_row_heat_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_mode_row_heat_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_mode_row_heat_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_mode_row_heat_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][14]));
    lv_style_set_text_color(get_style_mode_row_heat_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][31]));
    lv_style_set_border_color(get_style_mode_row_cool_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_mode_row_cool_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_mode_row_cool_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_mode_row_cool_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][13]));
    lv_style_set_text_color(get_style_mode_row_cool_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][30]));
    lv_style_set_border_color(get_style_mode_row_auto_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_mode_row_auto_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_mode_row_auto_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_mode_row_auto_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_mode_row_auto_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_border_color(get_style_mode_row_off_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_mode_row_off_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_mode_row_off_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_mode_row_off_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_mode_row_off_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_label_body_muted_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_energy_dot_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_energy_dot_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_alert_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][31]));
    lv_style_set_bg_color(get_style_dot_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_dot_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_pivot_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_line_color(get_style_hand_hour_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_line_color(get_style_hand_minute_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_line_color(get_style_hand_second_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_bar_level_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][4]));
    lv_style_set_bg_color(get_style_bar_level_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_bar_fresh_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_bar_fresh_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_bar_fresh_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][18]));
    lv_style_set_bg_grad_color(get_style_bar_fresh_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][17]));
    lv_style_set_bg_color(get_style_bar_grey_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_bar_grey_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_bar_grey_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][23]));
    lv_style_set_bg_color(get_style_bar_black_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_bar_black_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_bar_black_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][24]));
    lv_style_set_bg_color(get_style_field_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_field_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_field_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_field_TEXTAREA_PLACEHOLDER_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_field_CURSOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_arc_color(get_style_arc_value_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_arc_color(get_style_arc_value_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_arc_color(get_style_arc_thin_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_arc_color(get_style_arc_thin_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_arc_color(get_style_arc_thin_INDICATOR_CHECKED(), lv_color_hex(theme_colors[theme_index][11]));
    lv_style_set_arc_color(get_style_arc_thin_INDICATOR_DISABLED(), lv_color_hex(theme_colors[theme_index][14]));
    lv_style_set_arc_color(get_style_arc_thin_INDICATOR_PRESSED(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_label_eyebrow_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_label_hero_xl_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_label_metric_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_label_status_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_label_status_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][29]));
    lv_style_set_text_color(get_style_label_status_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][31]));
    lv_style_set_text_color(get_style_label_status_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_bg_color(get_style_back_chip_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_back_chip_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_back_chip_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][3]));
    lv_style_set_text_color(get_style_back_chip_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_arc_color(get_style_gesture_arc_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_arc_color(get_style_vol_arc_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_arc_color(get_style_vol_arc_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_border_color(get_style_climate_pill_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_border_color(get_style_climate_pill_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][14]));
    lv_style_set_border_color(get_style_climate_pill_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][13]));
    lv_style_set_border_color(get_style_climate_pill_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_pg_eyebrow_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_pg_eyebrow_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_bg_color(get_style_pg_orb_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_pg_orb_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_pg_orb_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][10]));
    lv_style_set_bg_color(get_style_pg_orb_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_shadow_color(get_style_pg_orb_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_border_color(get_style_pg_orb_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_pg_orb_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_pg_orb_icon_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][27]));
    lv_style_set_line_color(get_style_pg_ring_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][25]));
    lv_style_set_line_color(get_style_pg_ring_on_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_arc_color(get_style_pg_spin_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_arc_color(get_style_pg_spin_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_text_color(get_style_pg_q_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_pg_q_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_pg_q_MAIN_DISABLED(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_pg_a_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_pb_seg_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_pb_seg_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_pb_seg_btn_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_pb_seg_btn_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][3]));
    lv_style_set_text_color(get_style_pb_seg_text_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_pb_seg_text_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][27]));
    lv_style_set_text_color(get_style_pb_eyebrow_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_pb_eyebrow_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_pb_title_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_pb_progress_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_pb_progress_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_pb_time_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_text_color(get_style_pb_freq_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_text_color(get_style_pb_freq_unit_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_bg_color(get_style_pb_round_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][2]));
    lv_style_set_border_color(get_style_pb_round_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_bg_color(get_style_pb_round_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][3]));
    lv_style_set_text_color(get_style_pb_round_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_style_set_bg_color(get_style_pb_play_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_shadow_color(get_style_pb_play_MAIN_CHECKED(), lv_color_hex(theme_colors[theme_index][9]));
    lv_style_set_bg_color(get_style_pb_play_MAIN_PRESSED(), lv_color_hex(theme_colors[theme_index][28]));
    lv_style_set_text_color(get_style_pb_play_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][27]));
    lv_style_set_border_color(get_style_pb_chip_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_text_color(get_style_pb_chip_icon_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_text_color(get_style_pb_chip_text_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][7]));
    lv_style_set_line_color(get_style_pb_tune_ITEMS_DEFAULT(), lv_color_hex(theme_colors[theme_index][5]));
    lv_style_set_line_color(get_style_pb_tune_INDICATOR_DEFAULT(), lv_color_hex(theme_colors[theme_index][8]));
    lv_style_set_line_color(get_style_pb_tune_marker_MAIN_DEFAULT(), lv_color_hex(theme_colors[theme_index][6]));
    lv_obj_invalidate(objects.page_idle);
    lv_obj_invalidate(objects.page_menu);
    lv_obj_invalidate(objects.page_climate);
    lv_obj_invalidate(objects.page_climate_mode);
    lv_obj_invalidate(objects.page_devices);
    lv_obj_invalidate(objects.page_peregrine);
    lv_obj_invalidate(objects.page_playbill);
    lv_obj_invalidate(objects.page_energy);
    lv_obj_invalidate(objects.page_water);
    lv_obj_invalidate(objects.page_air);
    lv_obj_invalidate(objects.page_level);
    lv_obj_invalidate(objects.page_settings);
    lv_obj_invalidate(objects.page_alert);
    lv_obj_invalidate(objects.page_setup);
    lv_obj_invalidate(objects.page_locale);
    lv_obj_invalidate(objects.page_clock_face);
    lv_obj_invalidate(objects.page_brightness);
    lv_obj_invalidate(objects.page_bluetooth);
}
uint32_t theme_colors[2][32] = {
    { 0xffe8e8e8, 0xffe4e4e4, 0xffffffff, 0xffededed, 0xffffffff, 0xffc8c8c8, 0xff1a1a1a, 0xff4a4a4a, 0xff696969, 0xff52a441, 0xffcbe3c6, 0xffffc107, 0xfffff4d1, 0xff48e6fe, 0xffff5453, 0xff74fe00, 0xff505050, 0xff0088cc, 0xff48e6fe, 0xff777777, 0xffb5b5b5, 0xff333333, 0xff666666, 0xff9e9e9e, 0xff424242, 0xffe5e5e5, 0xffffffff, 0xff000000, 0xff3d7b31, 0xff7d5800, 0xff00697d, 0xffb3261e },
    { 0xff0a0a0a, 0xff000000, 0xff1a1a1a, 0xff252525, 0xff0a0a0a, 0xff333333, 0xffffffff, 0xffaaaaaa, 0xff6f6f6f, 0xff7bc96a, 0xff2e4a2a, 0xffffc107, 0xff3a2f00, 0xff48e6fe, 0xffff5453, 0xff74fe00, 0xffbdbdbd, 0xff0088cc, 0xff48e6fe, 0xff777777, 0xffb5b5b5, 0xff333333, 0xff666666, 0xff9e9e9e, 0xff424242, 0xff1e1e1e, 0xffffffff, 0xff000000, 0xff7bc96a, 0xffffc107, 0xff48e6fe, 0xffff5453 },
};

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_page_idle();
    create_screen_page_menu();
    create_screen_page_climate();
    create_screen_page_climate_mode();
    create_screen_page_devices();
    create_screen_page_peregrine();
    create_screen_page_playbill();
    create_screen_page_energy();
    create_screen_page_water();
    create_screen_page_air();
    create_screen_page_level();
    create_screen_page_settings();
    create_screen_page_alert();
    create_screen_page_setup();
    create_screen_page_locale();
    create_screen_page_clock_face();
    create_screen_page_brightness();
    create_screen_page_bluetooth();
}