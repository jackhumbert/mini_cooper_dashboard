#pragma once
#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void theme_init();
void apply_day_theme();

extern lv_style_t dash_style_value;
extern lv_style_t dash_style_label;

#ifdef __cplusplus
} // extern "C"
#endif
