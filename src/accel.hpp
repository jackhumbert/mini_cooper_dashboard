#include "widget.hpp"

class Accel : public Widget {
public:
    Accel(lv_obj_t * parent);
    virtual void update(void) override;

    lv_obj_t * center;
};