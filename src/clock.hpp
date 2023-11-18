#include "widget.hpp"

class Clock : public Widget {
public:
    Clock(lv_obj_t * parent);
    virtual void update(void) override;
};