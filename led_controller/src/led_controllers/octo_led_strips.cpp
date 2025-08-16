#include "octo_led_strips.h"
#include "config.h"

#if defined(TEENSY_B) || defined(TEENSY_C)

#include <OctoWS2811.h>
#include "shared/communication.h"

// LedPulse struct is defined in header

// OctoWS2811 setup for 8 strips
DMAMEM int displayMemoryLeds[LED_STRIP_NUM_LEDS * 8];
int drawingMemoryLeds[LED_STRIP_NUM_LEDS * 8];
const int config = WS2811_RGBW | WS2811_800kHz;
OctoWS2811 leds(LED_STRIP_NUM_LEDS, displayMemoryLeds, drawingMemoryLeds, config);

#define MAX_ACTIVE_PULSES 8
static LedPulse activePulses[MAX_ACTIVE_PULSES];
static const unsigned long pulseDuration = 400; // ms for pulse to travel full strip
static const uint32_t pulseColor = 0x00FFFFFF; // White
static const uint32_t backgroundColor = 0x00947BD3; // Off

void setupLedStrips() {
    leds.begin();
    leds.show(); // Clear LEDs to off

    // Initialize all pulses as inactive
    for (int i = 0; i < MAX_ACTIVE_PULSES; i++) {
        activePulses[i].active = false;
    }

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
        // Find first inactive slot
        for (int i = 0; i < MAX_ACTIVE_PULSES; i++) {
            if (!activePulses[i].active) {
                activePulses[i].startTime = timestamp;
                activePulses[i].strip = strip;
                activePulses[i].active = true;
                Serial.print("Triggered pulse on strip ");
                Serial.println(strip);
                break;
            }
        }
    }
}

void clearAllLEDs() {
    // Clear all LEDs
    for (int i = 0; i < LED_STRIP_NUM_LEDS * NUM_LED_STRIPS; i++) {
        leds.setPixel(i, backgroundColor);
    }
}

void drawAllPulses() {
    unsigned long now = millis();
    for (int i = 0; i < MAX_ACTIVE_PULSES; i++) {
        if (activePulses[i].active) {
            float progress = float(now - activePulses[i].startTime) / pulseDuration;
            int ledIndex = int(progress * LED_STRIP_NUM_LEDS);
            
            if (ledIndex >= 0 && ledIndex < LED_STRIP_NUM_LEDS) {
                int pixelIndex = activePulses[i].strip * LED_STRIP_NUM_LEDS + ledIndex;
                leds.setPixel(pixelIndex, pulseColor);
            } else {
                activePulses[i].active = false; // Remove finished pulse
            }
        }
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
    clearAllLEDs();
    // Draw active pulses
    drawAllPulses();
    
    leds.show();
}

#endif // TEENSY_B || TEENSY_C 