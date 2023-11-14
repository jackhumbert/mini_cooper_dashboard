#include "gas.h"

#define TANK_SIZE_GALLONS 13.2

static lv_obj_t * gas_label;
static lv_obj_t * meter;
static lv_meter_indicator_t * gas_indic;

void gas_update() {
    lv_label_set_text_fmt(gas_label, "%0.2f", get_dash()->fuel_level / 3.785);
    lv_meter_set_indicator_end_value(meter, gas_indic, get_dash()->fuel_level / 3.785 * 10);
}

lv_obj_t * gas_create(lv_obj_t * parent) {

    lv_obj_t * gas_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(gas_cont);
    // lv_obj_set_style_blend_mode(gas_cont, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_scrollbar_mode(gas_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(gas_cont, LV_DIR_NONE);
    lv_obj_set_size(gas_cont, 100, 100);
    // lv_obj_align(gas_cont, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_blend_mode(gas_cont, LV_BLEND_MODE_ADDITIVE, 0);
    // lv_obj_center(gas_cont);

    meter = lv_meter_create(gas_cont);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    // lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_align(meter, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_size(meter, 100, 100);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 11, 1, 50, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, scale, 5, 5, 50, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 0, TANK_SIZE_GALLONS * 10, 270, 135);

    lv_obj_set_style_text_color(meter, lv_color_black(), 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 13, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, TANK_SIZE_GALLONS * 10);

    gas_indic = lv_meter_add_arc(meter, scale, 13, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, gas_indic, 0);

{
    DASH_FONT(RAJDHANI_REGULAR, 24);

    gas_label = lv_label_create(meter);
    // lv_obj_align(gas_label, LV_ALIGN_RIGHT_MID, 0, 133);
    lv_obj_set_style_text_font(gas_label, RAJDHANI_REGULAR_24, 0);
    lv_label_set_text(gas_label, "-");
    lv_obj_align(gas_label, LV_ALIGN_CENTER, 0, -4);
    lv_obj_set_style_text_color(gas_label, AMBER_ON, 0);
}

{
    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    lv_obj_t * engine_temp_label = lv_label_create(gas_cont);
    lv_obj_align(engine_temp_label, LV_ALIGN_CENTER, 0, 14);
    lv_obj_set_style_text_font(engine_temp_label, RAJDHANI_SEMIBOLD_14, 0);
    lv_label_set_text(engine_temp_label, "Gas");
    lv_obj_set_style_text_color(engine_temp_label, AMBER_HALF, 0);
}

    return gas_cont;

}