#include "theme.h"
#include "dash.h"

static lv_style_t style_obj;
static lv_style_t style_btn;
static lv_theme_t day_theme;

static void apply_theme_cb(lv_theme_t * th, lv_obj_t * obj) {
    LV_UNUSED(th);

    if (lv_obj_check_type(obj, &lv_obj_class)) {
        lv_obj_add_style(obj, &style_obj, 0);
        lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_scroll_dir(obj, LV_DIR_NONE);
    }

    if (lv_obj_check_type(obj, &lv_btn_class)) {
        lv_obj_add_style(obj, &style_btn, 0);
    }
}

void theme_init() {
    DASH_FONT(RAJDHANI_MEDIUM, 16);

    lv_style_init(&style_obj);
    lv_style_set_border_width(&style_obj, 0);
    lv_style_set_bg_color(&style_obj, DASH_BACKGROUND);
    lv_style_set_radius(&style_obj, 0);
    lv_style_set_pad_all(&style_obj, 0);

    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, AMBER_ON);
    lv_style_set_text_color(&style_btn, lv_color_black());
    lv_style_set_text_font(&style_btn, RAJDHANI_MEDIUM_16);
    lv_style_set_pad_top(&style_btn, 6);
    lv_style_set_pad_bottom(&style_btn, 4);
    lv_style_set_pad_hor(&style_btn, 12);
    lv_style_set_radius(&style_btn, 8);
    
    lv_theme_t * th_act = lv_disp_get_theme(NULL);
    day_theme = *th_act;

    /*Set the parent theme and the style apply callback for the new theme*/
    lv_theme_set_parent(&day_theme, th_act);
    lv_theme_set_apply_cb(&day_theme, apply_theme_cb);

    /*Assign the new theme to the current display*/
    lv_disp_set_theme(NULL, &day_theme);
}

void apply_day_theme() {
    
}