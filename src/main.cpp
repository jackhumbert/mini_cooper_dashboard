#define _DEFAULT_SOURCE /* needed for usleep() */
#include "lvgl.h"

// DISPLAY

#define DISPLAY_BUFFER_SIZE 600 * 160
// #define DISPLAY_BUFFER_SIZE (unsigned int)((800 * 480) * 0.40)

static lv_disp_draw_buf_t draw_buf;
DMA_ATTR static lv_color_t disp_draw_buf[DISPLAY_BUFFER_SIZE];
static lv_color_t * disp_draw_buf2 = NULL;
// static lv_color_t disp_draw_buf2[DISPLAY_BUFFER_SIZE];

#include <stdlib.h>
#include <unistd.h>
#include "dash.h"
#include "messages.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "sd_card.h"
#include "can.h"
#include "serial_can.h"
#include "screen_brightness.hpp"

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
#include <stdio.h>
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
    // sd_card_logf("%08.3f CXX %s\n", xTaskGetTickCount() / 1000.0, buf);
    Serial.println(buf);
}

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

static bool validate_id(uint32_t id) {
    return (id >= 0x153 && id <= 0xA00);
}

static bool filter_id(uint32_t id) {
   switch (id) {
        case 0x1F0:
        // case 0x1F8:
            return true;
    }
    return false;
}


uint32_t get_id(uint8_t * data) {
    uint32_t id = 0;
    
    for(int i=0; i<4; i++) {
        id <<= 8;
        id += data[i];
    }
    return id;
}

static uint8_t buffer[12 + 8];
static int hangers = 0;
TaskHandle_t CAN_Task;

void CAN_Task_loop(void * p) {
    while (true) {
        int avail = Serial.available();
        if (avail >= 12) {
            unsigned long timer_s = millis();
            Serial.readBytes(buffer, 12);
            uint32_t id = 0;
            uint8_t offset = 0;

            while(!validate_id(get_id(buffer + offset)) && offset < 8)
                offset++;
            
            if (offset) {
                while (Serial.available() < offset)
                    vTaskDelay(1);
                Serial.readBytes(buffer + 12, offset);
            }

            if (validate_id(get_id(buffer + offset))) {     
                id = get_id(buffer + offset);
                if (filter_id(id)) {
                    continue;
                }       
                process_packet(buffer + offset);
            } else {
                get_changed()->activity |= ACTIVITY_ERROR;
                sd_card_logf("%08.3f CER %08X %02X %02X %02X %02X %02X %02X %02X %02X\n", 
                    xTaskGetTickCount() / 1000.0, get_id(buffer + offset),
                    buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);
            }
        } // else {
        //     if (hangers == avail && avail != 0) {
        //         get_changed()->activity |= ACTIVITY_ERROR;
        //         Serial.readBytes(buffer, avail);
        //         sd_card_logf("%08.3f CER DISCARD:", xTaskGetTickCount() / 1000.0);
        //         for (int i = 0; i < avail; i++)
        //             sd_card_logf(" %02X", buffer[i]);
        //         sd_card_logf("\n");
        //         hangers = 0;
        //     } else {
        //         hangers = avail;
        //     }
        // }
        vTaskDelay(1);
    }
}


#include <Wifi.h>
const char* ssid     = "Samuel";
const char* password = "8zciwf256cb6";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

// void timer_handler(void * _) {
//     while (true) {
//         lv_timer_handler_run_in_period(5);
//         vTaskDelay(1);
//     }
// }

#include "driver/uart.h"

void serial_error(hardwareSerial_error_t error) {
    switch(error) {
        case UART_FIFO_OVF:
            get_changed()->activity |= ACTIVITY_ERROR;
            // Serial.printf("FIFO Overflow. Consider adding Hardware Flow Control to your Application.\n");
            break;
        case UART_BUFFER_FULL:
            get_changed()->activity |= ACTIVITY_ERROR;
            // Serial.printf("Buffer Full. Consider increasing your buffer size of your Application.\n");
            break;
        case UART_BREAK:
            get_changed()->activity |= ACTIVITY_ERROR;
            // Serial.printf("RX break.\n");
            break;
        case UART_PARITY_ERR:
            get_changed()->activity |= ACTIVITY_ERROR;
            // Serial.printf("parity error.\n");
            break;
        case UART_FRAME_ERR:
            get_changed()->activity |= ACTIVITY_ERROR;
            // Serial.printf("frame error.\n");
            break;
        default:
            get_changed()->activity |= ACTIVITY_ERROR;
            // Serial.printf("unknown event type %d.\n", error);
            break;
    }
}

void setup_can() {
    // Serial.setRxFIFOFull(12);
    Serial.setRxBufferSize(1 << 15);
    // Serial.onReceive(CAN_Task_loop, false);
    Serial.onReceiveError(serial_error);
    can.begin(Serial, 115200);

    xTaskCreatePinnedToCore(
      CAN_Task_loop, /* Function to implement the task */
      "CAN Task", /* Name of the task */
      16000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      3,  /* Priority of the task */
      &CAN_Task,  /* Task handle. */
      0); /* Core where the task should run */
}

void process_standard() {
    char c_id[4];
    char c_length[2];
    char c_data[17];
    dbcc_time_stamp_t ts;

    while (!Serial.available());

    Serial.readBytes(c_id, 3);
    uint32_t id = strtoul(c_id, NULL, 16);

    while (!Serial.available());

    Serial.readBytes(c_length, 1);
    uint8_t length = strtoul(c_length, NULL, 10);

    while (!Serial.available());

    Serial.readBytes(c_data, length);
    uint64_t data = strtoull(c_data, NULL, 16);
    uint8_t * p_data = (uint8_t*)&data;

    Serial.printf("%04X %02X %02X %02X %02X %02X %02X %02X %02X\n", id, p_data[0], p_data[1], p_data[2], p_data[3], p_data[4], p_data[5], p_data[6], p_data[7]);

    if (decode_can_message(ts, id, p_data) < 0) {
        get_changed()->activity |= ACTIVITY_ERROR;
    } else {
        get_changed()->activity |= ACTIVITY_SUCCESS;
    }
}

TaskHandle_t CAN232_Task;

void can_232_task(void * parameter) {
    for (;;) {
        if (Serial.available()) {
            char cmd = Serial.read();
            switch (cmd) {
                case 't':
                    process_standard();

            }
        }
        vTaskDelay(1);
    }
}

void setup_can232() {
    Serial.begin(115200);

    xTaskCreatePinnedToCore(
      can_232_task, /* Function to implement the task */
      "CAN232 Task", /* Name of the task */
      16000,  /* Stack size in words */
      NULL,  /* Task input parameter */
      3,  /* Priority of the task */
      &CAN232_Task,  /* Task handle. */
      0); /* Core where the task should run */
}

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

    // Serial.setRxBufferSize(SOC_UART_FIFO_LEN * 64);
    // Serial.begin(115200); 
    setup_can();
    // setup_can232();

    gfx.init_without_reset();
    // gfx.init();
    gfx.setRotation(0);
    gfx.setBrightness(0);
    gfx.setColorDepth(16); 
    gfx.clearDisplay();

    ScreenBrightness::fade_in();

    lv_init();
    // lv_log_register_print_cb(my_log_cb);

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

    dash_create(disp);
    // sd_card_init();

    /*Initialize the input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read_new;
    lv_indev_drv_register(&indev_drv);

    // delay(500);

    // really only need once - it's store in the eeprom
    // can.baudRate(SERIAL_RATE_115200);

    // xTaskCreatePinnedToCore(
    //   timer_handler, /* Function to implement the task */
    //   "LVGL Timer Handler", /* Name of the task */
    //   12000,  /* Stack size in words */
    //   NULL,  /* Task input parameter */
    //   0,  /* Priority of the task */
    //   NULL,  /* Task handle. */
    //   1); /* Core where the task should run */

}

void loop() {
    lv_timer_handler_run_in_period(5);
    dash_loop();
}