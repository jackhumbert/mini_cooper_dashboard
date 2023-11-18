#include "odometer.hpp"

#define ODOMETER_WIDTH 150

void Odometer::update(void) {
    if (get_queued()->odometer) {
        // km to mi
        lv_label_set_text_fmt(lv_obj, "%0.1f mi", get_cache()->odometer / 1.609);
    }
}

Odometer::Odometer(lv_obj_t * parent) {
    DASH_FONT(RAJDHANI_MEDIUM, 24);
    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, ODOMETER_WIDTH, 50);
    lv_obj_align(cont, LV_ALIGN_TOP_RIGHT, -5, 150);

    lv_obj = lv_label_create(cont);
    lv_label_set_text(lv_obj, "- mi");
    lv_obj_set_style_text_align(lv_obj, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_font(lv_obj, RAJDHANI_MEDIUM_24, 0);
    lv_obj_set_style_text_color(lv_obj, AMBER_ON, 0);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_RIGHT, 0, 0);

    lv_obj_t * label = lv_label_create(cont);
    lv_label_set_text(label, "TOTAL");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_RIGHT, 0);
    lv_obj_set_style_text_font(label, RAJDHANI_SEMIBOLD_14, 0);
    lv_obj_set_style_text_color(label, AMBER_HALF, 0);
    lv_obj_set_width(label, ODOMETER_WIDTH);
    lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_border_side(&style, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_opa(&style, LV_OPA_COVER);
    lv_style_set_border_color(&style, AMBER_HALF);
    lv_style_set_border_width(&style, 1);

    lv_obj_t * line = lv_obj_create(cont);
    lv_obj_remove_style_all(line);
    lv_obj_set_size(line, ODOMETER_WIDTH, 24 + 5);
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(line, &style, 0);

}