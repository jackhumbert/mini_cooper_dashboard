#include "widget.hpp"

#define REDLINE 6750
#define TORQUE_MIN 3250

class Tach4 : public Widget {
public:
    Tach4(lv_obj_t * parent);
    virtual void update(void) override;
};