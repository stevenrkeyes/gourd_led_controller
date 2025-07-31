#include <Bounce2.h>
#include "pins.h"
#include "buttons.h"
#include "led_strips.h"
#include "eyes.h"
#include <Keyboard.h> 

// TODO: Remove Bounce buttons if we're going to read column GPIO pins directly.
std::vector<Bounce> buttons;
// Tracks the last state (pressed or not) of all button so we know when they change.
std::vector<bool> button_states;

void setupButtons() {
    // Configure pull-ups for all input pins
    // Column pins (0, 1, 23) - these are always INPUT_PULLUP
    IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_00 = IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_00 | IOMUXC_PAD_PKE | IOMUXC_PAD_PUE | IOMUXC_PAD_PUS(2);  // Pin 0
    IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_01 = IOMUXC_SW_PAD_CTL_PAD_GPIO_B0_01 | IOMUXC_PAD_PKE | IOMUXC_PAD_PUE | IOMUXC_PAD_PUS(2);  // Pin 1
    IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_02 = IOMUXC_SW_PAD_CTL_PAD_GPIO_B1_02 | IOMUXC_PAD_PKE | IOMUXC_PAD_PUE | IOMUXC_PAD_PUS(2);  // Pin 23
    
    // Row pins initially set to input so they don't pull down.
    for (int row_pin : ROW_PINS) {
        pinMode(row_pin, INPUT);
    }
    // TODO: Remove Bounce buttons if we're going to read column GPIO pins directly.
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
    // TODO: Use row / col to index into an alphabet instead of always sending 'a'.
    bool silent = true;
    if (!silent) {
        Keyboard.press('a');
        delay(10); // Short delay to ensure keypress is registered
        Keyboard.release('a');            
    }
}

void loopButtons() {
    for (int row_index = 0; row_index < ROW_PINS.size(); row_index++) {
        int row_pin = ROW_PINS[row_index];
        // Set the row to low output - when a button in that row is pressed, its INPUT_PULLUP will be pulled to low.
        pinMode(row_pin, OUTPUT);
        digitalWrite(row_pin, LOW);
        delay(1); // Small delay to ensure pin state is stable
        
        for (int col_index = 0; col_index < COL_PINS.size(); col_index++) {
            int col_pin = COL_PINS[col_index];
            int button_index = row_index * COL_PINS.size() + col_index;

            int buttonState = digitalRead(col_pin); 
            
            if (buttonState == LOW) { // Button is pressed (pulls column to ground)
                if (!button_states[button_index]) { // Button was not pressed before
                    Serial.println("button pressed: row " + String(row_index) + ", col " + String(col_index));
                    triggerLedPulse(millis(), col_index);
                    sendKey(row_index, col_index);
                    button_states[button_index] = true; // Mark as pressed
                    setEyeStatus(button_index, true);
                }
            } else if (buttonState == HIGH) { // Button is not pressed
                if (button_states[button_index]) { // Button was pressed before
                    Serial.println("button released: row " + String(row_index) + ", col " + String(col_index));
                    button_states[button_index] = false; // Mark as not pressed
                    setEyeStatus(button_index, false);
                }
            }
        }
        pinMode(row_pin, INPUT);            
    }
} 