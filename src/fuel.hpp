#include "widget.hpp"

// #define TANK_SIZE_GALLONS 13.2
#define TANK_SIZE_GALLONS 14

class Fuel : public Widget {
public:
    Fuel(lv_obj_t * parent);
    virtual void update(void) override;

    lv_obj_t * meter;
    lv_meter_indicator_t * background;
    lv_meter_indicator_t * indicator;
    lv_obj_t * label;
};