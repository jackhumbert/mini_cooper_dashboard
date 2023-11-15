#include "sd_card.h"
#include <SPI.h>
#include "messages.h"
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

#define NUM_BUFFERS 2
#if NUM_BUFFERS > 16
    #define buffer_index_t uint32_t
#else
#if NUM_BUFFERS > 8
    #define buffer_index_t uint16_t
#else
    #define buffer_index_t uint8_t
#endif
#endif

#define FILE_BUFFER_LENGTH 2048
#if FILE_BUFFER_LENGTH > 256
#define buffer_length_t uint16_t
#else
#define buffer_length_t uint8_t
#endif
static uint8_t file_buffer[NUM_BUFFERS][FILE_BUFFER_LENGTH] = {{0}};
static buffer_length_t file_buffer_length[NUM_BUFFERS] = {0};
static buffer_index_t buffer_mutex = 0;

static void buffer_write(uint8_t * buffer, buffer_length_t length) {
    buffer_index_t buffer_index = 0;

    pthread_mutex_lock(&log_file_mutex);
    while ((buffer_mutex & (1 << buffer_index) || FILE_BUFFER_LENGTH <= (file_buffer_length[buffer_index] + length))
        && buffer_index < NUM_BUFFERS) {
            buffer_index++;
    }
    if (buffer_index < NUM_BUFFERS) {
        buffer_mutex |= (1 << buffer_index);
        pthread_mutex_unlock(&log_file_mutex);

        memcpy(&file_buffer[buffer_index][file_buffer_length[buffer_index]], buffer, length);
        file_buffer_length[buffer_index] += length;

        pthread_mutex_lock(&log_file_mutex);
        buffer_mutex &= ~(1 << buffer_index);
        pthread_mutex_unlock(&log_file_mutex);
    } else {
        pthread_mutex_unlock(&log_file_mutex);
        // all buffers busy
        // add_message("All buffers busy");
        // Serial.println("All buffers busy");
        get_changed()->activity |= ACTIVITY_ERROR;
    }
}

static void rb_flusher(void * parameter) {
    while (true) {
        // if (rb.bytesUsed() > 512 && !log_file.isBusy()) {
        // pthread_mutex_lock(&log_file_mutex);
        buffer_index_t buffer_index = 0;
        while (buffer_index < NUM_BUFFERS) {
            pthread_mutex_lock(&log_file_mutex);
            if (file_buffer_length[buffer_index] && (buffer_mutex & (1 << buffer_index)) == 0) {
                buffer_mutex |= (1 << buffer_index);
                pthread_mutex_unlock(&log_file_mutex);
                
                if (log_file.write(file_buffer[buffer_index], file_buffer_length[buffer_index]) == file_buffer_length[buffer_index]) {
                    log_file.flush();
                } else {
                    // add_message_fmt("Logging error: %02X", log_file.getError());
                    // Serial.printf("Logging error: %02X\n", log_file.getError());
                    get_changed()->activity |= ACTIVITY_ERROR;
                }
                file_buffer_length[buffer_index] = 0;

                pthread_mutex_lock(&log_file_mutex);
                buffer_mutex &= ~(1 << buffer_index);
            }
            pthread_mutex_unlock(&log_file_mutex);

            buffer_index++;
        }
        // pthread_mutex_unlock(&log_file_mutex);
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
    
    // log_file.setBufferSize(1024 * 1024);
    sd_card_logf("%08.3f CXX R53 Custom Dash by Jack Humbert\n", xTaskGetTickCount() / 1000.0);
    // log_file.flush();

    xTaskCreatePinnedToCore(
        rb_flusher, // Task function.
        "writeTask", // String with name of task.
        5000, // Stack size in bytes. This seems enough for it not to crash.
        NULL, // Parameter passed as input of the task.
        0, // Priority of the task.
        NULL, // Task handle.
        1);
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
        // pthread_mutex_unlock(&log_file_mutex);
        return 0;
    } else {
        add_message_fmt("Couldn't find %s", filename);
        // pthread_mutex_unlock(&log_file_mutex);
        return -1;
    }
}