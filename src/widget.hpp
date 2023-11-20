#pragma once
#include "dash.h"
#include <stdint.h>

class Widget : public lv_obj_t {
public:
    Widget();
    Widget(lv_obj_t * parent);

    virtual ~Widget();
    virtual void update(void);
    virtual void construct(const lv_obj_class_t * cls);
    virtual void destruct(const lv_obj_class_t * cls);
    virtual void event(const lv_obj_class_t * cls, lv_event_t * event);

    static Widget * create(lv_obj_t * parent);
    static void construct_cb(const lv_obj_class_t * cls, lv_obj_t * obj);
    static void destruct_cb(const lv_obj_class_t * cls, lv_obj_t * obj);
    static void event_cb(const lv_obj_class_t * cls, lv_event_t * event);

    lv_obj_t * lv_obj;
};