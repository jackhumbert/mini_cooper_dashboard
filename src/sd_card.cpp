#include "sd_card.h"
#include <SPI.h>
#include <SD.h>
#include "messages.h"

// SD CARD

#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10

static char log_filename[20];
static File log_file;

static int get_file_count() {
	File d = SD.open("/");
    if (!d)
        return 0;
	int count_files = 0;
	while (true) {
 		File entry =  d.openNextFile();
		if(!entry) {
			return count_files;
		}
		String file_name = entry.name();
		if( file_name.indexOf('~') != 0 && !entry.isDirectory()) {
			count_files++;
		}
	}
}

void sd_card_init() {
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    // delay(100);
    if (!SD.begin(SD_CS)) {
        add_message("SD Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        add_message("No SD card attached");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    add_message_fmt("SD Card Size: %lluMB", cardSize);

    sprintf(log_filename, "/log_%08X.crtd", get_file_count());
    add_message_fmt("Log file: %s", log_filename);
    log_file = SD.open(log_filename, FILE_APPEND);
    log_file.printf("%08.3f CXX R53 Custom Dash by Jack Humbert\n", xTaskGetTickCount() / 1000.0);
    log_file.flush();
}

File * sd_card_get_log_file(void) {
    return &log_file;
}

void sd_card_dump_messages(void) {
    add_message("Dumping messages to serial");
    lv_task_handler();
    log_file.close();

    File file = SD.open(log_filename, FILE_READ);
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();

    log_file = SD.open(log_filename, FILE_APPEND);
}

void sd_card_clear_messages(void) {
    add_message("Closing log file");
    lv_task_handler();
    log_file.close();
}