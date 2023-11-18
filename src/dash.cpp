#include "dash.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "tach.h"
#include "tach2.h"
#include "tach3.hpp"
#include "tach4.hpp"
#include "speed.hpp"
#include "clock.hpp"
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
#include "accel.hpp"
#include "odometer.hpp"

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
        sd_card_logf("%08.3f R11 00000A00 %02X 00 00 00 00 00 00 00\n", xTaskGetTickCount() / 1000.0, event_num);
        add_message_fmt("Custom event %d logged", event_num);
        event_num++;
    }
}

static bool events = true;

static void toggle_events(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED) {
        events = !events;
    }
}

lv_obj_t * logging_label;

static void toggle_logging(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_CLICKED) {
        if (events) { 
            if (sd_card_init()) {
                events = false;
                lv_label_set_text(logging_label, "Disable Logging");
            } else {
                lv_obj_clear_state(obj, LV_STATE_CHECKED);
            }
        } else {
            stop_logging();
            lv_label_set_text(logging_label, "Enable Logging");
        }
    }
}

lv_obj_t * dash_create(lv_disp_t * disp) {
	pthread_mutex_init(&dashboard.mutex, NULL);
    theme_init();

    // slightly red screen bg
    lv_obj_set_style_bg_color(lv_scr_act(), DASH_BACKGROUND, 0);
    // lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(lv_scr_act(), LV_DIR_NONE);

    canvas = lv_scr_act();

    widgets.push_back(new Tach4(canvas));
    widgets.push_back(new Speed(canvas));
    widgets.push_back(new Clock(canvas));
    widgets.push_back(new OutsideTemp(canvas));

    lv_obj_t * turn_left = turn_signal_left_create(canvas);
    lv_obj_align(turn_left, LV_ALIGN_TOP_LEFT, 295 - 24, 308 - 40);

    lv_obj_t * turn_right = turn_signal_right_create(canvas);
    lv_obj_align(turn_right, LV_ALIGN_TOP_RIGHT, -(295 - 24), 308 - 40);

    widgets.push_back(new CarView(canvas));
    widgets.push_back(new OilPressure(canvas));
    widgets.push_back(new OilTemp(canvas));
    widgets.push_back(new Coolant(canvas));
    widgets.push_back(new Fuel(canvas));
    widgets.push_back(new Accel(canvas));
    widgets.push_back(new Odometer(canvas));

    lv_obj_t * messages_view = messages_create(canvas);
    lv_obj_align(messages_view, LV_ALIGN_TOP_LEFT, 5, 5);



    // static lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
    // lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_BETWEEN);
    // lv_style_set_layout(&style, LV_LAYOUT_FLEX);
    // lv_style_set_pad_top(&style, 40);

    // lv_obj_t * cont = lv_obj_create(canvas);
    // lv_obj_remove_style_all(cont);
    // lv_obj_set_size(cont, 800, 480);
    // lv_obj_center(cont);
    // lv_obj_add_style(cont, &style, 0);

    // widgets.push_back(new BitTable(cont, &dashboard_queued.x153, 0x153, &dashboard.x153));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x1F3, 0x1F3, &dashboard.x1F3));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x1F5, 0x1F5, &dashboard.x1F5));
    // widgets.push_back(new BitTable(cont, &dashboard_queued.x1F8, 0x1F8, &dashboard.x1F8));
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


// {
//     lv_obj_t * msg_dump = lv_btn_create(canvas);
//     lv_obj_add_event_cb(msg_dump, manually_log_event, LV_EVENT_ALL, NULL);
//     lv_obj_align(msg_dump, LV_ALIGN_TOP_RIGHT, -10, 50);

//     lv_obj_t * label = lv_label_create(msg_dump);
//     lv_obj_set_style_text_font(label, RAJDHANI_SEMIBOLD_16, 0);
//     lv_label_set_text(label, "Log Event");
// }

// {
//     lv_obj_t * msg_clea = lv_btn_create(canvas);
//     lv_obj_add_event_cb(msg_clea, toggle_events, LV_EVENT_ALL, NULL);
//     lv_obj_align(msg_clea, LV_ALIGN_TOP_RIGHT, -10, 100);

//     lv_obj_t * label = lv_label_create(msg_clea);
//     lv_label_set_text(label, "Close Log File");
// }

{
    lv_obj_t * msg_clea = lv_btn_create(canvas);
    lv_obj_add_flag(msg_clea, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(msg_clea, toggle_logging, LV_EVENT_ALL, NULL);
    lv_obj_align(msg_clea, LV_ALIGN_TOP_RIGHT, -5, 50);

    logging_label = lv_label_create(msg_clea);
    lv_label_set_text(logging_label, "Enable Logging");
}

// {
//     lv_obj_t * msg_clea = lv_btn_create(canvas);
//     lv_obj_add_flag(msg_clea, LV_OBJ_FLAG_CHECKABLE);
//     lv_obj_add_event_cb(msg_clea, toggle_events, LV_EVENT_ALL, NULL);
//     lv_obj_align(msg_clea, LV_ALIGN_TOP_RIGHT, -5, 80);

//     lv_obj_t * label = lv_label_create(msg_clea);
//     lv_label_set_text(label, "Toggle Updates");
// }

    activity_create(canvas);

    // memset(&dashboard_changed, 1, sizeof(dashboard_changed_t));
    // dashboard_queued.running_lights = 0;
    // dashboard_queued.activity = 0;
    // dashboard_queued.x610 = 0;
    // dashboard_queued.custom_value = 0;

    // setup_effect(canvas);
    return canvas;
}

void dash_loop(void) {
    pthread_mutex_lock(&get_dash()->mutex);
    memcpy(&dashboard_queued, &dashboard_changed, sizeof(dashboard_changed_t));
    memcpy(&dashboard_cache, &dashboard, sizeof(dashboard_t));
    memset(&dashboard_changed, 0, sizeof(dashboard_changed_t));
	pthread_mutex_unlock(&get_dash()->mutex);

    if (events) {
        for (auto widget : widgets) {
            widget->update();
        }
        if (dashboard_queued.x610) {
            uint8_t * vin = (uint8_t*)&dashboard_cache.x610;
            // add_message_fmt("VIN: %llX", dashboard_cache.x610);
            add_message_fmt("VIN: %c%c%02X%02X%X", vin[4], vin[3], vin[2], vin[1], vin[0] >> 4);
        }
        if (dashboard_queued.custom_value) {
            switch (dashboard_cache.stalk_state) {
                case average_speed: 
                    add_message_fmt("Average Speed: %0.1f mph", dashboard_cache.custom_value / 10.0);
                    break;
                case outside_temp: 
                    add_message_fmt("Outside Temp: %0.1f°F", dashboard_cache.custom_value / 10.0);
                    break;
                case range: 
                    add_message_fmt("Range: %0.1f miles", dashboard_cache.custom_value / 10.0);
                    break;
                case average_consumption: 
                    add_message_fmt("Average Consumption: %0.1f mpg", dashboard_cache.custom_value / 10.0);
                    break;
                case current_consumption: 
                    add_message_fmt("Current Consumption: %0.1f mpg", dashboard_cache.custom_value / 10.0);
                    break;
            }
        }
        turn_signal_update();
        if (dashboard_queued.running_lights) {
            start_screen_fade();
        }
    }
    activity_update(dashboard_queued.activity);
}