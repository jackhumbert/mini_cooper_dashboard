#include "car_view.hpp"

void CarView::update(void) {
    // if (get_queued()->left_door) {
    //     lv_obj_set_style_img_recolor(door_left_obj, get_cache()->left_door ? RED_ON : RED_OFF, 0);
    // }
    if (get_queued()->brights || get_queued()->running_lights) {
        if (get_cache()->brights) {
            lv_obj_set_style_img_recolor(headlights_obj, BLUE_ON, 0);
            lv_obj_set_style_opa(headlights_obj, LV_OPA_100, 0);
        } else if (get_cache()->running_lights) {
            lv_obj_set_style_img_recolor(headlights_obj, lv_color_hsv_to_rgb(20, 80, 100), 0);
            lv_obj_set_style_opa(headlights_obj, LV_OPA_100, 0);
        } else {
            lv_obj_set_style_opa(headlights_obj, LV_OPA_0, 0);
        }
    }
    if (get_queued()->handbrake) {
        if (get_cache()->handbrake) {
            lv_obj_set_style_bg_color(handbrake, RED_ON, 0);
        } else {
            lv_obj_set_style_bg_color(handbrake, RED_OFF, 0);
        }
    }
    if (get_queued()->hood) {
        if (get_cache()->hood) {
            // lv_obj_set_style_img_recolor(hood_obj, RED_ON, 0);
            lv_obj_set_style_opa(hood_obj, LV_OPA_100, 0);
        } else {
            // lv_obj_set_style_img_recolor(hood_obj, RED_OFF, 0);
            lv_obj_set_style_opa(hood_obj, LV_OPA_0, 0);
        }
    }
    if (get_queued()->eml_light) {
        if (get_cache()->eml_light) {
            lv_obj_set_style_img_recolor(eml_obj, AMBER_ON, 0);
        } else {
            lv_obj_set_style_img_recolor(eml_obj, AMBER_OFF, 0);
        }
    }
    if (get_queued()->check_engine_light) {
        if (get_cache()->check_engine_light) {
            lv_obj_set_style_img_recolor(check_engine_obj, AMBER_ON, 0);
        } else {
            lv_obj_set_style_img_recolor(check_engine_obj, AMBER_OFF, 0);
        }
    }
    if (get_queued()->charge_light) {
        if (get_cache()->charge_light) {
            lv_obj_set_style_img_recolor(battery_obj, RED_ON, 0);
        } else {
            lv_obj_set_style_img_recolor(battery_obj, RED_OFF, 0);
        }
    }
    if (get_queued()->abs_light) {
        if (get_cache()->abs_light) {
            lv_obj_set_style_img_recolor(abs_obj, AMBER_ON, 0);
        } else {
            lv_obj_set_style_img_recolor(abs_obj, AMBER_OFF, 0);
        }
    }
}

CarView::CarView(lv_obj_t * parent) {

    lv_obj = lv_obj_create(parent);
    lv_obj_remove_style_all(lv_obj);
    lv_obj_set_scrollbar_mode(lv_obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(lv_obj, LV_DIR_NONE);
    lv_obj_set_size(lv_obj, 84 + 48 * 2, 158 + 40); // 84
    lv_obj_align(lv_obj, LV_ALIGN_BOTTOM_MID, 0, -20);

    LV_IMG_DECLARE(car_view_fill);
    LV_IMG_DECLARE(car_view);
    LV_IMG_DECLARE(hood);
    LV_IMG_DECLARE(trunk);
    LV_IMG_DECLARE(headlights);
    LV_IMG_DECLARE(sunroof);
    LV_IMG_DECLARE(door_left);
    LV_IMG_DECLARE(door_right);
    LV_IMG_DECLARE(gas_cap);
    LV_IMG_DECLARE(foglights);

    lv_obj_t * car_view_fill_obj = lv_img_create(lv_obj);
    lv_img_set_src(car_view_fill_obj, &car_view_fill);
    lv_obj_align(car_view_fill_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_img_recolor(car_view_fill_obj, AMBER_OFF, 0);
    lv_obj_set_style_img_recolor_opa(car_view_fill_obj, LV_OPA_100, 0);

    car_view_obj = lv_img_create(lv_obj);
    lv_img_set_src(car_view_obj, &car_view);
    lv_obj_align(car_view_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_img_recolor(car_view_obj, DASH_BACKGROUND, 0);
    lv_obj_set_style_img_recolor_opa(car_view_obj, LV_OPA_100, 0);
    
    hood_obj = lv_img_create(lv_obj);
    lv_img_set_src(hood_obj, &hood);
    lv_obj_align(hood_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_img_recolor(hood_obj, RED_ON, 0);
    lv_obj_set_style_img_recolor_opa(hood_obj, LV_OPA_100, 0);
    lv_obj_set_style_opa(hood_obj, LV_OPA_0, 0);
    
    // trunk_obj = lv_img_create(lv_obj);
    // lv_img_set_src(trunk_obj, &trunk);
    // lv_obj_align(trunk_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    // lv_obj_set_style_img_recolor(trunk_obj, RED_OFF, 0);
    // lv_obj_set_style_opa(trunk_obj, LV_OPA_100, 0);

    headlights_obj = lv_img_create(lv_obj);
    lv_img_set_src(headlights_obj, &headlights);
    lv_obj_align(headlights_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_img_recolor(headlights_obj, AMBER_ON, 0);
    lv_obj_set_style_img_recolor_opa(headlights_obj, LV_OPA_100, 0);
    lv_obj_set_style_opa(headlights_obj, LV_OPA_0, 0);
    
    // sunroof_obj = lv_img_create(lv_obj);
    // lv_img_set_src(sunroof_obj, &sunroof);
    // lv_obj_align(sunroof_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    // lv_obj_set_style_img_recolor(sunroof_obj, RED_OFF, 0);
    // lv_obj_set_style_opa(sunroof_obj, LV_OPA_100, 0);
    
    // door_left_obj = lv_img_create(lv_obj);
    // lv_img_set_src(door_left_obj, &door_left);
    // lv_obj_align(door_left_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    // lv_obj_set_style_img_recolor(door_left_obj, RED_OFF, 0);
    // lv_obj_set_style_opa(door_left_obj, LV_OPA_100, 0);
    
    // door_right_obj = lv_img_create(lv_obj);
    // lv_img_set_src(door_right_obj, &door_right);
    // lv_obj_align(door_right_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    // lv_obj_set_style_img_recolor(door_right_obj, RED_OFF, 0);
    // lv_obj_set_style_opa(door_right_obj, LV_OPA_100, 0);
    
    gas_cap_obj = lv_img_create(lv_obj);
    lv_img_set_src(gas_cap_obj, &gas_cap);
    lv_obj_align(gas_cap_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_img_recolor(gas_cap_obj, RED_OFF, 0);
    lv_obj_set_style_img_recolor_opa(gas_cap_obj, LV_OPA_100, 0);
    lv_obj_set_style_opa(gas_cap_obj, LV_OPA_0, 0);

    foglights_obj = lv_img_create(lv_obj);
    lv_img_set_src(foglights_obj, &foglights);
    lv_obj_align(foglights_obj, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_img_recolor(foglights_obj, RED_OFF, 0);
    lv_obj_set_style_img_recolor_opa(foglights_obj, LV_OPA_100, 0);
    lv_obj_set_style_opa(foglights_obj, LV_OPA_0, 0);

    DASH_FONT(RAJDHANI_SEMIBOLD, 18);
    handbrake = lv_label_create(lv_obj);
    lv_obj_set_size(handbrake, 120, 25);
    lv_obj_set_style_text_align(handbrake, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(handbrake, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(handbrake, RED_OFF, 0);
    lv_obj_set_style_bg_opa(handbrake, LV_OPA_100, 0);
    lv_obj_set_style_radius(handbrake, 8, 0);
    lv_label_set_text(handbrake, "BRAKE");
    lv_obj_set_style_text_font(handbrake, RAJDHANI_SEMIBOLD_18, 0);
    lv_obj_set_style_text_color(handbrake, lv_color_black(), 0);
    lv_obj_set_style_pad_top(handbrake, 3, 0);

    // abs
    LV_IMG_DECLARE(slice12);

    abs_obj = lv_img_create(lv_obj);
    lv_img_set_src(abs_obj, &slice12);
    lv_obj_set_size(abs_obj, 48, 48);
    lv_obj_align(abs_obj, LV_ALIGN_BOTTOM_LEFT, 0, -48 * 2);
    lv_obj_set_style_img_recolor(abs_obj, AMBER_OFF, 0);
    lv_obj_set_style_img_recolor_opa(abs_obj, LV_OPA_100, 0);

    // spinny
    LV_IMG_DECLARE(slice3);

    spinny_obj = lv_img_create(lv_obj);
    lv_img_set_src(spinny_obj, &slice3);
    lv_obj_set_size(spinny_obj, 48, 48);
    lv_obj_align(spinny_obj, LV_ALIGN_BOTTOM_LEFT, 0, -48);
    lv_obj_set_style_img_recolor(spinny_obj, AMBER_OFF, 0);
    lv_obj_set_style_img_recolor_opa(spinny_obj, LV_OPA_100, 0);

    // tire pressure
    LV_IMG_DECLARE(slice8);

    tire_obj = lv_img_create(lv_obj);
    lv_img_set_src(tire_obj, &slice8);
    lv_obj_set_size(tire_obj, 48, 48);
    lv_obj_align(tire_obj, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_img_recolor(tire_obj, AMBER_OFF, 0);
    lv_obj_set_style_img_recolor_opa(tire_obj, LV_OPA_100, 0);


    // check_engine
    LV_IMG_DECLARE(slice11);

    check_engine_obj = lv_img_create(lv_obj);
    lv_img_set_src(check_engine_obj, &slice11);
    lv_obj_set_size(check_engine_obj, 48, 48);
    lv_obj_align(check_engine_obj, LV_ALIGN_BOTTOM_RIGHT, 0, -48 * 2);
    lv_obj_set_style_img_recolor(check_engine_obj, AMBER_OFF, 0);
    lv_obj_set_style_img_recolor_opa(check_engine_obj, LV_OPA_100, 0);

    // eml
    LV_IMG_DECLARE(slice2);

    eml_obj = lv_img_create(lv_obj);
    lv_img_set_src(eml_obj, &slice2);
    lv_obj_set_size(eml_obj, 48, 48);
    lv_obj_align(eml_obj, LV_ALIGN_BOTTOM_RIGHT, 0, -48);
    lv_obj_set_style_img_recolor(eml_obj, AMBER_OFF, 0);
    lv_obj_set_style_img_recolor_opa(eml_obj, LV_OPA_100, 0);

    // battery
    LV_IMG_DECLARE(slice6);

    battery_obj = lv_img_create(lv_obj);
    lv_img_set_src(battery_obj, &slice6);
    lv_obj_set_size(battery_obj, 48, 48);
    lv_obj_align(battery_obj, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_style_img_recolor(battery_obj, RED_OFF, 0);
    lv_obj_set_style_img_recolor_opa(battery_obj, LV_OPA_100, 0);

}