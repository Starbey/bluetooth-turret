#include "main.h"
#include "bluetooth.c"
#include "servo_control.c"

void app_main(void)
{
    bt_move_queue = xQueueCreate(1, sizeof(size_t)); // sizeof(size_t) returns 32 (bytes) bc 32-bit architecture

    bt_init();

    configASSERT(xTaskCreatePinnedToCore(servo_control_task_handler, "Servo_Control_Task", 4096, NULL, 2, &servo_control_task, 0) == pdPASS);
}
