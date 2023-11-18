#include "tach4.hpp"

#define INDIC_RADIUS (1850 / 2)
#define INDIC_RADIUS_OUTER (INDIC_RADIUS + 10)
#define INDIC_RADIUS_INNER (INDIC_RADIUS - 10)
#define INDIC_X 400
#define INDIC_Y 20
#define INDIC_ABS_Y 250
// overshoot makes the line more accurate, since it'll be rounded to a pixel when drawn
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

        lv_coord_t x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER));
        lv_coord_t y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER));
        if (line_points[1].x != x || line_points[1].y != y) {
            // lv_area_t area = {
            //     .x1 = 0,
            //     .y1 = INDIC_ABS_Y + 165,
            //     .x2 = LV_MAX(line_points[1].x, x),
            //     .y2 = LV_MAX(line_points[1].y, y)
            // };
            line_points[1].x = x;
            line_points[1].y = y;
            // lv_obj_invalidate_area(lv_obj, &area);
            lv_line_set_points(lv_obj, line_points, 2);
        }
    }
}

static lv_point_t arc_center = {
    .x = INDIC_X, 
    .y = INDIC_ABS_Y + INDIC_RADIUS_INNER - 25,
};

static lv_draw_mask_line_param_t line_mask_param;
static int16_t line_mask_id;

static float x_comps[9];
static float y_comps[9];

static float redline_angle = 66 + 180 + 48 * REDLINE / 8000.0;
static float redline_x_comp;
static float redline_y_comp;

static void indicator_draw(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
	lv_draw_ctx_t *ctx = (lv_draw_ctx_t *)lv_event_get_param(e);
	
	if (code == LV_EVENT_DRAW_MAIN_BEGIN) {

        float end = 66 + 180 + 48 * get_cache()->rpm / 8000.0;

        // background arc
        static lv_draw_arc_dsc_t arc_dsc;
        lv_draw_arc_dsc_init(&arc_dsc);
        arc_dsc.color = AMBER_OFF;
        arc_dsc.width = 20;

        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER, end, redline_angle);

        arc_dsc.color = RED_OFF;
        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER, redline_angle, 66 + 180 + 48);

        // top line
        lv_draw_arc_dsc_init(&arc_dsc);
        arc_dsc.color = AMBER_HALF;
        arc_dsc.width = 1;

        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER + 3, 66 + 180, redline_angle);

        arc_dsc.color = RED_HALF;
        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER + 3, redline_angle, 66 + 180 + 48);


        for (int i = 0; i < 9; i++) {
            // dashes
            static lv_draw_line_dsc_t line_dsc;
            lv_draw_line_dsc_init(&line_dsc);
            line_dsc.color = (i * 1000 < REDLINE) ? AMBER_HALF : RED_HALF;
            line_dsc.width = 1;

            lv_point_t start = {
                .x = (lv_coord_t)round(arc_center.x + x_comps[i] * (INDIC_RADIUS_INNER + 2)),
                .y = (lv_coord_t)round(arc_center.y + y_comps[i] * (INDIC_RADIUS_INNER + 2)),
            };

            lv_point_t end = {
                .x = (lv_coord_t)round(arc_center.x + x_comps[i] * (INDIC_RADIUS_INNER + 8)),
                .y = (lv_coord_t)round(arc_center.y + y_comps[i] * (INDIC_RADIUS_INNER + 8)),
            };

            lv_draw_line(ctx, &line_dsc, &start, &end);

            // labels
            DASH_FONT(RAJDHANI_SEMIBOLD, 14);
            static lv_draw_label_dsc_t label_dsc;
            lv_draw_label_dsc_init(&label_dsc);
            label_dsc.font = RAJDHANI_SEMIBOLD_14;
            label_dsc.color = (i * 1000 < REDLINE) ? AMBER_HALF : RED_HALF;
            label_dsc.align = LV_TEXT_ALIGN_CENTER;

            lv_point_t label_point = {
                .x = (lv_coord_t)round(arc_center.x + x_comps[i] * (INDIC_RADIUS_INNER + 10) - 3),
                .y = (lv_coord_t)round(arc_center.y + y_comps[i] * (INDIC_RADIUS_INNER + 10) - 14),
            };

            lv_draw_letter(ctx, &label_dsc, &label_point, i + '0');
        }

        // actual arc
        lv_point_t arc_end = {
            .x = (lv_coord_t)round(arc_center.x + (x_comp * (INDIC_RADIUS + INDIC_OVERSHOOT))),
            .y = (lv_coord_t)round(arc_center.y + (y_comp * (INDIC_RADIUS + INDIC_OVERSHOOT)))
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


        for (int i = 1; i < 8; i++) {
            // dashes within arc
            static lv_draw_line_dsc_t line_dsc;
            lv_draw_line_dsc_init(&line_dsc);
            line_dsc.color = DASH_BACKGROUND;
            line_dsc.width = 2;

            lv_point_t start = {
                .x = (lv_coord_t)round(arc_center.x + x_comps[i] * (INDIC_RADIUS_INNER + 1)),
                .y = (lv_coord_t)round(arc_center.y + y_comps[i] * (INDIC_RADIUS_INNER + 1)),
            };

            lv_point_t end = {
                .x = (lv_coord_t)round(arc_center.x + x_comps[i] * (INDIC_RADIUS_INNER - 21)),
                .y = (lv_coord_t)round(arc_center.y + y_comps[i] * (INDIC_RADIUS_INNER - 21)),
            };

            lv_draw_line(ctx, &line_dsc, &start, &end);
        }

    }

}

Tach4::Tach4(lv_obj_t * parent) {

    float end = 66 + 180;
    x_comp = sin_d(end + 90);
    y_comp = sin_d(end);

    line_points[0].x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER));
    line_points[0].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER));

    line_points[1].x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER));
    line_points[1].y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER));

    for (int i = 0; i < 9; i++) {
        end = 66 + 180 + 48 * i / 8.0;
        x_comps[i] = sin_d(end + 90);
        y_comps[i] = sin_d(end);
    }
    end = 66 + 180 + 48 * REDLINE / 8000.0;
    redline_x_comp = sin_d(end + 90);
    redline_y_comp = sin_d(end);

    /*Create style*/
    static lv_style_t style_line;
    lv_style_init(&style_line);
    lv_style_set_line_width(&style_line, 0);
    // lv_style_set_bg_color(&style_line, AMBER_ON);
    lv_style_set_line_color(&style_line, AMBER_ON);
    lv_style_set_line_rounded(&style_line, false);

    lv_obj = lv_line_create(parent);
    lv_obj_set_size(lv_obj, 800, 165);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_MID, 0, INDIC_ABS_Y);
    lv_line_set_points(lv_obj, line_points, 2);
    lv_obj_add_style(lv_obj, &style_line, 0);
    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_MAIN_BEGIN, this);
    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_MAIN_END, this);
    lv_obj_center(lv_obj);

    update();
}