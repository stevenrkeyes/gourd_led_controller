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
    // Handle incoming commands from Pi
    CommandPacket packet;
    if (receiveCommand(packet)) {
        Serial.print("[");
        Serial.print(millis());
        Serial.print("ms] Command received: 0x");
        Serial.print(packet.command, HEX);
        Serial.print(" with ");
        Serial.print(packet.data_length);
        Serial.println(" bytes");
        
        switch (packet.command) {
            case CMD_LED_PULSE:
                Serial.println("→ LED PULSE command - would trigger light pulse");
                if (packet.data_length > 0) {
                    Serial.print("  Strip: ");
                    Serial.println(packet.data[0]);
                }
                break;
                
            case CMD_LED_EFFECT:
                Serial.println("→ LED EFFECT command - would trigger LED effect");
                if (packet.data_length > 0) {
                    Serial.print("  Effect ID: ");
                    Serial.println(packet.data[0]);
                }
                break;
                
            default:
                Serial.print("→ Unknown command: 0x");
                Serial.println(packet.command, HEX);
                break;
        }
    }
    
    // Heartbeat to show we're alive
    static unsigned long lastHeartbeat = 0;
    if (millis() - lastHeartbeat > 5000) {
        lastHeartbeat = millis();
        Serial.println("💓 Teensy B alive and listening...");
    }
}

#endif // TEENSY_B 