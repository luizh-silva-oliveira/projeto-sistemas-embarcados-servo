#ifndef SERVO_TOOLS_H
#define SERVO_TOOLS_H

#include "esp_err.h"
#include "driver/ledc.h"
#include "driver/gpio.h"

typedef struct {
    uint16_t max_angle;        //Angulo máximo do servo
    uint16_t min_width_us;     //Largura do pulso para angulo minimo, geralmente é 500us
    uint16_t max_width_us;     //Largura do pulso para angulo minimo, geralmente é 2500us
    uint32_t freq;             //Frequencia PWM
    ledc_timer_t timer_number; 
    uint8_t channel_number;    //Canal do ledc
    ledc_mode_t speed_mode;    //Velocidade do canal ledc
    gpio_num_t servo_pin;      //Pino GPIO do servo
} ServoConfig;

typedef struct {
    float angle;
} ServoAngle;

esp_err_t servo_init(ServoConfig *config);
esp_err_t servo_set_angle(ServoConfig *config, ServoAngle angle);
esp_err_t servo_get_angle(const ServoConfig *config, ServoAngle *angle);

#endif // SERVO_TOOLS_H