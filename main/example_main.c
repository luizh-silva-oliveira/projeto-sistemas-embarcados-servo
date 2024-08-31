#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "servo_tools.h"
#include "servo_hw.h"

static const char *TAG_SERVO_TOOLS = "Angulo servo 1";
static const char *TAG_SERVO_TOOLS_2 = "Angulo servo 2";

ServoConfig servo_config = {
    .max_angle = 180,
    .freq = 200,
    .min_width_us = 500,
    .max_width_us = 2500,
    .timer_number = LEDC_TIMER_0,
    .channel_number = LEDC_CHANNEL_0,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .servo_pin = 19
};

ServoConfig servo_config_2 = {
    .max_angle = 180,
    .freq = 200,
    .min_width_us = 500,
    .max_width_us = 2500,
    .timer_number = LEDC_TIMER_0,
    .channel_number = LEDC_CHANNEL_1,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .servo_pin = 18
};

/*Estados do sistema*/
typedef enum {
    MOVE_SERVO = 0, GET_ANGLE, OUTPUT_VALUES
} States;

void app_main() {
  esp_err_t ret;

  servo_init(&servo_config);
  servo_init(&servo_config_2);

  static States state = MOVE_SERVO;
  static const int quantStates = 3; //variavel que informa a quantidade de estados

  uint8_t angle_index = 0;
  ServoAngle angles_to_move[6] = {
    {.angle = 0},
    {.angle = 20},
    {.angle = 180},
    {.angle = 100},
    {.angle = 90},
    {.angle = 45}
  };
  ServoAngle get_angle[2];

  while (true){
    for (int i = 0; i < quantStates; i++) {
      switch (state)
      {
      case MOVE_SERVO:
        ret = servo_set_angle(&servo_config, angles_to_move[angle_index]);
        ret |= servo_set_angle(&servo_config_2, angles_to_move[angle_index + 1]);
        if (ret == ESP_OK) state = GET_ANGLE;
        break;
      
      case GET_ANGLE:
        ret = servo_get_angle(&servo_config, &get_angle[0]);
        ret |= servo_get_angle(&servo_config_2, &get_angle[1]);
        if (ret == ESP_OK) state = OUTPUT_VALUES;
        break;

      case OUTPUT_VALUES:
        ESP_LOGI(TAG_SERVO_TOOLS, "%.2f\n", get_angle[0].angle);
        ESP_LOGI(TAG_SERVO_TOOLS_2, "%.2f\n", get_angle[1].angle);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        state = MOVE_SERVO;
        break;
      }
    }
    angle_index += 2;
    if (angle_index > 5) angle_index = 0;
  }
}