#include "eyes.h"
#include "pins_teensy_a.h"

CRGB eyeLeds[NUM_EYES * EYE_MAX_LEDS];
int eye_led_counts[NUM_EYES] = {12, 12, 12, 12, 12, 24, 24, 12, 24, 24, 24, 24, 12, 24, 24, 12};
int total_eye_leds = 0;
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

static int computeEyeOffset(int eye_index) {
    int offset = 0;
    for (int i = 0; i < eye_index; i++) {
        offset += eye_led_counts[i];
    }
    return offset;
}

void setupEyes() {
    // eye_led_counts = {12, 12, 12, 12, 24, 24, 24, 12, 24, 24, 24, 24, 12, 24, 24, 12};
    // // Initialize per-eye LED counts. Default to 12 for safety.
    // for (int eye_index = 0; eye_index < NUM_EYES; eye_index++) {
    //     eye_led_counts[eye_index] = 12;
    // }
    // // The system has 16 eyes: first 8 have 12 LEDs, second 8 have 24 LEDs.
    // for (int eye_index = 8; eye_index < NUM_EYES; eye_index++) {
    //     eye_led_counts[eye_index] = 24;
    // }

    // Compute total LEDs
    total_eye_leds = 0;
    for (int eye_index = 0; eye_index < NUM_EYES; eye_index++) total_eye_leds += eye_led_counts[eye_index];

    FastLED.addLeds<WS2811, EYE_LEDS_PIN, GRB>(eyeLeds, total_eye_leds);
    FastLED.clear();
    FastLED.show();

    for (int eye_index = 0; eye_index < NUM_EYES; eye_index++) {
        eye_states.push_back(false);
        eye_brightness_multipliers.push_back(1.0);
    }
    breathing_start = millis();
}

// Used externally; see buttons.cpp.
void triggerRipple(unsigned long timestamp, int eye_index) {
    for (int eye_index = 0; eye_index < MAX_ACTIVE_RIPPLES; eye_index++) {
        if (!active_ripples[eye_index].active) {
            active_ripples[eye_index].startTime = timestamp;
            active_ripples[eye_index].eye_index = eye_index;
            active_ripples[eye_index].active = true;
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
    for (int eye_index = 0; eye_index < NUM_EYES; eye_index++) {
        eye_brightness_multipliers[eye_index] = 1.0;
    }

    // For each active ripple, multiply brightness for the affected eyes.
    // At each step the ripple travels, the multiplier decreases.
    for (int eye_index = 0; eye_index < MAX_ACTIVE_RIPPLES; eye_index++) {
        auto ripple = active_ripples[eye_index];
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
    
    for (int eye_index = 0; eye_index < NUM_EYES; eye_index++) {
        int num_leds_for_eye = eye_led_counts[eye_index];

        float breathing_period = eye_states[eye_index] ? BREATHING_PERIOD / BREATHING_SPEEDUP : BREATHING_PERIOD;
        float breathing_progress = float(current_time - breathing_start) / breathing_period;
        float sine_value = sin(breathing_progress * 2 * PI); // -1 to +1
        float intensity = (sine_value + 1.0) / 2.0; // 0.0 to 1.0
        float max_intensity = eye_states[eye_index] ? MAX_INTENSITY / INTENSITY_DECREASE : MAX_INTENSITY;
        max_intensity *= eye_brightness_multipliers[eye_index];
        intensity = MIN_INTENSITY + (max_intensity - MIN_INTENSITY) * intensity;
    
        // CRGB base_color = eye_index % 2 == 0 ? CRGB::Blue : CRGB::Red;
        CRGB base_color = eye_states[eye_index] ? CRGB::White : CRGB::Red;
        CRGB color = base_color;
        color.r = uint8_t(color.r * intensity);
        color.g = uint8_t(color.g * intensity);
        color.b = uint8_t(color.b * intensity);
        
        for (int led_index = 0; led_index < num_leds_for_eye; led_index++) {
            eyeLeds[computeEyeOffset(eye_index) + led_index] = color;
        }
    }
    FastLED.show();
} 