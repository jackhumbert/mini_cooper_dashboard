#include "tach.h"
#include "dash.h"
#include "effect.h"
#include <stdio.h>

static lv_obj_t * meter;
// lv_obj_t * meter2;
static lv_anim_t * redline_anim;
static lv_meter_indicator_t * rpm_normal;
// lv_meter_indicator_t * rpm_normal2;
static lv_meter_indicator_t * rpm_red;
bool animation_active = false;
lv_font_t * font;

static void redline_anim_toggle(void * indic, int32_t v) {
    if (v) {
        // need to use a lv function to set/update
        rpm_normal->opa = LV_OPA_100;
        // rpm_normal2->type_data.arc.color = lv_color_change_lightness(lv_color_black(), 240);
    } else {
        rpm_normal->opa = LV_OPA_0;
        // rpm_normal->type_data.arc.color = lv_color_black();
        // rpm_normal2->type_data.arc.color = lv_color_black();
    }

    lv_obj_invalidate(meter);
    // lv_event_send(meter, LV_EVENT_STYLE_CHANGED, NULL);
}

static void start_redline_anim(void) {
    if (!redline_anim) {
        static lv_anim_t redline_anim_t;
        lv_anim_init(&redline_anim_t);
        lv_anim_set_exec_cb(&redline_anim_t, redline_anim_toggle);
        lv_anim_set_values(&redline_anim_t, 0, 1);
        lv_anim_set_repeat_delay(&redline_anim_t, 20);
        lv_anim_set_playback_delay(&redline_anim_t, 0);
        lv_anim_set_repeat_count(&redline_anim_t, LV_ANIM_REPEAT_INFINITE);
        lv_anim_set_time(&redline_anim_t, 20);
        lv_anim_set_playback_time(&redline_anim_t, 20);
        redline_anim = lv_anim_start(&redline_anim_t);
    }
    rpm_normal->type_data.arc.color = RED_ON;
    animation_active = true;
}

static void stop_redline_anim(void) {
    if (redline_anim) {
        lv_anim_del(redline_anim, NULL);
        redline_anim = NULL;
    }
    rpm_normal->type_data.arc.color = AMBER_ON;
    rpm_normal->opa = LV_OPA_100;
    animation_active = false;
}

static void set_value(void * indic, int32_t v)
{
    if (v > REDLINE) {
        if (!animation_active) {
            start_redline_anim();
        }
    } else {
        stop_redline_anim();
        // rpm_normal2->type_data.arc.color = lv_color_change_lightness(lv_color_black(), 240);
    }
    if (rpm_normal) {
        lv_meter_set_indicator_end_value(meter, rpm_normal, MIN(v, REDLINE));
        // lv_meter_set_indicator_end_value(meter2, rpm_normal2, MIN(v, REDLINE));
    }
    if (rpm_red) {
        lv_meter_set_indicator_end_value(meter, rpm_red, MAX(v, REDLINE));
    }
    // need_to_affect = true;
    update_effect();
}

void tach(lv_obj_t * canvas) {
    meter = lv_meter_create(canvas);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    // lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_center(meter);
    lv_obj_set_size(meter, 400, 400);

    // static lv_ft_info_t info;
    // /*FreeType uses C standard file system, so no driver letter is required.*/
    // // info.name = "./fonts/Microgramma-D-Extended-Medium.otf";
    // // info.name = "./fonts/Rajdhani-Bold.ttf";
    // info.name = "./fonts/Bould-Regular.otf";
    // info.weight = 24;
    // info.style = FT_FONT_STYLE_NORMAL;
    // info.mem = NULL;
    // if(!lv_ft_font_init(&info)) {
    //     LV_LOG_ERROR("create failed.");
    // }



    font = tiny_ttf_create_file("A:fonts/Rajdhani-Regular.ttf", 24);



    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 81, 1, 24, lv_color_black());
    // lv_meter_set_scale_major_ticks(meter, scale, 10, 5, 12, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 0, 8000, 270, 135);


    lv_meter_scale_t * label_scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, label_scale, 9, 1, 24, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, label_scale, 1, 5, 24, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, label_scale, 0, 8, 270, 135);

    lv_obj_set_style_text_color(meter, AMBER_ON, 0);
    lv_obj_set_style_text_font(meter, font, 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 20, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, REDLINE);

    rpm_normal = lv_meter_add_arc(meter, scale, 20, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal, REDLINE);


    // effect

    // meter2 = lv_meter_create(canvas);
    // /*Remove the circle from the middle*/
    // lv_obj_remove_style(meter2, NULL, LV_PART_INDICATOR);
    // lv_obj_remove_style(meter2, NULL, LV_PART_MAIN);
    // lv_obj_set_style_blend_mode(meter2, LV_BLEND_MODE_MULTIPLY, 0);
    // lv_obj_center(meter2);
    // lv_obj_set_size(meter2, 400, 400);
    // lv_obj_set_style_text_color(meter2, lv_color_white(), 0);


    // lv_meter_scale_t * scale2 = lv_meter_add_scale(meter2);
    // lv_meter_set_scale_ticks(meter2, scale2, 81, 5, 24, lv_color_white());
    // lv_meter_set_scale_major_ticks(meter2, scale2, 10, 9, 24, lv_color_white(), 20);
    // lv_meter_set_scale_range(meter2, scale2, 0, 8000, 270, 135);

    // rpm_normal2 = lv_meter_add_arc(meter2, scale2, 16, lv_color_change_lightness(lv_color_black(), 220), -3);
    // lv_meter_set_indicator_start_value(meter2, rpm_normal2, 0);
    // lv_meter_set_indicator_end_value(meter2, rpm_normal2, REDLINE);


    /*Create an animation to set the value*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value);
    lv_anim_set_values(&a, 800, 8000);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    lv_anim_set_time(&a, 3000);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_start(&a);

    lv_meter_indicator_t * rpm_red_bg = lv_meter_add_arc(meter, scale, 20, lv_color_change_lightness(lv_palette_main(LV_PALETTE_RED), 15), -1);
    lv_meter_set_indicator_start_value(meter, rpm_red_bg, REDLINE);
    lv_meter_set_indicator_end_value(meter, rpm_red_bg, 8000);

    /*Add a red arc to the end*/
    rpm_red = lv_meter_add_arc(meter, scale, 20, lv_palette_main(LV_PALETTE_RED), -1);
    lv_meter_set_indicator_start_value(meter, rpm_red, REDLINE);
    lv_meter_set_indicator_end_value(meter, rpm_red, 8000);

}
