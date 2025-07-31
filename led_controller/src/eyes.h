#pragma once

#include <FastLED.h>

constexpr int EYE_LEDS_PIN = 17;
constexpr int NUM_EYES = 6;
constexpr int EYE_NUM_LEDS = 24;

extern CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];

void setEyeStatus(int eye_index, bool status);

void setupEyes();
void loopEyes(); 