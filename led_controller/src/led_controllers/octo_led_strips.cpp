#include "octo_led_strips.h"
#include "config.h"

#ifdef HAS_OCTO_LED_STRIPS

#include <OctoWS2811.h>
#include "shared/communication.h"
#include <vector>

// Define LedPulse struct
struct LedPulse {
    unsigned long startTime;
    int strip;
};

// OctoWS2811 setup for 8 strips
DMAMEM int displayMemoryLeds[LED_STRIP_NUM_LEDS * 8];
int drawingMemoryLeds[LED_STRIP_NUM_LEDS * 8];
const int config = WS2811_RGBW | WS2811_800kHz;
OctoWS2811 leds(LED_STRIP_NUM_LEDS, displayMemoryLeds, drawingMemoryLeds, config, 8);

static std::vector<LedPulse> activePulses;
static const unsigned long pulseDuration = 400; // ms for pulse to travel full strip
static const uint32_t pulseColor = 0x00FFFFFF; // White
static const uint32_t backgroundColor = 0x00000000; // Off

void setupLedStrips() {
    leds.begin();
    leds.show(); // Clear LEDs to off

    // Set all LEDs to dim white using the white channel
    for (int strip = 0; strip < 8; strip++) {
        for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
            int pixelIndex = strip * LED_STRIP_NUM_LEDS + i;
            leds.setPixel(pixelIndex, 0x00AAAA00); // W=0, G=170, R=0, B=0
        }
    }
    leds.show();
    
    Serial.println("OctoWS2811 LED strips initialized");
}

void triggerLedPulse(unsigned long timestamp, int strip) {
    if (strip >= 0 && strip < 8) {
        activePulses.push_back({timestamp, strip});
        Serial.print("Triggered pulse on strip ");
        Serial.println(strip);
    }
}

void loopLedStrips() {
    // Handle incoming commands from Pi
    CommandPacket packet;
    if (receiveCommand(packet)) {
        if (packet.command == CMD_LED_PULSE) {
            int strip = (packet.data_length > 0) ? packet.data[0] : 0;
            triggerLedPulse(millis(), strip);
        }
    }

    // Update LED animations
    unsigned long now = millis();
    
    // Clear all LEDs
    for (int i = 0; i < LED_STRIP_NUM_LEDS * 8; i++) {
        leds.setPixel(i, backgroundColor);
    }
    
    // Draw active pulses
    for (auto it = activePulses.begin(); it != activePulses.end(); ) {
        float progress = float(now - it->startTime) / pulseDuration;
        int ledIndex = int(progress * LED_STRIP_NUM_LEDS);
        
        if (ledIndex >= 0 && ledIndex < LED_STRIP_NUM_LEDS) {
            int pixelIndex = it->strip * LED_STRIP_NUM_LEDS + ledIndex;
            leds.setPixel(pixelIndex, pulseColor);
            ++it;
        } else {
            it = activePulses.erase(it); // Remove finished pulse
        }
    }
    
    leds.show();
}

#endif // HAS_OCTO_LED_STRIPS 