#include "dash.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "tach.h"
#include "tach2.h"
#include "tach3.hpp"
#include "speed.hpp"
#include "clock.h"
#include "outside_temp.hpp"
#include "car_view.hpp"
#include "turn_signal.h"
#include "oil_temp.hpp"
#include "oil_pressure.hpp"
#include "coolant.hpp"
#include "fuel.hpp"
#include "messages.h"
#include "effect.h"
#include "theme.h"
#include "sd_card.h"
#include "activity.h"

extern void start_screen_fade(void);

#ifdef __cplusplus
} // extern "C"
#endif

#include "bit_table.hpp"
#include <Arduino.h>
#include <vector>

static dashboard_t dashboard;
static dashboard_t dashboard_cache;
static dashboard_changed_t dashboard_changed;
static dashboard_changed_t dashboard_queued;
static lv_obj_t * canvas;
std::vector<Widget*> widgets;

dashboard_t * get_dash(void) {
    return &dashboard;
}

dashboard_t * get_cache(void) {
    return &dashboard_cache;
}

dashboard_changed_t * get_changed(void) {
    return &dashboard_changed;
}

dashboard_changed_t * get_queued(void) {
    return &dashboard_queued;
}

static uint8_t event_num = 0;

static void manually_log_event(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED) {
        sd_card_logf("%08.3f R11 00000A00 %02X 00 00 00 00 00 00 00\n", xTaskGetTickCount() / 1000.0, event_num++);
    }
}

static void clear_messages(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED) {
        sd_card_clear_messages();
    }
}

lv_obj_t * dash_create(lv_disp_t * disp) {

	pthread_mutex_init(&dashboard.mutex, NULL);
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

    widgets.push_back(new Tach(canvas));
    widgets.push_back(new Speed(canvas));

    // static lv_coord_t col_dsc[] = {48, 48, 48, 48, 48, LV_GRID_TEMPLATE_LAST};
    // static lv_coord_t row_dsc[] = {72, 48, 48, 48, 48, 48, 157, 20, LV_GRID_TEMPLATE_LAST};

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
    lv_obj_align(clock, LV_ALIGN_TOP_LEFT, 300, 160);

    widgets.push_back(new OutsideTemp(canvas));

    lv_obj_t * turn_left = turn_signal_left_create(canvas);
    lv_obj_align(turn_left, LV_ALIGN_TOP_LEFT, 295 - 24, 308 - 40);
    // lv_obj_set_grid_cell(turn_left, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 2, 2);

    lv_obj_t * turn_right = turn_signal_right_create(canvas);
    lv_obj_align(turn_right, LV_ALIGN_TOP_RIGHT, -(295 - 24), 308 - 40);
    // lv_obj_set_grid_cell(turn_right, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 3, 2);

    widgets.push_back(new CarView(canvas));
    widgets.push_back(new OilPressure(canvas));
    widgets.push_back(new OilTemp(canvas));
    widgets.push_back(new Coolant(canvas));
    widgets.push_back(new Fuel(canvas));

    lv_obj_t * messages_view = messages_create(canvas);
    lv_obj_align(messages_view, LV_ALIGN_TOP_LEFT, 10, 10);

// {
//     lv_obj_t * msg_dump = lv_btn_create(canvas);
//     lv_obj_add_event_cb(msg_dump, manually_log_event, LV_EVENT_ALL, NULL);
//     lv_obj_align(msg_dump, LV_ALIGN_TOP_RIGHT, -10, 50);

//     DASH_FONT(RAJDHANI_SEMIBOLD, 16);
//     lv_obj_t * label = lv_label_create(msg_dump);
//     lv_obj_set_style_text_font(label, RAJDHANI_SEMIBOLD_16, 0);
//     lv_label_set_text(label, "Log Event");
// }

// {
//     lv_obj_t * msg_clea = lv_btn_create(canvas);
//     lv_obj_add_event_cb(msg_clea, clear_messages, LV_EVENT_ALL, NULL);
//     lv_obj_align(msg_clea, LV_ALIGN_TOP_RIGHT, -10, 100);

//     lv_obj_t * label = lv_label_create(msg_clea);
//     lv_label_set_text(label, "Close Log File");
// }

// {
//     lv_obj_t * msg_clea = lv_btn_create(canvas);
//     // lv_obj_add_event_cb(msg_clea, clear_messages, LV_EVENT_ALL, NULL);
//     lv_obj_align(msg_clea, LV_ALIGN_TOP_RIGHT, -10, 150);

//     lv_obj_t * label = lv_label_create(msg_clea);
//     lv_label_set_text(label, "Dummy Button");
// }

    activity_create(canvas);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
    lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_BETWEEN);
    lv_style_set_layout(&style, LV_LAYOUT_FLEX);

    lv_obj_t * cont = lv_obj_create(canvas);
    lv_obj_remove_style_all(cont);
    lv_obj_set_size(cont, 800, 480);
    lv_obj_center(cont);
    lv_obj_add_style(cont, &style, 0);

    // widgets.push_back(new BitTable(cont, &dashboard_queued.x1F3, 0x1F3, &dashboard.x1F3));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x316, 0x316, &dashboard.x316));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x329, 0x329, &dashboard.x329));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x336, 0x336, &dashboard.x336));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x501, 0x501, &dashboard.x501));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x545, 0x545, &dashboard.x545));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x565, 0x565, &dashboard.x565));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x610, 0x610, &dashboard.x610));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x613, 0x613, &dashboard.x613));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x615, 0x615, &dashboard.x615));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x618, 0x618, &dashboard.x618));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x61A, 0x61A, &dashboard.x61A));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x61F, 0x61F, &dashboard.x61F));


    // setup_effect(canvas);
    return canvas;
}

void dash_loop(void) {
    pthread_mutex_lock(&get_dash()->mutex);
    memcpy(&dashboard_queued, &dashboard_changed, sizeof(dashboard_changed_t));
    memcpy(&dashboard_cache, &dashboard, sizeof(dashboard_t));
    memset(&dashboard_changed, 0, sizeof(dashboard_changed_t));
	pthread_mutex_unlock(&get_dash()->mutex);

    for (auto widget : widgets) {
        widget->update();
    }
    if (dashboard_queued.running_clock) {
        clock_update();
    }
    if (dashboard_queued.left_turn_signal) {
        turn_signal_update();
    }
    if (dashboard_queued.right_turn_signal) {
        turn_signal_update();
    }
    if (dashboard_queued.running_lights) {
        start_screen_fade();
    }
    activity_update(dashboard_queued.activity);
    // sd_card_flush();
}