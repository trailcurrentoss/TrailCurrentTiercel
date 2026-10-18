/*
 * BRING-UP SCREEN -- compiled only while no EEZ Studio export exists.
 *
 * It proves the board before there is a product UI to prove it with: the
 * panel draws (colour swatches, round mask), touch tracks a finger, the knob
 * counts detents with haptic ticks and an end-stop bump at +/-10, and a long
 * press records three seconds from the microphone and plays it back through
 * the headphone jack.
 *
 * Every widget here is created in C with literal colours, which is exactly
 * what the product UI must never do -- this is a jig, not a screen anyone
 * ships, and it disappears from the binary at the first export (see
 * TIERCEL_HAVE_UI in main/CMakeLists.txt).
 */

#include "esp_heap_caps.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "bringup.h"
#include "tiercel_audio.h"
#include "tiercel_board.h"

static const char *TAG = "bringup";

#define LIMIT 10

static lv_obj_t *s_count, *s_dir, *s_touch, *s_dot, *s_audio, *s_heap;
static int s_pos;

static void on_rotate(int diff, void *ctx)
{
    (void)ctx;
    int want = s_pos + diff;
    if (want > LIMIT || want < -LIMIT) {
        /* Clamp, and bump: the behaviour every bounded knob value gets. */
        want = want > LIMIT ? LIMIT : -LIMIT;
        tiercel_board_haptic(TIERCEL_HAPTIC_LIMIT);
    }
    s_pos = want;
    lv_label_set_text_fmt(s_count, "%d", s_pos);
    lv_label_set_text(s_dir, diff > 0 ? "clockwise" : "anticlockwise");
}

static void on_press(lv_event_t *e)
{
    lv_indev_t *in = lv_indev_active();
    if (!in) {
        return;
    }
    lv_point_t p;
    lv_indev_get_point(in, &p);
    lv_obj_set_pos(s_dot, p.x - 8, p.y - 8);
    lv_obj_remove_flag(s_dot, LV_OBJ_FLAG_HIDDEN);
    lv_label_set_text_fmt(s_touch, "touch %d, %d", (int)p.x, (int)p.y);
    if (lv_event_get_code(e) == LV_EVENT_SHORT_CLICKED) {
        tiercel_board_haptic(TIERCEL_HAPTIC_CONFIRM);
    }
}

static void audio_test_task(void *arg)
{
    (void)arg;
    if (tiercel_board_lock(0)) {
        lv_label_set_text(s_audio, "recording 3 s...");
        tiercel_board_unlock();
    }
    tiercel_audio_mic_start();
    uint8_t peak = 0;
    for (int i = 0; i < 60; i++) {
        vTaskDelay(pdMS_TO_TICKS(50));
        const uint8_t l = tiercel_audio_mic_level();
        peak = l > peak ? l : peak;
    }
    const int16_t *pcm;
    size_t bytes;
    tiercel_audio_mic_stop(&pcm, &bytes);
    ESP_LOGI(TAG, "recorded %u bytes, peak level %u", (unsigned)bytes, peak);

    if (tiercel_board_lock(0)) {
        lv_label_set_text_fmt(s_audio, "playing (peak %u)", peak);
        tiercel_board_unlock();
    }
    if (tiercel_audio_play_start(TIERCEL_AUDIO_MIC_RATE)) {
        tiercel_audio_play_write(pcm, bytes);
        tiercel_audio_play_end();
    }
    if (tiercel_board_lock(0)) {
        lv_label_set_text(s_audio, "hold to test audio");
        tiercel_board_unlock();
    }
    vTaskDelete(NULL);
}

static void on_long_press(lv_event_t *e)
{
    (void)e;
    tiercel_board_haptic(TIERCEL_HAPTIC_CONFIRM);
    xTaskCreate(audio_test_task, "atest", 4096, NULL, 4, NULL);
}

static void heap_timer(lv_timer_t *t)
{
    (void)t;
    lv_label_set_text_fmt(s_heap, "int %u KB  psram %u KB",
        (unsigned)(heap_caps_get_free_size(MALLOC_CAP_INTERNAL) / 1024),
        (unsigned)(heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024));
}

static lv_obj_t *text(lv_obj_t *parent, const lv_font_t *f, uint32_t hex, int y)
{
    lv_obj_t *l = lv_label_create(parent);
    lv_obj_set_style_text_font(l, f, 0);
    lv_obj_set_style_text_color(l, lv_color_hex(hex), 0);
    lv_obj_align(l, LV_ALIGN_CENTER, 0, y);
    lv_label_set_text(l, "");
    return l;
}

void bringup_screen(void)
{
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
    lv_obj_remove_flag(scr, LV_OBJ_FLAG_SCROLLABLE);

    /* The glass edge: a ring at the panel's full diameter shows whether the
     * round mask is centred and whether the corners are really dark. */
    lv_obj_t *ring = lv_obj_create(scr);
    lv_obj_remove_style_all(ring);
    lv_obj_set_size(ring, 356, 356);
    lv_obj_center(ring);
    lv_obj_set_style_radius(ring, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_border_width(ring, 2, 0);
    lv_obj_set_style_border_color(ring, lv_color_hex(0x52A441), 0);
    lv_obj_remove_flag(ring, LV_OBJ_FLAG_CLICKABLE);

    static const uint32_t sw[] = { 0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF };
    for (int i = 0; i < 4; i++) {
        lv_obj_t *b = lv_obj_create(scr);
        lv_obj_remove_style_all(b);
        lv_obj_set_size(b, 36, 36);
        lv_obj_align(b, LV_ALIGN_CENTER, -66 + i * 44, -104);
        lv_obj_set_style_bg_color(b, lv_color_hex(sw[i]), 0);
        lv_obj_set_style_bg_opa(b, LV_OPA_COVER, 0);
        lv_obj_set_style_radius(b, 6, 0);
        lv_obj_remove_flag(b, LV_OBJ_FLAG_CLICKABLE);
    }

    lv_obj_t *t = text(scr, &lv_font_montserrat_14, 0x8A8A8A, -70);
    lv_label_set_text(t, "Tiercel bring-up");
    s_count = text(scr, &lv_font_montserrat_28, 0xFFFFFF, -24);
    lv_label_set_text(s_count, "0");
    s_dir   = text(scr, &lv_font_montserrat_14, 0x52A441, 10);
    lv_label_set_text(s_dir, "turn the knob");
    s_touch = text(scr, &lv_font_montserrat_14, 0xFFC107, 40);
    lv_label_set_text(s_touch, "touch the glass");
    s_audio = text(scr, &lv_font_montserrat_14, 0x48E6FE, 70);
    lv_label_set_text(s_audio, "hold to test audio");
    s_heap  = text(scr, &lv_font_montserrat_14, 0x6A6A6A, 100);

    s_dot = lv_obj_create(scr);
    lv_obj_remove_style_all(s_dot);
    lv_obj_set_size(s_dot, 16, 16);
    lv_obj_set_style_radius(s_dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(s_dot, lv_color_hex(0xFF5453), 0);
    lv_obj_set_style_bg_opa(s_dot, LV_OPA_COVER, 0);
    lv_obj_add_flag(s_dot, LV_OBJ_FLAG_HIDDEN);
    lv_obj_remove_flag(s_dot, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_add_flag(scr, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(scr, on_press, LV_EVENT_PRESSING, NULL);
    lv_obj_add_event_cb(scr, on_press, LV_EVENT_SHORT_CLICKED, NULL);
    lv_obj_add_event_cb(scr, on_long_press, LV_EVENT_LONG_PRESSED, NULL);

    tiercel_board_set_rotate_callback(on_rotate, NULL);
    lv_timer_create(heap_timer, 1000, NULL);
    heap_timer(NULL);
}
