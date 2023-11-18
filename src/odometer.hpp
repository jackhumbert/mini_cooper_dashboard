#include "widget.hpp"

class Odometer : public Widget {
public:
    Odometer(lv_obj_t * parent);
    virtual void update(void) override;
};