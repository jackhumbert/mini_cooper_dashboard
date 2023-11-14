#include "sd_card.h"
#include <SPI.h>
// #include "sd_card.hpp"
#include "messages.h"
// #include <Elog.h>
#include <SdFat.h>
#include <RingBuf.h>

// SD CARD

#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10
#define RING_BUF_CAPACITY 16 * 512

static char log_filename[20];

static FsFile log_file;
static pthread_mutex_t log_file_mutex;

SdFat sd;
RingBuf<FsFile, RING_BUF_CAPACITY> rb;

unsigned long sd_card_get_file_count() {
	// File d = SD.open("/");
    // if (!d)
    //     return 0;
	// unsigned long count_files = 0;
	// while (true) {
 	// 	File entry =  d.openNextFile();
	// 	if(!entry) {
	// 		return count_files;
	// 	}
	// 	String file_name = entry.name();
	// 	if( file_name.indexOf('~') != 0 && !entry.isDirectory()) {
	// 		count_files++;
	// 	}
	// }
    return 0;
}

static void rb_flusher(void * parameter) {
    while (true) {
        // if (rb.bytesUsed() > 512 && !log_file.isBusy()) {
        pthread_mutex_lock(&log_file_mutex);
        if (rb.writeOut(512)) {
                // rb.sync();
            if (!log_file.isBusy()) {
                log_file.flush();
            }
        }
        pthread_mutex_unlock(&log_file_mutex);
        vTaskDelay(1); // Feed the watchdog. It will trigger and crash if we dont give it some time
    }
}

void sd_card_init() {
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    // // delay(100);
	pthread_mutex_init(&log_file_mutex, NULL);
    if (!sd.begin(SD_CS)) {
        add_message("SD Card Mount Failed");
        return;
    }
    // uint8_t cardType = sd.cardType();

    // if (cardType == CARD_NONE) {
    //     add_message("No SD card attached");
    // }

    // uint64_t cardSize = sd.cardSize() / (1024 * 1024);
    // add_message_fmt("SD Card Size: %lluMB", cardSize);

    char number_buffer[8];
    FsFile number_file = sd.open("/log_number.txt", O_RDWR | O_CREAT);
    long log_number = 0;
    if (number_file.size()) {
        number_file.readBytes(number_buffer, number_file.size());
        log_number = strtol(number_buffer, NULL, 10) + 1;
    }

    number_file.seek(0);
    number_file.truncate();
    number_file.printf("%d", log_number);
    number_file.close();
    
    sprintf(log_filename, "/log_%08ld.crtd", log_number);

    add_message_fmt("Log file: %s", log_filename);
    log_file = sd.open(log_filename, O_WRONLY | O_CREAT | O_TRUNC);
    // log_file.preAllocate(10 * 25000 * 60);
    
    rb.begin(&log_file);

    // log_file.setBufferSize(1024 * 1024);
    rb.printf("%08.3f CXX R53 Custom Dash by Jack Humbert\n", xTaskGetTickCount() / 1000.0);
    rb.sync();
    // log_file.flush();

    xTaskCreatePinnedToCore(
        rb_flusher, // Task function.
        "writeTask", // String with name of task.
        5000, // Stack size in bytes. This seems enough for it not to crash.
        NULL, // Parameter passed as input of the task.
        1, // Priority of the task.
        NULL, // Task handle.
        1);
}

void sd_card_logf(const char * format, ...) {
    if (log_file) {
        static char buffer[256] = {0};
        va_list args;
        va_start(args, format);
        int length = vsnprintf(buffer, 255, format, args);

        pthread_mutex_lock(&log_file_mutex);
        rb.write(buffer, length);
        pthread_mutex_unlock(&log_file_mutex);
        // logger.log(INFO, buffer);

        va_end (args);
    }
}

void sd_card_flush() {
    // if (log_file) {
    //     pthread_mutex_lock(&log_file_mutex);
    //     log_file.flush();
    //     pthread_mutex_unlock(&log_file_mutex);
    // }
}

// File * sd_card_get_log_file(void) {
//     return &log_file;
// }

void sd_card_dump_messages(void) {
    add_message("Dumping messages to serial");
    lv_task_handler();
    log_file.close();

    // File file = SD.open(log_filename, FILE_READ);
    // while (file.available()) {
    //     Serial.write(file.read());
    // }
    // file.close();

    // log_file = SD.open(log_filename, FILE_APPEND);
}

void sd_card_clear_messages(void) {
    add_message("Closing log file");
    lv_task_handler();
    log_file.close();
}

int sd_card_get_log(unsigned char * data) {
    char filename[20];
    sprintf(filename, "/log_%08llu.crtd", *(uint64_t*)data);

    pthread_mutex_lock(&log_file_mutex);
    if (sd.exists(filename)) {
        add_message_fmt("Dumping %s", filename);
        FsFile file = sd.open(filename, O_RDONLY);
        Serial.printf("%08lX\n", file.size());
        // could get file info too
        // file.getLastWrite();
        while (file.available()) {
            Serial.write(file.read());
        }
        file.close();
        pthread_mutex_unlock(&log_file_mutex);
        return 0;
    } else {
        add_message_fmt("Couldn't find %s", filename);
        pthread_mutex_unlock(&log_file_mutex);
        return -1;
    }
}