#include "widget.hpp"

#define REDLINE 6750
#define TORQUE_MIN 3250

class Tach : public Widget {
public:
    Tach(lv_obj_t * parent);
    virtual void update(void) override;
    
    lv_meter_indicator_t * indicator;
    lv_obj_t * label;
    lv_obj_t * rotated_meter;
};