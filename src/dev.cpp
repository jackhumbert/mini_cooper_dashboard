#include "dev.h"
#include <Arduino.h>
#include "sd_card.h"
#include <SD.h>
#include <FS.h>
#include <ESP32Time.h>
#include "messages.h"
#include "dash.h"

ESP32Time rtc(-18000);  // offset in seconds GMT+1

static int list_logs() {
    uint64_t count = sd_card_get_file_count();
    Serial.printf("%08lX\n", count);
    return 0;
}

static int get_log(uint8_t * data) {
    char filename[20];
    sprintf(filename, "/log_%08llu.crtd", *(uint64_t*)data);
    File file = SD.open(filename, FILE_READ);
    if (file) {
        Serial.printf("%08lX\n", file.size());
        // could get file info too
        // file.getLastWrite();
        while (file.available()) {
            Serial.write(file.read());
        }
        file.close();
        return 0;
    } else {
        return -1;
    }
}

static int set_time(uint8_t * data) {
    rtc.setTime(*(time_t*)data);
    add_message(rtc.getTime("%A, %B %d %Y %H:%M:%S").c_str());
    get_dash()->running_clock = rtc.getHour() * 3600 + rtc.getMinute() * 60 + rtc.getSecond();
    get_changed()->running_clock = 1;
    return 0;
}

int dev_process(uint16_t id, uint8_t * data) {
    switch (id) {
        case 0x900:
            return list_logs();
        case 0x901:
            return get_log(data);
        case 0x902:
            return set_time(data);
    }
    return -1;
}