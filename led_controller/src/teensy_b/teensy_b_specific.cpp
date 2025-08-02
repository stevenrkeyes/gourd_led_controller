#include "teensy_b_specific.h"

#ifdef TEENSY_B

#include "config.h"
#include "pins_teensy_b.h"
#include "shared/communication.h"

void setupTeensyB() {
    Serial.println("Initializing Teensy B specific features");
    // Add any Teensy B specific initialization here
}

void loopTeensyB() {
    // Handle incoming commands from Pi
    CommandPacket packet;
    if (receiveCommand(packet)) {
        switch (packet.command) {
            case CMD_LED_PULSE:
                Serial.println("Received LED pulse command for Teensy B");
                // This will trigger LED pulse effects
                break;
            case CMD_LED_EFFECT:
                Serial.println("Received LED effect command for Teensy B");
                // This will handle different LED effects
                break;
        }
    }
}

#endif // TEENSY_B 