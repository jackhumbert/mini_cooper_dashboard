#include "sd_card.h"
#include <SPI.h>
#include <SD.h>
#include "messages.h"

// SD CARD

#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10

void sd_card_init() {
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    delay(100);
    if (!SD.begin(SD_CS)) {
        add_message("Card Mount Failed");
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        add_message("No TF card attached");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    add_message_fmt("TF Card Size: %lluMB", cardSize);
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
}

void sd_card_dump_messages(void) {
    add_message("Dumping messages to serial");
    lv_task_handler();
    File file = SD.open("/log.txt", FILE_READ);
    while (file.available()) {
        Serial.write(file.read());
    }
    file.close();
}

void sd_card_clear_messages(void) {
    add_message("Clearing messages");
    lv_task_handler();
    {
        File file = SD.open("/log.txt", FILE_WRITE);
        file.write((uint8_t *)"", 0);
        file.close();
    }
}