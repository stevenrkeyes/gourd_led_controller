#include "eyes.h"

CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];
// Tracks whether the button corresponding to each eye is pressed.
std::vector<bool> eye_states;
// Variables for gating updates to every 100ms.
unsigned long last_eye_update = 0;
const unsigned long EYE_UPDATE_INTERVAL = 100;
// Variables for breathing effect
unsigned long breathing_start = 0;
const unsigned long BREATHING_PERIOD = 2000; // 2 second breathing cycle
const float min_intensity = 0.1;
const float max_intensity = 0.7;

void setupEyes() {
    FastLED.addLeds<WS2811, EYE_LEDS_PIN, GRB>(eyeLeds, NUM_EYES * EYE_NUM_LEDS);
    FastLED.clear();
    FastLED.show();

    for (int i = 0; i < NUM_EYES; i++) {
        eye_states.push_back(false);
    }
    breathing_start = millis();
}

// Used externally; see buttons.cpp.
void setEyeStatus(int eye_index, bool status) {
    eye_states[eye_index] = status;
}   

void loopEyes() {
    unsigned long current_time = millis();
    // if (current_time - last_eye_update < EYE_UPDATE_INTERVAL) {
    //     return; // Not time to update yet
    // }
    // last_eye_update = current_time;
    
    // Calculate breathing intensity using min and max variables
    float breathing_progress = float(current_time - breathing_start) / BREATHING_PERIOD;
    float sine_value = sin(breathing_progress * 2 * PI); // -1 to +1
    float intensity = (sine_value + 1.0) / 2.0; // 0.0 to 1.0
    intensity = min_intensity + (max_intensity - min_intensity) * intensity; // min to max
    
    // Set LED values for each eye based on whether it's on or not.
    for (int i = 0; i < NUM_EYES; i++) {
        CRGB base_color = eye_states[i] ? CRGB::Red : CRGB::White;
        CRGB color = base_color;
        
        // Apply breathing effect by scaling the color intensity
        color.r = uint8_t(color.r * intensity);
        color.g = uint8_t(color.g * intensity);
        color.b = uint8_t(color.b * intensity);
        
        int eye_offset = i * EYE_NUM_LEDS;
        for (int j = 0; j < EYE_NUM_LEDS; j++) {
            eyeLeds[eye_offset + j] = color;
        }
    }
    FastLED.show();
} 