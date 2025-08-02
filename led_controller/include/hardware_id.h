#ifndef HARDWARE_ID_H
#define HARDWARE_ID_H

#include <Arduino.h>

// Hardware identification pins
#define ID_PIN_0  22  // LSB
#define ID_PIN_1  23  // MSB

// Device IDs based on pin states
#define DEVICE_ID_TEENSY_A  0  // Both pins HIGH (pullup, no jumpers)
#define DEVICE_ID_TEENSY_B  1  // Pin 0 LOW, Pin 1 HIGH (jumper on pin 22)
#define DEVICE_ID_TEENSY_C  2  // Pin 0 HIGH, Pin 1 LOW (jumper on pin 23)
#define DEVICE_ID_UNKNOWN   3  // Both pins LOW (both jumpers - error state)

enum DeviceType {
    TEENSY_A = DEVICE_ID_TEENSY_A,
    TEENSY_B = DEVICE_ID_TEENSY_B,
    TEENSY_C = DEVICE_ID_TEENSY_C,
    UNKNOWN = DEVICE_ID_UNKNOWN
};

// Functions
DeviceType getDeviceType();
const char* getDeviceName(DeviceType type);
bool hasButtons(DeviceType type);
bool hasButtonLEDs(DeviceType type);
bool hasOctoLEDStrips(DeviceType type);

#endif // HARDWARE_ID_H 