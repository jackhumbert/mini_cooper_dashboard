#include "tach5.hpp"
#include <new>
#include <iostream>


#define MY_CLASS (const lv_obj_class_t*)&lv_class

#define INDIC_RADIUS (1850 / 2)
#define INDIC_RADIUS_OUTER (INDIC_RADIUS + 10)
#define INDIC_RADIUS_INNER (INDIC_RADIUS - 10)
#define INDIC_X 400
#define INDIC_Y 20
#define INDIC_ABS_Y 250
// overshoot makes the line more accurate, since it'll be rounded to a pixel when drawn
#define INDIC_OVERSHOOT 40

#define sin_d(x) sin((x) * 3.1415 / 180)

static lv_point_t line_point;

static float x_comp = -0.5;
static float y_comp = -0.5;

Tach5::Tach5(lv_obj_t * parent) : Widget(parent) {
    lv_obj_class_init_obj(this);
}

Tach5 * Tach5::create(lv_obj_t * parent) {
    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (void *)class_p, (void *)parent);
    Tach5 * obj = (Tach5 *)lv_mem_alloc(sizeof(Tach5));
    if(obj == NULL) return NULL;
    lv_memset_00(obj, sizeof(Tach5));

    new (obj) Tach5(parent);
    return obj;
}

static lv_point_t arc_center = {
    .x = INDIC_X, 
    .y = INDIC_ABS_Y + INDIC_RADIUS_INNER - 25,
};

void Tach5::update(void) {
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
        float initial = this->end_angle;
        this->end_angle = this->start_angle + 48 * get_cache()->rpm / 8000.0;
        x_comp = sin_d(this->end_angle + 90);
        y_comp = sin_d(this->end_angle);
        lv_area_t area;
        lv_draw_arc_get_area(arc_center.x, arc_center.y, INDIC_RADIUS_INNER, LV_MIN(floor(initial), floor(end_angle)), LV_MAX(ceil(initial), ceil(end_angle)), 20, false, &area);
        lv_obj_invalidate_area(this, &area);
    }
}

static lv_draw_mask_line_param_t line_mask_param;
static int16_t line_mask_id;

static float x_comps[9];
static float y_comps[9];

static float redline_angle = 66 + 180 + 48 * REDLINE / 8000.0;
static float redline_x_comp;
static float redline_y_comp;

void Tach5::construct(const lv_obj_class_t * cls) {
    lv_obj_set_size(this, 800, 165);
    lv_obj_align(this, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_bg_color(this, lv_color_white(), 0);
    lv_obj_add_style(this, &dash_style_gauge, LV_PART_INDICATOR);
    lv_obj_add_style(this, &dash_style_gauge_bg, LV_PART_MAIN);

    this->start_angle = 66 + 180;
    this->end_angle = 66 + 180;
    x_comp = sin_d(this->end_angle + 90);
    y_comp = sin_d(this->end_angle);

    line_point.x = INDIC_X + (x_comp * (INDIC_RADIUS_INNER));
    line_point.y = INDIC_Y + INDIC_RADIUS + (y_comp * (INDIC_RADIUS_INNER));

    for (int i = 0; i < 9; i++) {
        float end = this->start_angle + 48 * i / 8.0;
        x_comps[i] = sin_d(end + 90);
        y_comps[i] = sin_d(end);
    }
    float end = this->start_angle + 48 * REDLINE / 8000.0;
    redline_x_comp = sin_d(end + 90);
    redline_y_comp = sin_d(end);

    // this->update();
}

void Tach5::destruct(const lv_obj_class_t * cls) {

}
void Tach5::event(const lv_obj_class_t * cls, lv_event_t * event) {
    lv_res_t res = lv_obj_event_base(this->class_p, event);
    if(res != LV_RES_OK) return;

	lv_event_code_t code = lv_event_get_code(event);
	
	if (code == LV_EVENT_DRAW_MAIN) {
        lv_draw_ctx_t *ctx = lv_event_get_draw_ctx(event);

        lv_area_t scale_area;
        lv_obj_get_content_coords(this, &scale_area);

        // background arc
        static lv_draw_arc_dsc_t arc_dsc;
        lv_draw_arc_dsc_init(&arc_dsc);
        // arc_dsc.color = AMBER_OFF;
        // lv_style_get_prop(&dash_style_gauge_bg, LV_STYLE_ARC_COLOR, (lv_style_value_t*)&arc_dsc.color);
        arc_dsc.color = lv_obj_get_style_arc_color(this, LV_PART_MAIN);
        arc_dsc.width = 20;

        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER, this->end_angle, redline_angle);

        arc_dsc.color = RED_OFF;
        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER, redline_angle, this->start_angle + 48);

        // top line
        lv_draw_arc_dsc_init(&arc_dsc);
        arc_dsc.color = AMBER_HALF;
        arc_dsc.width = 1;

        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER + 3, this->start_angle, redline_angle);

        arc_dsc.color = RED_HALF;
        lv_draw_arc(ctx, &arc_dsc, &arc_center, INDIC_RADIUS_INNER + 3, redline_angle, this->start_angle + 48);

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
        // arc_dsc2.color = AMBER_ON;
        // lv_style_get_prop(&dash_style_gauge, LV_STYLE_ARC_COLOR, (lv_style_value_t*)&arc_dsc2.color);
        arc_dsc2.color = lv_obj_get_style_arc_color(this, LV_PART_INDICATOR);
        arc_dsc2.width = 20;

        lv_event_send(this, LV_EVENT_DRAW_PART_BEGIN, &arc_dsc2);
        lv_draw_arc(ctx, &arc_dsc2, &arc_center, INDIC_RADIUS_INNER, this->start_angle, this->end_angle + 1);
        lv_event_send(this, LV_EVENT_DRAW_PART_END, &arc_dsc2);

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

        lv_coord_t r_edge = lv_area_get_width(&scale_area) / 2;
        lv_point_t scale_center;
        scale_center.x = scale_area.x1 + r_edge;
        scale_center.y = scale_area.y1 + r_edge;

        lv_draw_rect_dsc_t mid_dsc;
        lv_draw_rect_dsc_init(&mid_dsc);
        lv_obj_init_draw_rect_dsc(this, LV_PART_INDICATOR, &mid_dsc);
        lv_coord_t w = lv_obj_get_style_width(this, LV_PART_INDICATOR) / 2;
        lv_coord_t h = lv_obj_get_style_height(this, LV_PART_INDICATOR) / 2;
        lv_area_t nm_cord;
        nm_cord.x1 = scale_center.x - w;
        nm_cord.y1 = scale_center.y - h;
        nm_cord.x2 = scale_center.x + w;
        nm_cord.y2 = scale_center.y + h;
        lv_draw_rect(ctx, &mid_dsc, &nm_cord);

    }
}
