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

        int n = round(get_cache()->rpm / 8000.0 * 8) + 1;
        // line_points[3].x = INDIC_X + (x_comp * (INDIC_RADIUS_OUTER + INDIC_OVERSHOOT));
        // line_points[3].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_OUTER + INDIC_OVERSHOOT));

        // line_points[2].x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER - INDIC_OVERSHOOT));
        // line_points[2].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER - INDIC_OVERSHOOT));
        for (int i = 1; i <= n; i++) {
            float end = 66 + 180 + 48 * get_cache()->rpm / 8000.0 / n * i;
            float x_comp = sin_d(end + 90);
            float y_comp = sin_d(end);
            line_points[i].x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER));
            line_points[i].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER));
        }

        // line_points[4].y = line_points[2].y;
        lv_line_set_points(lv_obj, line_points, n + 1);
    }
}

static lv_area_t outer_coords = {
    .x1 = INDIC_X - INDIC_RADIUS_OUTER, 
    .y1 = INDIC_ABS_Y + INDIC_RADIUS - INDIC_RADIUS_OUTER, 
    .x2 = INDIC_X + INDIC_RADIUS, 
    .y2 = INDIC_ABS_Y + INDIC_RADIUS + INDIC_RADIUS_OUTER
};

static lv_draw_mask_radius_param_t outer_mask_param;
static int16_t outer_mask_id;

static lv_area_t inner_coords = {
    .x1 = INDIC_X - INDIC_RADIUS_INNER, 
    .y1 = INDIC_ABS_Y + INDIC_RADIUS - INDIC_RADIUS_INNER, 
    .x2 = INDIC_X + INDIC_RADIUS_INNER, 
    .y2 = INDIC_ABS_Y + INDIC_RADIUS + INDIC_RADIUS_INNER
};

static lv_draw_mask_radius_param_t inner_mask_param;
static int16_t inner_mask_id;

static void indicator_draw(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_draw_ctx_t *ctx = (lv_draw_ctx_t *)lv_event_get_param(e);
	
	if (code == LV_EVENT_DRAW_MAIN_BEGIN) {
        outer_mask_id = lv_draw_mask_add(&outer_mask_param, NULL);
        lv_draw_mask_radius_init(&outer_mask_param, &outer_coords, INDIC_RADIUS_OUTER, false);
        inner_mask_id = lv_draw_mask_add(&inner_mask_param, NULL);
        lv_draw_mask_radius_init(&inner_mask_param, &inner_coords, INDIC_RADIUS_INNER, true);

        static lv_draw_arc_dsc_t arc_dsc;
        lv_draw_arc_dsc_init(&arc_dsc);
        arc_dsc.color = AMBER_OFF;
        arc_dsc.start_angle = 66 + 180;
        arc_dsc.end_angle = 66 + 180 + 48;
        arc_dsc.width = 20;

        static lv_point_t center = {.x = INDIC_X, .y = INDIC_ABS_Y + INDIC_RADIUS_INNER};
        lv_draw_arc(ctx, &arc_dsc, &center, INDIC_RADIUS, 66 + 180, 66 + 180 + 48);
    }

	if (code == LV_EVENT_DRAW_MAIN_END) {
        lv_draw_mask_free_param(&inner_mask_param);
        lv_draw_mask_remove_id(inner_mask_id);
        lv_draw_mask_free_param(&outer_mask_param);
        lv_draw_mask_remove_id(outer_mask_id);
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
    lv_style_set_line_width(&style_line, 25);
    // lv_style_set_bg_color(&style_line, AMBER_ON);
    lv_style_set_line_color(&style_line, AMBER_ON);
    lv_style_set_line_rounded(&style_line, false);

    lv_obj = lv_line_create(parent);
    lv_obj_set_size(lv_obj, 800, 105);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_MID, 0, INDIC_ABS_Y);
    lv_line_set_points(lv_obj, line_points, 2);
    lv_obj_add_style(lv_obj, &style_line, 0);
    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_MAIN_BEGIN, this);
    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_MAIN_END, this);
    lv_obj_center(lv_obj);

    update();
}