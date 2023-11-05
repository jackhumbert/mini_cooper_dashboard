#include "time.h"

lv_obj_t * time_create(lv_obj_t * parent) {

    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    // info.name = "./fonts/Microgramma-D-Extended-Medium.otf";
    // info.name = "./fonts/Rajdhani-Regular.ttf";
    // info.name = "./fonts/DSEG14ClassicMini-Regular.ttf";
    info.name = "./fonts/Bould-Medium.otf";
    info.weight = 22;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

// {
//     static lv_style_t style;
//     lv_style_init(&style);
//     lv_style_set_text_opa(&style, LV_OPA_100);
//     lv_style_set_text_color(&style, AMBER_OFF);
//     lv_style_set_text_font(&style, info.font);

//     lv_obj_t * time_label = lv_label_create(cont);
//     lv_label_set_text(time_label, "88:88 8~");
//     lv_obj_add_style(time_label, &style, 0);
//     lv_obj_set_style_blend_mode(time_label, LV_BLEND_MODE_ADDITIVE, 0);
//     lv_obj_set_grid_cell(time_label, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 2, 1);

//     lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);

// }
 
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_ON);
    lv_style_set_text_font(&style, info.font);

    lv_obj_t * time_label = lv_label_create(parent);
    lv_label_set_text(time_label, "5:30 PM");
    lv_obj_add_style(time_label, &style, 0);
    lv_obj_set_style_blend_mode(time_label, LV_BLEND_MODE_ADDITIVE, 0);

    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);

    return time_label;
}