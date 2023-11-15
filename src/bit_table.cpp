#include "bit_table.hpp"

void BitTable::update(void) {
    if (*update_bit) {
        for (uint64_t i = 0; i < 64; i++) {
            bool active = *data & (1ULL << i);
            lv_obj_set_style_bg_color(bit_objs[i], active ? AMBER_ON : AMBER_OFF, 0);
        }
    }
}

BitTable::BitTable(lv_obj_t * parent, uint8_t * update_bit, uint32_t id, uint64_t * data) : 
    Widget(parent, update_bit), 
    id(id), 
    data(data)
{
    static lv_coord_t col_dsc[] = { 20, 20, 20, 20, 20, 20, 20, 20, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, LV_GRID_TEMPLATE_LAST};

    /*Create a container with grid*/
    lv_obj = lv_obj_create(parent);
    lv_obj_remove_style_all(lv_obj);
    lv_obj_set_style_grid_column_dsc_array(lv_obj, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(lv_obj, row_dsc, 0);
    lv_obj_set_size(lv_obj, 20 * 8, 20 * 9);
    lv_obj_set_layout(lv_obj, LV_LAYOUT_GRID);
    lv_obj_set_style_bg_color(lv_obj, AMBER_OFF, 0);

    lv_obj_t * label;

    label = lv_label_create(lv_obj);
    lv_obj_set_style_text_color(label, AMBER_ON, 0);
    lv_label_set_text_fmt(label, "0x%X", id);
    lv_obj_set_grid_cell(label, LV_GRID_ALIGN_STRETCH, 0, 8,
                             LV_GRID_ALIGN_STRETCH, 0, 1);

    uint8_t x, y;
    for (x = 0; x < 8; x++) {
        for (y = 0; y < 8; y++) {
            bit_objs[x + y * 8] = lv_obj_create(lv_obj);
            // lv_obj_remove_style_all(bit_objs[x + y * 8]);
            lv_obj_set_style_pad_all(bit_objs[x + y * 8], 0, 0);
            lv_obj_set_grid_cell(bit_objs[x + y * 8], LV_GRID_ALIGN_STRETCH, 7 - x, 1,
                             LV_GRID_ALIGN_STRETCH, y + 1, 1);
            lv_obj_set_style_bg_color(bit_objs[x + y * 8], AMBER_OFF, 0);
            lv_obj_set_style_bg_opa(bit_objs[x + y * 8], LV_OPA_100, 0);
            lv_obj_set_style_border_width(bit_objs[x + y * 8], 0, 0);
            lv_obj_set_style_radius(bit_objs[x + y * 8], 0, 0);

            label = lv_label_create(bit_objs[x + y * 8]);
            lv_label_set_text_fmt(label, "%d", x + y * 8);
            lv_obj_center(label);
        }
    }
}