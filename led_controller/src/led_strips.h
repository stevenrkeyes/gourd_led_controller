#ifndef LED_STRIPS_H
#define LED_STRIPS_H

#include "pins.h"

constexpr int LED_STRIP_NUM_LEDS = 15;

class OctoWS2811;
extern OctoWS2811 leds;

// Add your declarations here
void setupLedStrips();
void loopLedStrips();

#endif // LED_STRIPS_H 