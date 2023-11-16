#include "speed.hpp"

static lv_obj_t * speed_label;
static lv_obj_t * speed_label2;

// static void set_speed_value(void * indic, int32_t v) {
//     speed_set(v);
// }

void Speed::update(void) {
    if (get_queued()->speed) {
        int v = round(get_cache()->speed / 1.609);
        if (v >= 10) {
            lv_label_set_text_fmt(speed_label, "%d", (int)(v / 10));
            lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
        } else {
            lv_label_set_text(speed_label, "");
            lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, 0, 1);
        }
        lv_label_set_text_fmt(speed_label2, "%d", v % 10);
    }
    if (get_queued()->cruise) {
        lv_color_t color = get_cache()->cruise ? GREEN_ON : GREEN_OFF;
        lv_obj_set_style_text_color(cruise_label, color, 0);
    }
}

Speed::Speed(lv_obj_t * parent) {

    DASH_FONT(RAJDHANI_SEMIBOLD, 120);

    static lv_coord_t col_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {115, 20, LV_GRID_TEMPLATE_LAST};

    lv_obj = lv_obj_create(parent);
    lv_obj_remove_style_all(lv_obj);
    lv_obj_set_scrollbar_mode(lv_obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(lv_obj, LV_DIR_NONE);
    lv_obj_set_layout(lv_obj, LV_LAYOUT_GRID);
    lv_obj_set_grid_align(lv_obj, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_style_grid_column_dsc_array(lv_obj, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(lv_obj, row_dsc, 0);
    lv_obj_set_size(lv_obj, 200, 115 + 20);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_MID, 0, 20);
    // lv_obj_center(lv_obj);
    // lv_obj_align(lv_obj, LV_ALIGN_CENTER, 0, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, IMPORTANT_TEXT);
    lv_style_set_text_font(&style, RAJDHANI_SEMIBOLD_120);

    speed_label = lv_label_create(lv_obj);
    lv_label_set_text(speed_label, "");
    lv_obj_add_style(speed_label, &style, 0);
    // lv_obj_set_style_blend_mode(speed_label, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(speed_label, LV_GRID_ALIGN_END, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

    lv_obj_align(speed_label, LV_ALIGN_TOP_RIGHT, 0, 0);

    speed_label2 = lv_label_create(lv_obj);
    lv_label_set_text(speed_label2, "0");
    lv_obj_add_style(speed_label2, &style, 0);
    // lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, 0, 1);

    lv_obj_align(speed_label2, LV_ALIGN_TOP_LEFT, 0, 0);


{

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_border_side(&style, LV_BORDER_SIDE_TOP);
    lv_style_set_border_opa(&style, LV_OPA_COVER);
    lv_style_set_border_color(&style, AMBER_HALF);
    lv_style_set_border_width(&style, 1);

    lv_obj_t * line = lv_obj_create(lv_obj);
    lv_obj_remove_style_all(line);
    lv_obj_set_size(line, 200, 24);
    lv_obj_set_grid_cell(line, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_START, 1, 1);
    // lv_obj_align(line, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_style(line, &style, 0);
}

{
    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_HALF);
    lv_style_set_text_font(&style, RAJDHANI_SEMIBOLD_14);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_RIGHT);

    lv_obj_t * mph_label = lv_label_create(lv_obj);
    lv_obj_set_grid_cell(mph_label, LV_GRID_ALIGN_END, 0, 2, LV_GRID_ALIGN_END, 1, 1);
    lv_label_set_text(mph_label, "MPH");
    lv_obj_add_style(mph_label, &style, 0);
    // lv_obj_align(mph_label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
}

{
    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, GREEN_OFF);
    lv_style_set_text_font(&style, RAJDHANI_SEMIBOLD_14);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_RIGHT);

    cruise_label = lv_label_create(lv_obj);
    lv_obj_set_grid_cell(cruise_label, LV_GRID_ALIGN_START, 0, 2, LV_GRID_ALIGN_END, 1, 1);
    lv_label_set_text(cruise_label, "CRUISE");
    lv_obj_add_style(cruise_label, &style, 0);
    // lv_obj_align(cruise_label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
}

}