#include "dev.h"
#include "sd_card.h"
#include <ESP32Time.h>
#include "messages.h"
#include "dash.h"
#include <SD.h>

ESP32Time rtc(-18000);  // offset in seconds GMT+1
extern bool sd_mounted;

static int list_logs() {
    if (!sd_mounted) {
        SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
        if (!SD.begin()) {
            add_message("SD Card Mount Failed");
            return -1;
        }
        sd_mounted = true;
    }

    char number_buffer[8];
    File number_file = SD.open("/log_number.txt", "r", true);
    long log_number = 0;
    if (number_file.size()) {
        number_file.readBytes(number_buffer, number_file.size());
        log_number = strtol(number_buffer, NULL, 10);
    }
    number_file.close();
    Serial.printf("%08lX\n", log_number);
    return 0;
}


static int set_time(uint8_t * data) {
    rtc.setTime(*(time_t*)data);
    add_message(rtc.getTime("%A, %B %d %Y %H:%M:%S").c_str());
    get_dash()->running_clock = rtc.getHour() * 60 + rtc.getMinute();
    get_changed()->running_clock = 1;
    return 0;
}

int dev_process(uint16_t id, uint8_t * data) {
    switch (id) {
        case 0x900:
            return list_logs();
        case 0x901:
            return sd_card_get_log(data);
        case 0x902:
            return set_time(data);
    }
    return -1;
}