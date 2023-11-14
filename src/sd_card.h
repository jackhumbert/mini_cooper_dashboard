
#ifdef __cplusplus
extern "C"
{
#endif

void sd_card_init(void);
void sd_card_dump_messages(void);
void sd_card_clear_messages(void);
void sd_card_flush();
void sd_card_logf(const char * format, ...);
unsigned long sd_card_get_file_count();

#ifdef __cplusplus
} // extern "C"
#endif

int sd_card_get_log(unsigned char * data);
