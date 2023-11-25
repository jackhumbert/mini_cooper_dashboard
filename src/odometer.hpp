#include "widget.hpp"

#define ODO_SEGMENTS 10

class Odometer : public Widget {
public:
    Odometer(lv_obj_t * parent, bool trip, uint8_t decimals);
    virtual void update(void) override;

    bool trip;
    uint8_t decimals;
    uint64_t last_decimiles = 0;
    lv_obj_t * numbers[ODO_SEGMENTS];
    lv_obj_t * segments;
};