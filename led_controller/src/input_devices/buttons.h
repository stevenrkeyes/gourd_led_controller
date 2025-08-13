#ifndef BUTTONS_H
#define BUTTONS_H

#ifdef HAS_BUTTONS

#include <Arduino.h>

class Bounce;

// Eventually will be 16.
constexpr int NUM_BUTTONS = 4;

// Used these since they're easily available via breakout.
const std::vector<int> ROW_PINS {22, 23};
const std::vector<int> COL_PINS {0, 1};

void setupButtons();
void loopButtons();

#endif // HAS_BUTTONS

#endif // BUTTONS_H 