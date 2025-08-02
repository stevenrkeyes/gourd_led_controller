#ifndef BUTTON_LEDS_H
#define BUTTON_LEDS_H

#ifdef HAS_BUTTON_LEDS

#include <Arduino.h>
#include <FastLED.h>

void setupButtonLeds();
void loopButtonLeds();
void setButtonLed(uint8_t buttonId, CRGB color);
void setButtonLed(uint8_t buttonId, uint8_t r, uint8_t g, uint8_t b);

#endif // HAS_BUTTON_LEDS

#endif // BUTTON_LEDS_H 