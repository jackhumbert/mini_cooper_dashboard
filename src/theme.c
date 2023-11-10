#include "theme.h"
#include "dash.h"

static lv_style_t style_btn;
static lv_theme_t day_theme;

void theme_init() {
    /*Initialize the styles*/
    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, AMBER_ON);
    lv_style_set_text_color(&style_btn, lv_color_black());

    /*Initialize the new theme from the current theme*/
    lv_theme_t * th_act = lv_disp_get_theme(NULL);
    day_theme = *th_act;

    /*Set the parent theme and the style apply callback for the new theme*/
    lv_theme_set_parent(&day_theme, th_act);
    lv_theme_set_apply_cb(&day_theme, apply_theme_cb);

    /*Assign the new theme to the current display*/
    lv_disp_set_theme(NULL, &day_theme);
}

void apply_theme_cb(lv_theme_t * th, lv_obj_t * obj) {
    LV_UNUSED(th);

    if(lv_obj_check_type(obj, &lv_btn_class)) {
        lv_obj_add_style(obj, &style_btn, 0);
    }
}