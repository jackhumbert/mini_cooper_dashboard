#include "tach3.hpp"
#include "dash.h"
#include "effect.h"
#include <stdio.h>

// static lv_obj_t * meter;
// // lv_obj_t * meter2;
// static lv_anim_t * redline_anim;
// static lv_meter_indicator_t * rpm_normal;
// // lv_meter_indicator_t * rpm_normal2;
// static lv_meter_indicator_t * rpm_red;
// static bool animation_active = false;
// static lv_obj_t * rpm_label;
// // static lv_meter_indicator_t * tach_line;
// // static lv_meter_indicator_t * tach_line_bo;

// static void redline_anim_toggle(void * indic, int32_t v) {
//     lv_opa_t last = rpm_normal->opa;
//     if (v) {
//         // need to use a lv function to set/update
//         rpm_normal->opa = LV_OPA_100;
//         // rpm_normal2->type_data.arc.color = lv_color_change_lightness(lv_color_black(), 240);
//     } else {
//         rpm_normal->opa = LV_OPA_0;
//         // rpm_normal->type_data.arc.color = lv_color_black();
//         // rpm_normal2->type_data.arc.color = lv_color_black();
//     }

//     if (last != rpm_normal->opa) {
//         lv_obj_invalidate(meter);
//     }
//     // lv_event_send(meter, LV_EVENT_STYLE_CHANGED, NULL);
// }

// static void start_redline_anim(void) {
//     if (!redline_anim) {
//         static lv_anim_t redline_anim_t;
//         lv_anim_init(&redline_anim_t);
//         lv_anim_set_exec_cb(&redline_anim_t, redline_anim_toggle);
//         lv_anim_set_values(&redline_anim_t, 0, 1);
//         lv_anim_set_repeat_delay(&redline_anim_t, 20);
//         lv_anim_set_playback_delay(&redline_anim_t, 0);
//         lv_anim_set_repeat_count(&redline_anim_t, LV_ANIM_REPEAT_INFINITE);
//         lv_anim_set_time(&redline_anim_t, 20);
//         lv_anim_set_playback_time(&redline_anim_t, 20);
//         redline_anim = lv_anim_start(&redline_anim_t);
//     }
//     rpm_normal->type_data.arc.color = RED_ON;
//     animation_active = true;
// }

// static void stop_redline_anim(void) {
//     if (redline_anim) {
//         lv_anim_del(redline_anim, NULL);
//         redline_anim = NULL;
//     }
//     rpm_normal->type_data.arc.color = AMBER_ON;
//     rpm_normal->opa = LV_OPA_100;
//     lv_obj_invalidate(meter);
//     animation_active = false;
// }

// static void set_value(void * indic, int32_t v) {
//     // if (rpm_red) {
//     //     lv_meter_set_indicator_end_value(meter, rpm_red, MAX(v, REDLINE));
//     // }
//     tach3_update();
//     update_effect();
// }

void Tach::update(void) {
    if (get_queued()->rpm) {
        if (get_cache()->rpm > REDLINE) {
            // if (!animation_active) {
            //     start_redline_anim();
            // }

            indicator->type_data.arc.color = RED_ON;
        } else {
            // if (animation_active) {
            //     stop_redline_anim();
            // }
            indicator->type_data.arc.color = AMBER_ON;
        }
        // lv_coord_t angle = -480 + get_cache()->rpm / 8000 * 480;
        // lv_obj_set_style_transform_angle(rotated_meter, angle, 0);
        // lv_obj_invalidate(lv_obj);

       lv_meter_set_indicator_value(lv_obj, indicator, get_cache()->rpm);
        // lv_label_set_text_fmt(label, "%0.0f", get_cache()->rpm);
    }
}

#define INDIC_RADIUS (1850 / 2 + 15)
#define INDIC_RADIUS_CUTOFF (1850 / 2 - 5)
#define INDIC_WIDTH 20
#define INDIC_RESOLUTION 0.1

#define sin_d(x) sin((x) * 3.1415 / 180)

static void indicator_draw(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * lv_obj = ((Tach*)lv_event_get_user_data(e))->lv_obj;
	lv_obj_draw_part_dsc_t *dsc = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);
	
	// if (code == LV_EVENT_DRAW_PART_BEGIN) {
        // dsc->line_dsc->opa = LV_OPA_0;


        lv_area_t outer_coords = {
            .x1 = 400 - INDIC_RADIUS, 
            .y1 = 240 + 1850 / 2 - INDIC_RADIUS, 
            .x2 = 400 + INDIC_RADIUS, 
            .y2 = 240 + 1850 / 2 + INDIC_RADIUS
        };
        lv_draw_mask_radius_param_t outer_mask_param;
        lv_draw_mask_radius_init(&outer_mask_param, &outer_coords, INDIC_RADIUS, false);
        int16_t outer_mask_id = lv_draw_mask_add(&outer_mask_param, NULL);


        lv_area_t inner_coords = {
            .x1 = 400 - INDIC_RADIUS_CUTOFF, 
            .y1 = 240 + 1850 / 2 - INDIC_RADIUS_CUTOFF, 
            .x2 = 400 + INDIC_RADIUS_CUTOFF, 
            .y2 = 240 + 1850 / 2 + INDIC_RADIUS_CUTOFF
        };
        lv_draw_mask_radius_param_t inner_mask_param;
        lv_draw_mask_radius_init(&inner_mask_param, &inner_coords, INDIC_RADIUS_CUTOFF, true);
        int16_t inner_mask_id = lv_draw_mask_add(&inner_mask_param, NULL);


        static lv_draw_line_dsc_t line_dsc;
        line_dsc.opa = 255;
        line_dsc.color = AMBER_ON;
        line_dsc.width = 3;
        line_dsc.round_end = 0;
        line_dsc.round_start = 0;
        line_dsc.raw_end = 1;
        static lv_point_t p1, p2;

        float end = 66 + 180 + 48 * get_cache()->rpm / 8000;

        p1.x = 400 + (sin_d(end + 90) * (INDIC_RADIUS + 40));
        p1.y = 240 + 1850 / 2  + (sin_d(end) * (INDIC_RADIUS + 40));
        
        p2.x = 400 + (sin_d(end + 90) * (INDIC_RADIUS_CUTOFF - 40));
        p2.y = 240 + 1850 / 2 + (sin_d(end) * (INDIC_RADIUS_CUTOFF - 40));

        lv_draw_line(dsc->draw_ctx, &line_dsc, &p1, &p2);
        
        lv_draw_mask_free_param(&inner_mask_param);
        lv_draw_mask_remove_id(inner_mask_id);
        lv_draw_mask_free_param(&outer_mask_param);
        lv_draw_mask_remove_id(outer_mask_id);

}


Tach::Tach(lv_obj_t * parent) {
    lv_obj = lv_meter_create(parent);

    /*Remove the circle from the middle*/
    // lv_obj_remove_style(lv_obj, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(lv_obj, NULL, LV_PART_MAIN);
    // lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    // lv_obj_center(meter);
    lv_obj_set_size(lv_obj, 1850, 105);
    lv_obj_set_style_pad_all(lv_obj, 0, 0);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_MID, 0, 220);
    // lv_obj_set_style_radius(meter, 3200, 0);

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

    // extern const unsigned char RAJDHANI_REGULAR_TTF[];
    // extern const size_t RAJDHANI_REGULAR_TTF_SIZE;
    // font = tiny_ttf_create_data(RAJDHANI_REGULAR_TTF, RAJDHANI_REGULAR_TTF_SIZE, 24);
    DASH_FONT(RAJDHANI_REGULAR, 24);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(lv_obj);
    lv_meter_set_scale_ticks(lv_obj, scale, 81, 1, 24, lv_color_black());
    // lv_meter_set_scale_major_ticks(meter, scale, 10, 5, 12, lv_color_black(), 20);
    lv_meter_set_scale_range(lv_obj, scale, 0, 8000, 48, 66 + 180);


    lv_meter_scale_t * label_scale = lv_meter_add_scale(lv_obj);
    lv_meter_set_scale_ticks(lv_obj, label_scale, 9, 1, 24, lv_color_black());
    lv_meter_set_scale_major_ticks(lv_obj, label_scale, 1, 5, 24, lv_color_black(), 20);
    lv_meter_set_scale_range(lv_obj, label_scale, 0, 8, 48, 66 + 180);

    lv_obj_set_style_text_color(lv_obj, AMBER_ON, 0);
    lv_obj_set_style_text_font(lv_obj, RAJDHANI_REGULAR_24, 0);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(lv_obj, scale, 20, AMBER_OFF, -5);
    lv_meter_set_indicator_start_value(lv_obj, rpm_normal_bg, 0);
    lv_meter_set_indicator_end_value(lv_obj, rpm_normal_bg, REDLINE);

    lv_meter_indicator_t * rpm_red_bg = lv_meter_add_arc(lv_obj, scale, 20, RED_OFF, -5);
    lv_meter_set_indicator_start_value(lv_obj, rpm_red_bg, REDLINE);
    lv_meter_set_indicator_end_value(lv_obj, rpm_red_bg, 8000);

    // lv_meter_add_scale_lines(meter, scale, AMBER_ON, AMBER_ON, false, 5);

    // for (int i = 1; i < 8; i++) {
    //     lv_meter_indicator_t * line = lv_meter_add_needle_line(lv_obj, scale, 1, lv_color_black(), 0);
    //     lv_meter_set_indicator_value(lv_obj, line, i * 1000);
    // }

    // lv_meter_indicator_t * bottom = lv_meter_add_arc(lv_obj, scale, 1, AMBER_HALF, -1);
    // lv_meter_set_indicator_start_value(lv_obj, bottom, 0);
    // lv_meter_set_indicator_end_value(lv_obj, bottom, 0);

    // indicator = lv_meter_add_arc(lv_obj, scale, 20, AMBER_ON, -5);
    // lv_meter_set_indicator_start_value(lv_obj, indicator, 0);
    // lv_meter_set_indicator_end_value(lv_obj, indicator, 0);
    // lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_PART_END, NULL);


    // rotated meter

    {
        // rotated_meter = lv_meter_create(lv_obj);
        // lv_obj_remove_style(rotated_meter, NULL, LV_PART_INDICATOR);
        // lv_obj_remove_style(rotated_meter, NULL, LV_PART_MAIN);
        // // lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
        // // lv_obj_center(meter);
        // lv_obj_set_size(rotated_meter, 1850, 105);
        // lv_obj_set_style_pad_all(rotated_meter, 0, 0);
        // // lv_obj_align(rotated_meter, LV_ALIGN_TOP_MID, 0, 240);
        // lv_obj_align(rotated_meter, LV_ALIGN_CENTER, 0, 0);
        // lv_obj_set_style_transform_pivot_x(rotated_meter, LV_PCT(50), 0);
        // lv_obj_set_style_transform_pivot_y(rotated_meter, 1850 / 2, 0);
        // lv_obj_set_style_transform_angle(rotated_meter, -480, 0);
        // // lv_coord_t angle2 = -24 + 660 + 1800 + 240;
        // // lv_obj_set_style_translate_x(rotated_meter, (lv_trigo_sin(angle2 / 10 + 90) * INDIC_RADIUS) >> LV_TRIGO_SHIFT, 0);
        // // lv_obj_set_style_translate_y(rotated_meter, 1850 / 2 + (lv_trigo_sin(angle2 / 10) * INDIC_RADIUS) >> LV_TRIGO_SHIFT, 0);

        // lv_meter_scale_t * scale = lv_meter_add_scale(rotated_meter);
        // // lv_meter_set_scale_ticks(rotated_meter, scale, 81, 1, 24, lv_color_black());
        // // lv_meter_set_scale_major_ticks(meter, scale, 10, 5, 12, lv_color_black(), 20);
        // lv_meter_set_scale_range(rotated_meter, scale, 0, 8000, 48, 66 + 180);

        // indicator = lv_meter_add_arc(rotated_meter, scale, 20, AMBER_ON, -5);
        // lv_meter_set_indicator_start_value(rotated_meter, indicator, 0);
        // lv_meter_set_indicator_end_value(rotated_meter, indicator, 8000);


        indicator = lv_meter_add_needle_line(lv_obj, scale, 10, lv_color_black(), 0);
        lv_meter_set_indicator_value(lv_obj, indicator, 0);
        lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_PART_BEGIN, this);
        // lv_obj_set_style_bg_opa(lv_obj, LV_OPA_0, LV_PART_INDICATOR);
        // lv_obj_set_style_line_opa(lv_obj, LV_OPA_0, LV_PART_INDICATOR);
        indicator->opa = 0;
    }

    {
        lv_obj_t * tick_meter = lv_meter_create(lv_obj);
        lv_obj_remove_style(tick_meter, NULL, LV_PART_INDICATOR);
        lv_obj_remove_style(tick_meter, NULL, LV_PART_MAIN);
        // lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
        // lv_obj_center(meter);
        lv_obj_set_size(tick_meter, 1850, 105);
        lv_obj_set_style_pad_all(tick_meter, 0, 0);
        // lv_obj_align(tick_meter, LV_ALIGN_TOP_MID, 0, 240);
        lv_obj_align(tick_meter, LV_ALIGN_CENTER, 0, 0);
        // lv_coord_t angle2 = -24 + 660 + 1800 + 240;
        // lv_obj_set_style_translate_x(tick_meter, (lv_trigo_sin(angle2 / 10 + 90) * INDIC_RADIUS) >> LV_TRIGO_SHIFT, 0);
        // lv_obj_set_style_translate_y(tick_meter, 1850 / 2 + (lv_trigo_sin(angle2 / 10) * INDIC_RADIUS) >> LV_TRIGO_SHIFT, 0);

        lv_meter_scale_t * scale = lv_meter_add_scale(tick_meter);
        // lv_meter_set_scale_ticks(tick_meter, scale, 81, 1, 24, lv_color_black());
        // lv_meter_set_scale_major_ticks(meter, scale, 10, 5, 12, lv_color_black(), 20);
        lv_meter_set_scale_range(tick_meter, scale, 0, 8000, 48, 66 + 180);

        lv_meter_indicator_t * line = lv_meter_add_needle_line(tick_meter, scale, 3, lv_color_black(), 0);
        lv_meter_set_indicator_value(tick_meter, line, 0);

        // for (int i = 1; i < 8; i++) {
        //     line = lv_meter_add_needle_line(tick_meter, scale, 5, lv_color_black(), 0);
        //     lv_meter_set_indicator_value(tick_meter, line, i * 1000);
        // }

        // for (uint16_t i = 100; i < 8000; i+=100) {
        //     line = lv_meter_add_needle_line(tick_meter, scale, 1, lv_color_black(), 0);
        //     lv_meter_set_indicator_value(tick_meter, line, i);
        // }
        
        lv_meter_scale_t * scale2 = lv_meter_add_scale(tick_meter);
        // lv_meter_set_scale_ticks(tick_meter, scale, 81, 1, 24, lv_color_black());
        // lv_meter_set_scale_major_ticks(meter, scale, 10, 5, 12, lv_color_black(), 20);
        lv_meter_set_scale_range(tick_meter, scale2, 0, 1, 6, 60 + 180);

        lv_meter_indicator_t * blackout = lv_meter_add_arc(tick_meter, scale2, 22, lv_color_black(), -4);
        lv_meter_set_indicator_start_value(tick_meter, blackout, 0);
        lv_meter_set_indicator_end_value(tick_meter, blackout, 1);
    }

    // tach_line = lv_meter_add_needle_line(lv_obj, scale, 5, AMBER_ON, 0);
    // lv_meter_set_indicator_value(lv_obj, tach_line, 0);

    // label = lv_label_create(lv_obj);
    // lv_label_set_text(label, "-");
    // lv_obj_align(label, LV_ALIGN_TOP_LEFT, 1850 / 2 - 400 + 10, 50);

    // DASH_FONT(RAJDHANI_SEMIBOLD, 20);

    // static lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_text_opa(&style, LV_OPA_100);
    // lv_style_set_text_color(&style, AMBER_ON);
    // lv_style_set_text_font(&style, RAJDHANI_SEMIBOLD_20);

    // lv_obj_add_style(label, &style, 0);


    // effect

    // meter2 = lv_meter_create(parent);
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
    // lv_anim_t a;
    // lv_anim_init(&a);
    // lv_anim_set_exec_cb(&a, set_value);
    // lv_anim_set_values(&a, 800, 8000);
    // lv_anim_set_repeat_delay(&a, 100);
    // lv_anim_set_playback_delay(&a, 100);
    // lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    // lv_anim_set_time(&a, 3000);
    // lv_anim_set_playback_time(&a, 500);
    // lv_anim_start(&a);

    /*Add a red arc to the end*/
    // rpm_red = lv_meter_add_arc(meter, scale, 20, RED_ON, -1);
    // lv_meter_set_indicator_start_value(meter, rpm_red, REDLINE);
    // lv_meter_set_indicator_end_value(meter, rpm_red, 8000);

}
