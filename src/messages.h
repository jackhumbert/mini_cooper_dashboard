#include "dash.h"

lv_obj_t * messages_create(lv_obj_t * parent);


#ifdef __cplusplus
extern "C"
{
#endif

void add_message(const char * message);
void add_message_fmt(const char *fmt, ...);
void dash_log(char* message);
void set_log_filename(char * name);

#ifdef __cplusplus
} // extern "C"
#endif