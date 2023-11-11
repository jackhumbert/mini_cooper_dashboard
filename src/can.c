#include "can.h"
#include "speed.h"
#include "tach3.h"
#include "messages.h"
#include "temp.h"
#include "gas.h"

static can_obj_e46_h_t can_data;

static uint64_t u64_from_can_msg(const uint8_t m[8]) {
	return ((uint64_t)m[7] << 56) | ((uint64_t)m[6] << 48) | ((uint64_t)m[5] << 40) | ((uint64_t)m[4] << 32) 
		| ((uint64_t)m[3] << 24) | ((uint64_t)m[2] << 16) | ((uint64_t)m[1] << 8) | ((uint64_t)m[0] << 0);
}

static void u64_to_can_msg(const uint64_t u, uint8_t m[8]) {
	m[7] = u >> 56;
	m[6] = u >> 48;
	m[5] = u >> 40;
	m[4] = u >> 32;
	m[3] = u >> 24;
	m[2] = u >> 16;
	m[1] = u >>  8;
	m[0] = u >>  0;
}


int decode_can_message(dbcc_time_stamp_t timestamp, unsigned long id, uint8_t * data) {
    if (unpack_message(&can_data, id, u64_from_can_msg(data), 8, timestamp) < 0) {
        return -1;
    } else {
        double value_d;
        int value_i;
        uint8_t value_u8;
        uint16_t value_u16;
        switch (id) {
            case 0x153:
                decode_can_0x153_Speed(&can_data, &value_d);
                speed_set(value_d);
                return 0;
            case 0x316:
                decode_can_0x316_RPM(&can_data, &value_d);
                tach3_set(round(value_d));
                decode_can_0x316_Key(&can_data, &value_u8);
                add_message_fmt("Key: %d", value_u8);
                decode_can_0x316_Starter(&can_data, &value_u8);
                add_message_fmt("Starter: %d", value_u8);
                return 0;
            case 0x613:
                decode_can_0x613_Fuel_Level(&can_data, &value_u8);
                gas_set(value_u8);
                decode_can_0x613_Running_Clock(&can_data, &value_u16);
                add_message_fmt("Running clock: %d", value_u16);
                return 0;
            case 0x615:
                decode_can_0x615_OutsideTemp(&can_data, &value_u8);
                temp_set(value_u8);
                return 0;
        }
        return -1;
    }
}