#include "widget.hpp"

class OilTemp : public Widget {
public:
    OilTemp(lv_obj_t * parent);
    virtual void update(void) override;

    lv_meter_indicator_t * background;
    lv_meter_indicator_t * indicator;
    lv_obj_t * label;
};