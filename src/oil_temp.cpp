#include "oil_temp.hpp"

void OilTemp::update(void) {
    if (get_queued()->oil_temp) {
        int16_t temp = 0;
        // manually check offset applied by can processing
        if (get_dash()->oil_temp > -48) {
            // convert from C to F
            temp = round(get_dash()->oil_temp * 9 / 5.0 + 32);
            lv_label_set_text_fmt(label, "%d°F", temp);
        } else {
            lv_label_set_text(label, "-°F");
        }
        lv_meter_set_indicator_end_value(lv_obj, indicator, temp);
        lv_color_t old_color = indicator->type_data.arc.color;
        if (temp > 260) {
            indicator->type_data.arc.color = RED_ON;
            lv_obj_set_style_text_color(label, RED_ON, 0);
        } else {
            indicator->type_data.arc.color = AMBER_ON;
            lv_obj_set_style_text_color(label, IMPORTANT_TEXT, 0);
        }
        if (old_color.full != indicator->type_data.arc.color.full) {
            lv_obj_invalidate(lv_obj);
        }
    }
}

OilTemp::OilTemp(lv_obj_t * parent) {
    lv_obj = lv_meter_create(parent);

    lv_obj_remove_style(lv_obj, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(lv_obj, NULL, LV_PART_MAIN);
    lv_obj_set_size(lv_obj, GAUGE_SIZE);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_LEFT, 180, 316);

    lv_meter_scale_t * scale = lv_meter_add_scale(lv_obj);
    lv_meter_set_scale_ticks(lv_obj, scale, 5, 2, 50, DASH_BACKGROUND);
    // lv_meter_set_scale_major_ticks(lv_obj, scale, 5, 3, 50, DASH_BACKGROUND, 20);
    lv_meter_set_scale_range(lv_obj, scale, 100, 320, 270, 135);

    lv_obj_set_style_text_color(lv_obj, DASH_BACKGROUND, 0);
    lv_obj_set_style_text_opa(lv_obj, 0, 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(lv_obj, scale, GAUGE_WIDTH, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(lv_obj, rpm_normal_bg, 100);
    lv_meter_set_indicator_end_value(lv_obj, rpm_normal_bg, 320);

    indicator = lv_meter_add_arc(lv_obj, scale, GAUGE_WIDTH, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(lv_obj, indicator, 100);
    lv_meter_set_indicator_end_value(lv_obj, indicator, 100);

    DASH_FONT(RAJDHANI_REGULAR, 24);

    label = lv_label_create(lv_obj);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -4);
    lv_obj_set_style_text_font(label, RAJDHANI_REGULAR_24, 0);
    lv_label_set_text(label, "-°F");
    lv_obj_set_style_text_color(label, IMPORTANT_TEXT, 0);
    lv_obj_set_style_text_opa(label, 255, 0);

    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    lv_obj_t * description = lv_label_create(lv_obj);
    lv_obj_align(description, LV_ALIGN_CENTER, 0, 14);
    lv_obj_set_style_text_font(description, RAJDHANI_SEMIBOLD_14, 0);
    lv_label_set_text(description, "Oil Temp.");
    lv_obj_set_style_text_color(description, AMBER_HALF, 0);
    lv_obj_set_style_text_opa(description, 255, 0);

}