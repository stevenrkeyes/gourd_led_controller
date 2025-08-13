#ifndef PINS_TEENSY_A_H
#define PINS_TEENSY_A_H

// Teensy A - Input device with button LEDs
#define BOARD_LED_PIN 13

// Button configuration - 16 buttons
#define NUM_BUTTONS 16
const int BUTTON_PINS[NUM_BUTTONS] = {0, 1, 2, 3, 4, 5, 6, 7, 14, 15, 16, 17, 18, 19, 20, 21};

// Button LED pins (for future use - keeping original pins)
const int BUTTON_LED_PINS[] = {};

// Sensor pins
#define SENSOR_ANALOG_1_PIN A0
#define SENSOR_DIGITAL_1_PIN 8

// Eye LED pins
#define EYE_LEDS_PIN 22

#endif // PINS_TEENSY_A_H 