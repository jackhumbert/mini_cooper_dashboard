#pragma once
#include "widget.hpp"

class BitTable : public Widget {
public:
    BitTable(lv_obj_t * parent, uint8_t * update_bit, uint32_t id, uint64_t * data);
    virtual void update(void) override;

    uint32_t id;
    uint64_t * data;
    lv_obj_t * bit_objs[64];
};