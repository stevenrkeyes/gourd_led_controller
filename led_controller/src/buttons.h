#ifndef BUTTONS_H
#define BUTTONS_H

#include "pins.h"

class Bounce;

// Eventually will be 16.
constexpr int NUM_BUTTONS = 4;

// Eventually we'll use pins 0, 1, 3
const std::vector<int> ROW_PINS {22, 23};
// Eventually we'll use pins 4, 9, 10, 11
const std::vector<int> COL_PINS {0, 1};

// Add your declarations here
void setupButtons();
void loopButtons();

#endif // BUTTONS_H 