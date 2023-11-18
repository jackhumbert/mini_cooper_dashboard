#include "widget.hpp"

class Speed : public Widget {
public:
    Speed(lv_obj_t * parent);
    virtual void update(void) override;

    int rounded_speed = 0;
    lv_obj_t * cruise_label;
    lv_obj_t * speed_label;
    lv_obj_t * speed_label2;
};