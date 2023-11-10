#include "lvgl.h"
#include "font.h"

#define CANVAS_WIDTH  800
#define CANVAS_HEIGHT 480

#define AMBER_ON lv_color_hsv_to_rgb(25, 100, 100)
#define AMBER_HALF lv_color_hsv_to_rgb(24, 100, 50)
#define AMBER_OFF lv_color_hsv_to_rgb(20, 100, 8)

#define RED_ON lv_color_hsv_to_rgb(0, 100, 100)
#define RED_HALF lv_color_hsv_to_rgb(2, 100, 50)
#define RED_OFF lv_color_hsv_to_rgb(5, 100, 8)

#define BLUE_ON lv_color_hsv_to_rgb(200, 100, 100)
#define BLUE_HALF lv_color_hsv_to_rgb(190, 100, 50)
#define BLUE_OFF lv_color_hsv_to_rgb(185, 100, 8)

#define GREEN_ON lv_color_hsv_to_rgb(120, 100, 100)
#define GREEN_HALF lv_color_hsv_to_rgb(115, 100, 50)
#define GREEN_OFF lv_color_hsv_to_rgb(110, 100, 8)

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#if DASH_SIMULATION
    #include <stdlib.h>
    #define ps_malloc malloc
#else
    #include <esp32-hal-psram.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

lv_obj_t * dash(void);
void dash_loop(void);

#ifdef __cplusplus
} // extern "C"
#endif