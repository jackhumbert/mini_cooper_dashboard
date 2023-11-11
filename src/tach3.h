#include "lvgl.h"

lv_obj_t * tach3_create(lv_obj_t * canvas);
void tach3_set(int32_t tach);

#define REDLINE 6750
#define TORQUE_MIN 3250