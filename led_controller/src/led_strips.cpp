#include <OctoWS2811.h>
#include "pins.h"
#include "led_strips.h"
#include <vector>

DMAMEM int displayMemoryLeds[TOTAL_NUM_LEDS];
int drawingMemoryLeds[TOTAL_NUM_LEDS];
const int config = WS2811_RGBW | WS2811_800kHz;
OctoWS2811 leds(LED_STRIP_NUM_LEDS, displayMemoryLeds, drawingMemoryLeds, config, LED_STRIP_NUM_STRIPS);

static std::vector<LedPulse> activePulses;
static const unsigned long pulseDuration = 400; // ms for pulse to travel full strip
static const uint32_t pulseColor = 0x00AAFFFF; // White
static const uint32_t backgroundColor = 0x11000000; // Off

void setAllLEDs(uint32_t color) {
    for (int strip_num = 0; strip_num < LED_STRIP_NUM_STRIPS; strip_num++) {
        int offset = strip_num * LED_STRIP_NUM_LEDS;
        for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
            leds.setPixel(offset + i, color);
        }    
    }
}

void setupLedStrips() {
    leds.begin();
    leds.show(); // Clear LEDs to off

    // Set all LEDs to white using the white channel
    setAllLEDs(pulseColor);
    leds.show();
}

void triggerLedPulse(unsigned long timestamp, int index) {
    activePulses.push_back({timestamp, index});
    activePulses.push_back({timestamp, 3}); // testing only.
}

void loopLedStrips() {
    unsigned long now = millis();
    // Clear LEDs
    setAllLEDs(backgroundColor);

    // Draw all active pulses
    for (auto it = activePulses.begin(); it != activePulses.end(); ) {
        float progress = float(now - it->startTime) / pulseDuration;
        // TODO: Rename to be more clear that the index is just the index of the pulse, not the final index.
        int ledIndex = int(progress * LED_STRIP_NUM_LEDS);
        if (ledIndex >= 0 && ledIndex < LED_STRIP_NUM_LEDS) {
            int offset = it->index * LED_STRIP_NUM_LEDS;
            leds.setPixel(offset + ledIndex, pulseColor);
            ++it;
        } else {
            it = activePulses.erase(it); // Remove finished pulse
        }
    }
    leds.show();
} 