#include "e46.h"

#ifdef __cplusplus
extern "C"
{
#endif

int decode_can_message(dbcc_time_stamp_t timestamp, unsigned long id, uint8_t * data);

#ifdef __cplusplus
} // extern "C"
#endif