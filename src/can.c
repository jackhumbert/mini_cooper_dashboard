#include "can.h"
#include "speed.h"
#include "tach3.h"
#include "messages.h"
#include "temp.h"
#include "gas.h"
#include "oil.h"
#include "clock.h"
#include "coolant.h"
#include "turn_signal.h"
#include "car_view.h"

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
    if (id == 0xA00) {
        add_message_fmt("Custom event received: %lu", (uint64_t*)data);
        return 0;
    }
    if (id >= 0x900) {
        return dev_process(id, data);
    }
    if (unpack_message(&can_data, id, u64_from_can_msg(data), 8, timestamp) < 0) {
        return -1;
    } else {
        switch (id) {
            case 0x153:
                decode_can_0x153_Speed(&can_data, &get_dash()->speed);
                get_changed()->speed = 1;
                // if (can_data.can_0x153_ASC_1.Speed != get_dash()->speed) {
                //     get_dash()->speed = can_data.can_0x153_ASC_1.Speed;
                //     get_changed()->speed = 1;
                // }
                return 0;
            case 0x1F0: 
            case 0x1F3: return 0; // unk4 something that counts down
            case 0x1F5: return 0; // same as 0x610
            case 0x1F8: return 0; // never changes?
            case 0x329:
                decode_can_0x329_Engine_Temp(&can_data, &get_dash()->engine_temp);
                get_changed()->engine_temp = 1;
                decode_can_0x329_Clutch_Switch(&can_data, &get_dash()->clutch_switch);
                decode_can_0x329_Throttle_Position(&can_data, &get_dash()->throttle_position);
                get_changed()->throttle_position = 1;
                return 0;
            case 0x316:
                decode_can_0x316_RPM(&can_data, &get_dash()->rpm);
                // if (can_data.can_0x316_DME1.RPM != get_dash()->rpm) {
                    // get_dash()->rpm = can_data.can_0x316_DME1.RPM;
                    get_changed()->rpm = 1;
                // }
                if (can_data.can_0x316_DME1.Key != get_dash()->key) {
                    get_dash()->key = can_data.can_0x316_DME1.Key;
                    get_changed()->key = 1;
                }
                if (can_data.can_0x316_DME1.Starter != get_dash()->starter) {
                    get_dash()->starter = can_data.can_0x316_DME1.Starter;
                    get_changed()->starter = 1;
                }
                return 0;
            case 0x336: return 0;
            case 0x338:
                decode_can_0x338_Clutch(&can_data, &get_dash()->clutch);
                return 0;
            case 0x501: return 0;
            case 0x545: return 0;
            case 0x565: return 0;
            case 0x610: return 0;
            case 0x613:
                decode_can_0x613_Fuel_Level(&can_data, &get_dash()->fuel_level);
                get_changed()->fuel_level = 1;
                decode_can_0x613_Running_Clock(&can_data, &get_dash()->running_clock);
                get_changed()->running_clock = 1;
                return 0;
            case 0x615:
                decode_can_0x615_OutsideTemp(&can_data, &get_dash()->outside_temp);
                get_changed()->outside_temp = 1;
                return 0;
            case 0x618: return 0;
            case 0x61A: return 0;
            case 0x61F: 
                if (can_data.can_0x61f_x61F.Lights != get_dash()->lights) {
                    get_dash()->lights = can_data.can_0x61f_x61F.Lights;
                    get_changed()->lights = 1;
                }
                if (can_data.can_0x61f_x61F.Headlights != get_dash()->headlights) {
                    get_dash()->headlights = can_data.can_0x61f_x61F.Headlights;
                    get_changed()->headlights = 1;
                }
                if (can_data.can_0x61f_x61F.Lights != get_dash()->lights) {
                    get_dash()->lights = can_data.can_0x61f_x61F.Lights;
                    get_changed()->lights = 1;
                }
                if (can_data.can_0x61f_x61F.LeftTurnSignal != get_dash()->left_turn_signal) {
                    get_dash()->left_turn_signal = can_data.can_0x61f_x61F.LeftTurnSignal;
                    get_changed()->left_turn_signal = 1;
                }
                if (can_data.can_0x61f_x61F.RightTurnSignal != get_dash()->right_turn_signal) {
                    get_dash()->right_turn_signal = can_data.can_0x61f_x61F.RightTurnSignal;
                    get_changed()->right_turn_signal = 1;
                }
                return 0;
        }
        return -1;
    }
}