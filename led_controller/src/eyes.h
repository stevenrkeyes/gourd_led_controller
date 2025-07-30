#pragma once

#include <FastLED.h>

constexpr int EYE_LEDS_PIN = 23;
constexpr int NUM_EYES = 3;
constexpr int OUTER_EYE_NUM_LEDS = 24;
constexpr int INNER_EYE_NUM_LEDS = 8;
constexpr int EYE_NUM_LEDS = OUTER_EYE_NUM_LEDS + INNER_EYE_NUM_LEDS;

extern CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];

void setupEyes();
void loopEyes(); 