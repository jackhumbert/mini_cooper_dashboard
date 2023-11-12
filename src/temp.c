#include "temp.h"
#include <sys/time.h>

lv_obj_t * temp_label;

void temp_update() {
    lv_label_set_text_fmt(temp_label, "%d°F", get_dash()->outside_temp * 9/5 + 32);
}

lv_obj_t * temp_create(lv_obj_t * parent) {

    // static lv_ft_info_t info;
    // /*FreeType uses C standard file system, so no driver letter is required.*/
    // // info.name = "./fonts/Microgramma-D-Extended-Medium.otf";
    // // info.name = "./fonts/Rajdhani-Regular.ttf";
    // // info.name = "./fonts/DSEG14ClassicMini-Regular.ttf";
    // info.name = "./fonts/Bould-Medium.otf";
    // info.weight = 22;
    // info.style = FT_FONT_STYLE_NORMAL;
    // info.mem = NULL;
    // if(!lv_ft_font_init(&info)) {
    //     LV_LOG_ERROR("create failed.");
    // }

    DASH_FONT(RAJDHANI_REGULAR, 36);

// {
//     static lv_style_t style;
//     lv_style_init(&style);
//     lv_style_set_text_opa(&style, LV_OPA_100);
//     lv_style_set_text_color(&style, AMBER_OFF);
//     lv_style_set_text_font(&style, info.font);

//     lv_obj_t * temp_label = lv_label_create(cont);
//     lv_label_set_text(temp_label, "88:88 8~");
//     lv_obj_add_style(temp_label, &style, 0);
//     lv_obj_set_style_blend_mode(temp_label, LV_BLEND_MODE_ADDITIVE, 0);
//     lv_obj_set_grid_cell(temp_label, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 2, 1);

//     lv_obj_align(temp_label, LV_ALIGN_CENTER, 0, 0);

// }
 
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_ON);
    lv_style_set_text_font(&style, RAJDHANI_REGULAR_36);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_RIGHT);

    temp_label = lv_label_create(parent);
    // lv_label_set_text(temp_label, "5:30 PM");

    lv_label_set_text(temp_label, "--°F");
    lv_obj_add_style(temp_label, &style, 0);
    // lv_obj_set_style_blend_mode(temp_label, LV_BLEND_MODE_ADDITIVE, 0);

    // lv_obj_align(temp_label, LV_ALIGN_LEFT_MID, 0, 0);

    return temp_label;
}