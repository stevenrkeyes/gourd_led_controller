#include "button_leds.h"
#include "config.h"

#ifdef HAS_BUTTON_LEDS

#include <FastLED.h>

#ifdef TEENSY_A
#include "pins_teensy_a.h"
#endif

#define NUM_BUTTON_LEDS 4
CRGB buttonLeds[NUM_BUTTON_LEDS];

void setupButtonLeds() {
    // Initialize button LEDs using FastLED
    // Note: You may need to adjust the LED type and color order based on your actual LEDs
    FastLED.addLeds<WS2812, BUTTON_LED_1_PIN, GRB>(&buttonLeds[0], 1);
    FastLED.addLeds<WS2812, BUTTON_LED_2_PIN, GRB>(&buttonLeds[1], 1);
    FastLED.addLeds<WS2812, BUTTON_LED_3_PIN, GRB>(&buttonLeds[2], 1);
    FastLED.addLeds<WS2812, BUTTON_LED_4_PIN, GRB>(&buttonLeds[3], 1);
    
    // Set initial state - all LEDs off
    FastLED.clear();
    FastLED.show();
    
    Serial.println("Button LEDs initialized");
}

void loopButtonLeds() {
    // Handle any button LED animations
    // This could include breathing effects, color changes, etc.
    static unsigned long lastUpdate = 0;
    
    if (millis() - lastUpdate > 50) { // Update at 20Hz
        lastUpdate = millis();
        
        // Example: gentle breathing effect on button 1
        float breath = (sin(millis() * 0.003f) + 1.0f) * 0.5f; // 0.0 to 1.0
        uint8_t brightness = (uint8_t)(breath * 50); // Scale to 0-50
        buttonLeds[0] = CRGB(brightness, brightness, brightness);
        
        FastLED.show();
    }
}

void setButtonLed(uint8_t buttonId, CRGB color) {
    if (buttonId < NUM_BUTTON_LEDS) {
        buttonLeds[buttonId] = color;
        FastLED.show();
    }
}

void setButtonLed(uint8_t buttonId, uint8_t r, uint8_t g, uint8_t b) {
    setButtonLed(buttonId, CRGB(r, g, b));
}

#endif // HAS_BUTTON_LEDS 