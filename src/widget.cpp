#include "widget.hpp"

Widget::Widget(lv_obj_t * parent, uint8_t * _update_bit) {
    this->update_bit = _update_bit;
}

// void Widget::update(void) {
//     if (*this->update_bit) {
//         // update
//     }
// }