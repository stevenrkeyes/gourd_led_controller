#include "buttons.h"
#include "config.h"

#ifdef HAS_BUTTONS

#include <Bounce2.h>
#include "shared/communication.h"

#ifdef TEENSY_A
#include "pins_teensy_a.h"
#endif

// Array of Bounce objects for all buttons
Bounce buttons[NUM_BUTTONS] = {
    Bounce(BUTTON_PINS[0], 100),  // Increased debounce time from 50ms to 100ms
    Bounce(BUTTON_PINS[1], 100),
    Bounce(BUTTON_PINS[2], 100),
    Bounce(BUTTON_PINS[3], 100),
    Bounce(BUTTON_PINS[4], 100),
    Bounce(BUTTON_PINS[5], 100)
};

void setupButtons() {
    // Set up all button pins using a loop
    for (int i = 0; i < NUM_BUTTONS; i++) {
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
    }
    Serial.print("Initialized ");
    Serial.print(NUM_BUTTONS);
    Serial.println(" buttons");
}

void loopButtons() {
    // Update and check all buttons using a single loop
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].update();
        
        // Check for button presses (fell = pressed down)
        if (buttons[i].fell()) {
            // Send text command to Pi - much cleaner!
            Serial.print("BUTTON_PRESS:");
            Serial.print(i + 1);
            Serial.print("\n");
        }
    }
    
    // Reduced frequency debug info - every 10 seconds
    static unsigned long lastDebug = 0;
    if (millis() - lastDebug > 10000) {
        lastDebug = millis();
        Serial.print("PIN_STATES:");
        for (int i = 0; i < NUM_BUTTONS; i++) {
            Serial.print(digitalRead(BUTTON_PINS[i]));
            if (i < NUM_BUTTONS - 1) Serial.print(",");
        }
        Serial.print("\n");
    }
}

#endif // HAS_BUTTONS 