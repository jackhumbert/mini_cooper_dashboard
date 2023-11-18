#include "tach4.hpp"

#define INDIC_RADIUS (1850 / 2)
#define INDIC_RADIUS_OUTER (INDIC_RADIUS + 10)
#define INDIC_RADIUS_INNER (INDIC_RADIUS - 10)
#define INDIC_X 400
#define INDIC_Y 20
#define INDIC_ABS_Y 230
#define INDIC_OVERSHOOT 40

#define sin_d(x) sin((x) * 3.1415 / 180)

static lv_point_t line_points[10];

static float x_comp = -0.5;
static float y_comp = -0.5;

void Tach4::update(void) {
    // if (get_queued()->rpm) {
    //     if (get_cache()->rpm > REDLINE) {
    //         // if (!animation_active) {
    //         //     start_redline_anim();
    //         // }

    //         indicator->type_data.arc.color = RED_ON;
    //     } else {
    //         // if (animation_active) {
    //         //     stop_redline_anim();
    //         // }
    //         indicator->type_data.arc.color = AMBER_ON;
    //     }
    if (get_queued()->rpm) {

        float end = 66 + 180 + 48 * get_cache()->rpm / 8000.0;
        x_comp = sin_d(end + 90);
        y_comp = sin_d(end);
        line_points[1].x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER));
        line_points[1].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER));

        lv_line_set_points(lv_obj, line_points, 2);
    }
}

static lv_point_t arc_center = {
    .x = INDIC_X, 
    .y = INDIC_ABS_Y + INDIC_RADIUS_INNER - 15,
};

static lv_draw_mask_line_param_t line_mask_param;
static int16_t line_mask_id;

static void indicator_draw(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_draw_ctx_t *ctx = (lv_draw_ctx_t *)lv_event_get_param(e);
	
	if (code == LV_EVENT_DRAW_MAIN_BEGIN) {

        float end = 66 + 180 + 48 * get_cache()->rpm / 8000.0;

        static lv_draw_arc_dsc_t arc_dsc;
        lv_draw_arc_dsc_init(&arc_dsc);
        arc_dsc.color = AMBER_OFF;
        arc_dsc.start_angle = end;
        arc_dsc.end_angle = 66 + 180 + 48;
        arc_dsc.width = 20;

        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER, end, 66 + 180 + 48);

        lv_point_t arc_end = {
            .x = arc_center.x + (x_comp * (INDIC_RADIUS + INDIC_OVERSHOOT)),
            .y = arc_center.y + (y_comp * (INDIC_RADIUS + INDIC_OVERSHOOT))
        };

        lv_draw_mask_line_points_init(&line_mask_param, arc_center.x, arc_center.y, arc_end.x, arc_end.y, LV_DRAW_MASK_LINE_SIDE_LEFT);
        line_mask_id = lv_draw_mask_add(&line_mask_param, NULL);


        static lv_draw_arc_dsc_t arc_dsc2;
        lv_draw_arc_dsc_init(&arc_dsc2);
        arc_dsc2.color = AMBER_ON;
        arc_dsc2.start_angle = 66 + 180;
        arc_dsc2.end_angle = end + 1;
        arc_dsc2.width = 20;

        lv_draw_arc(ctx, &arc_dsc2, &arc_center, INDIC_RADIUS_INNER, 66 + 180, end + 1);
    }

	if (code == LV_EVENT_DRAW_MAIN_END) {
        lv_draw_mask_free_param(&line_mask_param);
        lv_draw_mask_remove_id(line_mask_id);
    }

}

Tach4::Tach4(lv_obj_t * parent) {

    float end = 66 + 180;
    float x_comp = sin_d(end + 90);
    float y_comp = sin_d(end);

    line_points[0].x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER));
    line_points[0].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER));

    line_points[1].x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER));
    line_points[1].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER));

    // line_points[0].x = INDIC_X + (x_comp * (INDIC_RADIUS_OUTER + INDIC_OVERSHOOT));
    // line_points[0].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_OUTER + INDIC_OVERSHOOT));
    
    // line_points[1].x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER - INDIC_OVERSHOOT));
    // line_points[1].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER - INDIC_OVERSHOOT));

    // line_points[2].x = line_points[1].x;
    // line_points[2].y = line_points[1].x;
    
    // line_points[3].x = line_points[0].x;
    // line_points[3].y = line_points[0].y;

    // line_points[4].x = line_points[0].x;
    // line_points[4].y = line_points[0].x;

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 0);
    // lv_style_set_bg_color(&style_line, AMBER_ON);
    lv_style_set_line_color(&style_line, AMBER_ON);
    lv_style_set_line_rounded(&style_line, false);

    lv_obj = lv_line_create(parent);
    lv_obj_set_size(lv_obj, 800, 150);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_MID, 0, INDIC_ABS_Y);
    lv_line_set_points(lv_obj, line_points, 2);
    lv_obj_add_style(lv_obj, &style_line, 0);
    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_MAIN_BEGIN, this);
    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_MAIN_END, this);
    lv_obj_center(lv_obj);

    update();
}