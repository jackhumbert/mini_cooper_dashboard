#include "coolant.h"

lv_font_t * font_big;
lv_font_t * font_small;

lv_obj_t * coolant_create(lv_obj_t * parent) {

    lv_obj_t * coolant_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(coolant_cont);
    // lv_obj_set_style_blend_mode(coolant_cont, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_scrollbar_mode(coolant_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(coolant_cont, LV_DIR_NONE);
    lv_obj_set_size(coolant_cont, 100, 100);
    // lv_obj_align(coolant_cont, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_blend_mode(coolant_cont, LV_BLEND_MODE_ADDITIVE, 0);
    // lv_obj_center(coolant_cont);

{
    // static lv_ft_info_t info;
    // /*FreeType uses C standard file system, so no driver letter is required.*/
    // // info.name = "./fonts/Rajdhani-Regular.ttf";
    // info.name = "./fonts/Bould-Regular.otf";
    // info.weight = 14;
    // info.style = FT_FONT_STYLE_NORMAL;
    // info.mem = NULL;
    // if(!lv_ft_font_init(&info)) {
    //     LV_LOG_ERROR("create failed.");
    // }

    font_small = tiny_ttf_create_file("A:fonts/Rajdhani-Regular.ttf", 14);

    lv_obj_t * engine_temp_label = lv_label_create(coolant_cont);
    lv_obj_align(engine_temp_label, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_text_font(engine_temp_label, font_small, 0);
    lv_label_set_text(engine_temp_label, "COOLANT");
    lv_obj_set_style_text_color(engine_temp_label, AMBER_ON, 0);
}


    lv_obj_t * meter = lv_meter_create(coolant_cont);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    // lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_align(meter, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_size(meter, 100, 100);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 11, 1, 50, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, scale, 5, 5, 50, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 30, 250, 270, 135);

    lv_obj_set_style_text_color(meter, lv_color_black(), 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 10, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 30);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, 250);

    lv_meter_indicator_t * indic1 = lv_meter_add_arc(meter, scale, 10, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, indic1, 30);
    lv_meter_set_indicator_end_value(meter, indic1, 140);


    // static lv_ft_info_t info;
    // /*FreeType uses C standard file system, so no driver letter is required.*/
    // // info.name = "./fonts/Rajdhani-Regular.ttf";
    // info.name = "./fonts/Bould-Regular.otf";
    // info.weight = 20;
    // info.style = FT_FONT_STYLE_NORMAL;
    // info.mem = NULL;
    // if(!lv_ft_font_init(&info)) {
    //     LV_LOG_ERROR("create failed.");
    // }



    font_big = tiny_ttf_create_file("A:fonts/Rajdhani-Regular.ttf", 20);

    lv_obj_t * engine_temp_label = lv_label_create(meter);
    // lv_obj_align(engine_temp_label, LV_ALIGN_RIGHT_MID, 0, 133);
    lv_obj_set_style_text_font(engine_temp_label, font_big, 0);
    lv_obj_center(engine_temp_label);
    lv_label_set_text_fmt(engine_temp_label, "%d°F", 140);
    lv_obj_set_style_text_color(engine_temp_label, AMBER_ON, 0);

    return coolant_cont;

}