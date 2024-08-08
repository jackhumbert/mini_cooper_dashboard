#include "outside_temp.hpp"
#include "tiny_display.h"

void OutsideTemp::update(void) {
    if (get_queued()->outside_temp) {
        lv_label_set_text_fmt(lv_obj, "%d°F", get_dash()->outside_temp * 9/5 + 32);
        auto data = get_peer_data();
        if (data) {
            data->outside_temp = (float)get_dash()->outside_temp * 9/5 + 32;
            send_peer_data();
        }
    }
}

OutsideTemp::OutsideTemp(lv_obj_t * parent) {

    setup_peer_data();

    DASH_FONT(RAJDHANI_REGULAR, 36);
 
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_ON);
    lv_style_set_text_font(&style, RAJDHANI_REGULAR_36);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_RIGHT);

    lv_obj = lv_label_create(parent);
    lv_label_set_text(lv_obj, "--°F");
    lv_obj_add_style(lv_obj, &style, 0);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_RIGHT, -300, 150);
}