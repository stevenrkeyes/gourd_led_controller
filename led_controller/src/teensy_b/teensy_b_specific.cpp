#include "teensy_b_specific.h"

#ifdef TEENSY_B

#include "config.h"
#include "pins_teensy_b.h"
#include "shared/communication.h"

void setupTeensyB() {
    Serial.println("=== TEENSY B TEST MODE ===");
    Serial.println("Waiting for commands from Pi...");
    Serial.println("Ready to receive LED commands");
}

void loopTeensyB() {
    // LED command handling is done in loopLedStrips() - no need to duplicate here
    
    // Heartbeat to show we're alive
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat > 5000) {
        lastHeartbeat = millis();
        Serial.println("💓 Teensy B alive and listening...");
    }
}

#endif // TEENSY_B 