#include "accel.hpp"

#define ACCEL_SIZE 62
#define ACCEL_INDIC_SIZE 24

void Accel::update(void) {
    if (get_queued()->lateral_force || get_queued()->forward_force) {
        int x = round(get_cache()->lateral_force / 512.0 * ACCEL_SIZE / 2);
        int y = round(get_cache()->forward_force / 512.0 * ACCEL_SIZE / 2);
        if (lv_obj_get_style_translate_x(center, 0) != x) {
            lv_obj_set_style_translate_x(center, x, 0);
        }
        if (lv_obj_get_style_translate_y(center, 0) != y) {
            lv_obj_set_style_translate_y(center, y, 0);
        }
        // lv_obj_align(center, LV_ALIGN_CENTER, x, y);
    }
}

Accel::Accel(lv_obj_t * parent) {
    lv_obj = lv_obj_create(parent);
    lv_obj_set_style_bg_color(lv_obj, AMBER_OFF, 0);
    // lv_obj_set_style_radius(lv_obj, 5, 0);
    lv_obj_set_style_radius(lv_obj, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_outline_color(lv_obj, AMBER_HALF, 0);
    lv_obj_set_style_outline_width(lv_obj, 1, 0);
    lv_obj_set_style_outline_pad(lv_obj, 3, 0);
    lv_obj_set_style_outline_opa(lv_obj, LV_OPA_100, 0);
    lv_obj_set_size(lv_obj, ACCEL_SIZE, ACCEL_SIZE);
    lv_obj_align(lv_obj, LV_ALIGN_BOTTOM_RIGHT, -119, -122);

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 2);
    lv_style_set_line_color(&style_line, DASH_BACKGROUND);

    auto ver = lv_line_create(lv_obj);
    static lv_point_t ver_points[2] = {
        {.x = ACCEL_SIZE / 2, .y = 0}, 
        {.x = ACCEL_SIZE / 2, .y = ACCEL_SIZE}
    };
    lv_line_set_points(ver, ver_points, 2);
    lv_obj_add_style(ver, &style_line, 0);
    lv_obj_align(ver, LV_ALIGN_TOP_LEFT, 0, 0);

    auto hor = lv_line_create(lv_obj);
    static lv_point_t hor_points[2] = {
        {.x = 0, .y = ACCEL_SIZE / 2}, 
        {.x = ACCEL_SIZE, .y = ACCEL_SIZE / 2}
    };
    lv_line_set_points(hor, hor_points, 2);
    lv_obj_add_style(hor, &style_line, 0);
    lv_obj_align(hor, LV_ALIGN_TOP_LEFT, 0, 0);

    center = lv_obj_create(lv_obj);
    lv_obj_set_size(center, ACCEL_INDIC_SIZE, ACCEL_INDIC_SIZE);
    lv_obj_set_style_border_width(center, 0, 0);
    lv_obj_set_style_bg_color(center, AMBER_ON, 0);
    lv_obj_set_style_radius(center, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_pad_all(center, 0, 0);
    lv_obj_align(center, LV_ALIGN_CENTER, 0, 0);
}
