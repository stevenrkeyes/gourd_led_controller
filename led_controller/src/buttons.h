#ifndef BUTTONS_H
#define BUTTONS_H

#include "pins.h"

class Bounce;

// Eventually will be 16.
constexpr int NUM_BUTTONS = 4;

// Used these since they're easily available via breakout.
const std::vector<int> ROW_PINS {22, 23};
const std::vector<int> COL_PINS {0, 1};

// Add your declarations here
void setupButtons();
void loopButtons();

#endif // BUTTONS_H 