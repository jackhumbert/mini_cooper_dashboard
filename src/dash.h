#pragma once

#include "lvgl.h"
#include "font.h"
#include "theme.h"

#include <math.h>
#include <pthread.h>

#define CANVAS_WIDTH  800
#define CANVAS_HEIGHT 480

#define GAUGE_SIZE 120, 120
#define GAUGE_WIDTH 16

#define AMBER_ON lv_color_hsv_to_rgb(20, 100, 100)
#define AMBER_HALF lv_color_hsv_to_rgb(18, 100, 50)
#define AMBER_OFF lv_color_hsv_to_rgb(15, 100, 8)

#define RED_ON lv_color_hsv_to_rgb(0, 100, 100)
#define RED_HALF lv_color_hsv_to_rgb(2, 100, 50)
#define RED_OFF lv_color_hsv_to_rgb(5, 100, 8)

#define BLUE_ON lv_color_hsv_to_rgb(200, 100, 100)
#define BLUE_HALF lv_color_hsv_to_rgb(190, 100, 50)
#define BLUE_OFF lv_color_hsv_to_rgb(185, 100, 8)

#define GREEN_ON lv_color_hsv_to_rgb(130, 100, 100)
#define GREEN_HALF lv_color_hsv_to_rgb(125, 100, 50)
#define GREEN_OFF lv_color_hsv_to_rgb(120, 100, 4)

#define WHITE_ON lv_color_hsv_to_rgb(0, 0, 100)
#define WHITE_HALF lv_color_hsv_to_rgb(0, 0, 50)
#define WHITE_OFF lv_color_hsv_to_rgb(0, 0, 8)

#define IMPORTANT_TEXT WHITE_ON
#define DASH_BACKGROUND lv_color_hex3(0x000000)
// #define DASH_BACKGROUND lv_color_hex3(0x0F0100)

#if DASH_SIMULATION
    #define xTaskGetTickCount() 0
#endif

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#if DASH_SIMULATION
    #include <stdlib.h>
    #define ps_malloc malloc
#else
    #include <esp32-hal-psram.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

enum light_state {
    turning_off = 127,
    turning_on = 160,

};

// 22 or 11030
// 43

typedef enum {
    // outside_temp = 1,       // 197 (55f)
    // range = 2,              // 617 (64m)
    // average_consumption = 3,// 652 (27.0mpg)
    // current_consumption = 4,// 270 (--mpg)
    // average_speed = 9,      // 32766 (19.7mph)
    // current_speed


    average_speed = 1,       // 197 (19.7mph)
    outside_temp = 2,        // 617 (55f)
    range = 3,               // 652 (64m)
    average_consumption = 4, // 270 (27.0mpg)
    current_consumption = 9, // 32766 (--mpg)
} stalk_state_t;

typedef struct dashboard_t {
	pthread_mutex_t	mutex;

    uint64_t x153;
    uint64_t x1F3;
    uint64_t x1F5;
    uint64_t x1F8;
    uint64_t x316;
    uint64_t x329;
    uint64_t x336;
    uint64_t x501;
    uint64_t x545;
    uint64_t x565;
    uint64_t x610;
    uint64_t x613;
    uint64_t x615;
    uint64_t x618;
    uint64_t x61A;
    uint64_t x61F;

    // 0x153
    double speed;
    uint8_t tire_pressure_set;
    uint8_t brake_pedal_pressed;

    // 0x1F0
    double wheel_lf_speed;
    double wheel_rf_speed;
    double wheel_lr_speed;
    double wheel_rr_speed;

    // 0x1F3
    int16_t forward_force;
    int16_t lateral_force;
    uint8_t lateral_force_sign;

    // 0x1F5
    double steering_angle;
    double steering_velocity;
    uint8_t steering_angle_direction;
    uint8_t steering_velocity_direction;

    // 0x1F8
    uint8_t brake_pressure;

    double rpm_fp;
    uint16_t rpm;
    uint8_t torque_loss_of_consumers;
    uint8_t torque_after_interventions;
    uint8_t torque_before_interventions;
    uint8_t key;
    uint8_t ignition_on_dme_ready;
    uint8_t starter;
    uint8_t ac_clutch;

    uint8_t driver_desired_torque;
    uint8_t throttle_position;
    double engine_temp_fp;
    uint16_t engine_temp;
    uint8_t cycling_number;
    double atmospheric_pressure;  
    uint8_t brake_light_switch_error; 
    uint8_t clutch_switch;
    uint8_t engine_running;
    uint8_t brake_light_switch;

    uint8_t throttle_position2;
    uint8_t clutch;
    uint8_t engine_running2;
    uint8_t brake_switch;

    // 0x545
    uint16_t fuel_consumption;
    uint8_t charge_light;
    uint8_t oil_pressure_light;
    double oil_temp;
    uint8_t unk7;
    uint8_t check_engine_light;
    uint8_t eml_light;

    double oil_pressure;

    // 0x613
    double odometer_fp;
    uint64_t odometer;
    uint64_t trip_odometer;
    uint16_t running_clock;
    uint8_t fuel_level;
    uint8_t low_fuel_light;

    int8_t outside_temp;
    uint8_t outside_temp_sign;
    uint8_t hood;

    // 0x61A
    stalk_state_t stalk_state;
    uint16_t custom_value;

    uint8_t brights;
    uint8_t running_lights;
    uint8_t left_turn_signal;
    uint8_t right_turn_signal;
    uint8_t handbrake;
    uint8_t cruise;
    uint8_t cruise_active;
    uint8_t interior_light_level;

    uint8_t left_door;
    uint8_t right_door;
} dashboard_t;


#define ACTIVITY_SUCCESS 1
#define ACTIVITY_ERROR 2

typedef struct dashboard_changed_t {
    uint8_t activity;

    uint8_t x153;
    uint8_t x1F3;
    uint8_t x1F5;
    uint8_t x1F8;
    uint8_t x316;
    uint8_t x329;
    uint8_t x336;
    uint8_t x501;
    uint8_t x545;
    uint8_t x565;
    uint8_t x610;
    uint8_t x613;
    uint8_t x615;
    uint8_t x618;
    uint8_t x61A;
    uint8_t x61F;

    // 0x153
    uint8_t speed;
    uint8_t tire_pressure_set;
    uint8_t brake_pedal_pressed;

    // 0x1F0
    uint8_t wheel_lf_speed;
    uint8_t wheel_rf_speed;
    uint8_t wheel_lr_speed;
    uint8_t wheel_rr_speed;

    // 0x1F3
    uint8_t forward_force;
    uint8_t lateral_force;

    // 0x1F5
    uint8_t steering_angle;
    uint8_t steering_velocity;
    uint8_t steering_angle_direction;
    uint8_t steering_velocity_direction;

    // 0x1F8
    uint8_t brake_pressure;

    // 0x316
    uint8_t rpm;
    uint8_t torque_loss_of_consumers;
    uint8_t torque_after_interventions;
    uint8_t torque_before_interventions;
    uint8_t key;
    uint8_t ignition_on_dme_ready;
    uint8_t starter;
    uint8_t ac_clutch;

    // 0x329
    uint8_t driver_desired_torque;
    uint8_t throttle_position;
    uint8_t engine_temp;
    uint8_t cycling_number;
    uint8_t atmospheric_pressure;
    uint8_t brake_light_switch_error;
    uint8_t clutch_switch;
    uint8_t engine_running;
    uint8_t brake_light_switch;

    uint8_t throttle_position2;
    uint8_t clutch;
    uint8_t engine_running2;
    uint8_t brake_switch;

    // 0x545
    uint8_t fuel_consumption;
    uint8_t charge_light;
    uint8_t oil_pressure_light;
    uint8_t oil_temp;
    uint8_t unk7;
    uint8_t check_engine_light;
    uint8_t eml_light;

    uint8_t odometer;
    uint8_t trip_odometer;
    uint8_t running_clock;
    uint8_t fuel_level;
    uint8_t low_fuel_light;
    uint8_t oil_pressure;

    uint8_t outside_temp;
    uint8_t outside_temp_sign;
    uint8_t hood;

    // 0x61A
    uint8_t stalk_state;
    uint8_t custom_value;

    uint8_t brights;
    uint8_t running_lights;
    uint8_t left_turn_signal;
    uint8_t right_turn_signal;
    uint8_t handbrake;
    uint8_t cruise;
    uint8_t cruise_active;
    uint8_t interior_light_level;

    uint8_t left_door;
    uint8_t right_door;
} dashboard_changed_t;

dashboard_t * get_dash(void);
dashboard_t * get_cache(void);

#define has_changed(t) (get_dash()->t != get_cache()->t)

dashboard_changed_t * get_changed(void);
dashboard_changed_t * get_queued(void);

#define update_changed(t) if (has_changed(t)) get_changed()->t = 1

lv_obj_t * dash_create(lv_disp_t *);
void dash_loop(void);

#ifdef __cplusplus
} // extern "C"
#endif