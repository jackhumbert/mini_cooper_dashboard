#pragma once

#include "lvgl.h"
#include "font.h"

#include <math.h>
#include <pthread.h>

#define CANVAS_WIDTH  800
#define CANVAS_HEIGHT 480

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
#define GREEN_OFF lv_color_hsv_to_rgb(120, 100, 8)

#define WHITE_ON lv_color_hsv_to_rgb(0, 0, 100)
#define WHITE_HALF lv_color_hsv_to_rgb(0, 0, 50)
#define WHITE_OFF lv_color_hsv_to_rgb(0, 0, 8)

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

typedef struct dashboard_t {
	pthread_mutex_t	mutex;

    uint64_t x1F3;
    uint64_t x316;
    uint64_t x329;
    uint64_t x501;
    uint64_t x545;
    uint64_t x565;
    uint64_t x610;
    uint64_t x613;
    uint64_t x615;
    uint64_t x618;
    uint64_t x61A;
    uint64_t x61F;

    double speed;
    uint8_t tire_pressure_set;
    uint8_t brake_pedal_pressed;

    double wheel_lf_speed;
    double wheel_rf_speed;
    double wheel_lr_speed;
    double wheel_rr_speed;

    double steering_angle;
    double steering_velocity;
    uint8_t steering_angle_direction;
    uint8_t steering_velocity_direction;

    double rpm;
    uint8_t torque_loss_of_consumers;
    uint8_t torque_after_interventions;
    uint8_t torque_before_interventions;
    uint8_t key;
    uint8_t ignition_on_dme_ready;
    uint8_t starter;
    uint8_t ac_clutch;

    uint8_t driver_desired_torque;
    uint8_t throttle_position;
    double engine_temp;
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

    // 0x613
    double odometer;
    uint16_t running_clock;
    uint8_t fuel_level;

    uint8_t outside_temp;

    uint8_t brights;
    uint8_t running_lights;
    uint8_t left_turn_signal;
    uint8_t right_turn_signal;
    uint8_t handbrake;
    uint8_t cruise;

    uint8_t left_door;
    uint8_t right_door;
} dashboard_t;


#define ACTIVITY_SUCCESS 1
#define ACTIVITY_ERROR 2

typedef struct dashboard_changed_t {
    uint8_t activity;

    uint8_t x1F3;
    uint8_t x316;
    uint8_t x329;
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

    // 0x1F5
    uint8_t steering_angle;
    uint8_t steering_velocity;
    uint8_t steering_angle_direction;
    uint8_t steering_velocity_direction;

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

    uint8_t odometer;
    uint8_t running_clock;
    uint8_t fuel_level;

    uint8_t outside_temp;

    uint8_t brights;
    uint8_t running_lights;
    uint8_t left_turn_signal;
    uint8_t right_turn_signal;
    uint8_t handbrake;
    uint8_t cruise;

    uint8_t left_door;
    uint8_t right_door;
} dashboard_changed_t;

dashboard_t * get_dash(void);
dashboard_t * get_cache(void);

#define has_changed(t) (get_dash()->##t != get_cache()->##t)

dashboard_changed_t * get_changed(void);

#define update_changed(t) if (has_changed(t)) get_changed()->##t = 1

lv_obj_t * dash_create(lv_disp_t *);
void dash_loop(void);

#ifdef __cplusplus
} // extern "C"
#endif