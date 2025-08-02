#ifndef BUTTONS_H
#define BUTTONS_H

#include "pins.h"

class Bounce;

// 2 rows × 3 columns = 6 buttons
constexpr int NUM_BUTTONS = 6;

// Used these since they're easily available via breakout.
const std::vector<int> COL_PINS {0, 1, 23};
// For 4.1
// const std::vector<int> ROW_PINS {22, 31};
// For 4.0
const std::vector<int> ROW_PINS {22, 3};


// Add your declarations here
void setupButtons();
void loopButtons();

#endif // BUTTONS_H 