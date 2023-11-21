#include "screen_brightness.hpp"
#include <Wire.h>
#include "screen.h"

#define SCREEN_FADE_TIME 1

extern LGFX gfx;
TaskHandle_t screen_fade_task;

void ScreenBrightness::update(void) {
    if (get_queued()->running_lights) {
        uint8_t end_value = get_cache()->running_lights ? get_cache()->interior_light_level * 4 : 255;
        gfx.setBrightness(end_value);
    }
}

static void screen_fade_cb(void * parameter) {
    unsigned long start = millis();
    uint8_t start_value = gfx.getBrightness();
    uint8_t end_value = get_cache()->running_lights ? get_cache()->interior_light_level * 4 : 255;
    while (millis() - start < (SCREEN_FADE_TIME * 1000)) {
        unsigned long time_diff = millis() - start;
        uint32_t value = round(pow(time_diff / (SCREEN_FADE_TIME * 1000.0), 2) * (end_value - start_value) + start_value);
        // uint32_t value = round(time_diff / (SCREEN_FADE_TIME * 1000.0) * (get_dash()->lights ? 10 : 255));
        // add_message_fmt("brightness: %d", value);
        gfx.setBrightness(value);
    }
    gfx.setBrightness(end_value);
    vTaskDelete(NULL);
};

void ScreenBrightness::fade_in(void) {
    xTaskCreatePinnedToCore(
      screen_fade_cb, /* Function to implement the task */
      "Screen Fade In", /* Name of the task */
      1000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      2,  /* Priority of the task */
      &screen_fade_task,  /* Task handle. */
      0); /* Core where the task should run */
}