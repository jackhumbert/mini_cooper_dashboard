#pragma once
#include "dash.h"
#include <stdint.h>

class Widget {
public:
    Widget(lv_obj_t * parent, uint8_t * _update_bit);
    virtual void update(void) = 0;

    lv_obj_t * lv_obj;
    uint8_t * update_bit;
};