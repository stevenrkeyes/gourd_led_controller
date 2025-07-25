#ifndef LED_STRIPS_H
#define LED_STRIPS_H

#include "pins.h"
#include <vector>

constexpr int LED_STRIP_NUM_LEDS = 45;

class OctoWS2811;
extern OctoWS2811 leds;

// Add your declarations here
void setupLedStrips();
void loopLedStrips();

struct LedPulse {
    unsigned long startTime;
};

void triggerLedPulse(unsigned long timestamp);

#endif // LED_STRIPS_H 