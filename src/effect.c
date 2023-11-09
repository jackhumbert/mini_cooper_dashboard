#include "effect.h"

lv_color_t cbuf_tmp[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
lv_img_dsc_t img;

lv_obj_t * effect1;
lv_obj_t * effect2;
lv_color_t ebuf1[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
lv_color_t ebuf2[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];

bool should_update_effect = false;

void effect_callback(lv_timer_t * timer) {
    if (should_update_effect && true) {
        lv_obj_t * canvas = timer->user_data;
        should_update_effect = false;
        lv_snapshot_take_to_buf(canvas, LV_IMG_CF_TRUE_COLOR, &img, &cbuf_tmp, sizeof(cbuf_tmp));
    
        // lv_canvas_transform(effect1, &img, 0, 260, (260.0/256.0 - 1) * CANVAS_WIDTH / 2, (260.0/256.0 - 1) * CANVAS_HEIGHT / 2, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
        lv_canvas_transform(effect1, &img, 0, 260, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, false);

        // lv_canvas_blur_hor(effect1, NULL, 3);
        // lv_canvas_blur_ver(effect1, NULL, 3);
        // lv_canvas_blur_hor(effect1, NULL, 2);
        // lv_canvas_blur_ver(effect1, NULL, 2);
        // lv_canvas_blur_hor(effect1, NULL, 2);
        // lv_canvas_blur_ver(effect1, NULL, 2);
        lv_snapshot_take_to_buf(effect1, LV_IMG_CF_TRUE_COLOR, &img, &cbuf_tmp, sizeof(cbuf_tmp));

        // lv_canvas_transform(effect2, &img, 0, 264, (264.0/256.0 - 1) * CANVAS_WIDTH / 2, (264.0/256.0 - 1) * CANVAS_HEIGHT / 2, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
        // lv_canvas_transform(effect2, &img, 0, 264, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
        lv_canvas_transform(effect2, &img, 0, 260, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, false);

        // lv_canvas_blur_hor(effect2, NULL, 3);
        // lv_canvas_blur_ver(effect2, NULL, 3);
        // lv_canvas_blur_hor(effect2, NULL, 2);
        // lv_canvas_blur_ver(effect2, NULL, 2);
        // lv_canvas_blur_hor(effect2, NULL, 2);
        // lv_canvas_blur_ver(effect2, NULL, 2);
        // lv_canvas_blur_hor(effect2, NULL, 2);
        // lv_canvas_blur_ver(effect2, NULL, 2);
        // lv_canvas_blur_hor(effect2, NULL, 2);
        // lv_canvas_blur_ver(effect2, NULL, 2);
        // lv_canvas_blur_hor(effect2, NULL, 2);
        // lv_canvas_blur_ver(effect2, NULL, 2);
    }
}

void setup_effect(lv_obj_t * canvas) {
    // memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));
    img.data = (void *)cbuf_tmp;
    img.header.cf = LV_IMG_CF_TRUE_COLOR;
    img.header.w = CANVAS_WIDTH;
    img.header.h = CANVAS_HEIGHT;

    effect1 = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(effect1, ebuf1, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_style_bg_color(effect1, lv_color_black(), 0);    
    // lv_obj_move_background(effect1);
    lv_obj_center(effect1);
    lv_obj_set_style_opa(effect1, 50, 0);
    lv_obj_set_style_blend_mode(effect1, LV_BLEND_MODE_ADDITIVE, 0);


    effect2 = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(effect2, ebuf2, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_style_bg_color(effect2, lv_color_black(), 0);    
    // lv_obj_move_background(effect2);
    lv_obj_center(effect2);
    lv_obj_set_style_opa(effect2, 25, 0);
    lv_obj_set_style_blend_mode(effect2, LV_BLEND_MODE_ADDITIVE, 0);


    lv_timer_t * timer = lv_timer_create(effect_callback, 5, canvas);
}

void update_effect(void) {
    should_update_effect = true;
}