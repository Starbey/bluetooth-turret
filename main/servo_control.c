#include "servo_control.h"

static const char *TAG = "servo_control";

static inline uint32_t example_angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

static int get_delay(int deg, int prev_deg) {
    return (int) ( (float) abs(deg - prev_deg) / 60  * 200 );
}

void servo_move_task_handler(void *parameters)
{
    ESP_LOGI(TAG, "Create timer and operator");
    mcpwm_timer_handle_t x_timer = NULL;
    mcpwm_timer_config_t x_timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    mcpwm_timer_handle_t y_timer = NULL;
    mcpwm_timer_config_t y_timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };

    ESP_ERROR_CHECK(mcpwm_new_timer(&x_timer_config, &x_timer));
    ESP_ERROR_CHECK(mcpwm_new_timer(&y_timer_config, &y_timer));

    mcpwm_oper_handle_t x_oper = NULL;
    mcpwm_operator_config_t x_operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&x_operator_config, &x_oper));

    mcpwm_oper_handle_t y_oper = NULL;
    mcpwm_operator_config_t y_operator_config = {
        .group_id = 0, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&y_operator_config, &y_oper));

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(x_oper, x_timer));
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(y_oper, y_timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    mcpwm_cmpr_handle_t x_comparator = NULL;
    mcpwm_comparator_config_t x_comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(x_oper, &x_comparator_config, &x_comparator));
    mcpwm_cmpr_handle_t y_comparator = NULL;
    mcpwm_comparator_config_t y_comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(y_oper, &y_comparator_config, &y_comparator));

    mcpwm_gen_handle_t x_generator = NULL;
    mcpwm_generator_config_t x_generator_config = {
        .gen_gpio_num = X_SERVO_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(x_oper, &x_generator_config, &x_generator));

    mcpwm_gen_handle_t y_generator = NULL;
    mcpwm_generator_config_t y_generator_config = {
        .gen_gpio_num = Y_SERVO_PULSE_GPIO,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(y_oper, &y_generator_config, &y_generator));

    // set the initial compare value, so that the servo will spin to the center position
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(x_comparator, example_angle_to_compare(X_START_ANGLE)));
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(y_comparator, example_angle_to_compare(Y_START_ANGLE)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(x_generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(y_generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(x_generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, x_comparator, MCPWM_GEN_ACTION_LOW)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(y_generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, x_comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(x_timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(x_timer, MCPWM_TIMER_START_NO_STOP));
    ESP_ERROR_CHECK(mcpwm_timer_enable(y_timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(y_timer, MCPWM_TIMER_START_NO_STOP));

    int8_t x_deg = 0;
    int8_t y_deg = 0;

    while (1) {
        uint8_t bt_data[2];
        int8_t prev_x_deg = x_deg;
        int delay;

        xQueueReceive(bt_move_queue, bt_data, portMAX_DELAY);
        x_deg = (int8_t) bt_data[1] - TRACKPAD_WIDTH / 2;
        x_deg = SERVO_MAX_DEGREE * ((float) x_deg / TRACKPAD_WIDTH * 2) * -1; // x-servo

        y_deg = (int8_t) ((float) Y_SERVO_RANGE / TRACKPAD_HEIGHT * bt_data[0]); // y-servo

        ESP_LOGI(TAG, "x: %d, y: %d", x_deg, y_deg);
        
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(x_comparator, example_angle_to_compare(x_deg)));
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(y_comparator, example_angle_to_compare(y_deg)));

        delay = get_delay(x_deg, prev_x_deg);

        ESP_LOGI(TAG, "delay: %d", delay);

        /* task delay scales linearly with angular displacement;
          makes movement smooth while preventing the motor from freezing 
          due to a receiving a request while moving */
        vTaskDelay( (TickType_t) pdMS_TO_TICKS(delay) );     
    }
}
