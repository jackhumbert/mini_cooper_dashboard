#include <lvgl.h>
#include <SPI.h>

SPIClass& spi = SPI;
uint16_t touchCalibration_x0 = 300, touchCalibration_x1 = 3600, touchCalibration_y0 = 300, touchCalibration_y1 = 3600;
uint8_t  touchCalibration_rotate = 1, touchCalibration_invert_x = 2, touchCalibration_invert_y = 0;
// static int val = 100;
// #include <Ticker.h>          //Call the ticker. H Library
// Ticker ticker1;

#include "touch.h"
#include <Wire.h>
#include <TAMC_GT911.h>
// #include <Arduino_GFX_Library.h>

#include "screen.h"

// extern Arduino_RGB_Display *gfx;
extern LGFX gfx;

TAMC_GT911 ts = TAMC_GT911(TOUCH_GT911_SDA, TOUCH_GT911_SCL, TOUCH_GT911_INT, TOUCH_GT911_RST, max(TOUCH_MAP_X1, TOUCH_MAP_X2), max(TOUCH_MAP_Y1, TOUCH_MAP_Y2));

int touch_last_x = 0, touch_last_y = 0;

void touch_init() {
    // Begin init wire, and Reset the device also, no need wire.begin() to be done here
    Wire.begin(TOUCH_GT911_SDA, TOUCH_GT911_SCL);
    // Wire.setPins(TOUCH_GT911_SDA, TOUCH_GT911_SCL);
    ts.begin(GT911_ADDR1);

    // // Need to solve floating INT pin so on reset changes between the 2 addresses
    // Wire.beginTransmission(GT911_ADDR1);
    // Wire.write(highByte(GT911_PRODUCT_ID));
    // Wire.write(lowByte(GT911_PRODUCT_ID));
    // Wire.endTransmission();
    // //returns rxLength - if 0 we have a problem
    // uint8_t returnSize = Wire.requestFrom(GT911_ADDR1, (uint8_t)1); 
    // if (returnSize == 0) {
    //     Serial.println("Setting address to ADDR2");
    //     // restart with other address
    //     ts.begin(GT911_ADDR2);
    // }

    // Now do your normal job
    ts.setRotation(TOUCH_GT911_ROTATION);
}

bool touch_touched() {
  ts.read();
  if (ts.isTouched) {
#if defined(TOUCH_SWAP_XY)
    touch_last_x = map(ts.points[0].y, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, SCREEN_WIDTH - 1);
    touch_last_y = map(ts.points[0].x, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, SCREEN_HEIGHT - 1);
#else
    touch_last_x = map(ts.points[0].x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, SCREEN_WIDTH - 1);
    touch_last_y = map(ts.points[0].y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, SCREEN_HEIGHT - 1);
#endif
    return true;
  } else {
    return false;
  }
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    if (touch_touched()) {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    //   Serial.print( "Data x " );
    //   Serial.println( data->point.x );
    //   Serial.print( "Data y " );
    //   Serial.println( data->point.y );
    }
}

#define Z_THRESHOLD 350 // Touch pressure threshold for validating touches
#define _RAWERR 10 // Deadband error allowed in successive position samples
void begin_touch_read_write(void) {
  digitalWrite(38, HIGH); // Just in case it has been left low
  spi.setFrequency(600000);
  digitalWrite(38, LOW);
}

void end_touch_read_write(void) {
  digitalWrite(38, HIGH); // Just in case it has been left low
  spi.setFrequency(600000);

}

uint16_t getTouchRawZ(void) {

  begin_touch_read_write();

  // Z sample request
  int16_t tz = 0xFFF;
  spi.transfer(0xb0);               // Start new Z1 conversion
  tz += spi.transfer16(0xc0) >> 3;  // Read Z1 and start Z2 conversion
  tz -= spi.transfer16(0x00) >> 3;  // Read Z2

  end_touch_read_write();

  return (uint16_t)tz;
}

uint8_t getTouchRaw(uint16_t *x, uint16_t *y) {
  uint16_t tmp;

  begin_touch_read_write();

  // Start YP sample request for x position, read 4 times and keep last sample
  spi.transfer(0xd0);                    // Start new YP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0xd0);                    // Read last 8 bits and start new YP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0xd0);                    // Read last 8 bits and start new YP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0xd0);                    // Read last 8 bits and start new YP conversion

  tmp = spi.transfer(0);                   // Read first 8 bits
  tmp = tmp << 5;
  tmp |= 0x1f & (spi.transfer(0x90) >> 3); // Read last 8 bits and start new XP conversion

  *x = tmp;

  // Start XP sample request for y position, read 4 times and keep last sample
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0x90);                    // Read last 8 bits and start new XP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0x90);                    // Read last 8 bits and start new XP conversion
  spi.transfer(0);                       // Read first 8 bits
  spi.transfer(0x90);                    // Read last 8 bits and start new XP conversion

  tmp = spi.transfer(0);                 // Read first 8 bits
  tmp = tmp << 5;
  tmp |= 0x1f & (spi.transfer(0) >> 3);  // Read last 8 bits

  *y = tmp;

  end_touch_read_write();

  return true;
}

uint8_t validTouch(uint16_t *x, uint16_t *y, uint16_t threshold) {
  uint16_t x_tmp, y_tmp, x_tmp2, y_tmp2;

  // Wait until pressure stops increasing to debounce pressure
  uint16_t z1 = 1;
  uint16_t z2 = 0;
  while (z1 > z2)
  {
    z2 = z1;
    z1 = getTouchRawZ();
    delay(1);
    Serial.print("z1:");
    Serial.println(z1);
  }


  if (z1 <= threshold) return false;

  getTouchRaw(&x_tmp, &y_tmp);


  delay(1); // Small delay to the next sample
  if (getTouchRawZ() <= threshold) return false;

  delay(2); // Small delay to the next sample
  getTouchRaw(&x_tmp2, &y_tmp2);


  if (abs(x_tmp - x_tmp2) > _RAWERR) return false;
  if (abs(y_tmp - y_tmp2) > _RAWERR) return false;

  *x = x_tmp;
  *y = y_tmp;

  return true;
}

void calibrateTouch(uint16_t *parameters, uint32_t color_fg, uint32_t color_bg, uint8_t size) {
  int16_t values[] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint16_t x_tmp, y_tmp;
  uint16_t _width = 800;
  uint16_t _height = 480;

  for (uint8_t i = 0; i < 4; i++) {
    gfx.fillRect(0, 0, size + 1, size + 1, color_bg);
    gfx.fillRect(0, _height - size - 1, size + 1, size + 1, color_bg);
    gfx.fillRect(_width - size - 1, 0, size + 1, size + 1, color_bg);
    gfx.fillRect(_width - size - 1, _height - size - 1, size + 1, size + 1, color_bg);

    if (i == 5) break; // used to clear the arrows

    switch (i) {
      case 0: // up left
        gfx.drawLine(0, 0, 0, size, color_fg);
        gfx.drawLine(0, 0, size, 0, color_fg);
        gfx.drawLine(0, 0, size , size, color_fg);
        break;
      case 1: // bot left
        gfx.drawLine(0, _height - size - 1, 0, _height - 1, color_fg);
        gfx.drawLine(0, _height - 1, size, _height - 1, color_fg);
        gfx.drawLine(size, _height - size - 1, 0, _height - 1 , color_fg);
        break;
      case 2: // up right
        gfx.drawLine(_width - size - 1, 0, _width - 1, 0, color_fg);
        gfx.drawLine(_width - size - 1, size, _width - 1, 0, color_fg);
        gfx.drawLine(_width - 1, size, _width - 1, 0, color_fg);
        break;
      case 3: // bot right
        gfx.drawLine(_width - size - 1, _height - size - 1, _width - 1, _height - 1, color_fg);
        gfx.drawLine(_width - 1, _height - 1 - size, _width - 1, _height - 1, color_fg);
        gfx.drawLine(_width - 1 - size, _height - 1, _width - 1, _height - 1, color_fg);
        break;
    }

    // user has to get the chance to release
    if (i > 0) delay(1000);

    for (uint8_t j = 0; j < 8; j++) {
        if (touch_touched())  {
          Serial.print( "Data x :" );
          Serial.println( touch_last_x );
          Serial.print( "Data y :" );
          Serial.println( touch_last_y );
          break;
        }
    }
  }
}


void touch_calibrate()//屏幕校准
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;
  Serial.println("屏幕校准");

  //校准
  //  gfx.fillScreen(BLACK);
  //  gfx.setCursor(20, 0);
  //  Serial.println("setCursor");
  //  gfx.setTextFont(2);
  //  Serial.println("setTextFont");
  //  gfx.setTextSize(1);
  //  Serial.println("setTextSize");
  //  gfx.setTextColor(TFT_WHITE, TFT_BLACK);

  //  gfx.println("按指示触摸角落");
  Serial.println("按指示触摸角落");
  //  gfx.setTextFont(1);
  //  gfx.println();
  //      gfx.setCursor(175, 100);
  //      gfx.printf("Touch Adjust");
  //  Serial.println("setTextFont(1)");
  lv_timer_handler();
  calibrateTouch(calData, 0xFFFF, 0x0000, 17);
  Serial.println("calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15)");
  Serial.println(); Serial.println();
  Serial.println("//在setup()中使用此校准代码:");
  Serial.print("uint16_t calData[5] = ");
  Serial.print("{ ");

  for (uint8_t i = 0; i < 5; i++)
  {
    Serial.print(calData[i]);
    if (i < 4) Serial.print(", ");
  }

  Serial.println(" };");
  //  Serial.print("  tft.setTouch(calData);");
  //  Serial.println(); Serial.println();
  //  gfx.fillScreen(BLACK);
  //  gfx.println("XZ OK!");
  //  gfx.println("Calibration code sent to Serial port.");


}

void setTouch(uint16_t *parameters) {
  touchCalibration_x0 = parameters[0];
  touchCalibration_x1 = parameters[1];
  touchCalibration_y0 = parameters[2];
  touchCalibration_y1 = parameters[3];

  if (touchCalibration_x0 == 0) touchCalibration_x0 = 1;
  if (touchCalibration_x1 == 0) touchCalibration_x1 = 1;
  if (touchCalibration_y0 == 0) touchCalibration_y0 = 1;
  if (touchCalibration_y1 == 0) touchCalibration_y1 = 1;

  touchCalibration_rotate = parameters[4] & 0x01;
  touchCalibration_invert_x = parameters[4] & 0x02;
  touchCalibration_invert_y = parameters[4] & 0x04;
}