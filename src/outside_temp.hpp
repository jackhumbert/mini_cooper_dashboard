#include "widget.hpp"

class OutsideTemp : public Widget {
public:
    OutsideTemp(lv_obj_t * parent);
    virtual void update(void) override;
};