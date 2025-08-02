#include <Bounce2.h>
#include "pins.h"
#include "buttons.h"
#include "led_strips.h"
#include "eyes.h"
#include <Keyboard.h> 

std::vector<Bounce> buttons;
std::vector<char> button_keys = {'0', '1', '2', '3', '4', '5'};

void setupButtons() {
    for (size_t i = 0; i < NUM_BUTTONS; i++) {
        buttons.emplace_back();
        buttons[i].attach(BUTTON_PINS[i], INPUT_PULLUP);
        // TODO: Check if the following line is actually necessary.
        pinMode(BUTTON_PINS[i], INPUT_PULLUP);
        buttons[i].interval(25);
    }
}

void sendKey(int button_index) {
    bool silent = false;
    if (!silent) {
        Keyboard.press(button_keys[button_index]);
        delay(10); // Short delay to ensure keypress is registered
        Keyboard.release(button_keys[button_index]);            
    }
}

void loopButtons() {
    for (size_t i = 0; i < NUM_BUTTONS; i++) {
        Bounce& button = buttons[i];
        button.update();
        if (button.fell()) {
            Serial.println("button pressed: " + String(i));
            // TODO: Remove this - we'll pipe input to raspberry pi and then to another teensy.
            // triggerLedPulse(millis(), i % LED_STRIP_NUM_STRIPS);
            sendKey(i);
        } else if (button.rose()) {
            Serial.println("button released: " + String(i));
            // TODO: Remove this - we'll pipe input to raspberry pi and then to another teensy.
            // triggerLedPulse(millis(), i % LED_STRIP_NUM_STRIPS);
        }
    }  
} 