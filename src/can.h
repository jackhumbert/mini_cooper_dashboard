#include "r53.h"

#ifdef __cplusplus
extern "C"
{
#endif

void process_packet(uint8_t * data);

int decode_can_message(dbcc_time_stamp_t timestamp, unsigned long id, uint8_t * data);

uint64_t u64_from_can_msg(const uint8_t m[8]);
void u64_to_can_msg(const uint64_t u, uint8_t m[8]);

#ifdef __cplusplus
} // extern "C"
#endif