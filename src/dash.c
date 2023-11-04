#include "dash.h"
#include "tach.h"

lv_obj_t * canvas;

void dash(void) {

    // lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_TRANSP, LV_PART_MAIN);
    // lv_disp_set_bg_opa(NULL, LV_OPA_TRANSP);

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex3(0x0F0100), 0);

    /*Create a buffer for the canvas*/
    // static uint8_t sbuf[CANVAS_WIDTH * CANVAS_HEIGHT * 4];

    // lv_obj_t * screen = lv_canvas_create(lv_scr_act());
    // lv_canvas_set_buffer(screen, sbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_ALPHA);
    // lv_obj_center(screen);

    // lv_canvas_fill_bg(screen, lv_palette_main(LV_PALETTE_NONE), LV_OPA_COVER);

    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(CANVAS_WIDTH, CANVAS_HEIGHT)];

    /*Create a canvas and initialize its palette*/
    canvas = lv_canvas_create(lv_scr_act());
    // lv_obj_add_event_cb(canvas, canvas_draw_end, LV_EVENT_DRAW_POST, NULL);
    lv_obj_set_size(canvas, 800, 480);
    lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_ALPHA);
    lv_obj_center(canvas);
    // lv_canvas_fill_bg(canvas, lv_palette_darken(LV_PALETTE_AMBER, 3), LV_OPA_COVER);

    // lv_canvas_fill_bg(canvas, lv_palette_main(LV_PALETTE_BLUE), LV_OPA_COVER);

    tach(canvas);

}