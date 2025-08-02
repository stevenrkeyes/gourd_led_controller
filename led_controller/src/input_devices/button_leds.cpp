#include "button_leds.h"
#include "config.h"

#ifdef HAS_BUTTON_LEDS

#include <FastLED.h>
#include "shared/communication.h"

#ifdef TEENSY_A
#include "pins_teensy_a.h"
#endif

// Array to hold button LED colors
CRGB buttonLeds[4];

void setupButtonLeds() {
    // TODO: Update this to use the new array-based pin definitions
    // Temporarily disabled to fix compilation
    /*
    // Initialize button LEDs using FastLED
    // Note: You may need to adjust the LED type and color order based on your actual LEDs
    FastLED.addLeds<WS2812, BUTTON_LED_1_PIN, GRB>(&buttonLeds[0], 1);
    FastLED.addLeds<WS2812, BUTTON_LED_2_PIN, GRB>(&buttonLeds[1], 1);
    FastLED.addLeds<WS2812, BUTTON_LED_3_PIN, GRB>(&buttonLeds[2], 1);
    FastLED.addLeds<WS2812, BUTTON_LED_4_PIN, GRB>(&buttonLeds[3], 1);
    
    // Set initial state - all LEDs off
    FastLED.clear();
    FastLED.show();
    */
    
    Serial.println("Button LEDs temporarily disabled - buttons only");
}

void loopButtonLeds() {
    // Button LED updates - temporarily disabled
}

void setButtonLed(int buttonId, uint8_t r, uint8_t g, uint8_t b) {
    // Temporarily disabled
    Serial.print("Would set button LED ");
    Serial.print(buttonId);
    Serial.print(" to RGB(");
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.print(b);
    Serial.println(")");
}

#endif // HAS_BUTTON_LEDS 