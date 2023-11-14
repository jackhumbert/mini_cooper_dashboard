#include "activity.h"

#define LED_FADE_OUT (3 * 30)

static lv_obj_t * success_led;
static lv_obj_t * failure_led;
static lv_anim_t s_a;
static lv_anim_t f_a;

static void activity_success_callback(void * indic, int32_t v) {
    lv_led_set_brightness(success_led, v / 2.0 * 255);
}

static void activity_failure_callback(void * indic, int32_t v) {
    lv_led_set_brightness(failure_led, v / 2.0 * 255);
}

void activity_update(bool success) {
    if (success) {
        lv_led_set_brightness(success_led,  255);
        lv_anim_start(&s_a);
    } else {
        lv_led_set_brightness(failure_led,  255);
        lv_anim_start(&f_a);
    }
}

void activity_create(lv_obj_t * parent) {
    success_led  = lv_led_create(parent);
    lv_obj_align(success_led, LV_ALIGN_TOP_RIGHT, -110, 5);
    lv_led_set_brightness(success_led, 0);
    lv_led_set_color(success_led, lv_palette_main(LV_PALETTE_GREEN));

    failure_led  = lv_led_create(parent);
    lv_obj_align(failure_led, LV_ALIGN_TOP_RIGHT, -80, 5);
    lv_led_set_brightness(failure_led, 0);
    lv_led_set_color(failure_led, lv_palette_main(LV_PALETTE_RED));

    lv_anim_init(&s_a);
    // lv_anim_set_var(&s_a, success_led);
    lv_anim_set_exec_cb(&s_a, activity_success_callback);
    lv_anim_set_values(&s_a, 2, 0);
    lv_anim_set_time(&s_a, LED_FADE_OUT);
    // lv_anim_set_playback_time(&s_a, LED_FADE_OUT);

    lv_anim_init(&f_a);
    // lv_anim_set_var(&f_a, failure_led);
    lv_anim_set_exec_cb(&f_a, activity_failure_callback);
    lv_anim_set_values(&f_a, 2, 0);
    lv_anim_set_time(&f_a, LED_FADE_OUT);
    // lv_anim_set_playback_time(&f_a, LED_FADE_OUT);
}