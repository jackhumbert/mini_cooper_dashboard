#include "messages.h"
#include <stdio.h>
#include <stdarg.h>

#define MESSAGES_WIDTH 280
#define MESSAGES_HEIGHT 240
#define MESSAGE_LIFETIME 30000
#define MESSAGE_FADE_OUT 300


static lv_obj_t * messages;

lv_obj_t * messages_create(lv_obj_t * parent) {

    lv_obj_t * messages_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(messages_cont);
    lv_obj_set_size(messages_cont, MESSAGES_WIDTH, MESSAGES_HEIGHT);
    lv_obj_set_scroll_dir(messages_cont, LV_DIR_NONE);

{
    DASH_FONT(RAJDHANI_SEMIBOLD, 14);

    lv_obj_t * messages_label = lv_label_create(messages_cont);
    lv_obj_align(messages_label, LV_ALIGN_TOP_MID, 0, 0);
    lv_label_set_text(messages_label, "MESSAGES");
    lv_obj_set_style_text_font(messages_label, RAJDHANI_SEMIBOLD_14, 0);
    lv_obj_set_style_text_color(messages_label, AMBER_HALF, 0);
    lv_obj_set_width(messages_label, MESSAGES_WIDTH);
}

{
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_border_side(&style, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_border_opa(&style, LV_OPA_COVER);
    lv_style_set_border_color(&style, AMBER_HALF);
    lv_style_set_border_width(&style, 1);

    lv_obj_t * line = lv_obj_create(messages_cont);
    lv_obj_remove_style_all(line);
    lv_obj_set_size(line, MESSAGES_WIDTH, 18);
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_style(line, &style, 0);
}

    messages = lv_obj_create(messages_cont);
    lv_obj_remove_style_all(messages);
    lv_obj_set_size(messages, MESSAGES_WIDTH, MESSAGES_HEIGHT - 24);
    lv_obj_align(messages, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_flex_flow(messages, LV_FLEX_FLOW_COLUMN_REVERSE);
    lv_obj_add_flag(messages, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_set_scrollbar_mode(messages, LV_SCROLLBAR_MODE_ON);
    lv_obj_set_scroll_dir(messages, LV_DIR_VER);

    add_message("System initialized");

    return messages_cont;
}

void add_message(const char * message) {
    // LV_LOG_USER(message);

    DASH_FONT(FIRACODE_REGULAR, 12);

    lv_obj_t * messages_label = lv_label_create(messages);
    lv_label_set_text(messages_label, message);
    lv_obj_set_style_text_font(messages_label, FIRACODE_REGULAR_12, 0);
    lv_obj_set_style_text_color(messages_label, AMBER_ON, 0);
    lv_obj_set_width(messages_label, MESSAGES_WIDTH);

    lv_obj_fade_out(messages_label, MESSAGE_FADE_OUT, MESSAGE_LIFETIME);
    lv_obj_del_delayed(messages_label, MESSAGE_LIFETIME + MESSAGE_FADE_OUT);
}

void add_message_fmt(char *format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, 255, format, args);

    add_message(buffer);

    va_end (args);
}