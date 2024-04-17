#include "main.h"

QueueHandle_t bt_move_queue;

TaskHandle_t servo_move_control_task;
TaskHandle_t push_task;
TaskHandle_t rev_task;

void app_main(void)
{
    bt_move_queue = xQueueCreate(1, sizeof(size_t)); // sizeof(size_t) returns 32 (bytes) bc 32-bit architecture

    bt_init();

    configASSERT(xTaskCreate(servo_move_task_handler, "Servo_Move_Control_Task", 4096, NULL, 2, &servo_move_control_task) == pdPASS);
    configASSERT(xTaskCreate(push_task_handler, "Push_Task", 4096, NULL, 2, &push_task) == pdPASS);
    configASSERT(xTaskCreate(rev_task_handler, "Rev_Task", 4096, NULL, 2, &rev_task) == pdPASS);
}
