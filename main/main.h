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

#define TRACKPAD_WIDTH                  250
#define TRACKPAD_HEIGHT                 250

QueueHandle_t bt_cmd_queue;

TaskHandle_t servo_control_task;