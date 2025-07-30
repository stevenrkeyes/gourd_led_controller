#ifndef LED_STRIPS_H
#define LED_STRIPS_H

#include "pins.h"
#include <vector>

// Main LED strip configuration
constexpr int LED_STRIP_NUM_LEDS = 40;
constexpr int LED_STRIP_NUM_STRIPS = 2;
constexpr int TOTAL_NUM_LEDS = LED_STRIP_NUM_STRIPS * LED_STRIP_NUM_LEDS;

class OctoWS2811;
extern OctoWS2811 leds;

// Add your declarations here
void setupLedStrips();
void loopLedStrips();

struct LedPulse {
    unsigned long startTime;
    int index;
};

void triggerLedPulse(unsigned long timestamp, int index);

#endif // LED_STRIPS_H