#define SD_MOSI 11
#define SD_MISO 13
#define SD_SCK 12
#define SD_CS 10

#ifdef __cplusplus
extern "C"
{
#endif

bool sd_card_init(void);
void sd_card_dump_messages(void);
void sd_card_clear_messages(void);
void sd_card_flush();
void sd_card_logf(const char * format, ...);
int sd_card_get_log(unsigned char * data);
void stop_logging(void);

void playback_latest_log(void);

#ifdef __cplusplus
} // extern "C"
#endif

