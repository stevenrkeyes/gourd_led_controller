#ifndef EYES_H
#define EYES_H

#include <FastLED.h>

#define NUM_EYES 16
#define EYE_MAX_LEDS 24

extern CRGB eyeLeds[NUM_EYES * EYE_MAX_LEDS];
extern int eye_led_counts[NUM_EYES];
extern int total_eye_leds;

void setEyeStatus(int eye_index, bool status);

void setupEyes();
void loopEyes();

#endif // CONFIG_H 