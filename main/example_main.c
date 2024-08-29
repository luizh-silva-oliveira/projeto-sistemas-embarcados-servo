#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "servo_hw.h"

static const char *TAG_SENSOR_IMU = "mpu6050 sensor_imu test";
static const char *TAG_IMU_TOOLS = "mpu6050 imu_tools test";


/*Estados do sistema*/
typedef enum {
    GET_IMU_DATA = 0, CALCULATE_QUATERNIONS, CALCULATE_EULER_ANGLES, OUTPUT_VALUES
} States;

void app_main() {
  hw_servo_init(19);

  static States state = GET_IMU_DATA;
  static const int quantStates = 4; //variavel que informa a quantidade de estados

  while (true){
    
  }
}