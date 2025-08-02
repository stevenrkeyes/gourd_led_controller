#include "buttons.h"
#include "config.h"

#ifdef HAS_BUTTONS

#include <Bounce2.h>
#include "shared/communication.h"

#ifdef TEENSY_A
#include "pins_teensy_a.h"
#endif

Bounce button1(BUTTON_COLUMN_1_PIN, 50);

void setupButtons() {
    pinMode(BUTTON_COLUMN_1_PIN, INPUT_PULLUP);
    Serial.println("Buttons initialized");
}

void loopButtons() {
    button1.update();
    if (button1.fell()) {
        Serial.println("Button pressed - sending to Pi");
        
        // Send button press command to Pi instead of directly triggering LED
        uint8_t buttonData[] = {0x01}; // Button 1 pressed
        sendCommand(CMD_BUTTON_PRESS, buttonData, 1);
    }
}

#endif // HAS_BUTTONS 