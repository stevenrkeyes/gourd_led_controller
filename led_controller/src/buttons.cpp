#include "buttons.h"

std::vector<Bounce> buttons;

void setupButtons() {
    // TODO: Shift this to loop only.
    for (int row_pin : ROW_PINS) {
        pinMode(row_pin, INPUT_PULLUP);
    }
  
    // Set up 
    for (size_t i = 0; i < COL_PINS.size(); i++) {
        buttons.emplace_back();
        buttons[i].attach(COL_PINS[i], INPUT_PULLUP);
        buttons[i].interval(25);
    }
}

void loopButtons() {
    int row_index = 0;
    for (int row_pin : ROW_PINS) {
        pinMode(row_pin, INPUT_PULLUP);
        
        int col_index = 0;
        for (auto& button : buttons) {
            button.update();
            if (button.fell()) {
                // Example: print to serial or trigger action
                Serial.println("button pressed");
            }
            col_index++;       
        }    
    }
    row_index++;
} 