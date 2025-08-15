#ifndef EFFECTS_H
#define EFFECTS_H

#if defined(TEENSY_B) || defined(TEENSY_C)

#include <Arduino.h>

void setupEffects();
void loopEffects();
void startRainbowEffect(uint8_t strip);
void startStrobeEffect(uint8_t strip, uint32_t color);
void stopAllEffects();

#endif // TEENSY_B || TEENSY_C

#endif // EFFECTS_H 