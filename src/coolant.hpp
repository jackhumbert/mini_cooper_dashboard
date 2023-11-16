#include "widget.hpp"

class Coolant : public Widget {
public:
    Coolant(lv_obj_t * parent);
    virtual void update(void) override;

    lv_obj_t * meter;
    lv_meter_indicator_t * indicator;
    lv_obj_t * label;
};