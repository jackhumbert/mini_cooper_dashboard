#include "widget.hpp"

Widget::Widget() {

}

Widget::~Widget() {
    if (lv_obj) {
        lv_obj_del(lv_obj);
        lv_obj = NULL;
    }
}

// void Widget::update(void) {
//     if (*this->update_bit) {
//         // update
//     }
// }