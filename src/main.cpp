#define _DEFAULT_SOURCE /* needed for usleep() */
#include "lvgl.h"

// DISPLAY

#define DISPLAY_BUFFER_SIZE 800 * 480 / 4
// #define DISPLAY_BUFFER_SIZE (unsigned int)((800 * 480) * 0.40)

static lv_disp_draw_buf_t draw_buf;
static lv_color_t disp_draw_buf[DISPLAY_BUFFER_SIZE];
static lv_color_t * disp_draw_buf2 = NULL;
// static lv_color_t disp_draw_buf2[DISPLAY_BUFFER_SIZE];

#include <stdlib.h>
#include <unistd.h>
#include "dash.h"
#include "messages.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include "touch.h"
#include "sd_card.h"
#include "sd_card.hpp"
#include "can.h"
#include "serial_can.h"
#include "activity.h"

Serial_CAN can;

// const float pixel_pitch_x = 0.0641 * 3;
// const float pixel_pitch_y = 0.1784;

// const float pixel_y_correction = pixel_pitch_x / pixel_pitch_y;
// const float pixel_x_correction = pixel_pitch_y / pixel_pitch_x;

static uint32_t screenWidth;
static uint32_t screenHeight;
// static lv_disp_draw_buf_t draw_buf;
// static lv_color_t disp_draw_buf[800 * 480 / 10];
// static lv_disp_drv_t disp_drv;

#include "screen.h"
LGFX gfx;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint16_t c;

    if (gfx.getStartCount() == 0)
    {   
        gfx.startWrite();
    }
    gfx.pushImageDMA(area->x1, area->y1, area->x2 - area->x1 + 1, area->y2 - area->y1 + 1, (lgfx::rgb565_t*)&color_p->full);
    lv_disp_flush_ready(disp);
}

void my_log_cb(const char * buf) {
    // add_message(buf);
    sd_card_logf("%08.3f CXX %s\n", xTaskGetTickCount() / 1000.0, buf);
}


// extern void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);

void my_touchpad_read_new( lv_indev_drv_t * indev_driver, lv_indev_data_t * data ) {
    uint16_t touchX, touchY;

    data->state = LV_INDEV_STATE_REL;

    if( gfx.getTouch( &touchX, &touchY ) )
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = touchX;
        data->point.y = touchY;
    }
}

int sd_card_log_printf(const char* format, va_list args) {
    char buffer[256];
    vsnprintf(buffer, 255, format, args);

    // add_message(buffer);
    sd_card_logf("%08.3f CXX %s\n", xTaskGetTickCount() / 1000.0, buffer);

    va_end(args);

    return 0;
}

TaskHandle_t CAN_Task;

void CAN_Task_loop(void * parameter) {
    while (true) {
        int8_t messages = Serial.available() / 12;
        while (messages-- && Serial.available()) {
        // while (Serial.available()) {
            unsigned long id = 0;
            uchar buf[13] = {0};
            if (can.recv(&id, buf)) {
                // add_message_fmt("Message from %X", id);
                unsigned long can_id = id & 0xFFFF;
                dbcc_time_stamp_t ts = (id >> 16) & 0xFFFF;

                // static char sbuf[35];
                // sprintf(sbuf, "%04X %04X: %02X %02X %02X %02X %02X %02X %02X %02X\0", ts, can_id, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);

                // static char log[35];
                // sprintf(log, "%04X %04X: %02X %02X %02X %02X %02X %02X %02X %02X\n", ts, can_id, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
                // File file = SD.open("/messages.log", FILE_APPEND);
                // if (file) {
                //     file.write((uint8_t*)log, 35);
                //     file.close();
                // }

                sd_card_logf("%08.3f R11 %08X ", xTaskGetTickCount() / 1000.0, id);
                for (int i = 0; i < 8; i++) {
                    sd_card_logf("%02X ", buf[i]);
                }
                sd_card_logf("\n");

                // add_message(sbuf);

	            pthread_mutex_lock(&get_dash()->mutex);
                if (decode_can_message(ts, can_id, (uint8_t*)buf) < 0) {
                    activity_update(false);
                    // if (id == 0x78300D0A) {
                    //     add_message((char*)buf);
                    // }

                    sd_card_logf("%08.3f CXX Could not decode latest message\n", xTaskGetTickCount() / 1000.0);

                } else {
                    activity_update(true);
                }
	            pthread_mutex_unlock(&get_dash()->mutex);
            } else {
                // if (buf[12]) {
                //     File * file = sd_card_get_log_file();
                //     if (file && *file) {
                //         uint32_t ticks = xTaskGetTickCount();
                //         sd_card_logf("[%08X] ", ticks);
                //         for (int i = 0; i < buf[12]; i++) {
                //             sd_card_logf("%02X ", buf[i]);
                //         }
                //         file->print('\n');
                //         file->flush();
                //     }
                // }
            }
        }
        // }
        // delay(5);
    }
}

#include <Wifi.h>
const char* ssid     = "Samuel";
const char* password = "8zciwf256cb6";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;


static void screen_fade_cb(void * indic, int32_t v) {
    gfx.setBrightness(v);
};

void setup() {
    // Connect to Wi-Fi
    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(100);
    // }
    
    // // Init and get the time
    // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // //disconnect WiFi as it's no longer needed
    // WiFi.disconnect(true);
    // WiFi.mode(WIFI_OFF);

    esp_log_set_vprintf(sd_card_log_printf);

    Serial.setRxBufferSize(4096);
    Serial.begin(115200); 

    gfx.init();
    gfx.setRotation(0);
    gfx.setBrightness(0);
    gfx.setColorDepth(16); 
    gfx.clearDisplay();
    // gfx.begin();

    lv_init();
    lv_log_register_print_cb(my_log_cb);

    screenWidth = gfx.width();
    screenHeight = gfx.height();

    // lv_color_t * disp_draw_buf = (lv_color_t *)heap_caps_malloc(DISPLAY_BUFFER_SIZE * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    // lv_color_t * disp_draw_buf = (lv_color_t *)ps_malloc(DISPLAY_BUFFER_SIZE * sizeof(lv_color_t));
    // static lv_color_t * disp_draw_buf = (lv_color_t *)ps_malloc(sizeof(lv_color_t) * DISPLAY_BUFFER_SIZE);
    // lv_color_t * disp_draw_buf2 = (lv_color_t *)ps_malloc(sizeof(lv_color_t) * CANVAS_WIDTH * CANVAS_HEIGHT);
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, disp_draw_buf2, DISPLAY_BUFFER_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;

    lv_disp_t * disp = lv_disp_drv_register(&disp_drv);

    DASH_FONT(RAJDHANI_MEDIUM, 12);

    lv_theme_t * th = lv_theme_default_init(disp, AMBER_ON, RED_ON, true, RAJDHANI_MEDIUM_12);
    lv_disp_set_theme(disp, th);

    lv_group_t * g = lv_group_create();
    lv_group_set_default(g);

    // touch_init();
    // /* Initialize the (dummy) input device driver */
    // static lv_indev_drv_t indev_drv;
    // lv_indev_drv_init(&indev_drv);
    // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // indev_drv.read_cb = my_touchpad_read;
    // lv_indev_drv_register(&indev_drv);

    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read_new;
    lv_indev_drv_register(&indev_drv);

    dash();
	pthread_mutex_init(&get_dash()->mutex, NULL);
    sd_card_init();

    // delay(500);

    can.begin(Serial, 115200);
    // really only need once - it's store in the eeprom
    // can.baudRate(SERIAL_RATE_115200);

    xTaskCreatePinnedToCore(
      CAN_Task_loop, /* Function to implement the task */
      "CAN Task", /* Name of the task */
      10000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      0,  /* Priority of the task */
      &CAN_Task,  /* Task handle. */
      0); /* Core where the task should run */
}

// uint32_t timer = 0;

static bool screen_on = false;

void loop() {
    if (!screen_on) {
        static lv_anim_t screen_fade;
        lv_anim_init(&screen_fade);
        lv_anim_set_exec_cb(&screen_fade, screen_fade_cb);
        lv_anim_set_values(&screen_fade, 0, 255);
        lv_anim_set_time(&screen_fade, 2000);
        lv_anim_start(&screen_fade);
        screen_on = true;
    }

    // lv_refr_now();
    lv_timer_handler();

    // if (xTaskGetTickCount() - timer > 10000) {
    //     add_message_fmt("%d bytes free", esp_get_free_heap_size());
    //     timer = xTaskGetTickCount();
    // }

    dash_loop();
}