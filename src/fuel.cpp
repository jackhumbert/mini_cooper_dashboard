#include "fuel.hpp"

void Fuel::update(void) {
    if (get_queued()->fuel_level) {
        lv_label_set_text_fmt(label, "%0.2f", get_dash()->fuel_level / 3.785);
        lv_meter_set_indicator_end_value(meter, indicator, get_dash()->fuel_level / 3.785 * 10);
    }
}

Fuel::Fuel(lv_obj_t * parent) {

    lv_obj = lv_obj_create(parent);
    lv_obj_remove_style_all(lv_obj);
    lv_obj_set_scrollbar_mode(lv_obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(lv_obj, LV_DIR_NONE);
    lv_obj_set_size(lv_obj, GAUGE_SIZE);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_RIGHT, -41, 357);

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
    lv_meter_set_scale_range(meter, scale, 0, TANK_SIZE_GALLONS * 10, 270, 135);

    // lv_obj_set_style_text_color(meter, DASH_BACKGROUND, 0);
    lv_obj_set_style_text_opa(meter, 0, 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, GAUGE_WIDTH, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, TANK_SIZE_GALLONS * 10);

    indicator = lv_meter_add_arc(meter, scale, GAUGE_WIDTH, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, indicator, 0);

    DASH_FONT(RAJDHANI_REGULAR, 24);

    label = lv_label_create(meter);
    // lv_obj_align(label, LV_ALIGN_RIGHT_MID, 0, 133);
    lv_obj_set_style_text_font(label, RAJDHANI_REGULAR_24, 0);
    lv_label_set_text(label, "-");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -4);
    lv_obj_set_style_text_color(label, IMPORTANT_TEXT, 0);
    lv_obj_set_style_text_opa(label, 255, 0);

    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    lv_obj_t * description = lv_label_create(lv_obj);
    lv_obj_align(description, LV_ALIGN_CENTER, 0, 14);
    lv_obj_set_style_text_font(description, RAJDHANI_SEMIBOLD_14, 0);
    lv_label_set_text(description, "Fuel");
    lv_obj_set_style_text_color(description, AMBER_HALF, 0);
    lv_obj_set_style_text_opa(description, 255, 0);


}