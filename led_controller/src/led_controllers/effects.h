#ifndef EFFECTS_H
#define EFFECTS_H

#ifdef HAS_OCTO_LED_STRIPS

#include <Arduino.h>

void setupEffects();
void loopEffects();
void startRainbowEffect(uint8_t strip);
void startStrobeEffect(uint8_t strip, uint32_t color);
void stopAllEffects();

#endif // HAS_OCTO_LED_STRIPS

#endif // EFFECTS_H 