#include "coolant.hpp"

void Coolant::update() {
    if (get_queued()->engine_temp) {
        uint16_t temp = round(get_cache()->engine_temp * 9.0 / 5.0 + 32);
        lv_meter_set_indicator_end_value(meter, indicator, temp);
        lv_label_set_text_fmt(label, "%d°F", temp);
    }
}

Coolant::Coolant(lv_obj_t * parent) {

    lv_obj = lv_obj_create(parent);
    lv_obj_remove_style_all(lv_obj);
    // lv_obj_set_style_blend_mode(lv_obj, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_scrollbar_mode(lv_obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(lv_obj, LV_DIR_NONE);
    lv_obj_set_size(lv_obj, GAUGE_SIZE);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_RIGHT, -180, 316);
    // lv_obj_align(lv_obj, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_blend_mode(lv_obj, LV_BLEND_MODE_ADDITIVE, 0);
    // lv_obj_center(lv_obj);

    meter = lv_meter_create(lv_obj);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    // lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_align(meter, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_size(meter, GAUGE_SIZE);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 5, 2, 50, DASH_BACKGROUND);
    // lv_meter_set_scale_major_ticks(meter, scale, 5, 3, 50, DASH_BACKGROUND, 20);
    lv_meter_set_scale_range(meter, scale, 30, 250, 270, 135);

    // lv_obj_set_style_text_color(meter, DASH_BACKGROUND, 0);
    lv_obj_set_style_text_opa(meter, 0, 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, GAUGE_WIDTH, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 30);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, 250);

    indicator = lv_meter_add_arc(meter, scale, GAUGE_WIDTH, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, indicator, 30);
    lv_meter_set_indicator_end_value(meter, indicator, 30);

{
    DASH_FONT(RAJDHANI_REGULAR, 24);

    label = lv_label_create(meter);
    // lv_obj_align(label, LV_ALIGN_RIGHT_MID, 0, 133);
    lv_obj_set_style_text_font(label, RAJDHANI_REGULAR_24, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -4);
    lv_label_set_text(label, "-°F");
    lv_obj_set_style_text_color(label, IMPORTANT_TEXT, 0);
    lv_obj_set_style_text_opa(label, 255, 0);
}

{
    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    lv_obj_t * label = lv_label_create(lv_obj);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 14);
    lv_obj_set_style_text_font(label, RAJDHANI_SEMIBOLD_14, 0);
    lv_label_set_text(label, "Coolant");
    lv_obj_set_style_text_color(label, AMBER_HALF, 0);
    lv_obj_set_style_text_opa(label, 255, 0);
}

}