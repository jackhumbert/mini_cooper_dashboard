#include "sd_card.h"
#include <SPI.h>
#include "messages.h"
#include <SD.h>

// SD CARD

#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10
#define RING_BUF_CAPACITY 16 * 512

static char log_filename[20];

static File log_file;
static pthread_mutex_t log_file_mutex;

#define NUM_BUFFERS 4
#define FILE_BUFFER_LENGTH 512
#if FILE_BUFFER_LENGTH > 256
#define buffer_length_t uint16_t
#else
#define buffer_length_t uint8_t
#endif
static uint8_t file_buffer[NUM_BUFFERS][FILE_BUFFER_LENGTH] = {{0}};
static buffer_length_t file_buffer_length[NUM_BUFFERS] = {0};
static pthread_mutex_t buffer_mutex[NUM_BUFFERS];
static uint8_t active_buffer = 0;

static void buffer_write(uint8_t * buffer, buffer_length_t length) {
    uint8_t * pos = buffer;
    buffer_length_t bytes_left = length;
    for (int i = active_buffer; i < NUM_BUFFERS; i = (i + 1) % NUM_BUFFERS) {
        if (FILE_BUFFER_LENGTH > file_buffer_length[i]) {
            pthread_mutex_lock(&buffer_mutex[i]);
            buffer_length_t bytes_to_write = bytes_left;
            if (file_buffer_length[i] + bytes_left > FILE_BUFFER_LENGTH) {
                bytes_to_write = FILE_BUFFER_LENGTH - file_buffer_length[i];
            }
            bytes_left -= bytes_to_write;
            memcpy(&file_buffer[i][file_buffer_length[i]], pos, bytes_to_write);
            pos += bytes_to_write;
            file_buffer_length[i] += bytes_to_write;
            pthread_mutex_unlock(&buffer_mutex[i]);
            if (!bytes_left) {
                if (file_buffer_length[i] < FILE_BUFFER_LENGTH) {
                    active_buffer = i;
                } else {
                    active_buffer = (i + 1) % NUM_BUFFERS;
                }
                return;
            }
        }
        // vTaskDelay(1);
    }
    // Serial.println("Buffers are full or busy");
    // get_changed()->activity |= ACTIVITY_ERROR;
}

TaskHandle_t rb_flusher_task;

static void rb_flusher(void * parameter) {
    int i = 0;
    while (true) {
        for (i = 0; i < NUM_BUFFERS; i++) {
            // if (file_buffer_length[i] && pthread_mutex_trylock(&buffer_mutex[i]) == 0) {
            if (file_buffer_length[i] == FILE_BUFFER_LENGTH) {
                pthread_mutex_lock(&buffer_mutex[i]);
                if (log_file.write(file_buffer[i], file_buffer_length[i]) == file_buffer_length[i]) {
                    log_file.flush();
                } else {
                    // add_message_fmt("Logging error: %02X", log_file.getWriteError());
                    Serial.printf("Logging error: %02X\n", log_file.getWriteError());
                    get_changed()->activity |= ACTIVITY_ERROR;
                }
                file_buffer_length[i] = 0;
                pthread_mutex_unlock(&buffer_mutex[i]);
            }
        }
        // pthread_mutex_unlock(&log_file_mutex);
        vTaskDelay(1); // Feed the watchdog. It will trigger and crash if we dont give it some time
    }
}

bool sd_card_init() {
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    // // delay(100);
	pthread_mutex_init(&log_file_mutex, NULL);
    if (!SD.begin()) {
        add_message("SD Card Mount Failed");
        return false;
    }
    // uint8_t cardType = sd.cardType();

    // if (cardType == CARD_NONE) {
    //     add_message("No SD card attached");
    // }

    // uint64_t cardSize = sd.cardSize() / (1024 * 1024);
    // add_message_fmt("SD Card Size: %lluMB", cardSize);

    char number_buffer[8];
    File number_file = SD.open("/log_number.txt", FILE_READ);
    long log_number = 0;
    if (number_file.size()) {
        number_file.readBytes(number_buffer, number_file.size());
        log_number = strtol(number_buffer, NULL, 10) + 1;
    }

    number_file.close();
    number_file = SD.open("/log_number.txt", FILE_WRITE);
    number_file.printf("%d", log_number);
    number_file.close();
    
    sprintf(log_filename, "/log_%08ld.crtd", log_number);

    add_message_fmt("Log file: %s", log_filename);
    set_log_filename(log_filename);
    log_file = SD.open(log_filename, FILE_WRITE, true);
    // log_file.preAllocate(10 * 25000 * 60);

    for (int i = 0; i < NUM_BUFFERS; i++) {
        pthread_mutex_init(&buffer_mutex[i], NULL);
    }
    
    // log_file.setBufferSize(1024 * 1024);
    // log_file.flush();

    xTaskCreatePinnedToCore(
        rb_flusher, // Task function.
        "writeTask", // String with name of task.
        10000, // Stack size in bytes. This seems enough for it not to crash.
        NULL, // Parameter passed as input of the task.
        1, // Priority of the task.
        &rb_flusher_task, // Task handle.
        1);

    sd_card_logf("%08.3f CXX R53 Custom Dash by Jack Humbert\n", xTaskGetTickCount() / 1000.0);
    return true;
}

void stop_logging(void) {
    vTaskDelete(rb_flusher_task);
    for (int i = active_buffer + 1; i != active_buffer; i = (i + 1) % NUM_BUFFERS) {
        // if (file_buffer_length[i] && pthread_mutex_trylock(&buffer_mutex[i]) == 0) {
        if (file_buffer_length[i]) {
            pthread_mutex_lock(&buffer_mutex[i]);
            if (log_file.write(file_buffer[i], file_buffer_length[i]) == file_buffer_length[i]) {
                // log_file.flush();
            } else {
                // add_message_fmt("Logging error: %02X", log_file.getError());
                // Serial.printf("Logging error: %02X\n", log_file.getError());
                get_changed()->activity |= ACTIVITY_ERROR;
            }
            file_buffer_length[i] = 0;
            pthread_mutex_unlock(&buffer_mutex[i]);
        }
    }
    set_log_filename("");
    log_file.close();
    SPI.end();
    add_message_fmt("Closed: %s", log_filename);
}

void sd_card_logf(const char * format, ...) {
    if (log_file) {
        static char buffer[256] = {0};
        va_list args;
        va_start(args, format);
        buffer_length_t length = vsnprintf(buffer, 255, format, args);

        // pthread_mutex_lock(&log_file_mutex);
        buffer_write((uint8_t*)buffer, length);
        // pthread_mutex_unlock(&log_file_mutex);
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

    // pthread_mutex_lock(&log_file_mutex);
    if (SD.exists(filename)) {
        add_message_fmt("Dumping %s", filename);
        File file = SD.open(filename, "r");
        Serial.printf("%08lX\n", file.size());
        // could get file info too
        // file.getLastWrite();
        while (file.available()) {
            Serial.write(file.read());
        }
        file.close();
        // pthread_mutex_unlock(&log_file_mutex);
        return 0;
    } else {
        add_message_fmt("Couldn't find %s", filename);
        // pthread_mutex_unlock(&log_file_mutex);
        return -1;
    }
}