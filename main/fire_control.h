#include "main.h"

#define SERVO_MIN_PULSEWIDTH_US         500  // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US         2500  // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE                -90    // Minimum angle
#define SERVO_MAX_DEGREE                90    // Maximum angle

#define DC_GPIO_PIN_NO                  2

#define PUSH_GPIO_PIN_NO                19        // GPIO connects to the PWM signal line
#define SERVO_TIMEBASE_RESOLUTION_HZ    1000000  // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD           20000    // 20000 ticks, 20ms

#define SERVO_START_ANGLE               -50
#define SERVO_END_ANGLE                 10