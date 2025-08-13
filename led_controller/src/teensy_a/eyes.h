#ifndef EYES_H
#define EYES_H

#include <FastLED.h>

#define EYE_LEDS_PIN 22
#define NUM_EYES 4
#define EYE_NUM_LEDS 24

extern CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];

void setEyeStatus(int eye_index, bool status);

void setupEyes();
void loopEyes();

#endif // CONFIG_H 