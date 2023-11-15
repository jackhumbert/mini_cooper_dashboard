#include "turn_signal.h"

static lv_obj_t * turn_left;
// static lv_style_t turn_left_style;
static lv_obj_t * turn_right;
// static lv_style_t turn_right_style;

void turn_signal_update() {
    lv_obj_set_style_img_recolor(turn_left, get_dash()->left_turn_signal ? GREEN_ON : GREEN_OFF, 0);
    lv_obj_set_style_img_recolor(turn_right, get_dash()->right_turn_signal ? GREEN_ON : GREEN_OFF, 0);
    // lv_obj_invalidate(turn_left);
    // lv_obj_invalidate(turn_right);
}

lv_obj_t * turn_signal_left_create(lv_obj_t * parent) {

    LV_IMG_DECLARE(slice15);
    
    turn_left = lv_img_create(parent);
    lv_img_set_src(turn_left, &slice15);
    lv_obj_set_size(turn_left, 48, 48);
    // lv_obj_set_style_blend_mode(turn_left, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_style_transform_pivot_x(turn_left, 24, 0);
    lv_obj_set_style_transform_pivot_y(turn_left, 24, 0);
    lv_obj_set_style_transform_angle(turn_left, 1800, 0);
    lv_obj_set_style_img_recolor(turn_left, GREEN_OFF, 0);
    lv_obj_set_style_img_recolor_opa(turn_left, LV_OPA_100, 0);

    // lv_style_init(&turn_left_style);
    // lv_style_set_img_recolor(&turn_left_style, GREEN_OFF);
    // lv_style_set_img_recolor_opa(&turn_left_style, LV_OPA_COVER);
    // lv_obj_add_style(turn_left, &turn_left_style, 0);
    return turn_left;
}

lv_obj_t * turn_signal_right_create(lv_obj_t * parent) {

    LV_IMG_DECLARE(slice15);
    
    turn_right = lv_img_create(parent);
    lv_img_set_src(turn_right, &slice15);
    lv_obj_set_size(turn_right, 48, 48);
    // lv_obj_set_style_blend_mode(turn_right, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_style_img_recolor(turn_right, GREEN_OFF, 0);
    lv_obj_set_style_img_recolor_opa(turn_right, LV_OPA_100, 0);

    // lv_style_init(&turn_right_style);
    // lv_style_set_img_recolor(&turn_right_style, GREEN_OFF);
    // lv_style_set_img_recolor_opa(&turn_right_style, LV_OPA_COVER);
    // lv_obj_add_style(turn_right, &turn_right_style, 0);
    return turn_right;
}