#include "buttons.h"
#include "config.h"

#ifdef HAS_BUTTONS

#include <Bounce2.h>
#include "shared/communication.h"

#ifdef TEENSY_A
#include "pins_teensy_a.h"
#endif

// Array of Bounce objects for all buttons (dynamically initialized)
Bounce buttons[NUM_BUTTONS];

void setupButtons() {
    // Initialize all buttons with bounce debouncing
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].attach(BUTTON_PINS[i], INPUT_PULLUP);
        buttons[i].interval(100);  // 100ms debounce time
    }
    Serial.print("✅ Initialized ");
    Serial.print(NUM_BUTTONS);
    Serial.println(" buttons");
    
    // Print pin mapping for reference
    Serial.println("📌 Button pin mapping:");
    for (int i = 0; i < NUM_BUTTONS; i++) {
        Serial.print("   Button ");
        Serial.print(i + 1);
        Serial.print(" → Pin ");
        Serial.println(BUTTON_PINS[i]);
    }
}

void loopButtons() {
    // Update and check all buttons using a single loop
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].update();
        
        // Check for button presses (fell = pressed down)
        if (buttons[i].fell()) {
            Serial.print("BUTTON_PRESS:");
            Serial.print(i + 1);
            Serial.print("\n");
        }
    }

#ifdef DEBUG_MODE
    // Detailed pin state info - every 15 seconds
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 15000) {
        lastDebug = millis();
        
        Serial.println("📊 PIN STATE REPORT:");
        Serial.println("   Buttons 1-8:  [Pin] State");
        for (int i = 0; i < 8; i++) {
            int pinState = digitalRead(BUTTON_PINS[i]);
            char stateChar = (pinState == HIGH) ? '^' : 'v';
            Serial.print("   Button ");
            Serial.print(i + 1);
            Serial.print(": [");
            Serial.print(BUTTON_PINS[i]);
            Serial.print("] ");
            Serial.print(stateChar);
            Serial.print(" (");
            Serial.print(pinState ? "HIGH" : "LOW");
            Serial.println(")");
        }
        
        Serial.println("   Buttons 9-16: [Pin] State");
        for (int i = 8; i < 16; i++) {
            int pinState = digitalRead(BUTTON_PINS[i]);
            char stateChar = (pinState == HIGH) ? '^' : 'v';
            Serial.print("   Button ");
            Serial.print(i + 1);
            Serial.print(": [");
            Serial.print(BUTTON_PINS[i]);
            Serial.print("] ");
            Serial.print(stateChar);
            Serial.print(" (");
            Serial.print(pinState ? "HIGH" : "LOW");
            Serial.println(")");
        }
        
        // Compact format for quick reference
        Serial.print("   Quick: ");
        for (int i = 0; i < NUM_BUTTONS; i++) {
            int pinState = digitalRead(BUTTON_PINS[i]);
            Serial.print(pinState ? "1" : "0");
            if (i == 7) Serial.print(" ");  // Space between groups of 8
        }
        Serial.println();
        Serial.println("   (1=HIGH/Released, 0=LOW/Pressed)");
        Serial.println("----------------------------------------");
    }
#endif // DEBUG_MODE
}

#endif // HAS_BUTTONS 