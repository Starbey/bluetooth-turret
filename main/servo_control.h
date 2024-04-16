
#define SERVO_MIN_PULSEWIDTH_US         500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US         2500  // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE                -90    // Minimum angle
#define SERVO_MAX_DEGREE                90    // Maximum angle

#define X_SERVO_RANGE                   180
#define Y_SERVO_RANGE                   90

#define X_SERVO_PULSE_GPIO              4        // GPIO connects to the PWM signal line
#define Y_SERVO_PULSE_GPIO              5        // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ    1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD           20000    // 20000 ticks, 20ms

#define TRACKPAD_WIDTH                  250 // trackpad width in pixels
#define TRACKPAD_HEIGHT                 250 // trackpad height in pixels

#define X_START_ANGLE                   0
#define Y_START_ANGLE                   -45

/**
 * Servo move task moves the x and y servos after a bluetooth move command is received
 * @param parameters not in use
*/
void servo_move_task_handler(void *parameters);