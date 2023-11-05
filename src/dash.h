#include "lvgl/lvgl.h"

#define CANVAS_WIDTH  800
#define CANVAS_HEIGHT 480

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

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

lv_obj_t * dash(void);