#include "mass_storage.h"
#include "messages.h"

// #include <Arduino.h>
// #include "Adafruit_TinyUSB.h"

// const int chipSelect = 10;

// // File system on SD Card
// SdFat sd;

// SdFile root;
// SdFile file;

// // USB Mass Storage object
// Adafruit_USBD_MSC usb_msc;

// #define SD_MOSI 11
// #define SD_MISO 13
// #define SD_SCK 12
// #define SD_CS 10

// // Set to true when PC write to flash
// bool fs_changed;

// // Callback invoked when received READ10 command.
// // Copy disk's data to buffer (up to bufsize) and
// // return number of copied bytes (must be multiple of block size)
// int32_t msc_read_cb (uint32_t lba, void* buffer, uint32_t bufsize)
// {
//   bool rc;

// #if SD_FAT_VERSION >= 20000
//   rc = sd.card()->readSectors(lba, (uint8_t*) buffer, bufsize/512);
// #else
//   rc = sd.card()->readBlocks(lba, (uint8_t*) buffer, bufsize/512);
// #endif

//   return rc ? bufsize : -1;
// }

// // Callback invoked when received WRITE10 command.
// // Process data in buffer to disk's storage and 
// // return number of written bytes (must be multiple of block size)
// int32_t msc_write_cb (uint32_t lba, uint8_t* buffer, uint32_t bufsize)
// {
//   bool rc;

// #if SD_FAT_VERSION >= 20000
//   rc = sd.card()->writeSectors(lba, buffer, bufsize/512);
// #else
//   rc = sd.card()->writeBlocks(lba, buffer, bufsize/512);
// #endif

//   return rc ? bufsize : -1;
// }

// // Callback invoked when WRITE10 command is completed (status received and accepted by host).
// // used to flush any pending cache.
// void msc_flush_cb (void)
// {
// #if SD_FAT_VERSION >= 20000
//   sd.card()->syncDevice();
// #else
//   sd.card()->syncBlocks();
// #endif

//   // clear file system's cache to force refresh
//   sd.cacheClear();

//   fs_changed = true;
// }

void mass_storage_begin(void) {

//   // Set disk vendor id, product id and revision with string up to 8, 16, 4 characters respectively
//   usb_msc.setID("Adafruit", "SD Card", "1.0");

//   // Set read write callback
//   usb_msc.setReadWriteCallback(msc_read_cb, msc_write_cb, msc_flush_cb);

//   // Still initialize MSC but tell usb stack that MSC is not ready to read/write
//   // If we don't initialize, board will be enumerated as CDC only
//   usb_msc.setUnitReady(false);
//   usb_msc.begin();

//   // Serial.begin(115200);
//   //while ( !Serial ) delay(10);   // wait for native usb

//   add_message("Adafruit TinyUSB Mass Storage SD Card example");
//   add_message_fmt("Initializing SD card ... CS = %d", chipSelect);

//   if ( !sd.begin(chipSelect, SD_SCK_MHZ(50)) )
//   {
//     add_message("initialization failed. Things to check:");
//     add_message("* is a card inserted?");
//     add_message("* is your wiring correct?");
//     add_message("* did you change the chipSelect pin to match your shield or module?");
//   }

//   // Size in blocks (512 bytes)
// #if SD_FAT_VERSION >= 20000
//   uint32_t block_count = sd.card()->sectorCount();
// #else
//   uint32_t block_count = sd.card()->cardSize();
// #endif

//   add_message_fmt("Volume size (MB):  %d", (block_count/2) / 1024);

//   // Set disk size, SD block size is always 512
//   usb_msc.setCapacity(block_count, 512);

//   // MSC is ready for read/write
//   usb_msc.setUnitReady(true);

//   fs_changed = true; // to print contents initially
}

void mass_storage_end(void) {

}