#include "car_view.h"

static lv_obj_t * car_view_cont;

lv_obj_t * car_view_create(lv_obj_t * parent) {

    car_view_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(car_view_cont);
    // lv_obj_set_style_blend_mode(car_view_cont, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_scrollbar_mode(car_view_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(car_view_cont, LV_DIR_NONE);
    // lv_obj_align(car_view_cont, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_blend_mode(car_view_cont, LV_BLEND_MODE_ADDITIVE, 0);
    // lv_obj_center(car_view_cont);
    lv_obj_set_size(car_view_cont, 84, 158);

{
    LV_IMG_DECLARE(car_view);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &car_view);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, AMBER_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(hood);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &hood);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(trunk);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &trunk);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(headlights);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &headlights);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    // lv_style_set_img_recolor(&style, BLUE_ON);
    lv_style_set_img_recolor(&style, lv_color_white());
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}


{
    LV_IMG_DECLARE(sunroof);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &sunroof);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



{
    LV_IMG_DECLARE(door_left);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &door_left);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



{
    LV_IMG_DECLARE(door_right);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &door_right);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



{
    LV_IMG_DECLARE(gas_cap);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &gas_cap);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



{
    LV_IMG_DECLARE(foglights);
    
    lv_obj_t * icon = lv_img_create(car_view_cont);
    lv_img_set_src(icon, &foglights);
    lv_obj_center(icon);
    // lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, lv_color_white());
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

    return car_view_cont;
}