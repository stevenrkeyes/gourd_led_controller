// buttons.cpp
// Add your function definitions here 

#include <Bounce2.h>
#include "pins.h"
#include "buttons.h"
#include "led_strips.h"

Bounce button1(BUTTON_COLUMN_1_PIN, 50);

void setupButtons() {
    pinMode(BUTTON_COLUMN_1_PIN, INPUT_PULLUP);
}

void loopButtons() {
    button1.update();
    if (button1.fell()) {
        Serial.println("button pressed");
        triggerLedPulse(millis());
    }
} 