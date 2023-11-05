#include "dash.h"
#include "tach.h"
#include "speed.h"
#include "time.h"
#include "car_view.h"
#include "turn_signal.h"
#include "oil.h"
#include "coolant.h"
#include "effect.h"

lv_obj_t * canvas;

// static void canvas_draw_end(lv_event_t * e) {
    // if (need_to_affect) {
    //     need_to_affect = false;
        // update_effect();
    // }
// }

lv_obj_t * dash(void) {

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
    // lv_obj_add_event_cb(canvas, canvas_draw_end, LV_EVENT_STYLE_CHANGED, NULL);
    lv_obj_set_size(canvas, 800, 480);
    lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_ALPHA);
    lv_obj_center(canvas);
    // lv_canvas_fill_bg(canvas, lv_palette_darken(LV_PALETTE_AMBER, 3), LV_OPA_COVER);

    // lv_canvas_fill_bg(canvas, lv_palette_main(LV_PALETTE_BLUE), LV_OPA_COVER);

    tach(canvas);

    static lv_coord_t col_dsc[] = {48, 48, 36, 48, 48, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {72, 48, 48, 48, 48, 48, 48, 48, 48, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t * cont = lv_obj_create(canvas);
    lv_obj_remove_style_all(cont);
    lv_obj_set_grid_align(cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 800, 480);
    lv_obj_center(cont);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    lv_obj_t * speed = speed_create(cont);
    lv_obj_set_grid_cell(speed, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 2, 2);

    lv_obj_t * time = time_create(cont);
    lv_obj_set_grid_cell(time, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 4, 1);

    lv_obj_t * turn_left = turn_signal_left_create(cont);
    lv_obj_set_grid_cell(turn_left, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 2, 2);

    lv_obj_t * turn_right = turn_signal_right_create(cont);
    lv_obj_set_grid_cell(turn_right, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 3, 2);

    lv_obj_t * car_view = car_view_create(cont);
    lv_obj_set_grid_cell(car_view, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 6, 2);

    lv_obj_t * oil_view = oil_create(canvas);
    lv_obj_align(oil_view, LV_ALIGN_CENTER, -280, 100);

    lv_obj_t * coolant_view = coolant_create(canvas);
    lv_obj_align(coolant_view, LV_ALIGN_CENTER, -300, 20);

    // setup_effect(canvas);
    return canvas;
}