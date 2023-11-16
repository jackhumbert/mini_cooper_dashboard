#include "widget.hpp"

class Speed : public Widget {
public:
    Speed(lv_obj_t * parent);
    virtual void update(void) override;

    lv_obj_t * cruise_label;
};