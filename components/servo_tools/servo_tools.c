#include "servo_tools.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include <esp_err.h>

static ServoConfig global_servo_config;
static uint32_t global_full_duty;

static uint32_t calculate_duty(ledc_mode_t speed_mode, float angle) {
    float angle_us = angle / global_servo_config.max_angle * (global_servo_config.max_width_us - global_servo_config.min_width_us) + global_servo_config.min_width_us;
    uint32_t duty = (uint32_t)((float)global_full_duty * (angle_us) * global_servo_config.freq / (1000000.0f));
    return duty;
}

static float calculate_angle(ledc_mode_t speed_mode, uint32_t duty) {
    float angle_us = (float)duty * 1000000.0f / (float)global_full_duty / (float)global_servo_config.freq;
    angle_us -= global_servo_config.min_width_us;
    angle_us = angle_us < 0.0f ? 0.0f : angle_us;
    float angle = angle_us * global_servo_config.max_angle / (global_servo_config.max_width_us - global_servo_config.min_width_us);
    return angle;
}

esp_err_t servo_init(ServoConfig *config) {
    esp_err_t ret;

    ledc_timer_config_t ledc_timer = {
        .clk_cfg = LEDC_AUTO_CLK,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = config->freq,
        .speed_mode = config->speed_mode,
        .timer_num = config->timer_number
    };

    ret = ledc_timer_config(&ledc_timer);
    if (ret != ESP_OK) return ESP_ERR_INVALID_ARG;

    ledc_channel_config_t ledc_ch = {
        .intr_type  = LEDC_INTR_DISABLE,
        .channel    = config->channel_number,
        .duty       = calculate_duty(config->speed_mode, 0),
        .gpio_num   = config->servo_pin,
        .speed_mode = config->speed_mode,
        .timer_sel  = config->timer_number,
        .hpoint     = 0
    };

    ret = ledc_channel_config(&ledc_ch);
    if (ret != ESP_OK) return ESP_ERR_INVALID_ARG;

    global_servo_config = *config;
    global_full_duty = (1 << LEDC_TIMER_10_BIT) - 1;
    return ESP_OK;
}

esp_err_t servo_set_angle(ServoConfig *config, ServoAngle angle) {
    esp_err_t ret;
    uint32_t duty = calculate_duty(config->speed_mode, angle.angle);
    ret = ledc_set_duty(config->speed_mode, (ledc_channel_t)config->channel_number, duty);
    ret |= ledc_update_duty(config->speed_mode, (ledc_channel_t)config->channel_number);

    if (ret != ESP_OK) return ESP_FAIL;
    return ESP_OK;
}

esp_err_t servo_get_angle(const ServoConfig *config, ServoAngle *angle) {
    uint32_t duty = ledc_get_duty(config->speed_mode, config->channel_number);
    float a = calculate_angle(config->speed_mode, duty);
    angle->angle = a;
    return ESP_OK;
}
