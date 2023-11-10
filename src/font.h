#include <stddef.h>
#include <lvgl.h>
#include <tiny_ttf.h>

#define RAJDHANI_MEDIUM RAJDHANI_MEDIUM
#define RAJDHANI_REGULAR RAJDHANI_REGULAR
#define RAJDHANI_SEMIBOLD RAJDHANI_SEMIBOLD
#define FIRACODE_REGULAR FIRACODE_REGULAR

#define DASH_FONT(name, size) \
    extern const unsigned char name##_TTF[]; \
    extern const size_t name##_TTF_SIZE; \
    static lv_font_t * name##_##size; \
    name##_##size = tiny_ttf_create_data(name##_TTF, name##_TTF_SIZE, size * 1.26)
