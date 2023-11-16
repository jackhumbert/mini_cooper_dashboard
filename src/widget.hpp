#pragma once
#include "dash.h"
#include <stdint.h>

class Widget {
public:
    Widget();
    virtual void update(void) = 0;

    lv_obj_t * lv_obj;
};