#include "widget.hpp"

#define REDLINE 6750
#define TORQUE_MIN 3250

class Tach5 : public Widget {
public:
    Tach5(lv_obj_t * parent);
    virtual void construct(const lv_obj_class_t * cls) override;
    virtual void destruct(const lv_obj_class_t * cls) override;
    virtual void event(const lv_obj_class_t * cls, lv_event_t * event) override;
    virtual void update(void) override;

    static Tach5 * create(lv_obj_t * parent);

    float start_angle;
    float end_angle;
};