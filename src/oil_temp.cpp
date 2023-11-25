#include "oil_temp.hpp"

void OilTemp::update(void) {
    if (get_queued()->oil_temp) {
        int16_t temp = 0;
        // manually check offset applied by can processing
        if (get_dash()->oil_temp > -48) {
            // convert from C to F
            temp = round(get_dash()->oil_temp * 9 / 5.0 + 32);
            lv_label_set_text_fmt(label, "%d°F", temp);
        } else {
            lv_label_set_text(label, "-°F");
        }
        lv_meter_set_indicator_end_value(lv_obj, indicator, temp);
        lv_color_t old_color = indicator->type_data.arc.color;
        if (temp > 260) {
            indicator->type_data.arc.color = RED_ON;
            lv_obj_set_style_text_color(label, RED_ON, 0);
        } else {
            indicator->type_data.arc.color = AMBER_ON;
            lv_obj_set_style_text_color(label, IMPORTANT_TEXT, 0);
        }
        if (old_color.full != indicator->type_data.arc.color.full) {
            lv_obj_invalidate(lv_obj);
        }
    }
}

static void indicator_draw(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
    OilTemp * t = (OilTemp*)lv_event_get_user_data(e);
	
	if (code == LV_EVENT_DRAW_MAIN_BEGIN) {
	    lv_draw_ctx_t *ctx = lv_event_get_draw_ctx(e);

        lv_area_t scale_area;
        lv_obj_get_content_coords(t->lv_obj, &scale_area);

        static lv_draw_arc_dsc_t arc_dsc;
        lv_draw_arc_dsc_init(&arc_dsc);
        // lv_style_get_prop(&dash_style_gauge, LV_STYLE_ARC_COLOR, (lv_style_value_t*)&arc_dsc.color);
        arc_dsc.color = AMBER_HALF;
        arc_dsc.width = 1;

        lv_point_t center = {
            .x = (int)round((scale_area.x1 + scale_area.x2) / 2) + 1,
            .y = (int)round((scale_area.y1 + scale_area.y2) / 2) + 1
        };

        lv_draw_arc(ctx, &arc_dsc, &center, 120/2 + 2, 135, 270 + 135);
    }

	if (code == LV_EVENT_DRAW_PART_BEGIN) {
	    lv_obj_draw_part_dsc_t *dsc = (lv_obj_draw_part_dsc_t *)lv_event_get_draw_part_dsc(e);
        if (dsc->sub_part_ptr == t->background) {
            lv_style_get_prop(&dash_style_gauge_bg, LV_STYLE_ARC_COLOR, (lv_style_value_t*)&dsc->arc_dsc->color);
        }
        if (dsc->sub_part_ptr == t->indicator) {
            lv_style_get_prop(&dash_style_gauge, LV_STYLE_ARC_COLOR, (lv_style_value_t*)&dsc->arc_dsc->color);
        }
    }
}

OilTemp::OilTemp(lv_obj_t * parent) {
    lv_obj = lv_meter_create(parent);

    lv_obj_remove_style(lv_obj, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(lv_obj, NULL, LV_PART_MAIN);
    lv_obj_set_size(lv_obj, 126, 126);
    lv_obj_set_style_pad_all(lv_obj, 3, 0);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_LEFT, 180, 316);

    lv_meter_scale_t * scale = lv_meter_add_scale(lv_obj);
    lv_meter_set_scale_ticks(lv_obj, scale, 9, 2, 50, DASH_BACKGROUND);
    // lv_meter_set_scale_major_ticks(lv_obj, scale, 5, 3, 50, DASH_BACKGROUND, 20);
    lv_meter_set_scale_range(lv_obj, scale, 100, 320, 270, 135);

    lv_obj_set_style_text_color(lv_obj, DASH_BACKGROUND, 0);
    lv_obj_set_style_text_opa(lv_obj, 0, 0);

    background = lv_meter_add_arc(lv_obj, scale, GAUGE_WIDTH, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(lv_obj, background, 100);
    lv_meter_set_indicator_end_value(lv_obj, background, 320);

    indicator = lv_meter_add_arc(lv_obj, scale, GAUGE_WIDTH, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(lv_obj, indicator, 100);
    lv_meter_set_indicator_end_value(lv_obj, indicator, 100);

    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_PART_BEGIN, this);
    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_MAIN_BEGIN, this);

    DASH_FONT(RAJDHANI_REGULAR, 24);

    label = lv_label_create(lv_obj);
    lv_obj_add_style(label, &dash_style_value, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -4);
    lv_obj_set_style_text_font(label, RAJDHANI_REGULAR_24, 0);
    lv_label_set_text(label, "-°F");

    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    lv_obj_t * description = lv_label_create(lv_obj);
    lv_obj_add_style(description, &dash_style_label, 0);
    lv_obj_align(description, LV_ALIGN_CENTER, 0, 14);
    lv_obj_set_style_text_font(description, RAJDHANI_SEMIBOLD_14, 0);
    lv_label_set_text(description, "Oil Temp.");

}