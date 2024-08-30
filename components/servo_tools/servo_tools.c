#include "servo_tools.h"
#include "servo_hw.h"
#include <esp_err.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

ServoAngle servo_angle = {
    .angle = 0
};

static const uint32_t global_full_duty = (1 << LEDC_TIMER_10_BIT) - 1;

static void calculate_duty(float angle) {
    float angle_us = angle / global_servo_config.max_angle * (global_servo_config.max_width_us - global_servo_config.min_width_us) + global_servo_config.min_width_us;
    uint32_t duty = (uint32_t)((float) global_full_duty * (angle_us) * global_servo_config.freq / (1000000.0f));
    hw_servo_set_pulse_width(19, duty);
}

static float calculate_angle(uint32_t duty) {
    float angle_us = (float)duty * 1000000.0f / (float)global_full_duty / (float)global_servo_config.freq;
    angle_us -= global_servo_config.min_width_us;
    angle_us = angle_us < 0.0f ? 0.0f : angle_us;
    float angle = angle_us * global_servo_config.max_angle / (global_servo_config.max_width_us - global_servo_config.min_width_us);
    return angle;
}

esp_err_t servo_init(ServoConfig *config) {
    esp_err_t ret;

    global_servo_config = *config;
    calculate_duty(0); //Inicia o servo em 0 graus
    hw_servo_init(19); //Iniciando PWM na GPIO19

    //inicia no angulo 0
    ret = servo_set_angle(&global_servo_config, servo_angle);
    if (ret != ESP_OK) return ESP_ERR_NOT_FOUND;
    vTaskDelay(500 / portTICK_PERIOD_MS);

    //Rotaciona até 180 graus
    servo_angle.angle = 180;
    ret = servo_set_angle(&global_servo_config, servo_angle);
    if (ret != ESP_OK) return ESP_ERR_NOT_FOUND;
    vTaskDelay(500 / portTICK_PERIOD_MS);

    //Rotaciona de volta para 0 graus
    servo_angle.angle = 0;
    ret = servo_set_angle(&global_servo_config, servo_angle);
    if (ret != ESP_OK) return ESP_ERR_NOT_FOUND;

    return ESP_OK;
}

esp_err_t servo_set_angle(ServoConfig *config, ServoAngle angle) {
    esp_err_t ret;
    calculate_duty(angle.angle);
    uint32_t duty = config->duty;
    if (stop_pwm) return ESP_OK; //Caso eu tenha desligado o PWM não é mais para atualizar o duty cycle
    ret = ledc_set_duty(config->speed_mode, (ledc_channel_t)config->channel_number, duty);
    ret |= ledc_update_duty(config->speed_mode, (ledc_channel_t)config->channel_number);

    if (ret != ESP_OK) return ESP_FAIL;
    return ESP_OK;
}

esp_err_t servo_get_angle(const ServoConfig *config, ServoAngle *angle) {
    uint32_t duty = ledc_get_duty(config->speed_mode, config->channel_number);
    float a = calculate_angle(duty);
    angle->angle = a;
    return ESP_OK;
}
