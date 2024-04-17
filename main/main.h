#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "driver/mcpwm_prelude.h"

#include "time.h"
#include "sys/time.h"

extern QueueHandle_t bt_move_queue;

extern TaskHandle_t servo_move_control_task;

extern TaskHandle_t push_task;
extern TaskHandle_t rev_task;

/**
 * Initializes bluetooth
*/
void bt_init(void);

/**
 * Servo move task moves the x and y servos after a bluetooth move command is received
 * @param parameters not in use
*/
void servo_move_task_handler(void *parameters);

/**
 * Rev task starts and stops the flywheels
 * @param parameters not in use
*/
void rev_task_handler(void *parameters);

/**
 * Push task moves the dart pusher servo to push a dart into the flywheels
 * @param parameters not in use
*/
void push_task_handler(void *parameters);