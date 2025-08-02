#ifndef PINS_TEENSY_A_H
#define PINS_TEENSY_A_H

// Teensy A - Input device with button LEDs
#define BOARD_LED_PIN 13

// Button configuration
#define NUM_BUTTONS 6  // Change this to scale up to 16 later
const int BUTTON_PINS[NUM_BUTTONS] = {0, 1, 2, 3, 4, 5};

// Button LED pins (moved to higher pins)
const int BUTTON_LED_PINS[NUM_BUTTONS] = {6, 7, 14, 15, 16, 17};

// Sensor pins
#define SENSOR_ANALOG_1_PIN A0
#define SENSOR_DIGITAL_1_PIN 8

#endif // PINS_TEENSY_A_H 