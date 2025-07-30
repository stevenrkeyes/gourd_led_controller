#include "eyes.h"

CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];

void setupEyes() {
    FastLED.addLeds<WS2811, EYE_LEDS_PIN, GRB>(eyeLeds, NUM_EYES * EYE_NUM_LEDS);
    FastLED.clear();
    FastLED.show();
    // Set eye LED pattern
    for (int i = 0; i < NUM_EYES; i++) {
        int eye_offset = i * EYE_NUM_LEDS;
        int inner_eye_offset = eye_offset + OUTER_EYE_NUM_LEDS;
        for (int j = 0; j < OUTER_EYE_NUM_LEDS; j++) {
            eyeLeds[eye_offset + j] = CRGB::Red; // Red outer ring
        }
        for (int j = 0; j < INNER_EYE_NUM_LEDS; j++) {
            eyeLeds[inner_eye_offset + j] = CRGB::DarkCyan; // Green inner ring
        }
    }
    FastLED.show();
}

void loopEyes() {
    // Does nothing, at the moment.
    FastLED.show();
} 