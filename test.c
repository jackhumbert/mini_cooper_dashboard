#include "lvgl/lvgl.h"
#include "test.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define CANVAS_WIDTH  800
#define CANVAS_HEIGHT  480

extern lv_font_t lcddot;

static lv_obj_t * meter;
static lv_obj_t * meter2;
static lv_obj_t * canvas;

static lv_obj_t * speed_label;
static lv_obj_t * speed_label2;

#define REDLINE 6750

#define AMBER_ON lv_palette_main(LV_PALETTE_AMBER)
#define AMBER_OFF lv_color_change_lightness(lv_palette_main(LV_PALETTE_AMBER), 20)
#define RED_ON lv_palette_main(LV_PALETTE_RED)
#define RED_HALF lv_color_change_lightness(lv_palette_main(LV_PALETTE_RED), 64)
#define RED_OFF lv_color_change_lightness(lv_palette_main(LV_PALETTE_RED), 20)
#define BLUE_ON lv_palette_main(LV_PALETTE_BLUE)
#define BLUE_OFF lv_color_change_lightness(lv_palette_main(LV_PALETTE_BLUE), 20)
#define GREEN_ON lv_palette_main(LV_PALETTE_GREEN)
#define GREEN_OFF lv_color_change_lightness(lv_palette_main(LV_PALETTE_GREEN), 20)

static lv_meter_indicator_t * rpm_normal;
static lv_meter_indicator_t * rpm_normal2;
static lv_meter_indicator_t * rpm_red;

bool need_to_affect = false;

void setup_effect(void);
void update_effect(void);

static lv_anim_t * redline_anim;

static void redline_anim_toggle(void * indic, int32_t v) {
    if (v) {
        rpm_normal->type_data.arc.color = RED_ON;
        rpm_normal2->type_data.arc.color = lv_color_change_lightness(lv_color_black(), 240);
    } else {
        rpm_normal->type_data.arc.color = lv_color_black();
        rpm_normal2->type_data.arc.color = lv_color_black();
    }
}

static void start_redline_anim(void) {
    if (!redline_anim) {
        static lv_anim_t redline_anim_t;
        lv_anim_init(&redline_anim_t);
        lv_anim_set_exec_cb(&redline_anim_t, redline_anim_toggle);
        lv_anim_set_values(&redline_anim_t, 0, 1);
        lv_anim_set_repeat_delay(&redline_anim_t, 2);
        lv_anim_set_playback_delay(&redline_anim_t, 2);
        lv_anim_set_repeat_count(&redline_anim_t, LV_ANIM_REPEAT_INFINITE);
        lv_anim_set_time(&redline_anim_t, 1);
        lv_anim_set_playback_time(&redline_anim_t, 1);
        redline_anim = lv_anim_start(&redline_anim_t);
    }
}

static void stop_redline_anim(void) {
    if (redline_anim) {
        lv_anim_del(redline_anim, NULL);
        redline_anim = NULL;
    }
}

static void set_value(void * indic, int32_t v)
{
    if (v > REDLINE) {
        start_redline_anim();
    } else {
        stop_redline_anim();
        rpm_normal->type_data.arc.color = AMBER_ON;
        rpm_normal2->type_data.arc.color = lv_color_change_lightness(lv_color_black(), 240);
    }
    if (rpm_normal) {
        lv_meter_set_indicator_end_value(meter, rpm_normal, MIN(v, REDLINE));
        lv_meter_set_indicator_end_value(meter2, rpm_normal2, MIN(v, REDLINE));
    }
    if (rpm_red) {
        lv_meter_set_indicator_end_value(meter, rpm_red, MAX(v, REDLINE));
    }
    // need_to_affect = true;
    update_effect();
}

static void set_speed_value(void * indic, int32_t v) {
    if (v >= 10) {
        lv_label_set_text_fmt(speed_label, "%d", (int)(v / 10));
        lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    } else {
        lv_label_set_text(speed_label, "");
        lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_CENTER, 0, 2, LV_GRID_ALIGN_CENTER, 0, 1);
    }
    lv_label_set_text_fmt(speed_label2, "%d", v % 10);
}

static void set_temp(void * bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

static void canvas_draw_end(lv_event_t * e) {
    // if (need_to_affect) {
    //     need_to_affect = false;
    //     update_effect();
    // }
}


static void draw_meter_tick(lv_event_t * e) {

}

void test(void) {

    // lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_TRANSP, LV_PART_MAIN);
    // lv_disp_set_bg_opa(NULL, LV_OPA_TRANSP);

    lv_obj_set_style_bg_color(lv_scr_act(), lv_color_hex3(0x0F0100), 0);

    /*Create a buffer for the canvas*/
    // static uint8_t sbuf[CANVAS_WIDTH * CANVAS_HEIGHT * 4];

    // lv_obj_t * screen = lv_canvas_create(lv_scr_act());
    // lv_canvas_set_buffer(screen, sbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_ALPHA);
    // lv_obj_center(screen);

    // lv_canvas_fill_bg(screen, lv_palette_main(LV_PALETTE_NONE), LV_OPA_COVER);

    static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(CANVAS_WIDTH, CANVAS_HEIGHT)];

    /*Create a canvas and initialize its palette*/
    canvas = lv_canvas_create(lv_scr_act());
    lv_obj_add_event_cb(canvas, canvas_draw_end, LV_EVENT_DRAW_POST, NULL);
    lv_obj_set_size(canvas, 800, 480);
    lv_canvas_set_buffer(canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_ALPHA);
    lv_obj_center(canvas);
    // lv_canvas_fill_bg(canvas, lv_palette_darken(LV_PALETTE_AMBER, 3), LV_OPA_COVER);

    // lv_canvas_fill_bg(canvas, lv_palette_main(LV_PALETTE_BLUE), LV_OPA_COVER);



#define GAS_ANGLE 72

// gas
{
    meter = lv_meter_create(canvas);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_center(meter);
    lv_obj_set_size(meter, 480, 480);
    // lv_obj_set_style_translate_x(meter, 200, 0);
    // lv_obj_set_style_translate_y(meter, 50, 0);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 16, 1, 24, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, scale, 5, 5, 24, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 150, 0, GAS_ANGLE * 15.0/13.2, 360 + GAS_ANGLE / 2 - GAS_ANGLE * (15.0/13.2));


    lv_obj_set_style_text_color(meter, lv_color_black(), NULL);


    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 20, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 132);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, 0);

    /*Add a three arc indicator*/
    lv_meter_indicator_t * indic1 = lv_meter_add_arc(meter, scale, 20, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, indic1, 120);
    lv_meter_set_indicator_end_value(meter, indic1, 0);
    // indic1->start_value = 0;
    // indic1->end_value = 50;

//     lv_anim_set_time(&a, 2000);
//     lv_anim_set_playback_time(&a, 500);
//     lv_anim_set_var(&a, indic1);
//     lv_anim_start(&a);

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

    lv_obj_t * gas_label = lv_label_create(meter);
    lv_obj_align(gas_label, LV_ALIGN_RIGHT_MID, 0, 133);
    lv_obj_set_style_text_font(gas_label, info.font, NULL);
    lv_label_set_text(gas_label, "12.0");
    lv_obj_set_style_text_color(gas_label, AMBER_ON, NULL);

    lv_obj_t * fuel_label = lv_label_create(meter);
    // lv_obj_align(fuel_label, LV_ALIGN_RIGHT_MID, 3, -146);
    lv_obj_align(fuel_label, LV_ALIGN_RIGHT_MID, 0, -142);
    lv_obj_set_style_text_font(fuel_label, info.font, NULL);
    lv_label_set_text(fuel_label, "FUEL");
    lv_obj_set_style_text_color(fuel_label, AMBER_ON, NULL);
    // lv_obj_set_style_transform_angle(fuel_label, 570, 0);
}


// #define TEMP_ANGLE 72

// // temps
// {
//     meter = lv_meter_create(canvas);

//     /*Remove the circle from the middle*/
//     lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
//     lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
//     lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
//     lv_obj_center(meter);
//     lv_obj_set_size(meter, 530, 530);
//     // lv_obj_set_style_translate_x(meter, 200, 0);
//     // lv_obj_set_style_translate_y(meter, 50, 0);

//     /*Add a scale first*/
//     lv_meter_scale_t * scale = lv_meter_add_scale(meter);
//     lv_meter_set_scale_ticks(meter, scale, 10, 1, 50, lv_color_black());
//     lv_meter_set_scale_major_ticks(meter, scale, 5, 5, 50, lv_color_black(), 20);
//     lv_meter_set_scale_range(meter, scale, 80, 220, TEMP_ANGLE, 180 - TEMP_ANGLE / 2);


//     lv_obj_set_style_text_color(meter, lv_color_black(), NULL);


//     lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 10, AMBER_OFF, -1);
//     lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 80);
//     lv_meter_set_indicator_end_value(meter, rpm_normal_bg, 220);

//     lv_meter_indicator_t * rpm_normal_bg2 = lv_meter_add_arc(meter, scale, 10, AMBER_OFF, -12);
//     lv_meter_set_indicator_start_value(meter, rpm_normal_bg2, 80);
//     lv_meter_set_indicator_end_value(meter, rpm_normal_bg2, 220);

//     lv_meter_indicator_t * rpm_normal_bg3 = lv_meter_add_arc(meter, scale, 10, AMBER_OFF, -40);
//     lv_meter_set_indicator_start_value(meter, rpm_normal_bg3, 80);
//     lv_meter_set_indicator_end_value(meter, rpm_normal_bg3, 220);

//     /*Add a three arc indicator*/
//     lv_meter_indicator_t * indic1 = lv_meter_add_arc(meter, scale, 10, AMBER_ON, -1);
//     lv_meter_set_indicator_start_value(meter, indic1, 80);
//     lv_meter_set_indicator_end_value(meter, indic1, 180);

//     lv_meter_indicator_t * indic2 = lv_meter_add_arc(meter, scale, 10, AMBER_ON, -12);
//     lv_meter_set_indicator_start_value(meter, indic2, 80);
//     lv_meter_set_indicator_end_value(meter, indic2, 160);

//     lv_meter_indicator_t * indic3 = lv_meter_add_arc(meter, scale, 10, AMBER_ON, -40);
//     lv_meter_set_indicator_start_value(meter, indic3, 80);
//     lv_meter_set_indicator_end_value(meter, indic3, 160);
//     // indic1->start_value = 0;
//     // indic1->end_value = 50;

// //     lv_anim_set_time(&a, 2000);
// //     lv_anim_set_playback_time(&a, 500);
// //     lv_anim_set_var(&a, indic1);
// //     lv_anim_start(&a);
// }

// engine temp

{

    lv_obj_t * meter = lv_meter_create(canvas);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_center(meter);
    lv_obj_set_size(meter, 100, 100);
    lv_obj_set_style_translate_x(meter, -300, 0);
    lv_obj_set_style_translate_y(meter, -100, 0);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 11, 1, 50, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, scale, 5, 5, 50, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 30, 250, 180, 90);

    lv_obj_set_style_text_color(meter, lv_color_black(), NULL);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 10, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 30);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, 250);

    lv_meter_indicator_t * indic1 = lv_meter_add_arc(meter, scale, 10, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, indic1, 30);
    lv_meter_set_indicator_end_value(meter, indic1, 140);


    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    // info.name = "./fonts/Rajdhani-Regular.ttf";
    info.name = "./fonts/Bould-Regular.otf";
    info.weight = 24;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    lv_obj_t * engine_temp_label = lv_label_create(meter);
    // lv_obj_align(engine_temp_label, LV_ALIGN_RIGHT_MID, 0, 133);
    lv_obj_set_style_text_font(engine_temp_label, info.font, NULL);
    lv_obj_center(engine_temp_label);
    lv_label_set_text(engine_temp_label, "140");
    lv_obj_set_style_text_color(engine_temp_label, AMBER_ON, NULL);

}

// oil


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

    lv_obj_t * engine_temp_label = lv_label_create(canvas);
    lv_obj_align(engine_temp_label, LV_ALIGN_CENTER, -290, 55);
    lv_obj_set_style_text_font(engine_temp_label, info.font, NULL);
    lv_label_set_text(engine_temp_label, "OIL");
    lv_obj_set_style_text_color(engine_temp_label, AMBER_ON, NULL);
}


// oil temp

{

    lv_obj_t * meter = lv_meter_create(canvas);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_size(meter, 100, 100);
    lv_obj_align(meter, LV_ALIGN_CENTER, -250, 100);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 11, 1, 50, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, scale, 5, 5, 50, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 100, 320, 180, 270);

    lv_obj_set_style_text_color(meter, lv_color_black(), NULL);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 10, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 100);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, 320);

    lv_meter_indicator_t * indic1 = lv_meter_add_arc(meter, scale, 10, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, indic1, 100);
    lv_meter_set_indicator_end_value(meter, indic1, 210);

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

        lv_obj_t * engine_temp_label = lv_label_create(meter);
        // lv_obj_align(engine_temp_label, LV_ALIGN_RIGHT_MID, 0, 133);
        lv_obj_set_style_text_font(engine_temp_label, info.font, NULL);
        lv_obj_center(engine_temp_label);
        lv_label_set_text(engine_temp_label, "210°F");
        lv_obj_set_style_text_color(engine_temp_label, AMBER_ON, NULL);
    }

}

// oil pressure

{

    lv_obj_t * meter = lv_meter_create(canvas);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_size(meter, 100, 100);
    lv_obj_align(meter, LV_ALIGN_CENTER, -330, 100);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 11, 1, 50, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, scale, 5, 5, 50, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 0, 70, 180, 90);

    lv_obj_set_style_text_color(meter, lv_color_black(), NULL);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 10, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, 70);

    lv_meter_indicator_t * indic1 = lv_meter_add_arc(meter, scale, 10, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, indic1, 0);
    lv_meter_set_indicator_end_value(meter, indic1, 35);

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

        lv_obj_t * engine_temp_label = lv_label_create(meter);
        // lv_obj_align(engine_temp_label, LV_ALIGN_RIGHT_MID, 0, 133);
        lv_obj_set_style_text_font(engine_temp_label, info.font, NULL);
        lv_obj_center(engine_temp_label);
        lv_label_set_text(engine_temp_label, "35 psi");
        lv_obj_set_style_text_color(engine_temp_label, AMBER_ON, NULL);
    }

}


// tach
{
    meter = lv_meter_create(canvas);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter, NULL, LV_PART_MAIN);
    lv_obj_set_style_blend_mode(meter, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_center(meter);
    lv_obj_set_size(meter, 400, 400);

    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    // info.name = "./fonts/Microgramma-D-Extended-Medium.otf";
    // info.name = "./fonts/Rajdhani-Bold.ttf";
    info.name = "./fonts/Bould-Regular.otf";
    info.weight = 24;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }



    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 81, 1, 24, lv_color_black());
    // lv_meter_set_scale_major_ticks(meter, scale, 10, 5, 12, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 0, 8000, 270, 135);


    lv_meter_scale_t * label_scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, label_scale, 9, 1, 24, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, label_scale, 1, 5, 24, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, label_scale, 0, 8, 270, 135);

    lv_obj_set_style_text_color(meter, AMBER_ON, NULL);
    lv_obj_set_style_text_font(meter, info.font, NULL);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 20, AMBER_OFF, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, REDLINE);

    rpm_normal = lv_meter_add_arc(meter, scale, 20, AMBER_ON, -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal, REDLINE);


    // effect

    meter2 = lv_meter_create(canvas);
    /*Remove the circle from the middle*/
    lv_obj_remove_style(meter2, NULL, LV_PART_INDICATOR);
    lv_obj_remove_style(meter2, NULL, LV_PART_MAIN);
    lv_obj_set_style_blend_mode(meter2, LV_BLEND_MODE_MULTIPLY, 0);
    lv_obj_center(meter2);
    lv_obj_set_size(meter2, 400, 400);
    lv_obj_set_style_text_color(meter2, lv_color_white(), NULL);


    lv_meter_scale_t * scale2 = lv_meter_add_scale(meter2);
    lv_meter_set_scale_ticks(meter2, scale2, 81, 5, 24, lv_color_white());
    lv_meter_set_scale_major_ticks(meter2, scale2, 10, 9, 24, lv_color_white(), 20);
    lv_meter_set_scale_range(meter2, scale2, 0, 8000, 270, 135);

    rpm_normal2 = lv_meter_add_arc(meter2, scale2, 16, lv_color_change_lightness(lv_color_black(), 220), -3);
    lv_meter_set_indicator_start_value(meter2, rpm_normal2, 0);
    lv_meter_set_indicator_end_value(meter2, rpm_normal2, REDLINE);


    /*Create an animation to set the value*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value);
    lv_anim_set_values(&a, 800, 7500);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    lv_anim_set_time(&a, 2000);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_var(&a, rpm_normal);
    lv_anim_start(&a);

    lv_meter_indicator_t * rpm_red_bg = lv_meter_add_arc(meter, scale, 20, lv_color_change_lightness(lv_palette_main(LV_PALETTE_RED), 15), -1);
    lv_meter_set_indicator_start_value(meter, rpm_red_bg, REDLINE);
    lv_meter_set_indicator_end_value(meter, rpm_red_bg, 8000);

    /*Add a red arc to the end*/
    rpm_red = lv_meter_add_arc(meter, scale, 20, lv_palette_main(LV_PALETTE_RED), -1);
    lv_meter_set_indicator_start_value(meter, rpm_red, REDLINE);
    lv_meter_set_indicator_end_value(meter, rpm_red, 8000);

}


// icons
{
    static lv_coord_t col_dsc[] = {48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {48, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t * cont = lv_obj_create(canvas);
    lv_obj_remove_style_all(cont);
    lv_obj_set_grid_align(cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 800, 48);
    lv_obj_center(cont);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 200);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

{
    LV_IMG_DECLARE(slice1);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice1);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(slice2);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice2);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 1, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, AMBER_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(slice3);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice3);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 2, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, AMBER_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(slice5);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice5);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 3, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(slice6);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice6);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 4, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

// {
//     LV_IMG_DECLARE(slice7);
    
//     lv_obj_t * icon = lv_img_create(cont);
//     lv_img_set_src(icon, &slice7);
//     lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
//     lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 5, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

//     static lv_style_t style;
//     lv_style_init(&style);
//     lv_style_set_img_recolor(&style, lv_palette_main(LV_PALETTE_RED));
//     lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
//     lv_obj_add_style(icon, &style, 0);
// }

{
    LV_IMG_DECLARE(slice8);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice8);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 7, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, AMBER_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

// {
//     LV_IMG_DECLARE(slice9);
    
//     lv_obj_t * icon = lv_img_create(cont);
//     lv_img_set_src(icon, &slice9);
//     lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
//     lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 7, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

//     static lv_style_t style;
//     lv_style_init(&style);
//     lv_style_set_img_recolor(&style, lv_palette_main(LV_PALETTE_BLUE));
//     lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
//     lv_obj_add_style(icon, &style, 0);
// }


{
    LV_IMG_DECLARE(slice10);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice10);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 8, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}


{
    LV_IMG_DECLARE(slice11);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice11);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 9, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, AMBER_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}


{
    LV_IMG_DECLARE(slice12);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice12);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 10, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, AMBER_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}


{
    LV_IMG_DECLARE(slice13);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice13);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 11, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}
}

// center area
{

    static lv_coord_t col_dsc[] = {48, 48, 48, 48, 48, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {48, 48, 48, 48, 48, 48, 48, 48, 48, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t * cont = lv_obj_create(canvas);
    lv_obj_remove_style_all(cont);
    lv_obj_set_grid_align(cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
    lv_obj_set_size(cont, 800, 480);
    lv_obj_center(cont);
    lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_layout(cont, LV_LAYOUT_GRID);

// speed
{

    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    // info.name = "./fonts/Rajdhani-Bold.ttf";
    // info.name = "./fonts/DSEG14ClassicMini-Regular.ttf";
    info.name = "./fonts/Bould-Medium.otf";
    info.weight = 96;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

    // {
    //     static lv_style_t style;
    //     lv_style_init(&style);
    //     lv_style_set_text_opa(&style, LV_OPA_100);
    //     lv_style_set_text_color(&style, AMBER_OFF);
    //     lv_style_set_text_font(&style, info.font);

    //     lv_obj_t * speed_label = lv_label_create(cont);
    //     lv_label_set_text(speed_label, "188");
    //     lv_obj_add_style(speed_label, &style, 0);
    //     lv_obj_set_style_blend_mode(speed_label, LV_BLEND_MODE_ADDITIVE, 0);
    //     lv_obj_set_grid_cell(speed_label, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 3, 3);

    //     lv_obj_align(speed_label, LV_ALIGN_CENTER, 0, 0);
    // }


    static lv_coord_t col_dsc[] = {48, 48, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {48, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj_t * speed_cont = lv_obj_create(cont);
    lv_obj_remove_style_all(speed_cont);
    lv_obj_set_grid_align(speed_cont, LV_GRID_ALIGN_CENTER, LV_GRID_ALIGN_CENTER);
    lv_obj_set_style_grid_column_dsc_array(speed_cont, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(speed_cont, row_dsc, 0);
    lv_obj_set_size(speed_cont, 800, 480);
    lv_obj_center(speed_cont);
    lv_obj_align(speed_cont, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_layout(speed_cont, LV_LAYOUT_GRID);
    lv_obj_set_grid_cell(speed_cont, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 3, 3);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_ON);
    lv_style_set_text_font(&style, info.font);

        speed_label = lv_label_create(speed_cont);
        lv_label_set_text_fmt(speed_label, "%d", (int)(50 / 10));
        lv_obj_add_style(speed_label, &style, 0);
        lv_obj_set_style_blend_mode(speed_label, LV_BLEND_MODE_ADDITIVE, 0);
        lv_obj_set_grid_cell(speed_label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        lv_obj_align(speed_label, LV_ALIGN_RIGHT_MID, 0, 0);

        speed_label2 = lv_label_create(speed_cont);
        lv_label_set_text_fmt(speed_label2, "%d", 50 % 10);
        lv_obj_add_style(speed_label2, &style, 0);
        lv_obj_set_style_blend_mode(speed_label2, LV_BLEND_MODE_ADDITIVE, 0);
        lv_obj_set_grid_cell(speed_label2, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);

        lv_obj_align(speed_label2, LV_ALIGN_LEFT_MID, 0, 0);

        lv_anim_t a;
        lv_anim_init(&a);
        lv_anim_set_exec_cb(&a, set_speed_value);
        lv_anim_set_values(&a, 0, 80);
        lv_anim_set_repeat_delay(&a, 100);
        lv_anim_set_playback_delay(&a, 100);
        lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

        lv_anim_set_time(&a, 2000);
        lv_anim_set_playback_time(&a, 500);
        lv_anim_start(&a);
}


// time

{
    static lv_ft_info_t info;
    /*FreeType uses C standard file system, so no driver letter is required.*/
    // info.name = "./fonts/Microgramma-D-Extended-Medium.otf";
    // info.name = "./fonts/Rajdhani-Regular.ttf";
    // info.name = "./fonts/DSEG14ClassicMini-Regular.ttf";
    info.name = "./fonts/Bould-Regular.otf";
    info.weight = 22;
    info.style = FT_FONT_STYLE_NORMAL;
    info.mem = NULL;
    if(!lv_ft_font_init(&info)) {
        LV_LOG_ERROR("create failed.");
    }

// {
//     static lv_style_t style;
//     lv_style_init(&style);
//     lv_style_set_text_opa(&style, LV_OPA_100);
//     lv_style_set_text_color(&style, AMBER_OFF);
//     lv_style_set_text_font(&style, info.font);

//     lv_obj_t * time_label = lv_label_create(cont);
//     lv_label_set_text(time_label, "88:88 8~");
//     lv_obj_add_style(time_label, &style, 0);
//     lv_obj_set_style_blend_mode(time_label, LV_BLEND_MODE_ADDITIVE, 0);
//     lv_obj_set_grid_cell(time_label, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 2, 1);

//     lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);

// }
 
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_ON);
    lv_style_set_text_font(&style, info.font);

    lv_obj_t * time_label = lv_label_create(cont);
    lv_label_set_text(time_label, "5:30 PM");
    lv_obj_add_style(time_label, &style, 0);
    lv_obj_set_style_blend_mode(time_label, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(time_label, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 2, 1);

    lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);
}
}


// car view

{
    LV_IMG_DECLARE(car_view);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &car_view);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, AMBER_OFF);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(hood);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &hood);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(trunk);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &trunk);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(headlights);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &headlights);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    // lv_style_set_img_recolor(&style, BLUE_ON);
    lv_style_set_img_recolor(&style, lv_color_white());
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}


{
    LV_IMG_DECLARE(sunroof);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &sunroof);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



{
    LV_IMG_DECLARE(door_left);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &door_left);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



{
    LV_IMG_DECLARE(door_right);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &door_right);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



{
    LV_IMG_DECLARE(gas_cap);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &gas_cap);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, RED_ON);
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



{
    LV_IMG_DECLARE(foglights);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &foglights);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 2, 1, LV_GRID_ALIGN_CENTER, 7, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, lv_color_white());
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}



// brake

// {
//     lv_obj_t * btn2 = lv_btn_create(cont);
//     lv_obj_set_size(btn2, 140, 40);
//     // lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 70);
//     lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
//     lv_obj_set_style_blend_mode(btn2, LV_BLEND_MODE_ADDITIVE, 0);
//     lv_obj_set_style_pad_all(btn2, 5, 0);
//     lv_obj_set_grid_cell(btn2, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 5, 2);

//     // lv_obj_remove_style_all(btn2);


//     static lv_ft_info_t info;
//     /*FreeType uses C standard file system, so no driver letter is required.*/
//     // info.name = "./fonts/Microgramma-D-Extended-Bold.otf";
//     info.name = "./fonts/Rajdhani-Bold.ttf";
//     info.weight = 18;
//     info.style = FT_FONT_STYLE_NORMAL;
//     info.mem = NULL;
//     if(!lv_ft_font_init(&info)) {
//         LV_LOG_ERROR("create failed.");
//     }

//     static lv_style_t style;
//     lv_style_init(&style);
//     lv_style_set_bg_color(&style, lv_color_change_lightness(lv_palette_main(LV_PALETTE_RED), 15));
//     lv_style_set_outline_width(&style, 0);
//     lv_style_set_border_width(&style, 0);
//     lv_style_set_shadow_width(&style, 0);
//     // lv_obj_add_style(btn2, &style, LV_PART_MAIN | LV_STATE_DEFAULT);
//     lv_obj_add_style(btn2, &style, LV_STATE_FOCUS_KEY);

//     static lv_style_t style_pr;
//     lv_style_init(&style_pr);
//     lv_style_set_bg_color(&style_pr, lv_palette_main(LV_PALETTE_RED));
//     lv_style_set_outline_width(&style_pr, 0);
//     lv_style_set_border_width(&style_pr, 0);
//     lv_style_set_shadow_width(&style_pr, 0);
//     lv_obj_add_style(btn2, &style_pr, LV_STATE_FOCUS_KEY | LV_STATE_CHECKED);
//     // lv_obj_add_style(btn2, &style_pr, LV_PART_MAIN | LV_STATE_PRESSED);

//     lv_obj_set_height(btn2, LV_SIZE_CONTENT);

//     lv_obj_t * label = lv_label_create(btn2);
//     lv_obj_set_style_text_color(label, lv_color_black(), NULL);
//     lv_obj_set_style_text_font(label, info.font, 0);
//     // lv_obj_set_style_text_letter_space(label, 1, 0);
//     lv_label_set_text(label, "BRAKE");
//     lv_obj_center(label);

// }


{
    LV_IMG_DECLARE(slice15);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice15);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 4, 1, LV_GRID_ALIGN_CENTER, 4, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, lv_color_change_lightness(lv_palette_main(LV_PALETTE_GREEN), 15));
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

{
    LV_IMG_DECLARE(slice15);
    
    lv_obj_t * icon = lv_img_create(cont);
    lv_img_set_src(icon, &slice15);
    lv_obj_set_style_blend_mode(icon, LV_BLEND_MODE_ADDITIVE, 0);
    lv_obj_set_style_transform_angle(icon, 1800, 0);
    lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 5, 1);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_img_recolor(&style, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_img_recolor_opa(&style, LV_OPA_COVER);
    lv_obj_add_style(icon, &style, 0);
}

}


    update_effect();


    // static lv_style_t style;
    // lv_style_init(&style);
    // lv_style_set_bg_opa(&style, LV_OPA_10);
    // lv_obj_add_style(effect, &style, 0);

    // LV_BLEND_MODE_ADDITIVE



    // lv_img_dsc_t * snapshot = (void *)lv_img_get_src(snapshot_obj);
    // if (snapshot) {
    //     lv_snapshot_free(snapshot);
    // }

    // /*Update the snapshot, we know parent of object is the container.*/
    // snapshot = lv_snapshot_take(canvas, LV_IMG_CF_TRUE_COLOR_ALPHA);
    // if (snapshot != NULL) {
    //     lv_img_set_src(snapshot_obj, snapshot);
    // }

    // lv_snapshot_take_to_buf(canvas, LV_IMG_CF_TRUE_COLOR_ALPHA, &img, &cbuf_tmp, sizeof(cbuf_tmp));

    // lv_scr_load(s2);

}

static lv_color_t cbuf_tmp[LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(CANVAS_WIDTH, CANVAS_HEIGHT)];
static lv_img_dsc_t img;

static lv_obj_t * effect1;
static lv_obj_t * effect2;
static lv_color_t ebuf1[LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(CANVAS_WIDTH, CANVAS_HEIGHT)];
static lv_color_t ebuf2[LV_CANVAS_BUF_SIZE_TRUE_COLOR_ALPHA(CANVAS_WIDTH, CANVAS_HEIGHT)];

void setup_effect() {
    // memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));
    img.data = (void *)cbuf_tmp;
    img.header.cf = LV_IMG_CF_TRUE_COLOR;
    img.header.w = CANVAS_WIDTH;
    img.header.h = CANVAS_HEIGHT;

    effect1 = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(effect1, ebuf1, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_style_bg_color(effect1, lv_color_black(), 0);    
    // lv_obj_move_background(effect1);
    lv_obj_center(effect1);
    lv_obj_set_style_opa(effect1, 25, 0);
    lv_obj_set_style_blend_mode(effect1, LV_BLEND_MODE_ADDITIVE, 0);


    effect2 = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(effect2, ebuf2, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_style_bg_color(effect2, lv_color_black(), 0);    
    // lv_obj_move_background(effect2);
    lv_obj_center(effect2);
    lv_obj_set_style_opa(effect2, 15, 0);
    lv_obj_set_style_blend_mode(effect2, LV_BLEND_MODE_ADDITIVE, 0);
}

void update_effect() {
    if (!effect1) {
        setup_effect();
    }
    lv_snapshot_take_to_buf(canvas, LV_IMG_CF_TRUE_COLOR, &img, &cbuf_tmp, sizeof(cbuf_tmp));
 
    // lv_canvas_transform(effect1, &img, 0, 260, (260.0/256.0 - 1) * CANVAS_WIDTH / 2, (260.0/256.0 - 1) * CANVAS_HEIGHT / 2, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
    lv_canvas_transform(effect1, &img, 0, 260, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);

    // lv_canvas_blur_hor(effect1, NULL, 3);
    // lv_canvas_blur_ver(effect1, NULL, 3);
    // lv_canvas_blur_hor(effect1, NULL, 2);
    // lv_canvas_blur_ver(effect1, NULL, 2);
    // lv_canvas_blur_hor(effect1, NULL, 2);
    // lv_canvas_blur_ver(effect1, NULL, 2);
    lv_snapshot_take_to_buf(effect1, LV_IMG_CF_TRUE_COLOR, &img, &cbuf_tmp, sizeof(cbuf_tmp));

    // lv_canvas_transform(effect2, &img, 0, 264, (264.0/256.0 - 1) * CANVAS_WIDTH / 2, (264.0/256.0 - 1) * CANVAS_HEIGHT / 2, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
    // lv_canvas_transform(effect2, &img, 0, 264, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
    lv_canvas_transform(effect2, &img, 0, 260, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);

    lv_canvas_blur_hor(effect2, NULL, 3);
    lv_canvas_blur_ver(effect2, NULL, 3);
    // lv_canvas_blur_hor(effect2, NULL, 2);
    // lv_canvas_blur_ver(effect2, NULL, 2);
    // lv_canvas_blur_hor(effect2, NULL, 2);
    // lv_canvas_blur_ver(effect2, NULL, 2);
    // lv_canvas_blur_hor(effect2, NULL, 2);
    // lv_canvas_blur_ver(effect2, NULL, 2);
    // lv_canvas_blur_hor(effect2, NULL, 2);
    // lv_canvas_blur_ver(effect2, NULL, 2);
    // lv_canvas_blur_hor(effect2, NULL, 2);
    // lv_canvas_blur_ver(effect2, NULL, 2);
}