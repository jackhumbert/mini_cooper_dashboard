#include "odometer.hpp"
#include <Wire.h>

#define ODO_SEGMENT_WIDTH 19
#define ODO_WIDTH (ODO_SEGMENT_WIDTH * ODO_SEGMENTS)
#define ODO_NUM_DECIMALS 1

float additional_km = 0;
uint64_t last_measure = 0;
uint64_t last_decimiles = 0;

void Odometer::update(void) {
    if (get_cache()->speed > 0) {
        uint64_t measure = xTaskGetTickCount();
        additional_km += (measure - last_measure) / (1000.0 * 60.0 * 60.0) * get_cache()->speed;
    }
    last_measure = xTaskGetTickCount();
    if (get_queued()->odometer) {
        additional_km = 0;
    }
    // km to mi
    // uint64_t decimiles = (get_cache()->odometer + additional_km) / 1.609 * pow(10, ODO_NUM_DECIMALS);
    // already miles
    uint64_t decimiles = (get_cache()->odometer + additional_km / 1.609) * pow(10, ODO_NUM_DECIMALS);
    if (last_decimiles != decimiles ) {
        last_decimiles = decimiles;
        int max = 0;
        for (int i = 0; i < ODO_SEGMENTS; i++) {
            if ((uint64_t) (decimiles / pow(10, i))) {
                lv_label_set_text_fmt(numbers[i], "%d", (uint64_t)(decimiles / pow(10, i)) % 10);
                lv_obj_set_style_border_width(numbers[i], 1, 0);
                lv_obj_set_size(numbers[i], ODO_SEGMENT_WIDTH, 26);
                max = i;
            } else {
                lv_label_set_text(numbers[i], "");
                lv_obj_set_style_border_width(numbers[i], 0, 0);
                lv_obj_set_size(numbers[i], 0, 26);
            }
        }
        lv_obj_set_width(segments, ODO_SEGMENT_WIDTH * (max + 1));
    }
}

Odometer::Odometer(lv_obj_t * parent) {
    DASH_FONT(RAJDHANI_MEDIUM, 24);
    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    lv_obj = lv_obj_create(parent);
    lv_obj_set_size(lv_obj, ODO_WIDTH, 50);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_RIGHT, -5, 198);
    lv_obj_set_style_bg_opa(lv_obj, LV_OPA_0, 0);

    segments = lv_obj_create(lv_obj);
    lv_obj_align(segments, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_style_radius(segments, 3, 0);
    lv_obj_set_style_border_width(segments, 1, 0);
    lv_obj_set_style_border_color(segments, AMBER_HALF, 0);
    lv_obj_set_height(segments, 26);
    lv_obj_set_width(segments, ODO_SEGMENT_WIDTH * (ODO_NUM_DECIMALS + 1));

    for (int i = 0; i < ODO_SEGMENTS; i++) {
        numbers[i] = lv_label_create(segments);
        lv_label_set_text(numbers[i], "");
        lv_obj_set_style_text_align(numbers[i], LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(numbers[i], RAJDHANI_MEDIUM_24, 0);
        lv_obj_set_style_text_color(numbers[i], IMPORTANT_TEXT, 0);
        lv_obj_set_size(numbers[i], ODO_SEGMENT_WIDTH, 28);
        lv_obj_set_style_border_side(numbers[i], LV_BORDER_SIDE_LEFT, 0);
        lv_obj_set_style_border_width(numbers[i], 1, 0);
        lv_obj_set_style_border_color(numbers[i], AMBER_HALF, 0);
        lv_obj_align(numbers[i], LV_ALIGN_TOP_RIGHT, -ODO_SEGMENT_WIDTH * i, 0);
        lv_obj_set_style_pad_top(numbers[i], -1, 0);
        lv_obj_set_style_pad_left(numbers[i], 1, 0);
        // lv_obj_set_style_pad_bottom(numbers[i], 4, 0);
    }

    int i = 0;
    for (i = 0; i < ODO_NUM_DECIMALS; i++) {
        lv_label_set_text(numbers[i], "0");
        lv_obj_set_style_text_color(numbers[i], DASH_BACKGROUND, 0);
        lv_obj_set_style_bg_color(numbers[i], AMBER_ON, 0);
        lv_obj_set_style_bg_opa(numbers[i], LV_OPA_100, 0);
    }
    lv_label_set_text(numbers[i], "0");

    lv_obj_t * label = lv_label_create(lv_obj);
    lv_label_set_text(label, "TOTAL");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_font(label, RAJDHANI_SEMIBOLD_14, 0);
    lv_obj_set_style_text_color(label, AMBER_HALF, 0);
    lv_obj_set_width(label, ODO_WIDTH);
    lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    // static lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_border_side(&style, LV_BORDER_SIDE_BOTTOM);
    // lv_style_set_border_opa(&style, LV_OPA_COVER);
    // lv_style_set_border_color(&style, AMBER_HALF);
    // lv_style_set_border_width(&style, 1);

    // lv_obj_t * line = lv_obj_create(lv_obj);
    // lv_obj_remove_style_all(line);
    // lv_obj_set_size(line, ODO_WIDTH, 24 + 5);
    // lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 0);
    // lv_obj_add_style(line, &style, 0);
    last_measure = xTaskGetTickCount();
}