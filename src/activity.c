#include "activity.h"

#define LED_FADE_OUT (5 * 30)

static lv_obj_t * success_led;
static lv_obj_t * failure_led;
static lv_anim_t s_a;
static lv_anim_t f_a;
static uint8_t success_brightness = 0;
static uint8_t failure_brightness = 0;

static void activity_success_callback(void * indic, int32_t v) {
    lv_led_set_brightness(success_led, v);
}

static void activity_failure_callback(void * indic, int32_t v) {
    lv_led_set_brightness(failure_led, v);
}

void activity_update(uint8_t activity) {
    if (activity & ACTIVITY_SUCCESS) {
        lv_anim_start(&s_a);
    }
    if (activity & ACTIVITY_ERROR) {
        lv_anim_start(&f_a);
    }
    // if (activity & ACTIVITY_SUCCESS) {
    //     success_brightness = 255;
    // } else {
    //     success_brightness = success_brightness * 0.95;
    // }
    // if (activity & ACTIVITY_ERROR) {
    //     failure_brightness = 255;
    // } else {
    //     failure_brightness = failure_brightness * 0.95;
    // }
    // if (lv_led_get_brightness(success_led) != success_brightness)
    //     lv_led_set_brightness(success_led, success_brightness);
    // if (lv_led_get_brightness(failure_led) != failure_brightness)
    //     lv_led_set_brightness(failure_led, failure_brightness);
}

void activity_create(lv_obj_t * parent) {
    success_led  = lv_led_create(parent);
    lv_obj_align(success_led, LV_ALIGN_TOP_RIGHT, -95, 5);
    lv_led_set_brightness(success_led, 0);
    lv_led_set_color(success_led, lv_palette_main(LV_PALETTE_GREEN));

    failure_led  = lv_led_create(parent);
    lv_obj_align(failure_led, LV_ALIGN_TOP_RIGHT, -70, 5);
    lv_led_set_brightness(failure_led, 0);
    lv_led_set_color(failure_led, lv_palette_main(LV_PALETTE_RED));

    lv_anim_init(&s_a);
    // lv_anim_set_var(&s_a, success_led);
    lv_anim_set_exec_cb(&s_a, activity_success_callback);
    lv_anim_set_values(&s_a, 255, 0);
    lv_anim_set_time(&s_a, LED_FADE_OUT);
    // lv_anim_set_playback_time(&s_a, LED_FADE_OUT);

    lv_anim_init(&f_a);
    // lv_anim_set_var(&f_a, failure_led);
    lv_anim_set_exec_cb(&f_a, activity_failure_callback);
    lv_anim_set_values(&f_a, 255, 0);
    lv_anim_set_time(&f_a, LED_FADE_OUT);
    // lv_anim_set_playback_time(&f_a, LED_FADE_OUT);
}