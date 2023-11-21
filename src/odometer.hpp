#include "widget.hpp"

class Odometer : public Widget {
public:
    Odometer(lv_obj_t * parent);
    virtual void update(void) override;

    lv_obj_t * numbers[8];
    lv_obj_t * segments;
};