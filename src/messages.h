#include "dash.h"

lv_obj_t * messages_create(lv_obj_t * parent);


#ifdef __cplusplus
extern "C"
{
#endif

void add_message(const char * message);
void add_message_fmt(char *fmt, ...);
void dash_log(char* message);

#ifdef __cplusplus
} // extern "C"
#endif