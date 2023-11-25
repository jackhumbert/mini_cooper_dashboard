#include "coolant.hpp"

void Coolant::update() {
    if (get_queued()->engine_temp) {
        uint16_t temp = round(get_cache()->engine_temp * 9.0 / 5.0 + 32);
        lv_meter_set_indicator_end_value(meter, indicator, temp);
        lv_label_set_text_fmt(label, "%d°F", temp);
    }
}

static void indicator_draw(lv_event_t * e) {
	lv_event_code_t code = lv_event_get_code(e);
    Coolant * t = (Coolant*)lv_event_get_user_data(e);
	
	if (code == LV_EVENT_DRAW_MAIN_BEGIN) {
	    lv_draw_ctx_t *ctx = lv_event_get_draw_ctx(e);

        lv_area_t scale_area;
        lv_obj_get_content_coords(t->meter, &scale_area);

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

Coolant::Coolant(lv_obj_t * parent) {

    lv_obj = lv_obj_create(parent);
    lv_obj_remove_style_all(lv_obj);
    // lv_obj_set_style_blend_mode(lv_obj, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_scrollbar_mode(lv_obj, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(lv_obj, LV_DIR_NONE);
    lv_obj_set_size(lv_obj, 126, 126);
    lv_obj_align(lv_obj, LV_ALIGN_TOP_RIGHT, -180, 316);
    // lv_obj_align(lv_obj, LV_ALIGN_CENTER, 0, 0);
    // lv_obj_set_style_blend_mode(lv_obj, LV_BLEND_MODE_ADDITIVE, 0);
    // lv_obj_center(lv_obj);

    meter = lv_meter_create(lv_obj);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    // lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_align(meter, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_size(meter, GAUGE_SIZE);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 9, 2, 50, DASH_BACKGROUND);
    // lv_meter_set_scale_major_ticks(meter, scale, 1, 1, 50, DASH_BACKGROUND, 20);
    lv_meter_set_scale_range(meter, scale, 30, 250, 270, 135);

    // lv_obj_set_style_text_color(meter, DASH_BACKGROUND, 0);
    lv_obj_set_style_text_opa(meter, 0, 0);

    background = lv_meter_add_arc(meter, scale, GAUGE_WIDTH, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, background, 30);
    lv_meter_set_indicator_end_value(meter, background, 250);

    indicator = lv_meter_add_arc(meter, scale, GAUGE_WIDTH, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, indicator, 30);
    lv_meter_set_indicator_end_value(meter, indicator, 30);

    lv_obj_add_event_cb(meter, indicator_draw, LV_EVENT_DRAW_PART_BEGIN, this);
    lv_obj_add_event_cb(lv_obj, indicator_draw, LV_EVENT_DRAW_MAIN_BEGIN, this);
    
{
    DASH_FONT(RAJDHANI_REGULAR, 24);

    label = lv_label_create(meter);
    lv_obj_add_style(label, &dash_style_value, 0);
    // lv_obj_align(label, LV_ALIGN_RIGHT_MID, 0, 133);
    lv_obj_set_style_text_font(label, RAJDHANI_REGULAR_24, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -4);
    lv_label_set_text(label, "-°F");
}

{
    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    description = lv_label_create(lv_obj);
    lv_obj_add_style(description, &dash_style_label, 0);
    lv_obj_align(description, LV_ALIGN_CENTER, 0, 14);
    lv_obj_set_style_text_font(description, RAJDHANI_SEMIBOLD_14, 0);
    lv_label_set_text(description, "Coolant");
}

}