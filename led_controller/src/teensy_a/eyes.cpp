#include "eyes.h"
#include "pins_teensy_a.h"

CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];
// Tracks whether the button corresponding to each eye is pressed.
std::vector<bool> eye_states;

// Variables for breathing effect
const unsigned long BREATHING_PERIOD = 5000;
const float BREATHING_SPEEDUP = 2.5;
const float MIN_INTENSITY = 0.05;
const float MAX_INTENSITY = 0.5;
const float INTENSITY_DECREASE = 2.0;
unsigned long breathing_start = 0;

// TODO: Move to header?
struct EyeRipple {
    unsigned long startTime;
    int eye_index;
    bool active;
};
std::vector<float> eye_brightness_multipliers;

#define MAX_ACTIVE_RIPPLES 8
#define RIPPLE_MAX_TRAVEL_DISTANCE 3
// Probably bad practice to iterate over this while clearing stuff.
// Can do it with iterators.
// std::vector<EyeRipple> active_ripples;
static EyeRipple active_ripples[MAX_ACTIVE_RIPPLES];
static const unsigned long ripple_duration = 1000; // ms for ripple to travel full strip
static const uint32_t ripple_color = 0x00FFFFFF; // White
static const uint32_t background_color = 0x00000000; // Off

void setupEyes() {
    FastLED.addLeds<WS2811, EYE_LEDS_PIN, GRB>(eyeLeds, NUM_EYES * EYE_NUM_LEDS);
    FastLED.clear();
    FastLED.show();

    for (int i = 0; i < NUM_EYES; i++) {
        eye_states.push_back(false);
        eye_brightness_multipliers.push_back(1.0);
    }
    breathing_start = millis();
}

void triggerRipple(unsigned long timestamp, int eye_index) {
    // Find first inactive slot
    for (int i = 0; i < MAX_ACTIVE_RIPPLES; i++) {
        if (!active_ripples[i].active) {
            active_ripples[i].startTime = timestamp;
            active_ripples[i].eye_index = eye_index;
            active_ripples[i].active = true;
            Serial.print("Triggered pulse from eye ");
            Serial.println(eye_index);
            break;
        }
    }
}

// Used externally; see buttons.cpp.
void setEyeStatus(int eye_index, bool status) {
    eye_states[eye_index] = status;
    triggerRipple(millis(), eye_index);
}   

void loopEyes() {
    unsigned long current_time = millis();

    // Reset eye brightness multipliers; they'll be updated as we process ripples.
    for (int i = 0; i < NUM_EYES; i++) {
        eye_brightness_multipliers[i] = 1.0;
    }

    // For each active ripple, multiply brightness for the affected eyes.
    // At each step the ripple travels, the multiplier decreases.
    for (int i = 0; i < MAX_ACTIVE_RIPPLES; i++) {
        auto ripple = active_ripples[i];
        if (ripple.active) {
            float progress = float(current_time - ripple.startTime) / ripple_duration;
            int ripple_travel_distance = int(progress * RIPPLE_MAX_TRAVEL_DISTANCE);
            
            if (ripple_travel_distance >= 0 && ripple_travel_distance < RIPPLE_MAX_TRAVEL_DISTANCE) {
                // This is wrong; unidirectional.
                // Instead, should define helper function that takes epicenter and progress and returns sets of affected eyes.
                int ripple_wave_index = min(NUM_EYES - 1, ripple.eye_index + ripple_travel_distance);
                eye_brightness_multipliers[ripple_wave_index] = 1.5 * eye_brightness_multipliers[ripple_wave_index];
            } else {
                ripple.active = false; // Remove finished pulse
                // Serial.println("RIP(ple)");
            }
        }
    }
    
    // Set LED values for each eye based on whether it's on or not.
    for (int i = 0; i < NUM_EYES; i++) {
        // Breathing speeds up when button is pressed.
        float breathing_period = eye_states[i] ? BREATHING_PERIOD / BREATHING_SPEEDUP : BREATHING_PERIOD;
        float breathing_progress = float(current_time - breathing_start) / breathing_period;
        float sine_value = sin(breathing_progress * 2 * PI); // -1 to +1
        float intensity = (sine_value + 1.0) / 2.0; // 0.0 to 1.0
        float max_intensity = eye_states[i] ? MAX_INTENSITY / INTENSITY_DECREASE : MAX_INTENSITY;
        max_intensity = max_intensity * eye_brightness_multipliers[i];
        intensity = MIN_INTENSITY + (max_intensity - MIN_INTENSITY) * intensity; // min to max
    
        CRGB base_color = eye_states[i] ? CRGB::White : CRGB::Red;
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