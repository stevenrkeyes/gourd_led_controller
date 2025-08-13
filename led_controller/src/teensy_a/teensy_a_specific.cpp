#include "teensy_a_specific.h"
#include "eyes.h"

#ifdef TEENSY_A


#include "config.h"
#include "pins_teensy_a.h"
#include "shared/communication.h"
#include <FastLED.h>

// Ring LED array
CRGB ringLeds[TOTAL_RING_LED_COUNT];

void setupTeensyA() {
    Serial.println("Initializing Teensy A specific features");
    
    setupEyes();
    
    // Setup any additional sensors or hardware specific to Teensy A
    pinMode(SENSOR_DIGITAL_1_PIN, INPUT_PULLUP);
}

void loopTeensyA() {
    // Handle incoming commands from Pi
    CommandPacket packet;
    if (receiveCommand(packet)) {
        switch (packet.command) {
            case CMD_BUTTON_LED:
                handleButtonLedCommand(packet);
                break;
            // Is this needed? Since the buttons are on this Teensy, we don't need the pi to send any commands here.
            // case CMD_RING_LED_TEST:
            //     handleRingLedCommand(packet);
            //     break;
            // Add more Teensy A specific command handling
        }
    }

    loopEyes();
    
    // Read sensors and send to Pi
    static unsigned long lastSensorRead = 0;
    if (millis() - lastSensorRead > 100) { // Read sensors every 100ms
        lastSensorRead = millis();
        // readAndSendSensorData();  // Commented out for clean debug output
    }
}

void handleButtonLedCommand(const CommandPacket& packet) {
    // Implementation for controlling button LEDs
    uint8_t buttonId = packet.data[0];
    uint8_t brightness = packet.data[1];
    uint8_t r = packet.data[2];
    uint8_t g = packet.data[3];
    uint8_t b = packet.data[4];
    
    Serial.print("Setting button LED ");
    Serial.print(buttonId);
    Serial.print(" to RGB(");
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.print(b);
    Serial.println(")");
    
    // This will call the button LED functions when they're implemented
}

// Is this needed? Since the buttons are on this Teensy, we don't need the pi to send any commands here.
// void handleRingLedCommand(const CommandPacket& packet) {
//     uint8_t pattern = packet.data[0];
//     uint8_t r = packet.data[1];
//     uint8_t g = packet.data[2]; 
//     uint8_t b = packet.data[3];
    
//     Serial.print("Ring LED command - Pattern: ");
//     Serial.print(pattern);
//     Serial.print(", RGB(");
//     Serial.print(r);
//     Serial.print(",");
//     Serial.print(g);
//     Serial.print(",");
//     Serial.print(b);
//     Serial.println(")");
    
//     switch (pattern) {
//         case 0: case 1: case 2: case 3: // Solid colors
//             fill_solid(ringLeds, TOTAL_RING_LED_COUNT, CRGB(r, g, b));
//             break;
//         case 4: // Rainbow
//             fill_rainbow(ringLeds, TOTAL_RING_LED_COUNT, 0, 255/TOTAL_RING_LED_COUNT);
//             break;
//         case 5: // Chase pattern
//             FastLED.clear();
//             for (int i = 0; i < TOTAL_RING_LED_COUNT; i += 3) {
//                 ringLeds[i] = CRGB(r, g, b);
//             }
//             break;
//         case 99: // Turn off
//             FastLED.clear();
//             break;
//         default:
//             Serial.println("Unknown ring LED pattern");
//             return;
//     }
    
//     FastLED.show();
// }

void readAndSendSensorData() {
    // Read analog sensor
    int sensorValue = analogRead(SENSOR_ANALOG_1_PIN);
    
    // Send sensor data to Pi
    uint8_t sensorData[4];
    sensorData[0] = 0x01; // Sensor ID
    sensorData[1] = (sensorValue >> 8) & 0xFF; // High byte
    sensorData[2] = sensorValue & 0xFF; // Low byte
    sensorData[3] = digitalRead(SENSOR_DIGITAL_1_PIN);
    
    sendCommand(CMD_SENSOR_DATA, sensorData, 4);
}

#endif // TEENSY_A 