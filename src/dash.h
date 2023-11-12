#pragma once

#include "lvgl.h"
#include "font.h"

#include <math.h>

#define CANVAS_WIDTH  800
#define CANVAS_HEIGHT 480

#define AMBER_ON lv_color_hsv_to_rgb(25, 100, 100)
#define AMBER_HALF lv_color_hsv_to_rgb(24, 100, 50)
#define AMBER_OFF lv_color_hsv_to_rgb(20, 100, 8)

#define RED_ON lv_color_hsv_to_rgb(0, 100, 100)
#define RED_HALF lv_color_hsv_to_rgb(2, 100, 50)
#define RED_OFF lv_color_hsv_to_rgb(5, 100, 8)

#define BLUE_ON lv_color_hsv_to_rgb(200, 100, 100)
#define BLUE_HALF lv_color_hsv_to_rgb(190, 100, 50)
#define BLUE_OFF lv_color_hsv_to_rgb(185, 100, 8)

#define GREEN_ON lv_color_hsv_to_rgb(130, 100, 100)
#define GREEN_HALF lv_color_hsv_to_rgb(125, 100, 50)
#define GREEN_OFF lv_color_hsv_to_rgb(120, 100, 8)

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

typedef struct dashboard_t {
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

    uint16_t fuel_consumption;
    uint8_t charge_light;
    uint8_t oil_pressure_light;

    double odometer;
    uint16_t running_clock;
    uint8_t fuel_level;

    uint8_t outside_temp;

    uint8_t blinker;
} dashboard_t;

dashboard_t * get_dash(void);

lv_obj_t * dash(void);
void dash_loop(void);

#ifdef __cplusplus
} // extern "C"
#endif