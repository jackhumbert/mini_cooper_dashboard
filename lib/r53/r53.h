/** CAN message encoder/decoder: automatically generated - do not edit
  * Generated by dbcc: See https://github.com/howerj/dbcc */
#ifndef R53_H
#define R53_H

/* If the contents of this file have caused breaking changes for you, you could try using
   an older version of the generator. You can specify this on the command line with
   the -n option. */
#define DBCC_GENERATOR_VERSION (3)

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" { 
#endif

#ifndef PREPACK
#define PREPACK
#endif

#ifndef POSTPACK
#define POSTPACK
#endif

#ifndef DBCC_TIME_STAMP
#define DBCC_TIME_STAMP
typedef uint32_t dbcc_time_stamp_t; /* Time stamp for message; you decide on units */
#endif

#ifndef DBCC_STATUS_ENUM
#define DBCC_STATUS_ENUM
typedef enum {
	DBCC_SIG_STAT_UNINITIALIZED_E = 0, /* Message never sent/received */
	DBCC_SIG_STAT_OK_E            = 1, /* Message ok */
	DBCC_SIG_STAT_ERROR_E         = 2, /* Encode/Decode/Timestamp/Any error */
} dbcc_signal_status_e;
#endif

#define CAN_ID_ASC_1 (339) /* 0x153 */
#define CAN_ID_X1F0_ABS_1 (496) /* 0x1f0 */
#define CAN_ID_X1F3 (499) /* 0x1f3 */
#define CAN_ID_X1F5_STEERING_ANGLE_SENSOR (501) /* 0x1f5 */
#define CAN_ID_X1F8 (504) /* 0x1f8 */
#define CAN_ID_X316_DME1 (790) /* 0x316 */
#define CAN_ID_X329_DME2 (809) /* 0x329 */
#define CAN_ID_X336 (822) /* 0x336 */
#define CAN_ID_X501 (1281) /* 0x501 */
#define CAN_ID_X545_DME4 (1349) /* 0x545 */
#define CAN_ID_X565 (1381) /* 0x565 */
#define CAN_ID_X610 (1552) /* 0x610 */
#define CAN_ID_X613_INSTRUMENT_CLUSTER (1555) /* 0x613 */
#define CAN_ID_X615 (1557) /* 0x615 */
#define CAN_ID_X618 (1560) /* 0x618 */
#define CAN_ID_X61A (1562) /* 0x61a */
#define CAN_ID_X61F (1567) /* 0x61f */

typedef PREPACK struct {
	uint16_t Speed; /* scaling 0.1, offset -0.6, units none  */
	uint8_t Unk4; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Brake_Pedal_Pressed; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk6; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk7; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk8; /* scaling 1.0, offset 0.0, units none  */
	uint8_t CheckEngineLight; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk32; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk60; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Tyre_Pressure_Set; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk63; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk2; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x153_ASC_1_t;

typedef PREPACK struct {
	uint16_t Wheel_1_Left_Front_Speed; /* scaling 0.1, offset 0.0, units none  */
	uint16_t Wheel_2_Right_Front_Speed; /* scaling 0.1, offset 0.0, units none  */
	uint16_t Wheel_3_Left_Rear_Speed; /* scaling 0.1, offset 0.0, units none  */
	uint16_t Wheel_4_Right_Rear_Speed; /* scaling 0.1, offset 0.0, units none  */
} POSTPACK can_0x1f0_x1F0_ABS_1_t;

typedef PREPACK struct {
	uint16_t ForwardForce; /* scaling 1.0, offset 0.0, units none  */
	uint16_t unk38; /* scaling 1.0, offset 0.0, units none  */
	uint16_t LateralForce; /* scaling 1.0, offset 0.0, units none  */
	uint8_t LateralForceSign; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk59; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x1f3_x1F3_t;

typedef PREPACK struct {
	uint16_t Steering_Angle; /* scaling 0.0, offset 0.0, units none  */
	uint16_t Steering_Angle_Velocity; /* scaling 0.0, offset 0.0, units none  */
	uint8_t Steering_Counter; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Steering_Angle_Direction; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Steering_Angle_Velocity_Direction; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk39; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk40; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x1f5_x1F5_Steering_Angle_Sensor_t;

typedef PREPACK struct {
	uint8_t Unk0; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Unk2; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x1f8_x1F8_t;

typedef PREPACK struct {
	uint16_t RPM; /* scaling 0.2, offset 0.0, units none  */
	uint8_t Key; /* scaling 1.0, offset 0.0, units none  */
	uint8_t AC_Clutch; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x316_x316_DME1_t;

typedef PREPACK struct {
	uint8_t Cycling_Number; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Engine_Temp; /* scaling 0.8, offset -48.4, units deg_c  */
	uint8_t Throttle_Position; /* scaling 1.0, offset 0.0, units %  */
} POSTPACK can_0x329_x329_DME2_t;

typedef PREPACK struct {
	uint16_t Unk0; /* scaling 1.0, offset 0.0, units none  */
	uint16_t Unk2; /* scaling 1.0, offset 0.0, units none  */
	uint16_t Unk4; /* scaling 1.0, offset 0.0, units none  */
	uint16_t Unk6; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x336_x336_t;

typedef PREPACK struct {
	uint8_t Unk1; /* scaling 1.0, offset 0.0, units none  */
	int8_t Unk3; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk40; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Unk56; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x501_x501_t;

typedef PREPACK struct {
	uint16_t Fuel_Consumption; /* scaling 1.0, offset 0.0, units mpg  */
	uint8_t OilTemp; /* scaling 1.0, offset -48.4, units deg_c  */
	uint8_t Unk7; /* scaling 1.0, offset 0.0, units none  */
	uint8_t CheckGasCap; /* scaling 1.0, offset 0.0, units none  */
	uint8_t OilLevel; /* scaling 1.0, offset 0.0, units none  */
	uint8_t OverheatLight; /* scaling 1.0, offset 0.0, units none  */
	uint8_t CheckEngineLight; /* scaling 1.0, offset 0.0, units none  */
	uint8_t RPM6_5k; /* scaling 1.0, offset 0.0, units none  */
	uint8_t RPM5_5k; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Charge_Light; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Oil_Pressure_Light; /* scaling 1.0, offset 0.0, units none  */
	uint8_t RPM7k; /* scaling 1.0, offset 0.0, units none  */
	uint8_t CruiseLight; /* scaling 1.0, offset 0.0, units none  */
	uint8_t EML; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x545_x545_DME4_t;

typedef PREPACK struct {
	uint8_t OilPressure; /* scaling 2.0, offset 0.0, units mPa  */
	uint8_t IDK2_unk4; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Unk7; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Unk4; /* scaling 1.0, offset 0.0, units none  */
	uint8_t IDK2_unk6; /* scaling 1.0, offset 0.0, units none  */
	uint8_t IDK2_unk1; /* scaling 1.0, offset 0.0, units none  */
	uint8_t IDK2_unk5; /* scaling 1.0, offset 0.0, units none  */
	uint8_t IDK2_unk2; /* scaling 1.0, offset 0.0, units none  */
	uint8_t IDK2_unk3; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x565_x565_t;

typedef PREPACK struct {
	uint64_t VIN; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x610_x610_t;

typedef PREPACK struct {
	uint16_t Odometer; /* scaling 10.0, offset 0.0, units none  */
	uint16_t Running_Clock; /* scaling 1.0, offset 0.0, units minutes  */
	uint8_t Fuel_Level; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Unk40; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Unk46; /* scaling 1.0, offset 0.0, units none  */
	int8_t Unk52; /* scaling 1.0, offset 0.0, units none  */
	uint8_t LowFuelLight; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x613_x613_Instrument_Cluster_t;

typedef PREPACK struct {
	int8_t OutsideTemp; /* scaling 1.0, offset 0.0, units deg_c  */
	uint8_t AC_Status; /* scaling 1.0, offset 0.0, units none  */
	uint8_t RunningLights2; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Hood; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Handbrake; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x615_x615_t;

typedef PREPACK struct {
	uint8_t Unk2; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x618_x618_t;

typedef PREPACK struct {
	uint16_t CustomValue; /* scaling 1.0, offset 0.0, units none  */
	uint8_t StalkState; /* scaling 1.0, offset 0.0, units none  */
	uint8_t SyncClockNotRunning; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk27; /* scaling 1.0, offset 0.0, units none  */
	uint8_t SyncClockRunning; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk63; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x61a_x61A_t;

typedef PREPACK struct {
	uint8_t InteriorLightLevel; /* scaling 1.0, offset 0.0, units none  */
	uint8_t AutomaticLights; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk19; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk17; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk16; /* scaling 1.0, offset 0.0, units none  */
	uint8_t LeftTurnSignal; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Brights; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unkStartup24; /* scaling 1.0, offset 0.0, units none  */
	uint8_t RunningHandbrake; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unkStartup26; /* scaling 1.0, offset 0.0, units none  */
	uint8_t RightTurnSignal; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unkStartup28; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unkStartup29; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unkStartup30; /* scaling 1.0, offset 0.0, units none  */
	uint8_t Cruise; /* scaling 1.0, offset 0.0, units none  */
	uint8_t RunningLights; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unk38; /* scaling 1.0, offset 0.0, units none  */
	uint8_t unkStartup18; /* scaling 1.0, offset 0.0, units none  */
} POSTPACK can_0x61f_x61F_t;

typedef PREPACK struct {
	dbcc_time_stamp_t can_0x153_ASC_1_time_stamp_rx;
	dbcc_time_stamp_t can_0x1f0_x1F0_ABS_1_time_stamp_rx;
	dbcc_time_stamp_t can_0x1f3_x1F3_time_stamp_rx;
	dbcc_time_stamp_t can_0x1f5_x1F5_Steering_Angle_Sensor_time_stamp_rx;
	dbcc_time_stamp_t can_0x1f8_x1F8_time_stamp_rx;
	dbcc_time_stamp_t can_0x316_x316_DME1_time_stamp_rx;
	dbcc_time_stamp_t can_0x329_x329_DME2_time_stamp_rx;
	dbcc_time_stamp_t can_0x336_x336_time_stamp_rx;
	dbcc_time_stamp_t can_0x501_x501_time_stamp_rx;
	dbcc_time_stamp_t can_0x545_x545_DME4_time_stamp_rx;
	dbcc_time_stamp_t can_0x565_x565_time_stamp_rx;
	dbcc_time_stamp_t can_0x610_x610_time_stamp_rx;
	dbcc_time_stamp_t can_0x613_x613_Instrument_Cluster_time_stamp_rx;
	dbcc_time_stamp_t can_0x615_x615_time_stamp_rx;
	dbcc_time_stamp_t can_0x618_x618_time_stamp_rx;
	dbcc_time_stamp_t can_0x61a_x61A_time_stamp_rx;
	dbcc_time_stamp_t can_0x61f_x61F_time_stamp_rx;
	unsigned can_0x153_ASC_1_status : 2;
	unsigned can_0x153_ASC_1_tx : 1;
	unsigned can_0x153_ASC_1_rx : 1;
	unsigned can_0x1f0_x1F0_ABS_1_status : 2;
	unsigned can_0x1f0_x1F0_ABS_1_tx : 1;
	unsigned can_0x1f0_x1F0_ABS_1_rx : 1;
	unsigned can_0x1f3_x1F3_status : 2;
	unsigned can_0x1f3_x1F3_tx : 1;
	unsigned can_0x1f3_x1F3_rx : 1;
	unsigned can_0x1f5_x1F5_Steering_Angle_Sensor_status : 2;
	unsigned can_0x1f5_x1F5_Steering_Angle_Sensor_tx : 1;
	unsigned can_0x1f5_x1F5_Steering_Angle_Sensor_rx : 1;
	unsigned can_0x1f8_x1F8_status : 2;
	unsigned can_0x1f8_x1F8_tx : 1;
	unsigned can_0x1f8_x1F8_rx : 1;
	unsigned can_0x316_x316_DME1_status : 2;
	unsigned can_0x316_x316_DME1_tx : 1;
	unsigned can_0x316_x316_DME1_rx : 1;
	unsigned can_0x329_x329_DME2_status : 2;
	unsigned can_0x329_x329_DME2_tx : 1;
	unsigned can_0x329_x329_DME2_rx : 1;
	unsigned can_0x336_x336_status : 2;
	unsigned can_0x336_x336_tx : 1;
	unsigned can_0x336_x336_rx : 1;
	unsigned can_0x501_x501_status : 2;
	unsigned can_0x501_x501_tx : 1;
	unsigned can_0x501_x501_rx : 1;
	unsigned can_0x545_x545_DME4_status : 2;
	unsigned can_0x545_x545_DME4_tx : 1;
	unsigned can_0x545_x545_DME4_rx : 1;
	unsigned can_0x565_x565_status : 2;
	unsigned can_0x565_x565_tx : 1;
	unsigned can_0x565_x565_rx : 1;
	unsigned can_0x610_x610_status : 2;
	unsigned can_0x610_x610_tx : 1;
	unsigned can_0x610_x610_rx : 1;
	unsigned can_0x613_x613_Instrument_Cluster_status : 2;
	unsigned can_0x613_x613_Instrument_Cluster_tx : 1;
	unsigned can_0x613_x613_Instrument_Cluster_rx : 1;
	unsigned can_0x615_x615_status : 2;
	unsigned can_0x615_x615_tx : 1;
	unsigned can_0x615_x615_rx : 1;
	unsigned can_0x618_x618_status : 2;
	unsigned can_0x618_x618_tx : 1;
	unsigned can_0x618_x618_rx : 1;
	unsigned can_0x61a_x61A_status : 2;
	unsigned can_0x61a_x61A_tx : 1;
	unsigned can_0x61a_x61A_rx : 1;
	unsigned can_0x61f_x61F_status : 2;
	unsigned can_0x61f_x61F_tx : 1;
	unsigned can_0x61f_x61F_rx : 1;
	can_0x153_ASC_1_t can_0x153_ASC_1;
	can_0x1f0_x1F0_ABS_1_t can_0x1f0_x1F0_ABS_1;
	can_0x1f3_x1F3_t can_0x1f3_x1F3;
	can_0x1f5_x1F5_Steering_Angle_Sensor_t can_0x1f5_x1F5_Steering_Angle_Sensor;
	can_0x1f8_x1F8_t can_0x1f8_x1F8;
	can_0x316_x316_DME1_t can_0x316_x316_DME1;
	can_0x329_x329_DME2_t can_0x329_x329_DME2;
	can_0x336_x336_t can_0x336_x336;
	can_0x501_x501_t can_0x501_x501;
	can_0x545_x545_DME4_t can_0x545_x545_DME4;
	can_0x565_x565_t can_0x565_x565;
	can_0x610_x610_t can_0x610_x610;
	can_0x613_x613_Instrument_Cluster_t can_0x613_x613_Instrument_Cluster;
	can_0x615_x615_t can_0x615_x615;
	can_0x618_x618_t can_0x618_x618;
	can_0x61a_x61A_t can_0x61a_x61A;
	can_0x61f_x61F_t can_0x61f_x61F;
} POSTPACK can_obj_r53_h_t;

int unpack_message(can_obj_r53_h_t *o, const unsigned long id, uint64_t data, uint8_t dlc, dbcc_time_stamp_t time_stamp);
int pack_message(can_obj_r53_h_t *o, const unsigned long id, uint64_t *data);
int print_message(const can_obj_r53_h_t *o, const unsigned long id, FILE *output);

int decode_can_0x153_Speed(const can_obj_r53_h_t *o, double *out);
int encode_can_0x153_Speed(can_obj_r53_h_t *o, double in);
int decode_can_0x153_Unk4(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_Unk4(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_Brake_Pedal_Pressed(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_Brake_Pedal_Pressed(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_unk6(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_unk6(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_unk7(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_unk7(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_unk8(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_unk8(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_CheckEngineLight(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_CheckEngineLight(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_unk32(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_unk32(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_unk60(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_unk60(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_Tyre_Pressure_Set(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_Tyre_Pressure_Set(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_unk63(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_unk63(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x153_unk2(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x153_unk2(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x1f0_Wheel_1_Left_Front_Speed(const can_obj_r53_h_t *o, double *out);
int encode_can_0x1f0_Wheel_1_Left_Front_Speed(can_obj_r53_h_t *o, double in);
int decode_can_0x1f0_Wheel_2_Right_Front_Speed(const can_obj_r53_h_t *o, double *out);
int encode_can_0x1f0_Wheel_2_Right_Front_Speed(can_obj_r53_h_t *o, double in);
int decode_can_0x1f0_Wheel_3_Left_Rear_Speed(const can_obj_r53_h_t *o, double *out);
int encode_can_0x1f0_Wheel_3_Left_Rear_Speed(can_obj_r53_h_t *o, double in);
int decode_can_0x1f0_Wheel_4_Right_Rear_Speed(const can_obj_r53_h_t *o, double *out);
int encode_can_0x1f0_Wheel_4_Right_Rear_Speed(can_obj_r53_h_t *o, double in);


int decode_can_0x1f3_ForwardForce(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x1f3_ForwardForce(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x1f3_unk38(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x1f3_unk38(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x1f3_LateralForce(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x1f3_LateralForce(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x1f3_LateralForceSign(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f3_LateralForceSign(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x1f3_unk59(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f3_unk59(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x1f5_Steering_Angle(const can_obj_r53_h_t *o, double *out);
int encode_can_0x1f5_Steering_Angle(can_obj_r53_h_t *o, double in);
int decode_can_0x1f5_Steering_Angle_Velocity(const can_obj_r53_h_t *o, double *out);
int encode_can_0x1f5_Steering_Angle_Velocity(can_obj_r53_h_t *o, double in);
int decode_can_0x1f5_Steering_Counter(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f5_Steering_Counter(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x1f5_Steering_Angle_Direction(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f5_Steering_Angle_Direction(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x1f5_Steering_Angle_Velocity_Direction(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f5_Steering_Angle_Velocity_Direction(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x1f5_unk39(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f5_unk39(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x1f5_unk40(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f5_unk40(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x1f8_Unk0(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f8_Unk0(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x1f8_Unk2(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x1f8_Unk2(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x316_RPM(const can_obj_r53_h_t *o, double *out);
int encode_can_0x316_RPM(can_obj_r53_h_t *o, double in);
int decode_can_0x316_Key(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x316_Key(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x316_AC_Clutch(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x316_AC_Clutch(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x329_Cycling_Number(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x329_Cycling_Number(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x329_Engine_Temp(const can_obj_r53_h_t *o, double *out);
int encode_can_0x329_Engine_Temp(can_obj_r53_h_t *o, double in);
int decode_can_0x329_Throttle_Position(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x329_Throttle_Position(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x336_Unk0(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x336_Unk0(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x336_Unk2(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x336_Unk2(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x336_Unk4(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x336_Unk4(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x336_Unk6(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x336_Unk6(can_obj_r53_h_t *o, uint16_t in);


int decode_can_0x501_Unk1(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x501_Unk1(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x501_Unk3(const can_obj_r53_h_t *o, int8_t *out);
int encode_can_0x501_Unk3(can_obj_r53_h_t *o, int8_t in);
int decode_can_0x501_unk40(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x501_unk40(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x501_Unk56(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x501_Unk56(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x545_Fuel_Consumption(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x545_Fuel_Consumption(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x545_OilTemp(const can_obj_r53_h_t *o, double *out);
int encode_can_0x545_OilTemp(can_obj_r53_h_t *o, double in);
int decode_can_0x545_Unk7(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_Unk7(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_CheckGasCap(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_CheckGasCap(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_OilLevel(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_OilLevel(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_OverheatLight(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_OverheatLight(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_CheckEngineLight(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_CheckEngineLight(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_RPM6_5k(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_RPM6_5k(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_RPM5_5k(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_RPM5_5k(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_Charge_Light(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_Charge_Light(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_Oil_Pressure_Light(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_Oil_Pressure_Light(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_RPM7k(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_RPM7k(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_CruiseLight(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_CruiseLight(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x545_EML(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x545_EML(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x565_OilPressure(const can_obj_r53_h_t *o, double *out);
int encode_can_0x565_OilPressure(can_obj_r53_h_t *o, double in);
int decode_can_0x565_IDK2_unk4(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x565_IDK2_unk4(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x565_Unk7(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x565_Unk7(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x565_Unk4(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x565_Unk4(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x565_IDK2_unk6(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x565_IDK2_unk6(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x565_IDK2_unk1(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x565_IDK2_unk1(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x565_IDK2_unk5(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x565_IDK2_unk5(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x565_IDK2_unk2(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x565_IDK2_unk2(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x565_IDK2_unk3(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x565_IDK2_unk3(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x610_VIN(const can_obj_r53_h_t *o, uint64_t *out);
int encode_can_0x610_VIN(can_obj_r53_h_t *o, uint64_t in);


int decode_can_0x613_Odometer(const can_obj_r53_h_t *o, double *out);
int encode_can_0x613_Odometer(can_obj_r53_h_t *o, double in);
int decode_can_0x613_Running_Clock(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x613_Running_Clock(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x613_Fuel_Level(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x613_Fuel_Level(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x613_Unk40(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x613_Unk40(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x613_Unk46(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x613_Unk46(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x613_Unk52(const can_obj_r53_h_t *o, int8_t *out);
int encode_can_0x613_Unk52(can_obj_r53_h_t *o, int8_t in);
int decode_can_0x613_LowFuelLight(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x613_LowFuelLight(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x615_OutsideTemp(const can_obj_r53_h_t *o, int8_t *out);
int encode_can_0x615_OutsideTemp(can_obj_r53_h_t *o, int8_t in);
int decode_can_0x615_AC_Status(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x615_AC_Status(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x615_RunningLights2(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x615_RunningLights2(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x615_Hood(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x615_Hood(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x615_Handbrake(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x615_Handbrake(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x618_Unk2(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x618_Unk2(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x61a_CustomValue(const can_obj_r53_h_t *o, uint16_t *out);
int encode_can_0x61a_CustomValue(can_obj_r53_h_t *o, uint16_t in);
int decode_can_0x61a_StalkState(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61a_StalkState(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61a_SyncClockNotRunning(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61a_SyncClockNotRunning(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61a_unk27(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61a_unk27(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61a_SyncClockRunning(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61a_SyncClockRunning(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61a_unk63(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61a_unk63(can_obj_r53_h_t *o, uint8_t in);


int decode_can_0x61f_InteriorLightLevel(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_InteriorLightLevel(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_AutomaticLights(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_AutomaticLights(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unk19(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unk19(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unk17(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unk17(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unk16(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unk16(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_LeftTurnSignal(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_LeftTurnSignal(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_Brights(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_Brights(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unkStartup24(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unkStartup24(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_RunningHandbrake(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_RunningHandbrake(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unkStartup26(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unkStartup26(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_RightTurnSignal(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_RightTurnSignal(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unkStartup28(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unkStartup28(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unkStartup29(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unkStartup29(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unkStartup30(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unkStartup30(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_Cruise(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_Cruise(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_RunningLights(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_RunningLights(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unk38(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unk38(can_obj_r53_h_t *o, uint8_t in);
int decode_can_0x61f_unkStartup18(const can_obj_r53_h_t *o, uint8_t *out);
int encode_can_0x61f_unkStartup18(can_obj_r53_h_t *o, uint8_t in);


#ifdef __cplusplus
} 
#endif

#endif
