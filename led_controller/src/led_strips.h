#pragma once

#include <OctoWS2811.h>
#include <vector>

// Main LED strip configuration
constexpr int LED_STRIP_NUM_LEDS = 40;
constexpr int NUM_STRIPS = 2;
constexpr int TOTAL_NUM_LEDS = NUM_STRIPS * LED_STRIP_NUM_LEDS;

// LED color options (GRB)
const std::vector<int> LED_COLOR_LIST {
    0x00AA0000, 0x0000AA00, 0x000000AA
};

extern OctoWS2811 leds;

void setupLedStrips();
void loopLedStrips(); 