#include "servo_hw.h"
#include "servo_tools.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

ServoConfig servo_config = {
    .max_angle = 180,
    .freq = 200,
    .min_width_us = 500,
    .max_width_us = 2500,
    .timer_number = LEDC_TIMER_0,
    .channel_number = LEDC_CHANNEL_0,
    .speed_mode = LEDC_HIGH_SPEED_MODE
};

ServoConfig servo_config_2 = {
    .max_angle = 180,
    .freq = 200,
    .min_width_us = 500,
    .max_width_us = 2500,
    .timer_number = LEDC_TIMER_1,
    .channel_number = LEDC_CHANNEL_1,
    .speed_mode = LEDC_HIGH_SPEED_MODE
};

ServoAngle servo_angle = {
    .angle = 0
};

esp_err_t hw_servo_init(uint8_t gpio_num) {
    esp_err_t ret;
    servo_config.servo_pin = gpio_num;

    ret = servo_init(&servo_config);
    if (ret != ESP_OK) return ESP_ERR_NOT_FOUND;

    //inicia no angulo 0
    ret = servo_set_angle(&servo_config, servo_angle);
    if (ret != ESP_OK) return ESP_ERR_NOT_FOUND;
    vTaskDelay(500 / portTICK_PERIOD_MS);

    //Rotaciona até 180 graus
    servo_angle.angle = 180;
    ret = servo_set_angle(&servo_config, servo_angle);
    if (ret != ESP_OK) return ESP_ERR_NOT_FOUND;
    vTaskDelay(500 / portTICK_PERIOD_MS);

    //Rotaciona de volta para 0 graus
    servo_angle.angle = 0;
    ret = servo_set_angle(&servo_config, servo_angle);
    if (ret != ESP_OK) return ESP_ERR_NOT_FOUND;

    return ESP_OK;
}