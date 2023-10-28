#include "lvgl/lvgl.h"
#include "test.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define CANVAS_WIDTH  800
#define CANVAS_HEIGHT  480

extern lv_font_t lcddot;

static lv_obj_t * meter;
static lv_obj_t * canvas;

#define REDLINE 5500

static lv_meter_indicator_t * rpm_normal;
static lv_meter_indicator_t * rpm_red;

bool need_to_affect = false;

void setup_effect(void);
void update_effect(void);

static void set_value(void * indic, int32_t v)
{
    if (rpm_normal) {
        lv_meter_set_indicator_end_value(meter, rpm_normal, MIN(v, REDLINE));
    }
    if (rpm_red) {
        lv_meter_set_indicator_end_value(meter, rpm_red, MAX(v, REDLINE));
    }
    // need_to_affect = true;
    update_effect();
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



#define GAS_ANGLE 90

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
    lv_meter_set_scale_ticks(meter, scale, 16, 1, 14, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, scale, 5, 5, 14, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 150, 0, GAS_ANGLE * 15.0/13.2, 360 - GAS_ANGLE * 15.0/13.2/2);


    lv_obj_set_style_text_color(meter, lv_color_black(), NULL);


    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 10, lv_color_change_lightness(lv_palette_main(LV_PALETTE_AMBER), 20), -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 132);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, 0);

    /*Add a three arc indicator*/
    lv_meter_indicator_t * indic1 = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_AMBER), -1);
    lv_meter_set_indicator_start_value(meter, indic1, 120);
    lv_meter_set_indicator_end_value(meter, indic1, 0);
    // indic1->start_value = 0;
    // indic1->end_value = 50;

//     lv_anim_set_time(&a, 2000);
//     lv_anim_set_playback_time(&a, 500);
//     lv_anim_set_var(&a, indic1);
//     lv_anim_start(&a);
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

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 81, 1, 12, lv_color_black());
    lv_meter_set_scale_major_ticks(meter, scale, 10, 5, 12, lv_color_black(), 20);
    lv_meter_set_scale_range(meter, scale, 0, 8000, 270, 135);

    lv_obj_set_style_text_color(meter, lv_color_white(), NULL);
    lv_obj_set_style_text_font(meter, &lv_font_montserrat_8, NULL);

    lv_meter_indicator_t * rpm_normal_bg = lv_meter_add_arc(meter, scale, 10, lv_color_change_lightness(lv_palette_main(LV_PALETTE_AMBER), 15), -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal_bg, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal_bg, REDLINE);

    rpm_normal = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_AMBER), -1);
    lv_meter_set_indicator_start_value(meter, rpm_normal, 0);
    lv_meter_set_indicator_end_value(meter, rpm_normal, REDLINE);

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

    lv_meter_indicator_t * rpm_red_bg = lv_meter_add_arc(meter, scale, 10, lv_color_change_lightness(lv_palette_main(LV_PALETTE_RED), 15), -1);
    lv_meter_set_indicator_start_value(meter, rpm_red_bg, REDLINE);
    lv_meter_set_indicator_end_value(meter, rpm_red_bg, 8000);

    /*Add a red arc to the end*/
    rpm_red = lv_meter_add_arc(meter, scale, 10, lv_palette_main(LV_PALETTE_RED), -1);
    lv_meter_set_indicator_start_value(meter, rpm_red, REDLINE);
    lv_meter_set_indicator_end_value(meter, rpm_red, 8000);

}

// brake


{

    lv_obj_t * btn2 = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn2, 140, 40);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 70);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_style_pad_all(btn2, 5, 0);

    // lv_obj_remove_style_all(btn2);

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_bg_color(&style, lv_color_change_lightness(lv_palette_main(LV_PALETTE_RED), 20));
    lv_style_set_outline_width(&style, 0);
    lv_style_set_border_width(&style, 0);
    lv_style_set_shadow_width(&style, 0);
    // lv_obj_add_style(btn2, &style, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_style(btn2, &style, LV_STATE_FOCUS_KEY);

    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_bg_color(&style_pr, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_outline_width(&style_pr, 0);
    lv_style_set_border_width(&style_pr, 0);
    lv_style_set_shadow_width(&style_pr, 0);
    lv_obj_add_style(btn2, &style_pr, LV_STATE_FOCUS_KEY | LV_STATE_CHECKED);
    // lv_obj_add_style(btn2, &style_pr, LV_PART_MAIN | LV_STATE_PRESSED);

    lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    lv_obj_t * label = lv_label_create(btn2);
    lv_obj_set_style_text_color(label, lv_color_black(), NULL);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_letter_space(label, 1, 0);
    lv_label_set_text(label, "BRAKE");
    lv_obj_center(label);

}
// // gas
// {

//     static lv_style_t style_indic;

//     lv_style_init(&style_indic);
//     lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
//     lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_AMBER));
//     // lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
//     // lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

//     lv_obj_t * bar = lv_bar_create(canvas);
//     lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
//     lv_obj_set_size(bar, 20, 200);
//     lv_obj_set_style_translate_x(bar, 600, 0);
//     lv_obj_set_style_translate_y(bar, 200, 0);
//     lv_bar_set_range(bar, -20, 40);

//     lv_anim_t a;
//     lv_anim_init(&a);
//     lv_anim_set_exec_cb(&a, set_temp);
//     lv_anim_set_time(&a, 3000);
//     lv_anim_set_playback_time(&a, 3000);
//     lv_anim_set_var(&a, bar);
//     lv_anim_set_values(&a, -20, 40);
//     lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
//     lv_anim_start(&a);

// }

// misc text

{

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_palette_main(LV_PALETTE_AMBER);
    label_dsc.font = &lv_font_montserrat_16;

    lv_canvas_draw_text(canvas, 100, 300, 300, &label_dsc, "Goin fast");
}

// {

//     lv_draw_label_dsc_t label_dsc;
//     lv_draw_label_dsc_init(&label_dsc);
//     label_dsc.color = lv_palette_main(LV_PALETTE_AMBER);
//     label_dsc.font = &lv_font_montserrat_16;

//     lv_canvas_draw_text(canvas, 700, 100, 100, &label_dsc, "Mini Cooper");
// }

// speed
{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, lv_palette_main(LV_PALETTE_AMBER));
    lv_style_set_text_font(&style, &lcddot);

    lv_obj_t * main_label = lv_label_create(canvas);
    lv_label_set_text(main_label, "50");
    lv_obj_add_style(main_label, &style, 0);

    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, 0);
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
    img.header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
    img.header.w = CANVAS_WIDTH;
    img.header.h = CANVAS_HEIGHT;

    effect1 = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(effect1, ebuf1, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_ALPHA);
    lv_obj_set_style_bg_color(effect1, lv_color_black(), 0);    
    // lv_obj_move_background(effect1);
    lv_obj_center(effect1);
    lv_obj_set_style_opa(effect1, LV_OPA_20, 0);
    lv_obj_set_style_blend_mode(effect1, LV_BLEND_MODE_ADDITIVE, 0);


    effect2 = lv_canvas_create(lv_scr_act());
    lv_canvas_set_buffer(effect2, ebuf2, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR_ALPHA);
    lv_obj_set_style_bg_color(effect2, lv_color_black(), 0);    
    // lv_obj_move_background(effect2);
    lv_obj_center(effect2);
    lv_obj_set_style_opa(effect2, LV_OPA_20, 0);
    lv_obj_set_style_blend_mode(effect2, LV_BLEND_MODE_ADDITIVE, 0);
}

void update_effect() {
    if (!effect1) {
        setup_effect();
    }
    lv_snapshot_take_to_buf(canvas, LV_IMG_CF_TRUE_COLOR_ALPHA, &img, &cbuf_tmp, sizeof(cbuf_tmp));
 
    // lv_canvas_transform(effect1, &img, 0, 260, (260.0/256.0 - 1) * CANVAS_WIDTH / 2, (260.0/256.0 - 1) * CANVAS_HEIGHT / 2, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);
    lv_canvas_transform(effect1, &img, 0, 260, 0, 0, CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, true);

    lv_canvas_blur_hor(effect1, NULL, 3);
    lv_canvas_blur_ver(effect1, NULL, 3);
    // lv_canvas_blur_hor(effect1, NULL, 2);
    // lv_canvas_blur_ver(effect1, NULL, 2);
    // lv_canvas_blur_hor(effect1, NULL, 2);
    // lv_canvas_blur_ver(effect1, NULL, 2);
    lv_snapshot_take_to_buf(effect1, LV_IMG_CF_TRUE_COLOR_ALPHA, &img, &cbuf_tmp, sizeof(cbuf_tmp));

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