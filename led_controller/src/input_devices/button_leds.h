#ifndef BUTTON_LEDS_H
#define BUTTON_LEDS_H

#ifdef TEENSY_A

#include <Arduino.h>
#include <FastLED.h>

void setupButtonLeds();
void loopButtonLeds();
void setButtonLed(int buttonId, uint8_t r, uint8_t g, uint8_t b);

#endif // TEENSY_A

#endif // BUTTON_LEDS_H 