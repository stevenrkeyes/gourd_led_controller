#include <Arduino.h>
#include "config.h"
#include "shared/communication.h"
#include "hardware_id.h"

// Include all possible modules
#include "input_devices/buttons.h"
#include "input_devices/button_leds.h"
#include "led_controllers/octo_led_strips.h"
#include "led_controllers/effects.h"

// Global device type
DeviceType deviceType;
unsigned long previousMillis = 0;
const unsigned long interval = 500;
bool ledState = false;

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    
    // Determine device type from hardware
    deviceType = getDeviceType();
    
    Serial.print("Starting ");
    Serial.println(getDeviceName(deviceType));
    
    if (deviceType == UNKNOWN) {
        Serial.println("ERROR: Unknown device type! Check jumper configuration.");
        while(1) {
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
        }
    }
    
    pinMode(LED_BUILTIN, OUTPUT);
    initCommunication();
    
    // Initialize based on detected capabilities
    if (hasButtons(deviceType)) {
        setupButtons();
        Serial.println("Buttons enabled");
    }
    
    if (hasButtonLEDs(deviceType)) {
        setupButtonLeds();
        Serial.println("Button LEDs enabled");
    }
    
    if (hasOctoLEDStrips(deviceType)) {
        setupLedStrips();
        setupEffects();
        Serial.println("OctoWS2811 LED strips enabled");
    }
    
    Serial.println("Setup complete");
}

void loop() {
    // Heartbeat LED
    unsigned long currentTime = millis();
    if (currentTime - previousMillis > interval) {
        previousMillis = currentTime;
        ledState = !ledState;
        digitalWrite(LED_BUILTIN, ledState);
        sendHeartbeat();
    }
    
    // Run based on detected capabilities
    if (hasButtons(deviceType)) {
        loopButtons();
    }
    
    if (hasButtonLEDs(deviceType)) {
        loopButtonLeds();
    }
    
    if (hasOctoLEDStrips(deviceType)) {
        loopLedStrips();
        loopEffects();
    }
    
    delay(LOOP_DELAY_MS);
} 