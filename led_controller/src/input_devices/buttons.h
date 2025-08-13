#ifndef BUTTONS_H
#define BUTTONS_H

#ifdef TEENSY_A

#include <Arduino.h>

class Bounce;

// Used these since they're easily available via breakout.
const std::vector<int> ROW_PINS {22, 23};
const std::vector<int> COL_PINS {0, 1};

void setupButtons();
void loopButtons();

#endif // TEENSY_A

#endif // BUTTONS_H 