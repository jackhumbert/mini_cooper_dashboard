#include "widget.hpp"
#include <new>
#include <iostream>

static const lv_obj_class_t lv_class = {
    .base_class = &lv_obj_class,
    .constructor_cb = &Widget::construct_cb,
    .destructor_cb = &Widget::destruct_cb,
    .event_cb = &Widget::event_cb,
    .width_def = LV_SIZE_CONTENT,
    .height_def = LV_SIZE_CONTENT,
    .group_def = LV_OBJ_CLASS_GROUP_DEF_TRUE,
    .instance_size = sizeof(Widget)
};

Widget::Widget() {

}

Widget * Widget::create(lv_obj_t * parent) {
    LV_TRACE_OBJ_CREATE("Creating object with %p class on %p parent", (void *)&lv_class, (void *)parent);
    uint32_t s = sizeof(Widget);
    Widget * obj = (Widget *)lv_mem_alloc(s);
    if(obj == NULL) return NULL;
    lv_memset_00(obj, s);

    new (obj) Widget(parent);
    return obj;
}

Widget::Widget(lv_obj_t * parent) {
    this->parent = parent;
    this->class_p = &lv_class;
    if (parent == NULL) {
        LV_TRACE_OBJ_CREATE("creating a screen");
        lv_disp_t * disp = lv_disp_get_default();
        if(!disp) {
            LV_LOG_WARN("No display created yet. No place to assign the new screen");
            lv_mem_free(this);
            return;
        }

        if(disp->screens == NULL) {
            disp->screens = (_lv_obj_t**)lv_mem_alloc(sizeof(lv_obj_t *));
            disp->screens[0] = this;
            disp->screen_cnt = 1;
        }
        else {
            disp->screen_cnt++;
            disp->screens = (_lv_obj_t**)lv_mem_realloc(disp->screens, sizeof(lv_obj_t *) * disp->screen_cnt);
            disp->screens[disp->screen_cnt - 1] = this;
        }

        /*Set coordinates to full screen size*/
        this->coords.x1 = 0;
        this->coords.y1 = 0;
        this->coords.x2 = lv_disp_get_hor_res(NULL) - 1;
        this->coords.y2 = lv_disp_get_ver_res(NULL) - 1;
    } else {
        LV_TRACE_OBJ_CREATE("creating normal object");
        LV_ASSERT_OBJ(parent, MY_CLASS);
        if (parent->spec_attr == NULL) {
            lv_obj_allocate_spec_attr(parent);
        }

        if (parent->spec_attr->children == NULL) {
            parent->spec_attr->children = (_lv_obj_t**)lv_mem_alloc(sizeof(lv_obj_t *));
            parent->spec_attr->children[0] = this;
            parent->spec_attr->child_cnt = 1;
        } else {
            parent->spec_attr->child_cnt++;
            parent->spec_attr->children = (_lv_obj_t**)lv_mem_realloc(parent->spec_attr->children,
                                                            sizeof(lv_obj_t *) * parent->spec_attr->child_cnt);
            parent->spec_attr->children[parent->spec_attr->child_cnt - 1] = this;
        }
    }
}

void Widget::construct(const lv_obj_class_t * cls) {
    
}

void Widget::destruct(const lv_obj_class_t * cls) {

}

void Widget::event(const lv_obj_class_t * cls, lv_event_t * event) {

}

void Widget::update(void) {
    
}


void Widget::construct_cb(const lv_obj_class_t * cls, lv_obj_t * obj) {
    auto widget = (Widget*)obj;
    widget->construct(cls);
}

void Widget::destruct_cb(const lv_obj_class_t * cls, lv_obj_t * obj) {
    auto widget = (Widget*)obj;
    widget->destruct(cls);
}

void Widget::event_cb(const lv_obj_class_t * cls, lv_event_t * event) {
    auto obj = (Widget*)lv_event_get_target(event);
    obj->event(cls, event);
}

Widget::~Widget() {
    if (lv_obj) {
        lv_obj_del(lv_obj);
        lv_obj = NULL;
    }
    lv_obj_del(this);
}