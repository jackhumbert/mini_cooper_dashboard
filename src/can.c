#include "can.h"
#include "messages.h"
#include "dev.h"
#include "sd_card.h"

void process_packet(uint8_t * data) {
    uint32_t id;

    for(int i=0; i<4; i++) {
        id <<= 8;
        id += data[i];
    }
                
    sd_card_logf("%08.3f R11 %08X %02X %02X %02X %02X %02X %02X %02X %02X\n", 
        xTaskGetTickCount() / 1000.0, id,
        data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11]);

    uint16_t can_id = id & 0xFFFF;
    dbcc_time_stamp_t ts = (id >> 16) & 0xFFFF;

    pthread_mutex_lock(&get_dash()->mutex);
    if (decode_can_message(ts, can_id, (uint8_t*)data + 4) < 0) {
        get_changed()->activity |= ACTIVITY_ERROR;
        sd_card_logf("%08.3f CER Could not decode latest message\n", xTaskGetTickCount() / 1000.0);
    } else {
        get_changed()->activity |= ACTIVITY_SUCCESS;
    }
    pthread_mutex_unlock(&get_dash()->mutex);
    // vTaskDelete(process_packet_task);
}

static can_obj_r53_h_t can_data;

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
                update_changed(speed);
                decode_can_0x153_ABSLight(&can_data, &get_dash()->abs_light);
                update_changed(abs_light);
                // get_dash()->x153 = *(uint64_t*)data;
                // get_changed()->x153 = 1;
                return 0;
            case 0x1F0: 
            case 0x1F3:  // unk4 something that counts down
                // get_dash()->x1F3 = *(uint64_t*)data;
                // get_changed()->x1F3 = 1;
                // decode_can_0x1f3_ForwardForce(&can_data, &get_dash()->forward_force);
                // get_dash()->forward_force -= 512;
                decode_can_0x1f3_AX_REF(&can_data, &get_dash()->forward_force);
                get_dash()->forward_force -= 512;
                update_changed(forward_force);
                // decode_can_0x1f3_LateralForce(&can_data, &get_dash()->lateral_force);
                // decode_can_0x1f3_LateralForceSign(&can_data, &get_dash()->lateral_force_sign);
                // get_dash()->lateral_force *= get_dash()->lateral_force_sign ? -1 : 1;
                decode_can_0x1f3_AY_REF(&can_data, &get_dash()->lateral_force);
                get_dash()->lateral_force -= 512;
                update_changed(lateral_force);
                return 0;
            case 0x1F5:
                // get_dash()->x1F5 = *(uint64_t*)data;
                // get_changed()->x1F5 = 1;
                decode_can_0x1f5_Steering_Angle(&can_data, &get_dash()->steering_angle);
                decode_can_0x1f5_Steering_Angle_Direction(&can_data, &get_dash()->steering_angle_direction);
                if (get_dash()->steering_angle_direction)
                    get_dash()->steering_angle *= -1;
                update_changed(steering_angle);
                return 0;
            case 0x1F8: 
                // get_dash()->x1F8 = *(uint64_t*)data;
                // get_changed()->x1F8 = 1;
                return 0;
            case 0x316:
                // get_dash()->x316 = *(uint64_t*)data;
                // get_changed()->x316 = 1;
                decode_can_0x316_RPM(&can_data, &get_dash()->rpm_fp);
                get_dash()->rpm = get_dash()->rpm_fp;
                update_changed(rpm);
                decode_can_0x316_Key(&can_data, &get_dash()->key);
                update_changed(key);
                return 0;
            case 0x329:
                // get_dash()->x329 = *(uint64_t*)data;
                // get_changed()->x329 = 1;
                decode_can_0x329_Engine_Temp(&can_data, &get_dash()->engine_temp_fp);
                get_dash()->engine_temp = get_dash()->engine_temp_fp;
                update_changed(engine_temp);
                decode_can_0x329_Throttle_Position(&can_data, &get_dash()->throttle_position);
                update_changed(throttle_position);
                decode_can_0x329_CruiseState(&can_data, &get_dash()->cruise_active);
                update_changed(cruise_active);
                return 0;
            case 0x336: 
                // get_dash()->x336 = *(uint64_t*)data;
                // get_changed()->x336 = 1;
                return 0;
            case 0x501: 
                // get_dash()->x501 = *(uint64_t*)data;
                // get_changed()->x501 = 1;
                return 0;
            case 0x545: 
                decode_can_0x545_OilTemp(&can_data, &get_dash()->oil_temp);
                update_changed(oil_temp);
                decode_can_0x545_Unk7(&can_data, &get_dash()->unk7);
                update_changed(unk7);
                decode_can_0x545_CheckEngineLight(&can_data, &get_dash()->check_engine_light);
                update_changed(check_engine_light);
                decode_can_0x545_EML(&can_data, &get_dash()->eml_light);
                update_changed(eml_light);
                decode_can_0x545_Charge_Light(&can_data, &get_dash()->charge_light);
                update_changed(charge_light);
                // get_dash()->x545 = *(uint64_t*)data;
                // get_changed()->x545 = 1;
                return 0;
            case 0x565: 
                decode_can_0x565_OilPressure(&can_data, &get_dash()->oil_pressure);
                update_changed(oil_pressure);
                // get_dash()->x565 = *(uint64_t*)data;
                // get_changed()->x565 = 1;
                return 0;
            case 0x610: 
                // get_dash()->x610 = *(uint64_t*)data;
                // get_changed()->x610 = 1;
                return 0;
            case 0x613:
                // get_dash()->x613 = *(uint64_t*)data;
                // get_changed()->x613 = 1;
                decode_can_0x613_Fuel_Level(&can_data, &get_dash()->fuel_level);
                update_changed(fuel_level);
                decode_can_0x613_LowFuelLight(&can_data, &get_dash()->low_fuel_light);
                update_changed(low_fuel_light);
                decode_can_0x613_Running_Clock(&can_data, &get_dash()->running_clock);
                update_changed(running_clock);
                // decode_can_0x613_OdometerTens(&can_data, &get_dash()->odometer_fp);
                // get_dash()->odometer = get_dash()->odometer_fp;
                // update_changed(odometer);
                return 0;
            case 0x615:
                // get_dash()->x615 = *(uint64_t*)data;
                // get_changed()->x615 = 1;
                decode_can_0x615_OutsideTemp(&can_data, &get_dash()->outside_temp);
                decode_can_0x615_OutsideTempSign(&can_data, &get_dash()->outside_temp_sign);
                if (get_dash()->outside_temp_sign)
                    get_dash()->outside_temp *= -1;
                update_changed(outside_temp);
                decode_can_0x615_Handbrake(&can_data, &get_dash()->handbrake);
                update_changed(handbrake);
                decode_can_0x615_Hood(&can_data, &get_dash()->hood);
                update_changed(hood);
                return 0;
            case 0x618: 
                // get_dash()->x618 = *(uint64_t*)data;
                // get_changed()->x618 = 1;
                return 0;
            case 0x61A: 
                // get_dash()->x61A = *(uint64_t*)data;
                // get_changed()->x61A = 1;
                decode_can_0x61a_CustomValue(&can_data, &get_dash()->custom_value);
                update_changed(custom_value);
                decode_can_0x61a_StalkState(&can_data, (uint8_t*)&get_dash()->stalk_state);
                update_changed(stalk_state);
                decode_can_0x61a_OdometerOnes(&can_data, &get_dash()->odometer);
                update_changed(odometer);
                decode_can_0x61a_OdometerTenths(&can_data, &get_dash()->trip_odometer);
                update_changed(trip_odometer);
                return 0;
            case 0x61F: 
                // get_dash()->x61F = *(uint64_t*)data;
                // get_changed()->x61F = 1;
                // only when running
                // if (can_data.can_0x61f_x61F.Handbrake != get_dash()->handbrake) {
                //     get_dash()->handbrake = can_data.can_0x61f_x61F.Handbrake;
                //     get_changed()->handbrake = 1;
                // }
                if (can_data.can_0x61f_x61F.Brights != get_dash()->brights) {
                    get_dash()->brights = can_data.can_0x61f_x61F.Brights;
                    get_changed()->brights = 1;
                }
                if (can_data.can_0x61f_x61F.RunningLights != get_dash()->running_lights) {
                    get_dash()->running_lights = can_data.can_0x61f_x61F.RunningLights;
                    get_changed()->running_lights = 1;
                }
                if (can_data.can_0x61f_x61F.LeftTurnSignal != get_dash()->left_turn_signal) {
                    get_dash()->left_turn_signal = can_data.can_0x61f_x61F.LeftTurnSignal;
                    get_changed()->left_turn_signal = 1;
                }
                if (can_data.can_0x61f_x61F.RightTurnSignal != get_dash()->right_turn_signal) {
                    get_dash()->right_turn_signal = can_data.can_0x61f_x61F.RightTurnSignal;
                    get_changed()->right_turn_signal = 1;
                }
                if (can_data.can_0x61f_x61F.Cruise != get_dash()->cruise) {
                    get_dash()->cruise = can_data.can_0x61f_x61F.Cruise;
                    get_changed()->cruise = 1;
                }
                decode_can_0x61f_InteriorLightLevel(&can_data, &get_dash()->interior_light_level);
                update_changed(interior_light_level);
                return 0;
        }
        return -1;
    }
}