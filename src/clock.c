#include "clock.h"
#include "time.h"
#include <sys/time.h>

#define SEC_PER_DAY   86400
#define SEC_PER_HOUR  3600
#define SEC_PER_MIN   60

static lv_obj_t * time_label;
static bool overflow;
long offset = -18000;

static uint16_t minute_offset = 12 * 60 - 7;

void clock_update() {
    // running_clock is in minutes
    uint16_t corrected = get_dash()->running_clock + minute_offset;
    int hour = corrected / 60 % 24;
    int min = corrected % 60;
    //   int sec = (hms % SEC_PER_HOUR) % SEC_PER_MIN; // or hms % SEC_PER_MIN

    bool pm =  hour >= 12;

    if (hour > 12) {
      hour -= 12;
    } else if (hour == 0) {
      hour += 12;
    }

    lv_label_set_text_fmt(time_label, "%d:%02d %s", hour, min, pm ? "PM" : "AM");
}

lv_obj_t * clock_create(lv_obj_t * parent) {

    // static lv_ft_info_t info;
    // /*FreeType uses C standard file system, so no driver letter is required.*/
    // // info.name = "./fonts/Microgramma-D-Extended-Medium.otf";
    // // info.name = "./fonts/Rajdhani-Regular.ttf";
    // // info.name = "./fonts/DSEG14ClassicMini-Regular.ttf";
    // info.name = "./fonts/Bould-Medium.otf";
    // info.weight = 22;
    // info.style = FT_FONT_STYLE_NORMAL;
    // info.mem = NULL;
    // if(!lv_ft_font_init(&info)) {
    //     LV_LOG_ERROR("create failed.");
    // }

    DASH_FONT(RAJDHANI_REGULAR, 36);

// {
//     static lv_style_t style;
//     lv_style_init(&style);
//     lv_style_set_text_opa(&style, LV_OPA_100);
//     lv_style_set_text_color(&style, AMBER_OFF);
//     lv_style_set_text_font(&style, info.font);

//     lv_obj_t * time_label = lv_label_create(cont);
//     lv_label_set_text(time_label, "88:88 8~");
//     lv_obj_add_style(time_label, &style, 0);
//     lv_obj_set_style_blend_mode(time_label, LV_BLEND_MODE_ADDITIVE, 0);
//     lv_obj_set_grid_cell(time_label, LV_GRID_ALIGN_CENTER, 1, 3, LV_GRID_ALIGN_CENTER, 2, 1);

//     lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 0);

// }
 
    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_opa(&style, LV_OPA_100);
    lv_style_set_text_color(&style, AMBER_ON);
    lv_style_set_text_font(&style, RAJDHANI_REGULAR_36);
    lv_style_set_text_align(&style, LV_TEXT_ALIGN_LEFT);

    time_label = lv_label_create(parent);
    lv_obj_add_style(time_label, &style, 0);

    // struct timeval tv;
    // struct timezone tz;
    // gettimeofday(&tv, &tz);

    struct tm timeinfo;
    time_t now;
    time(&now);
    localtime_r(&now, &timeinfo);
    time_t tt = mktime (&timeinfo);
      
    if (overflow){
      tt += 63071999;
    }
    if (offset > 0){
      tt += (unsigned long) offset;
    } else {
      tt -= (unsigned long) (offset * -1);
    }
    struct tm * tn = localtime(&tt);
    if (overflow){
      tn->tm_year += 64;
    }

    // Form the seconds of the day
    long hms = timeinfo.tm_min % 60;
    hms += timeinfo.tm_hour * 60;
    // mod `hms` to insure in positive range of [0...SEC_PER_DAY)
    hms = (hms + 3600) % 3600;


    // // Form the seconds of the day
    // long hms = tv.tv_sec % SEC_PER_DAY;
    // hms += tz.tz_dsttime * SEC_PER_HOUR;
    // hms -= tz.tz_minuteswest * SEC_PER_MIN;
    // // mod `hms` to insure in positive range of [0...SEC_PER_DAY)
    // hms = (hms + SEC_PER_DAY) % SEC_PER_DAY;

    // get_dash()->running_clock = hms;
    // clock_update();
    lv_label_set_text(time_label, "--:-- AM");

    // lv_obj_set_style_blend_mode(time_label, LV_BLEND_MODE_ADDITIVE, 0);


    return time_label;
}