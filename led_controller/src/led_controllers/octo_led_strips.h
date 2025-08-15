#ifndef OCTO_LED_STRIPS_H
#define OCTO_LED_STRIPS_H

#if defined(TEENSY_B) || defined(TEENSY_C)

#include <Arduino.h>
#include <vector>

struct LedPulse {
    unsigned long startTime;
    int strip;
    bool active;
};

class OctoWS2811;
extern OctoWS2811 leds;

void setupLedStrips();
void loopLedStrips();
void triggerLedPulse(unsigned long timestamp, int strip = 0);

#endif // TEENSY_B || TEENSY_C

#endif // OCTO_LED_STRIPS_H 