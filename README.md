| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-P4 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# _Bluetooth Controlled Turret_

A custom motorized NERF sentry turret. Firmware is for ESP32 and made using ESP-IDF and FreeRTOS.

(04/18/2024): Can only be controlled via a bluetooth app. Auto-aim mode wil be implemented in the future.



## Features
- MG90s servomotors allow turret to rotate 180° horizontally and 90° vertically
- Two flywheels to accelerate dart; turned on/off by the ESP32 with the help of a MOSFET
- Third servo to drive a pusher arm to push darts into the flywheels
- Bluetooth task handles data sent over SPP
- Rotation control task waits for data from a queue and rotates x and y servos accordingly
- Fire control task waits for a notification and either toggles flywheel motors or pushes a dart