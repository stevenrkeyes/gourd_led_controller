#include "teensy_a_specific.h"

#ifdef TEENSY_A

#include "config.h"
#include "pins_teensy_a.h"
#include "shared/communication.h"

void setupTeensyA() {
    Serial.println("Initializing Teensy A specific features");
    
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
            // Add more Teensy A specific command handling
        }
    }
    
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