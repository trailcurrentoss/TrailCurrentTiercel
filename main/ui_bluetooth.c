/*
 * Settings > Bluetooth. See ui_bluetooth.h; the layout is page_bluetooth()
 * in GUI/tmp/screens_layout.py.
 */

#include "ui_bluetooth.h"

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "lvgl.h"
#include "screens.h"
#include "tiercel_board.h"
#include "tiercel_bt.h"
#include "ui_lv.h"

static const char *TAG = "ui.bt";

/* BT_ROWS in screens_layout.py; the row pitch is its 52 px. */
#define BT_ROWS   TIERCEL_BT_ROWS
#define ROW_PITCH 52
#define POLL_MS   250

static lv_timer_t *s_poll;
static uint32_t    s_gen = UINT32_MAX;

static lv_obj_t *row_obj(int i)
{
    lv_obj_t *const rows[BT_ROWS] = {
        objects.bt_row0, objects.bt_row1, objects.bt_row2, objects.bt_row3,
        objects.bt_row4, objects.bt_row5, objects.bt_row6, objects.bt_row7,
    };
    return rows[i];
}

static lv_obj_t *row_name(int i)
{
    lv_obj_t *const names[BT_ROWS] = {
        objects.bt_row0_name, objects.bt_row1_name, objects.bt_row2_name,
        objects.bt_row3_name, objects.bt_row4_name, objects.bt_row5_name,
        objects.bt_row6_name, objects.bt_row7_name,
    };
    return names[i];
}

static void status_text(const tiercel_bt_state_t *s, char *out, size_t len)
{
    if (!s->present) {
        snprintf(out, len, "Bluetooth is not available");
    } else if (s->conn == TIERCEL_BT_CONNECTED) {
        snprintf(out, len, "Connected to %s", s->peer.name);
    } else if (s->conn == TIERCEL_BT_CONNECTING) {
        snprintf(out, len, "Connecting to %s...", s->peer.name);
    } else if (s->failed) {
        snprintf(out, len, "Could not connect. Is it in pairing mode?");
    } else if (s->searching) {
        snprintf(out, len, "Searching... put headphones in pairing mode");
    } else if (s->ndev > 0 || s->has_saved) {
        snprintf(out, len, "Tap headphones to connect");
    } else {
        snprintf(out, len, "Put headphones in pairing mode, then Search");
    }
}

static void refresh(void)
{
    tiercel_bt_state_t s;
    tiercel_bt_get(&s);
    if (s.gen == s_gen) {
        return;
    }
    s_gen = s.gen;

    char buf[96];
    status_text(&s, buf, sizeof(buf));
    ui_lv_set_text(objects.bt_status, buf);

    for (int i = 0; i < BT_ROWS; i++) {
        tiercel_bt_dev_t d;
        const bool shown = s.present && tiercel_bt_row(&s, i, &d);
        ui_lv_set_hidden(row_obj(i), !shown);
        if (!shown) {
            continue;
        }
        ui_lv_set_text(row_name(i), d.name);
        /* The connected (or connecting) headphones are the checked row. */
        const bool live = s.conn != TIERCEL_BT_IDLE &&
                          strcmp(d.addr, s.peer.addr) == 0;
        ui_lv_set_state_in(row_obj(i), LV_STATE_CHECKED,
                           live ? LV_STATE_CHECKED : 0);
    }

    ui_lv_set_hidden(objects.bt_search, !s.present);
    ui_lv_set_text(objects.bt_search_label, s.searching ? "Stop" : "Search");
    ui_lv_set_hidden(objects.bt_forget,
                     !s.present || (!s.has_saved && s.conn == TIERCEL_BT_IDLE));
}

static void poll_cb(lv_timer_t *t)
{
    (void)t;
    refresh();
}

void ui_bluetooth_enter(void)
{
    tiercel_bt_state_t s;
    tiercel_bt_get(&s);
    /* Search straight away -- the person came here to pair something -- but
     * not over connected headphones: a search drops them. */
    if (s.present && s.conn == TIERCEL_BT_IDLE && !s.searching) {
        tiercel_bt_search();
    }
    lv_obj_scroll_to_y(objects.bt_list, 0, LV_ANIM_OFF);
    s_gen = UINT32_MAX;
    refresh();
    if (!s_poll) {
        s_poll = lv_timer_create(poll_cb, POLL_MS, NULL);
    }
}

void ui_bluetooth_leave(void)
{
    if (s_poll) {
        lv_timer_delete(s_poll);
        s_poll = NULL;
    }
    tiercel_bt_state_t s;
    tiercel_bt_get(&s);
    if (s.searching) {
        tiercel_bt_stop_search();
    }
}

void ui_bluetooth_rotate(int diff)
{
    lv_obj_t *list = objects.bt_list;
    lv_obj_update_layout(list);
    /* Scrolling down shows later rows: positive y scrolls content up. */
    const int32_t top = lv_obj_get_scroll_top(list);
    const int32_t bottom = lv_obj_get_scroll_bottom(list);
    if ((diff > 0 && bottom <= 0) || (diff < 0 && top <= 0)) {
        tiercel_board_haptic(TIERCEL_HAPTIC_LIMIT);
        return;
    }
    int32_t dy = diff * ROW_PITCH;
    dy = dy > bottom ? bottom : dy < -top ? -top : dy;
    lv_obj_scroll_by(list, 0, -dy, LV_ANIM_ON);
}

void ui_bluetooth_row_pressed(int row)
{
    tiercel_bt_state_t s;
    tiercel_bt_get(&s);
    tiercel_bt_dev_t d;
    if (row < 0 || row >= BT_ROWS || !tiercel_bt_row(&s, row, &d)) {
        return;
    }
    if (s.conn != TIERCEL_BT_IDLE && strcmp(d.addr, s.peer.addr) == 0) {
        tiercel_board_haptic(TIERCEL_HAPTIC_LIMIT);   /* already this one */
        return;
    }
    ESP_LOGI(TAG, "connect %s (%s)", d.addr, d.name);
    tiercel_board_haptic(TIERCEL_HAPTIC_CONFIRM);
    tiercel_bt_connect(d.addr);
    refresh();
}

void ui_bluetooth_search_pressed(void)
{
    tiercel_bt_state_t s;
    tiercel_bt_get(&s);
    tiercel_board_haptic(TIERCEL_HAPTIC_TAP);
    if (s.searching) {
        tiercel_bt_stop_search();
    } else {
        tiercel_bt_search();
    }
}

void ui_bluetooth_forget_pressed(void)
{
    tiercel_board_haptic(TIERCEL_HAPTIC_CONFIRM);
    tiercel_bt_forget();
}

const char *ui_bluetooth_value(bool *ok)
{
    static char s_name[TIERCEL_BT_NAME_MAX];
    tiercel_bt_state_t s;
    tiercel_bt_get(&s);
    *ok = false;
    if (!s.present) {
        return "Unavailable";
    }
    switch (s.conn) {
    case TIERCEL_BT_CONNECTED:
        *ok = true;
        strlcpy(s_name, s.peer.name, sizeof(s_name));
        return s_name;
    case TIERCEL_BT_CONNECTING:
        return "Connecting";
    default:
        return s.has_saved ? "Not connected" : "Off";
    }
}
