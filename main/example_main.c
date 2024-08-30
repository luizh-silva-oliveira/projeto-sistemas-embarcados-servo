#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "servo_tools.h"
#include "servo_hw.h"

static const char *TAG_SERVO_TOOLS = "servo_tools test";

ServoConfig servo_config = {
    .max_angle = 180,
    .freq = 200,
    .min_width_us = 500,
    .max_width_us = 2500,
    .timer_number = LEDC_TIMER_0,
    .channel_number = LEDC_CHANNEL_0,
    .speed_mode = LEDC_HIGH_SPEED_MODE
};


/*Estados do sistema*/
typedef enum {
    GET_IMU_DATA = 0, CALCULATE_QUATERNIONS, CALCULATE_EULER_ANGLES, OUTPUT_VALUES
} States;

void app_main() {

  servo_init(&servo_config);

  static States state = GET_IMU_DATA;
  static const int quantStates = 4; //variavel que informa a quantidade de estados

  ServoAngle angle;
  ServoAngle get_angle;

  while (true){
    angle.angle = 0;
    servo_set_angle(&global_servo_config, angle);
    servo_get_angle(&global_servo_config, &get_angle);
    ESP_LOGI(TAG_SERVO_TOOLS, "%.2f\n", get_angle.angle);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    angle.angle = 180;
    servo_set_angle(&global_servo_config, angle);
    servo_get_angle(&global_servo_config, &get_angle);
    ESP_LOGI(TAG_SERVO_TOOLS, "%.2f\n", get_angle.angle);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    angle.angle = 90;
    servo_set_angle(&global_servo_config, angle);
    servo_get_angle(&global_servo_config, &get_angle);
    ESP_LOGI(TAG_SERVO_TOOLS, "%.2f\n", get_angle.angle);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    hw_servo_deinit(19);
  }
}