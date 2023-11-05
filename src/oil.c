#include "oil.h"


lv_obj_t * oil_temp_meter;
lv_meter_indicator_t * oil_temp_indic;

lv_obj_t * oil_temp_label;

static void set_oil_temp_value(void * indic, int32_t v) {
    lv_label_set_text_fmt(oil_temp_label, "%d°F", v);
    lv_meter_set_indicator_start_value(oil_temp_meter, oil_temp_indic, v);
    lv_color_t old_color = oil_temp_indic->type_data.arc.color;
    if (v > 260) {
        oil_temp_indic->type_data.arc.color = RED_ON;
        lv_obj_set_style_text_color(oil_temp_label, RED_ON, 0);
    } else {
        oil_temp_indic->type_data.arc.color = AMBER_ON;
        lv_obj_set_style_text_color(oil_temp_label, AMBER_ON, 0);
    }
    if (old_color.full != oil_temp_indic->type_data.arc.color.full) {
        lv_obj_invalidate(oil_temp_meter);
    }
}

lv_obj_t * oil_pressure_meter;
lv_meter_indicator_t * oil_pressure_indic;

lv_obj_t * oil_pressure_label;

static void set_oil_pressure_value(void * indic, int32_t v) {
    lv_label_set_text_fmt(oil_pressure_label, "%d psi", v);
    lv_meter_set_indicator_end_value(oil_pressure_meter, oil_pressure_indic, v);
    lv_color_t old_color = oil_pressure_indic->type_data.arc.color;
    if (v > 65 || v < 2) {
        oil_pressure_indic->type_data.arc.color = RED_ON;
        lv_obj_set_style_text_color(oil_pressure_label, RED_ON, 0);
    } else {
        oil_pressure_indic->type_data.arc.color = AMBER_ON;
        lv_obj_set_style_text_color(oil_pressure_label, AMBER_ON, 0);
    }
    if (old_color.full != oil_pressure_indic->type_data.arc.color.full) {
        lv_obj_invalidate(oil_pressure_meter);
    }
}


lv_obj_t * oil_create(lv_obj_t * parent) {

    lv_obj_t * oil_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(oil_cont);
    // lv_obj_set_style_blend_mode(oil_cont, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_scrollbar_mode(oil_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(oil_cont, LV_DIR_NONE);
    lv_obj_set_size(oil_cont, 300, 200);
    // lv_obj_align(oil_cont, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_blend_mode(oil_cont, LV_BLEND_MODE_ADDITIVE, 0);
    // lv_obj_center(oil_cont);

{
    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    // info.name = "./fonts/Rajdhani-Regular.ttf";
    info.name = "./fonts/Bould-Regular.otf";
    info.weight = 16;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    lv_obj_t * engine_temp_label = lv_label_create(oil_cont);
    lv_obj_align(engine_temp_label, LV_ALIGN_CENTER, 0, 5);
    lv_obj_set_style_text_font(engine_temp_label, info.font, 0);
    lv_label_set_text(engine_temp_label, "OIL");
    lv_obj_set_style_text_color(engine_temp_label, AMBER_ON, 0);
}


// oil temp

{

    oil_temp_meter = lv_meter_create(oil_cont);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(oil_temp_meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(oil_temp_meter, NULL, LV_PART_MAIN);
    lv_obj_set_style_blend_mode(oil_temp_meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_size(oil_temp_meter, 100, 100);
    lv_obj_align(oil_temp_meter, LV_ALIGN_CENTER, 45, 50);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(oil_temp_meter);
    lv_meter_set_scale_ticks(oil_temp_meter, scale, 11, 1, 50, lv_color_black());
    lv_meter_set_scale_major_ticks(oil_temp_meter, scale, 5, 5, 50, lv_color_black(), 20);
    lv_meter_set_scale_range(oil_temp_meter, scale, 320, 100, 180 + 90, 270);

    lv_obj_set_style_text_color(oil_temp_meter, lv_color_black(), 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(oil_temp_meter, scale, 10, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(oil_temp_meter, rpm_normal_bg, 320);
    lv_meter_set_indicator_end_value(oil_temp_meter, rpm_normal_bg, 100);

    oil_temp_indic = lv_meter_add_arc(oil_temp_meter, scale, 10, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(oil_temp_meter, oil_temp_indic, 210);
    lv_meter_set_indicator_end_value(oil_temp_meter, oil_temp_indic, 100);

    {
        static lv_ft_info_t info;
        /*FreeType uses C standard file system, so no driver letter is required.*/
        // info.name = "./fonts/Rajdhani-Regular.ttf";
        info.name = "./fonts/Bould-Regular.otf";
        info.weight = 20;
        info.style = FT_FONT_STYLE_NORMAL;
        info.mem = NULL;
        if(!lv_ft_font_init(&info)) {
            LV_LOG_ERROR("create failed.");
        }

        oil_temp_label = lv_label_create(oil_temp_meter);
        // lv_obj_align(oil_temp_label, LV_ALIGN_RIGHT_MID, 0, 133);
        lv_obj_set_style_text_font(oil_temp_label, info.font, 0);
        lv_obj_center(oil_temp_label);
        lv_label_set_text(oil_temp_label, "210°F");
        lv_obj_set_style_text_color(oil_temp_label, AMBER_ON, 0);
    }

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_exec_cb(&a, set_oil_temp_value);
        lv_anim_set_values(&a, 100, 320);
        lv_anim_set_repeat_delay(&a, 100);
        lv_anim_set_playback_delay(&a, 100);
        lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

        lv_anim_set_time(&a, 5000);
        lv_anim_set_playback_time(&a, 500);
        lv_anim_start(&a);

}

// oil pressure

{

    oil_pressure_meter = lv_meter_create(oil_cont);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(oil_pressure_meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(oil_pressure_meter, NULL, LV_PART_MAIN);
    lv_obj_set_style_blend_mode(oil_pressure_meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_size(oil_pressure_meter, 100, 100);
    lv_obj_align(oil_pressure_meter, LV_ALIGN_CENTER, -45, 50);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(oil_pressure_meter);
    lv_meter_set_scale_ticks(oil_pressure_meter, scale, 11, 1, 50, lv_color_black());
    lv_meter_set_scale_major_ticks(oil_pressure_meter, scale, 5, 5, 50, lv_color_black(), 20);
    lv_meter_set_scale_range(oil_pressure_meter, scale, 0, 70, 180 + 45, 90 - 45);

    lv_obj_set_style_text_color(oil_pressure_meter, lv_color_black(), 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(oil_pressure_meter, scale, 10, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(oil_pressure_meter, rpm_normal_bg, 0);
    lv_meter_set_indicator_end_value(oil_pressure_meter, rpm_normal_bg, 70);

    oil_pressure_indic = lv_meter_add_arc(oil_pressure_meter, scale, 10, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(oil_pressure_meter, oil_pressure_indic, 0);
    lv_meter_set_indicator_end_value(oil_pressure_meter, oil_pressure_indic, 35);

    {
        static lv_ft_info_t info;
        /*FreeType uses C standard file system, so no driver letter is required.*/
        // info.name = "./fonts/Rajdhani-Regular.ttf";
        info.name = "./fonts/Bould-Regular.otf";
        info.weight = 20;
        info.style = FT_FONT_STYLE_NORMAL;
        info.mem = NULL;
        if(!lv_ft_font_init(&info)) {
            LV_LOG_ERROR("create failed.");
        }

        oil_pressure_label = lv_label_create(oil_pressure_meter);
        lv_obj_set_style_text_font(oil_pressure_label, info.font, 0);
        lv_obj_center(oil_pressure_label);
        lv_label_set_text(oil_pressure_label, "35 psi");
        lv_obj_set_style_text_color(oil_pressure_label, AMBER_ON, 0);
    }


        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_exec_cb(&a, set_oil_pressure_value);
        lv_anim_set_values(&a, 0, 70);
        lv_anim_set_repeat_delay(&a, 100);
        lv_anim_set_playback_delay(&a, 100);
        lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

        lv_anim_set_time(&a, 5000);
        lv_anim_set_playback_time(&a, 500);
        lv_anim_start(&a);

}

    return oil_cont;

}