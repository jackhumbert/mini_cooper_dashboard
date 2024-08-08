#include "clock.hpp"
#include "time.h"
#include <sys/time.h>
#include "messages.h"
#include "sd_card.h"
#include "tiny_display.h"

#define SEC_PER_DAY   86400
#define SEC_PER_HOUR  3600
#define SEC_PER_MIN   60

// #define OVERFLOW_ADD 0x10000
#define OVERFLOW_ADD 54015

static lv_obj_t * time_label;
static bool overflow;
long offset = -18000;

#if not DASH_SIMULATION
  #include <ESP32Time.h>
  extern ESP32Time rtc;
#endif

// eventually expose a ui to change this via Preferences, and account for uint16_t overflow
// static uint64_t epoch_offset = 1700229360ull - 9903 * 60 + (OVERFLOW_ADD * 60 * 5);
// Fri Nov 10 2023 11:53:00 GMT-0500 (Eastern Standard Time)
static uint64_t epoch_offset = 1699635180ull;
// static uint64_t epoch_offset = 1699635180ull + (OVERFLOW_ADD * 60 * 7);
// November 15 2023, 3:07 AM
// August 3 2024, 2:00 PM

// June 29, 4:27 ??

bool loaded_from_file = false;

void Clock::update(void) {
    if (get_queued()->running_clock) {
      if (!loaded_from_file) {
        epoch_offset += get_time_offset();
        loaded_from_file = true;
      }
      #if DASH_SIMULATION
        uint16_t corrected = get_dash()->running_clock + epoch_offset;
        int hour = corrected / 60 % 24;
        int min = corrected % 60;

        bool pm =  hour >= 12;

        if (hour > 12) {
          hour -= 12;
        } else if (hour == 0) {
          hour += 12;
        }
        lv_label_set_text_fmt(lv_obj, "%d:%02d %s", hour, min, pm ? "PM" : "AM");
      #else
        int day = rtc.getDayofYear();
        rtc.setTime(epoch_offset + get_cache()->running_clock * 60);
        auto data = get_peer_data();
        if (data) {
          data->time = rtc.getLocalEpoch();
          send_peer_data();
        }
        if (day != rtc.getDayofYear()) {
          add_message(rtc.getTime("%A, %B %d %Y").c_str());
        }
        lv_label_set_text_fmt(lv_obj, "%d:%02d %s", rtc.getHour(false), rtc.getMinute(), rtc.getAmPm().c_str());
      #endif
    }
}

Clock::Clock(lv_obj_t * parent) {

    setup_peer_data();

    DASH_FONT(RAJDHANI_REGULAR, 36);
 
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_ON);
    lv_style_set_text_font(&style, RAJDHANI_REGULAR_36);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_LEFT);

    lv_obj = lv_label_create(parent);
    lv_obj_add_style(lv_obj, &style, 0);
    lv_label_set_text(lv_obj, "--:-- AM");
    lv_obj_align(lv_obj, LV_ALIGN_TOP_LEFT, 300, 150);

    // struct tm timeinfo;
    // time_t now;
    // time(&now);
    // localtime_r(&now, &timeinfo);
    // time_t tt = mktime (&timeinfo);
      
    // if (overflow){
    //   tt += 63071999;
    // }
    // if (offset > 0){
    //   tt += (unsigned long) offset;
    // } else {
    //   tt -= (unsigned long) (offset * -1);
    // }
    // struct tm * tn = localtime(&tt);
    // if (overflow){
    //   tn->tm_year += 64;
    // }

    // long hms = timeinfo.tm_min % 60;
    // hms += timeinfo.tm_hour * 60;
    // hms = (hms + 3600) % 3600;

}