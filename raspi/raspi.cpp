#include "sd_card.h"
#include <stddef.h>
#include <stdint.h>
#include "dev.h"

bool sd_card_init(void) { return false; }
void stop_logging(void) { }
void sd_card_logf(const char * format, ...) { }
void start_screen_fade(void) { }

int dev_process(unsigned short id, unsigned char * data) {
    return 0;
}

void playback_latest_log(void) {

}