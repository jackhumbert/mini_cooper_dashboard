#include "widget.hpp"

class Wheel : public Widget {
public:
    Wheel(lv_obj_t * parent);
    virtual void update(void) override;
};