#include "widget.hpp"

class CarView : public Widget {
public:
    CarView(lv_obj_t * parent);
    virtual void update(void) override;

    lv_obj_t * car_view_obj;
    lv_obj_t * hood_obj;
    lv_obj_t * trunk_obj;
    lv_obj_t * headlights_obj;
    lv_obj_t * sunroof_obj;
    lv_obj_t * door_left_obj;
    lv_obj_t * door_right_obj;
    lv_obj_t * gas_cap_obj;
    lv_obj_t * foglights_obj;
    
    lv_obj_t * handbrake;

    lv_obj_t * abs_obj;
    lv_obj_t * spinny_obj;
    lv_obj_t * tire_obj;
    lv_obj_t * check_engine_obj;
    lv_obj_t * eml_obj;
    lv_obj_t * battery_obj;
};