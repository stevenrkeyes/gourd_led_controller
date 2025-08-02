#include "hardware_id.h"

DeviceType getDeviceType() {
    // Configure ID pins as inputs with pullups
    pinMode(ID_PIN_0, INPUT_PULLUP);
    pinMode(ID_PIN_1, INPUT_PULLUP);
    
    // Small delay to let pins stabilize
    delay(10);
    
    // Read pin states (LOW = jumper to ground, HIGH = no jumper)
    bool pin0 = digitalRead(ID_PIN_0);
    bool pin1 = digitalRead(ID_PIN_1);
    
    // Calculate device ID
    uint8_t deviceId = (pin1 << 1) | pin0;
    
    // Invert bits because LOW = jumper present
    deviceId = (~deviceId) & 0x03;
    
    return (DeviceType)deviceId;
}

const char* getDeviceName(DeviceType type) {
    switch (type) {
        case TEENSY_A: return "Teensy A - Input Controller";
        case TEENSY_B: return "Teensy B - LED Strip Controller";
        case TEENSY_C: return "Teensy C - LED Strip Controller";
        default: return "Unknown Device";
    }
}

bool hasButtons(DeviceType type) {
    return (type == TEENSY_A);
}

bool hasButtonLEDs(DeviceType type) {
    return (type == TEENSY_A);
}

bool hasOctoLEDStrips(DeviceType type) {
    return (type == TEENSY_B || type == TEENSY_C);
} 