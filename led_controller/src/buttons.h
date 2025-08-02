#ifndef BUTTONS_H
#define BUTTONS_H

#include "pins.h"
#include <vector>

class Bounce;

// 2 rows × 3 columns = 6 buttons
constexpr int NUM_BUTTONS = 6;

// Used these since they're easily available via breakout.
// const std::vector<int> BUTTON_PINS {24, 25, 26, 27, 28, 29};
const std::vector<int> BUTTON_PINS {0, 1, 2, 3, 4, 5};

// Add your declarations here
void setupButtons();
void loopButtons();

#endif // BUTTONS_H 