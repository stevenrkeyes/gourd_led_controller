#include <OctoWS2811.h>
#include "pins.h"
#include "led_strips.h"
#include <vector>

DMAMEM int displayMemoryLeds[LED_STRIP_NUM_LEDS];
int drawingMemoryLeds[LED_STRIP_NUM_LEDS];
const int config = WS2811_RGBW | WS2811_800kHz;
const int NUM_PINS = 1;
OctoWS2811 leds(LED_STRIP_NUM_LEDS, displayMemoryLeds, drawingMemoryLeds, config, NUM_PINS);

static std::vector<LedPulse> activePulses;
static const unsigned long pulseDuration = 400; // ms for pulse to travel full strip
static const uint32_t pulseColor = 0x00FFFFFF; // White
static const uint32_t backgroundColor = 0x00000000; // Off

void setupLedStrips() {
    leds.begin();
    leds.show(); // Clear LEDs to off

    // Set all LEDs to white using the white channel
    for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
        leds.setPixel(i, 0x00AAAA00); // W=0, G=170, R=0, B=0
    }
    leds.show();
}

void triggerLedPulse(unsigned long timestamp) {
    activePulses.push_back({timestamp});
}

void loopLedStrips() {
    unsigned long now = millis();
    // Clear LEDs
    for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
        leds.setPixel(i, backgroundColor);
    }
    // Draw all active pulses
    for (auto it = activePulses.begin(); it != activePulses.end(); ) {
        float progress = float(now - it->startTime) / pulseDuration;
        int ledIndex = int(progress * LED_STRIP_NUM_LEDS);
        if (ledIndex >= 0 && ledIndex < LED_STRIP_NUM_LEDS) {
            leds.setPixel(ledIndex, pulseColor);
            ++it;
        } else {
            it = activePulses.erase(it); // Remove finished pulse
        }
    }
    leds.show();
} 