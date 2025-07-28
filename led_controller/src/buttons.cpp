// buttons.cpp
// Add your function definitions here 

#include <Bounce2.h>
#include "pins.h"
#include "buttons.h"
#include "led_strips.h"
#include <Keyboard.h>

Bounce button1(BUTTON_COLUMN_1_PIN, 50);

// Keyboard state tracking
static bool keyboardPressed = false;
static unsigned long keyboardPressTime = 0;
static const unsigned long keyboardPressDuration = 10; // ms

void setupButtons() {
    pinMode(BUTTON_COLUMN_1_PIN, INPUT_PULLUP);
}

void loopButtons() {
    button1.update();
    
    // Handle keyboard press/release timing
    if (keyboardPressed && (millis() - keyboardPressTime >= keyboardPressDuration)) {
        Keyboard.release('a');
        keyboardPressed = false;
    }
    
    // Button press now only sends keyboard
    if (button1.fell()) {
        Serial.println("button pressed");
        Keyboard.press('a');
        keyboardPressed = true;
        keyboardPressTime = millis();
    }
} 