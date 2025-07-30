#pragma once

#include <Bounce2.h>
#include <vector>

// 3 rows x 4 columns = 12 buttons.
// We'll also have 4 potentiometers.

// Eventually we'll use pins 0, 1, 3
const std::vector<int> ROW_PINS {0};
// Eventually we'll use pins 4, 9, 10, 11
const std::vector<int> COL_PINS {1};

void setupButtons();
void loopButtons(); 