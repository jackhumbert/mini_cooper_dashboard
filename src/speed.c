#include "speed.h"

lv_obj_t * speed_label;
lv_obj_t * speed_label2;

static void set_speed_value(void * indic, int32_t v) {
    if (v >= 10) {
        lv_label_set_text_fmt(speed_label, "%d", (int)(v / 10));
        lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    } else {
        lv_label_set_text(speed_label, "");
        lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, 0, 1);
    }
    lv_label_set_text_fmt(speed_label2, "%d", v % 10);
    // update_effect();
}


lv_obj_t * speed_create(lv_obj_t * parent) {

    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    // info.name = "./fonts/Rajdhani-Bold.ttf";
    // info.name = "./fonts/DSEG14ClassicMini-Regular.ttf";
    info.name = "./fonts/Bould-Medium.otf";
    info.weight = 96;
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

    //     lv_obj_t * speed_label = lv_label_create(cont);
    //     lv_label_set_text(speed_label, "188");
    //     lv_obj_add_style(speed_label, &style, 0);
    //     lv_obj_set_style_blend_mode(speed_label, LV_BLEND_MODE_ADDITIVE, 0);
    //     lv_obj_set_grid_cell(speed_label, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 3, 3);

    //     lv_obj_align(speed_label, LV_ALIGN_CENTER, 0, 0);
    // }


    static lv_coord_t col_dsc[] = {1, 1, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {48, LV_GRID_TEMPLATE_LAST};

    lv_obj_t * speed_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(speed_cont);
    lv_obj_set_layout(speed_cont, LV_LAYOUT_GRID);
    lv_obj_set_grid_align(speed_cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_style_grid_column_dsc_array(speed_cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(speed_cont, row_dsc, 0);
    lv_obj_set_size(speed_cont, 800, 480);
    lv_obj_center(speed_cont);
    lv_obj_align(speed_cont, LV_ALIGN_CENTER, 0, 0);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_ON);
    lv_style_set_text_font(&style, info.font);

        speed_label = lv_label_create(speed_cont);
        lv_label_set_text_fmt(speed_label, "%d", (int)(50 / 10));
        lv_obj_add_style(speed_label, &style, 0);
        // lv_obj_set_style_blend_mode(speed_label, LV_BLEND_MODE_ADDITIVE, 0);
        lv_obj_set_grid_cell(speed_label, LV_GRID_ALIGN_END, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        lv_obj_align(speed_label, LV_ALIGN_RIGHT_MID, 0, 0);

        speed_label2 = lv_label_create(speed_cont);
        lv_label_set_text_fmt(speed_label2, "%d", 50 % 10);
        lv_obj_add_style(speed_label2, &style, 0);
        // lv_obj_set_style_blend_mode(speed_label2, LV_BLEND_MODE_ADDITIVE, 0);
        lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        lv_obj_align(speed_label2, LV_ALIGN_LEFT_MID, 0, 0);

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_exec_cb(&a, set_speed_value);
        lv_anim_set_values(&a, 0, 80);
        lv_anim_set_repeat_delay(&a, 100);
        lv_anim_set_playback_delay(&a, 100);
        lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

        lv_anim_set_time(&a, 5000);
        lv_anim_set_playback_time(&a, 500);
        lv_anim_start(&a);

        return speed_cont;
}