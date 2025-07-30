// buttons.cpp
// Add your function definitions here 

#include <Bounce2.h>
#include "pins.h"
#include "buttons.h"
#include "led_strips.h"
#include <Keyboard.h> 

// TODO: Remove buttons if we're not going to use it (i.e. read column pins directly).
std::vector<Bounce> buttons;
std::vector<bool> button_states;

void setupButtons() {
    // TODO: Shift this to loop only?
    for (int row_pin : ROW_PINS) {
        pinMode(row_pin, INPUT);
    }
    // TODO: Maybe implicitly done by button object creation.
    for (int col_pin : COL_PINS) {
        pinMode(col_pin, INPUT_PULLUP);
    }
    for (size_t i = 0; i < COL_PINS.size(); i++) {
        buttons.emplace_back();
        buttons[i].attach(COL_PINS[i], INPUT_PULLUP);
        buttons[i].interval(25);
    }

    for (int i = 0; i < NUM_BUTTONS; i++) {
        button_states.push_back(false);
    }
}

void sendKey(int row, int col) {
    // TODO: Index into an alphabet with row and col.
    Keyboard.press('a');
    delay(10); // Short delay to ensure keypress is registered
    Keyboard.release('a');        
}

void loopButtons() {
    int row_index = 0;
    for (int row_pin : ROW_PINS) {
        pinMode(row_pin, OUTPUT);
        digitalWrite(row_pin, LOW);
        delay(1); // Small delay to ensure pin state is stable
        
        int col_index = 0;
        for (int col_pin : COL_PINS) {
            // Read the column pin directly
            int buttonState = digitalRead(col_pin);
            if (buttonState == LOW) { // Button is pressed (pulls column to ground)
                Serial.println("button pressed: row " + String(row_index) + ", col " + String(col_index));
                triggerLedPulse(millis(), col_index);
                
                // TODO: Keep track of previous state in button_states and only do things on changes.
                
                // Broken since iteration over ROW_PINS and not row pin indices.
                sendKey(row_pin, col_pin);
            }
            col_index++;       
        }
        pinMode(row_pin, INPUT);            
    }
    row_index++;
} 