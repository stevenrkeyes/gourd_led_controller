#ifndef OCTO_LED_STRIPS_H
#define OCTO_LED_STRIPS_H

#if defined(TEENSY_B) || defined(TEENSY_C)

#include <Arduino.h>
#include <vector>

struct LedPulse {
    bool active;
    unsigned long startTime;
    int strip;
};

class OctoWS2811;
extern OctoWS2811 leds;

void setupLedStrips();
void triggerLedPulse(unsigned long timestamp, int strip);
void clearAllLEDs();
void drawFire();
void updateFire();
void drawAllPulses();
void loopLedStrips();

#endif // TEENSY_B || TEENSY_C

#endif // OCTO_LED_STRIPS_H 