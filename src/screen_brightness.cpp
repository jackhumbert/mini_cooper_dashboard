#include "screen_brightness.hpp"
#if ESP32
    #include <Wire.h>
#endif
#define SCREEN_FADE_TIME 1

#ifdef ESP32
    #include "screen.h"

    extern LGFX gfx;
    TaskHandle_t screen_fade_task;
#endif

void ScreenBrightness::update(void) {
    if (get_queued()->running_lights || get_queued()->interior_light_level) {
        // uint8_t end_value = get_cache()->running_lights ? get_cache()->interior_light_level * 4 : 255;
        // gfx.setBrightness(end_value);
        fade_in();
    }
}

static void screen_fade_cb(void * parameter) {

#ifdef ESP32
    unsigned long start = millis();
    uint8_t start_value = gfx.getBrightness();
    uint8_t end_value = get_cache()->running_lights ? get_cache()->interior_light_level * 4 : 255;
    while (millis() - start < (SCREEN_FADE_TIME * 1000)) {
        unsigned long time_diff = millis() - start;
        // uint32_t value = round(pow(time_diff / (SCREEN_FADE_TIME * 1000.0), 2) * (end_value - start_value) + start_value);
        uint32_t value = round(time_diff / (SCREEN_FADE_TIME * 1000.0) * (end_value - start_value) + start_value);
        // add_message_fmt("brightness: %d", value);
        gfx.setBrightness(value);
        vTaskDelay(1);
    }
    gfx.setBrightness(end_value);
    screen_fade_task = NULL;
    vTaskDelete(NULL);
#endif
};

void ScreenBrightness::fade_in(void) {
#ifdef ESP32
    if (screen_fade_task != NULL) {
        vTaskDelete(screen_fade_task);
        screen_fade_task = NULL;
    } else {
        xTaskCreatePinnedToCore(
            screen_fade_cb, /* Function to implement the task */
            "Screen Fade In", /* Name of the task */
            1000,  /* Stack size in words */
            NULL,  /* Task input parameter */
            2,  /* Priority of the task */
            &screen_fade_task,  /* Task handle. */
            0); /* Core where the task should run */
    }
#endif
}