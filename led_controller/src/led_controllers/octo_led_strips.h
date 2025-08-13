#ifndef OCTO_LED_STRIPS_H
#define OCTO_LED_STRIPS_H

#ifdef HAS_OCTO_LED_STRIPS

#include <Arduino.h>
#include <vector>

struct LedPulse {
    unsigned long startTime;
    int strip;
};

class OctoWS2811;
extern OctoWS2811 leds;

void setupLedStrips();
void loopLedStrips();
void triggerLedPulse(unsigned long timestamp, int strip = 0);

#endif // HAS_OCTO_LED_STRIPS

#endif // OCTO_LED_STRIPS_H 