#ifndef EYES_H
#define EYES_H

#include <FastLED.h>

// TODO: This is wrong - we actually have 8 eyes of size 12 and 8 of size 24.
#define NUM_EYES 16
#define EYE_NUM_LEDS 12

extern CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];

void setEyeStatus(int eye_index, bool status);

void setupEyes();
void loopEyes();

#endif // CONFIG_H 