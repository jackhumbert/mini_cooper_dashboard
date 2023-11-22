#include "widget.hpp"

#define ODO_SEGMENTS 10

class Odometer : public Widget {
public:
    Odometer(lv_obj_t * parent);
    virtual void update(void) override;

    lv_obj_t * numbers[ODO_SEGMENTS];
    lv_obj_t * segments;
};