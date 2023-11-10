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

// #include <Arduino_GFX_Library.h>
#define TFT_BL 2
#define GFX_BL 2

// Arduino_ESP32RGBPanel * panel = new Arduino_ESP32RGBPanel(
//     41 /* DE */, 
//     40 /* VSYNC */, 
//     39 /* HSYNC */, 
//     0 /* PCLK */,
//     14 /* R0 */, 
//     21 /* R1 */, 
//     47 /* R2 */, 
//     48 /* R3 */, 
//     45 /* R4 */,
//     9 /* G0 */, 
//     46 /* G1 */, 
//     3 /* G2 */, 
//     8 /* G3 */, 
//     16 /* G4 */, 
//     1 /* G5 */,
//     15 /* B0 */, 
//     7 /* B1 */, 
//     6 /* B2 */, 
//     5 /* B3 */, 
//     4 /* B4 */,
//     0 /* hsync_polarity */, 
//     40 /* hsync_front_porch */, 
//     48 /* hsync_pulse_width */, 
//     40 /* hsync_back_porch */,
//     0 /* vsync_polarity */, 
//     13 /* vsync_front_porch */, 
//     1 /* vsync_pulse_width */, 
//     31 /* vsync_back_porch */,
//     1 /* pclk_active_neg */,
//     33300000 /* prefer_speed */,
//     false /* useBigEndian */,
//     0 /* de_idle_high */,
//     0 /* pclk_idle_high */
// );

// Arduino_RGB_Display * gfx = new Arduino_RGB_Display(
//     800 /* width */, 
//     480 /* height */, 
//     panel, 
//     0 /* rotation */, 
//     false /* auto_flush */
// );

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
  Serial.print(buf);
}

// SD CARD

#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10

int SD_init() {
  if (!SD.begin(SD_CS)) {
    add_message("Card Mount Failed");
    return 1;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    add_message("No TF card attached");
    return 1;
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  add_message_fmt("TF Card Size: %lluMB\n", cardSize);
//   listDir(SD, "/", 2);

  //  listDir(SD, "/", 0);
  //  createDir(SD, "/mydir");
  //  listDir(SD, "/", 0);
  //  removeDir(SD, "/mydir");
  //  listDir(SD, "/", 2);
  //  writeFile(SD, "/hello.txt", "Hello ");
  //  appendFile(SD, "/hello.txt", "World!\n");
  //  readFile(SD, "/hello.txt");
  //  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  //  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));
  //  Serial.println("SD init over.");

  return 0;
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

void setup() {
    Serial.begin(9600); 

    gfx.init();

    gfx.setRotation(0);
    gfx.setBrightness(128);
    gfx.setColorDepth(16); 

    gfx.begin();

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

#ifdef TFT_BL
    // ledcSetup(0, 20000, 4);
    // ledcAttachPin(TFT_BL, 0);
    // ledcWrite(0, 31);
#endif

    dash();

    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    delay(100);
    if (SD_init() == 1) {
        add_message("SD Card Mounting Failed");
    } else {
        add_message("SD Card Mounted");
        // setupMassStorage();
    }
}

int get_message(unsigned int * id, char * buf) {
    if (Serial.available()) {

        unsigned long timer_s = millis();
        
        int len = 0;
        char dta[20];
        
        while(1) {
            while(Serial.available()) {
                dta[len++] = Serial.read();
                if(len == 12)
                    break;
 
            	if ((millis()-timer_s) > 10) {
                    // add_message(dta);
                    return 0;
                }
            }
            
            if(len == 12) // Just to be sure, must be 12 here
            {
                unsigned int __id = 0;
                
                for(int i=0; i<4; i++) // Store the id of the sender
                {
                    __id <<= 8;
                    __id += dta[i];
                }
                
                *id = __id;
                
                for(int i=0; i<8; i++) // Store the message in the buffer
                {
                    buf[i] = dta[i+4];
                }
                return 1;
            }
            
            if ((millis()-timer_s) > 10) {
                // add_message(dta);
                return 0; // Reading 12 bytes should be faster than 10ms, abort if it takes longer, we loose the partial message in this case
            }
        }
    }
}

void loop() {
    // lv_refr_now();
    lv_timer_handler();

    dash_loop();
    static unsigned int id;
    static char buf[9];
    if (get_message(&id, buf)) {
        if (id == 0x78300D0A) {
            add_message(buf);
        } else {
            static char sbuf[36];
            sprintf(sbuf, "%08X: %02X %02X %02X %02X %02X %02X %02X %02X\n", id, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
            File file = SD.open("/messages.log", FILE_APPEND);
            if (file) {
                file.write((uint8_t*)sbuf, 36);
                file.close();
            }
            add_message(sbuf);
        }
    }
    delay(5);
}