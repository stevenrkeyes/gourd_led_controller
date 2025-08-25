#include "teensy_c_specific.h"

#ifdef TEENSY_C

#include "config.h"
#include "pins_teensy_c.h"
#include "shared/communication.h"

void setupTeensyC() {
    Serial.println("Initializing Teensy C specific features");
    // Add any Teensy C specific initialization here
}

void loopTeensyC() {
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat > 5000) {
        lastHeartbeat = millis();
        Serial.println("💓 Teensy C alive and listening...");
    }
    // TODO: Decide if we're handling all input here and routing to relevant functions (e.g.in octo_led_strips).
    // Handle incoming commands from Pi
    // CommandPacket packet;
    // if (receiveCommand(packet)) {
    //     switch (packet.command) {
    //         case CMD_LED_PULSE:
    //             Serial.println("Received LED pulse command for Teensy C");
    //             // This will trigger LED pulse effects
    //             break;
    //         case CMD_LED_EFFECT:
    //             Serial.println("Received LED effect command for Teensy C");
    //             // This will handle different LED effects
    //             break;
    //     }
    // }
}

#endif // TEENSY_C 