#include "dash.h"
#include "tach.h"
#include "tach2.h"
#include "tach3.h"
#include "speed.h"
#include "clock.h"
#include "temp.h"
#include "car_view.h"
#include "turn_signal.h"
#include "oil.h"
#include "coolant.h"
#include "gas.h"
#include "messages.h"
#include "effect.h"
#include "theme.h"
#include "sd_card.h"
#include "activity.h"

static dashboard_t dashboard;
static dashboard_changed_t dashboard_changed;
static lv_obj_t * canvas;

dashboard_t * get_dash(void) {
    return &dashboard;
}

dashboard_changed_t * get_changed(void) {
    return &dashboard_changed;
}

static void dump_messages(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED) {
        sd_card_dump_messages();
    }
}

static void clear_messages(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED) {
        sd_card_clear_messages();
    }
}

lv_obj_t * dash(void) {

    theme_init();

    // fs_init();

    // lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_TRANSP, LV_PART_MAIN);
    // lv_disp_set_bg_opa(NULL, LV_OPA_TRANSP);

    // slightly red screen bg
    // lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex3(0x0F0100), 0);
    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(lv_scr_act(), LV_DIR_NONE);

    /*Create a buffer for the canvas*/
    // static uint8_t sbuf[CANVAS_WIDTH * CANVAS_HEIGHT * 4];

    // lv_obj_t * screen = lv_canvas_create(lv_scr_act());
    // lv_canvas_set_buffer(screen, sbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_ALPHA);
    // lv_obj_center(screen);

    // lv_canvas_fill_bg(screen, lv_palette_main(LV_PALETTE_NONE), LV_OPA_COVER);

    // lv_color_t * cbuf = heap_caps_malloc((CANVAS_WIDTH * CANVAS_HEIGHT * sizeof(lv_color_t)) / 4, MALLOC_CAP_DMA | MALLOC_CAP_SPIRAM);

    // DMA_ATTR lv_color_t cbuf = (lv_color_t *)malloc(screenWidth * BUFF_SIZE * sizeof(lv_color_t));

    /*Create a canvas and initialize its palette*/
    // canvas = lv_canvas_create(lv_scr_act());
    // // lv_obj_add_event_cb(canvas, canvas_draw_end, LV_EVENT_STYLE_CHANGED, NULL);
    // lv_obj_set_size(canvas, 800, 480);

    // lv_color_t * cbuf = (lv_color_t *)ps_malloc(LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT));
    // lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    // lv_obj_center(canvas);
    // // lv_obj_set_style_blend_mode(canvas, LV_BLEND_MODE_ADDITIVE, 0);
    // lv_canvas_fill_bg(canvas, lv_color_black(), LV_OPA_COVER);

    
    canvas = lv_scr_act();

    lv_obj_t * tach = tach3_create(canvas);
    lv_obj_align(tach, LV_ALIGN_TOP_MID, 0, 240);

    lv_obj_t * speed = speed_create(canvas);
    lv_obj_align(speed, LV_ALIGN_TOP_MID, 0, 40);
    // lv_obj_set_grid_cell(speed, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 2, 2);

    static lv_coord_t col_dsc[] = {48, 48, 48, 48, 48, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {72, 48, 48, 48, 48, 48, 157, 20, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    // lv_obj_t * cont = lv_obj_create(canvas);
    // lv_obj_remove_style_all(cont);
    // lv_obj_set_grid_align(cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    // lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    // lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    // lv_obj_set_size(cont, 800, 480);
    // lv_obj_center(cont);
    // lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_layout(cont, LV_LAYOUT_GRID);

    lv_obj_t * clock = clock_create(canvas);
    lv_obj_align(clock, LV_ALIGN_TOP_LEFT, 300, 180);

    lv_obj_t * temp = temp_create(canvas);
    lv_obj_align(temp, LV_ALIGN_TOP_RIGHT, -300, 180);

    lv_obj_t * turn_left = turn_signal_left_create(canvas);
    lv_obj_align(turn_left, LV_ALIGN_TOP_LEFT, 295 - 24, 308 - 24);
    // lv_obj_set_grid_cell(turn_left, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 2, 2);

    lv_obj_t * turn_right = turn_signal_right_create(canvas);
    lv_obj_align(turn_right, LV_ALIGN_TOP_LEFT, 505 - 24, 308 - 24);
    // lv_obj_set_grid_cell(turn_right, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 3, 2);

    lv_obj_t * car_view = car_view_create(canvas);
    lv_obj_align(car_view, LV_ALIGN_BOTTOM_MID, 0, -20);
    // lv_obj_set_grid_cell(car_view, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 6, 1);

    lv_obj_t * oil_pressure_view = oil_pressure_create(canvas);
    lv_obj_align(oil_pressure_view, LV_ALIGN_TOP_LEFT, 41, 357);

    lv_obj_t * oil_temp_view = oil_temp_create(canvas);
    lv_obj_align(oil_temp_view, LV_ALIGN_TOP_LEFT, 180, 316);

    lv_obj_t * coolant_view = coolant_create(canvas);
    lv_obj_align(coolant_view, LV_ALIGN_TOP_RIGHT, -180, 316);

    lv_obj_t * gas_view = gas_create(canvas);
    lv_obj_align(gas_view, LV_ALIGN_TOP_RIGHT, -41, 357);

    lv_obj_t * messages_view = messages_create(canvas);
    lv_obj_align(messages_view, LV_ALIGN_TOP_LEFT, 10, 10);

{
    lv_obj_t * msg_dump = lv_btn_create(canvas);
    lv_obj_add_event_cb(msg_dump, dump_messages, LV_EVENT_ALL, NULL);
    lv_obj_align(msg_dump, LV_ALIGN_TOP_RIGHT, -10, 50);

    lv_obj_t * label = lv_label_create(msg_dump);
    lv_label_set_text(label, "Dump Messages");
}

{
    lv_obj_t * msg_clea = lv_btn_create(canvas);
    lv_obj_add_event_cb(msg_clea, clear_messages, LV_EVENT_ALL, NULL);
    lv_obj_align(msg_clea, LV_ALIGN_TOP_RIGHT, -10, 100);

    lv_obj_t * label = lv_label_create(msg_clea);
    lv_label_set_text(label, "Close Log File");
}

{
    lv_obj_t * msg_clea = lv_btn_create(canvas);
    // lv_obj_add_event_cb(msg_clea, clear_messages, LV_EVENT_ALL, NULL);
    lv_obj_align(msg_clea, LV_ALIGN_TOP_RIGHT, -10, 150);

    lv_obj_t * label = lv_label_create(msg_clea);
    lv_label_set_text(label, "Dummy Button");
}

    activity_create(canvas);


    // setup_effect(canvas);
    return canvas;
}

void dash_loop(void) {
    pthread_mutex_lock(&get_dash()->mutex);
    if (get_changed()->speed) {
        speed_update();
        get_changed()->speed = 0;
    }
    if (get_changed()->engine_temp) {
        coolant_update();
        get_changed()->engine_temp = 0;
    }
    if (get_changed()->rpm) {
        tach3_update();
        get_changed()->rpm = 0;
    }
    if (get_changed()->fuel_level) {
        gas_update();
        get_changed()->fuel_level = 0;
    }
    if (get_changed()->running_clock) {
        clock_update();
        get_changed()->running_clock = 0;
    }
    if (get_changed()->outside_temp) {
        temp_update();
        get_changed()->outside_temp = 0;
    }
    if (get_changed()->left_turn_signal) {
        turn_signal_update();
        get_changed()->left_turn_signal = 0;
    }
    if (get_changed()->right_turn_signal) {
        turn_signal_update();
        get_changed()->right_turn_signal = 0;
    }
    if (get_changed()->left_door) {
        car_view_update();
        get_changed()->left_door = 0;
    }
	pthread_mutex_unlock(&get_dash()->mutex);
    sd_card_flush();
}