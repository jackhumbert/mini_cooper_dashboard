#include "wheel.hpp"

#define WHEEL_SIZE 61
#define WHEEL_WIDTH 7

void Wheel::update(void) {
    if (get_queued()->steering_angle) {
        int angle = round(get_cache()->steering_angle * -10);
        if (lv_obj_get_style_transform_angle(lv_obj, 0) != angle) {
            lv_obj_set_style_transform_angle(lv_obj, angle, 0);
        }
    }
}

Wheel::Wheel(lv_obj_t * parent) {
    lv_obj = lv_obj_create(parent);
    lv_obj_set_style_bg_color(lv_obj, AMBER_OFF, 0);
    // lv_obj_set_style_radius(lv_obj, 5, 0);
    lv_obj_set_style_radius(lv_obj, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_outline_color(lv_obj, AMBER_HALF, 0);
    lv_obj_set_style_outline_width(lv_obj, 1, 0);
    lv_obj_set_style_outline_pad(lv_obj, 3, 0);
    lv_obj_set_style_outline_opa(lv_obj, LV_OPA_100, 0);
    lv_obj_set_style_border_color(lv_obj, AMBER_ON, 0);
    lv_obj_set_style_border_width(lv_obj, WHEEL_WIDTH, 0);
    lv_obj_set_style_border_opa(lv_obj, LV_OPA_100, 0);
    lv_obj_set_size(lv_obj, WHEEL_SIZE, WHEEL_SIZE);
    lv_obj_align(lv_obj, LV_ALIGN_BOTTOM_LEFT, 119, -122);
    lv_obj_set_style_transform_pivot_x(lv_obj, WHEEL_SIZE / 2, 0);
    lv_obj_set_style_transform_pivot_y(lv_obj, WHEEL_SIZE / 2, 0);

    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, WHEEL_WIDTH);
    lv_style_set_line_color(&style_line, AMBER_ON);

    auto ver = lv_line_create(lv_obj);
    static lv_point_t ver_points[2] = {
        {.x = WHEEL_SIZE / 2, .y = WHEEL_SIZE / 2}, 
        {.x = WHEEL_SIZE / 2, .y = WHEEL_SIZE}
    };
    lv_line_set_points(ver, ver_points, 2);
    lv_obj_add_style(ver, &style_line, 0);
    lv_obj_align(ver, LV_ALIGN_TOP_LEFT, -WHEEL_WIDTH, -WHEEL_WIDTH);

    auto hor = lv_line_create(lv_obj);
    static lv_point_t hor_points[2] = {
        {.x = 0, .y = WHEEL_SIZE / 2}, 
        {.x = WHEEL_SIZE, .y = WHEEL_SIZE / 2}
    };
    lv_line_set_points(hor, hor_points, 2);
    lv_obj_add_style(hor, &style_line, 0);
    lv_obj_align(hor, LV_ALIGN_TOP_LEFT, -WHEEL_WIDTH, -WHEEL_WIDTH);
}
