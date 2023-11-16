#include "widget.hpp"

class Gauge : public Widget {
public:
    Gauge(lv_obj_t * parent, const char * units);
    virtual void update(void) override;

    const char * units;
    lv_meter_indicator_t * indicator;
    lv_obj_t * label;
};