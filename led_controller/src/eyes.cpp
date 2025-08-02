#include "eyes.h"

CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];
// Tracks whether the button corresponding to each eye is pressed.
std::vector<bool> eye_states;
// Variables for gating updates to every 100ms.
unsigned long last_eye_update = 0;
const unsigned long EYE_UPDATE_INTERVAL = 100;

void setupEyes() {
    FastLED.addLeds<WS2811, EYE_LEDS_PIN, GRB>(eyeLeds, NUM_EYES * EYE_NUM_LEDS);
    FastLED.clear();
    FastLED.show();

    for (int i = 0; i < NUM_EYES; i++) {
        eye_states.push_back(false);
    }
}

// Used externally; see buttons.cpp.
void setEyeStatus(int eye_index, bool status) {
    eye_states[eye_index] = status;
}   

void loopEyes() {
    unsigned long current_time = millis();
    if (current_time - last_eye_update < EYE_UPDATE_INTERVAL) {
        return; // Not time to update yet
    }
    last_eye_update = current_time;
    
    // Set LED values for each eye based on whether it's on or not.
    for (int i = 0; i < NUM_EYES; i++) {
        CRGB color = eye_states[i] ? CRGB::Red : CRGB::Blue;
        int eye_offset = i * EYE_NUM_LEDS;
        for (int j = 0; j < EYE_NUM_LEDS; j++) {
            eyeLeds[eye_offset + j] = color;
        }
    }
    FastLED.show();
} 