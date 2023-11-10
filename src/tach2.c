#include "tach2.h"

#define TACH_WIDTH (16*40)
#define TACH_HEIGHT 20
#define TACH_SEGMENTS 8

static lv_font_t * font;

lv_obj_t * tach2(lv_obj_t * canvas) {
    static lv_style_t style_bg;
    static lv_style_t style_indic;

    lv_obj_t * tach_cont = lv_obj_create(canvas);
    lv_obj_remove_style_all(tach_cont);
    lv_obj_set_size(tach_cont, TACH_WIDTH + 8, TACH_HEIGHT + 40);
    lv_obj_set_scrollbar_mode(tach_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(tach_cont, LV_DIR_NONE);

    // static lv_ft_info_t info;
    // /*FreeType uses C standard file system, so no driver letter is required.*/
    // // info.name = "./fonts/Rajdhani-Regular.ttf";
    // info.name = "./fonts/Bould-Regular.otf";
    // info.weight = 16;
    // info.style = FT_FONT_STYLE_NORMAL;
    // info.mem = NULL;
    // if(!lv_ft_font_init(&info)) {
    //     LV_LOG_ERROR("create failed.");
    // }

    extern const unsigned char RAJDHANI_REGULAR_TTF[];
    extern const size_t RAJDHANI_REGULAR_TTF_SIZE;
    font = tiny_ttf_create_data(RAJDHANI_REGULAR_TTF, RAJDHANI_REGULAR_TTF_SIZE, 16);

    {
        lv_obj_t * label = lv_label_create(tach_cont);
        lv_obj_align(label, LV_ALIGN_BOTTOM_LEFT, 0, 0);
        lv_obj_set_style_text_font(label, font, 0);
        lv_label_set_text(label, "0");
        lv_obj_set_style_text_color(label, AMBER_ON, 0);
    }
    {
        lv_obj_t * label = lv_label_create(tach_cont);
        lv_obj_align(label, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
        lv_obj_set_style_text_font(label, font, 0);
        lv_label_set_text(label, "8");
        lv_obj_set_style_text_color(label, AMBER_ON, 0);
    }

    lv_style_init(&style_bg);
    // lv_style_set_border_color(&style_bg, AMBER_OFF);
    // lv_style_set_bg_color(&style_bg, AMBER_OFF);
    // lv_style_set_bg_opa(&style_bg, LV_OPA_100);
    // lv_style_set_border_width(&style_bg, 2);
    lv_style_set_radius(&style_bg, 1);
    lv_style_set_anim_time(&style_bg, 1000);

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, AMBER_HALF);
    lv_style_set_blend_mode(&style_indic, LV_BLEND_MODE_ADDITIVE);
    // lv_style_set_pad_all(&style_indic, 6); /*To make the indicator smaller*/
    // lv_style_set_radius(&style_indic, 0);

    lv_obj_t * bar = lv_bar_create(tach_cont);
    lv_obj_remove_style_all(bar);  /*To have a clean start*/
    lv_bar_set_range(bar, 0, 80);
    lv_obj_align(bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(bar, &style_bg, 0);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    // lv_obj_set_style_blend_mode(bar, LV_BLEND_MODE_ADDITIVE, 0);

    lv_obj_set_size(bar, TACH_WIDTH, TACH_HEIGHT);
    // lv_obj_set_style_pad_all(bar, 0, 0);
    lv_obj_center(bar);
    lv_bar_set_value(bar, 55, LV_ANIM_ON);
    // lv_obj_set_style_blend_mode(bar, LV_BLEND_MODE_ADDITIVE, 0);


    for (int i = 0; i < TACH_SEGMENTS; i++) {
        lv_obj_t * line = lv_obj_create(bar);
        lv_obj_remove_style_all(line);
        lv_obj_set_size(line, (TACH_WIDTH / TACH_SEGMENTS) - 2, TACH_HEIGHT);
        lv_obj_set_style_pad_all(line, 0, 0);
        lv_obj_set_scrollbar_mode(line, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_scroll_dir(line, LV_DIR_NONE);
        lv_obj_align(line, LV_ALIGN_LEFT_MID, (TACH_WIDTH / TACH_SEGMENTS) * i + 1, 0);
        // lv_obj_set_style_border_color(line, AMBER_ON, 0);
        // lv_obj_set_style_border_width(line, 1, 0);
        // lv_obj_set_style_border_opa(line, LV_OPA_100, 0);
        // lv_obj_set_style_border_post(line, true, 0);
        lv_obj_set_style_outline_color(line, lv_color_black(), 0);
        lv_obj_set_style_outline_opa(line, LV_OPA_100, 0);
        lv_obj_set_style_outline_width(line, 2, 0);
        lv_obj_set_style_outline_pad(line, 1, 0);
        lv_obj_set_style_radius(line, 4, 0);
    }
    for (int i = 0; i < TACH_SEGMENTS; i++) {
        lv_obj_t * line = lv_obj_create(bar);
        lv_obj_remove_style_all(line);
        lv_obj_set_size(line, (TACH_WIDTH / TACH_SEGMENTS) - 2, TACH_HEIGHT);
        lv_obj_set_style_pad_all(line, 0, 0);
        lv_obj_set_scrollbar_mode(line, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_scroll_dir(line, LV_DIR_NONE);
        lv_obj_align(line, LV_ALIGN_LEFT_MID, (TACH_WIDTH / TACH_SEGMENTS) * i + 1, 0);
        lv_obj_set_style_border_color(line, AMBER_ON, 0);
        // lv_obj_set_style_blend_mode(line, LV_BLEND_MODE_ADDITIVE, 0);
        lv_obj_set_style_border_width(line, 1, 0);
        lv_obj_set_style_border_opa(line, LV_OPA_50, 0);
        lv_obj_set_style_border_post(line, true, 0);
        // lv_obj_set_style_outline_color(line, lv_color_black(), 0);
        // lv_obj_set_style_outline_opa(line, LV_OPA_100, 0);
        // lv_obj_set_style_outline_width(line, 2, 0);
        lv_obj_set_style_radius(line, 4, 0);
    }
    return tach_cont;
}