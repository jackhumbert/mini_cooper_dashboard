#include "turn_signal.h"

static lv_obj_t * turn_left;
static lv_obj_t * turn_right;

lv_obj_t * turn_signal_left_create(lv_obj_t * parent) {

    LV_IMG_DECLARE(slice15);
    
    turn_left = lv_img_create(parent);
    lv_img_set_src(turn_left, &slice15);
    // lv_obj_set_style_blend_mode(turn_left, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, lv_color_change_lightness(lv_palette_main(LV_PALETTE_GREEN), 15));
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(turn_left, &style, 0);
    return turn_left;
}

lv_obj_t * turn_signal_right_create(lv_obj_t * parent) {

    LV_IMG_DECLARE(slice15);
    
    turn_right = lv_img_create(parent);
    lv_img_set_src(turn_right, &slice15);
    // lv_obj_set_style_blend_mode(turn_right, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_style_transform_angle(turn_right, 1800, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(turn_right, &style, 0);
    return turn_right;
}