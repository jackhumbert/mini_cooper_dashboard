#include "lvgl.h"

lv_obj_t * tach3_create(lv_obj_t * canvas);
void tach3_update();

#define REDLINE 6750
#define TORQUE_MIN 3250