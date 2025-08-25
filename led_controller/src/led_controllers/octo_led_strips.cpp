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
static const uint32_t backgroundColor = 0x00000000; // Black background

void setupLedStrips() {
    leds.begin();
    leds.show(); // Clear LEDs to off

    // Initialize all pulses as inactive
    for (int i = 0; i < MAX_ACTIVE_PULSES; i++) {
        activePulses[i].active = false;
    }

    Serial.println("OctoWS2811 LED strips");
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
    // Clear all LEDs to background color
    for (int i = 0; i < LED_STRIP_NUM_LEDS * NUM_LED_STRIPS; i++) {
        leds.setPixel(i, backgroundColor);
    }
}

// TODO: decide if we want this.
const unsigned long BREATHING_PERIOD = 5000;
const float BREATHING_SPEEDUP = 2.5;
const float MIN_INTENSITY = 0.05;
const float MAX_INTENSITY = 0.5;
const float INTENSITY_DECREASE = 2.0;
unsigned long breathing_start = 0;

void drawSineBreathing() {
    unsigned long current_time = millis();

    for (int strip_index = 0; strip_index < NUM_LED_STRIPS; strip_index++) {
        float breathing_progress = float(current_time - breathing_start) / BREATHING_PERIOD;
        float sine_value = sin(breathing_progress * 2 * PI); // -1 to +1
        float intensity = (sine_value + 1.0) / 2.0; // 0.0 to 1.0
        intensity = MIN_INTENSITY + (MAX_INTENSITY - MIN_INTENSITY) * intensity;
    
        // Apply intensity to RGB values directly
        uint8_t r = 0xFF * intensity;
        uint8_t g = 0x22 * intensity;
        uint8_t b = 0xAA * intensity;

        // Correct color packing using OR (|) not XOR (^)
        uint32_t color = (g << 16) | (r << 8) | b;
        
        for (int led_index = 0; led_index < LED_STRIP_NUM_LEDS; led_index++) {
            int offset = strip_index * LED_STRIP_NUM_LEDS;
            leds.setPixel(offset + led_index, color);
        }
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
    drawAllPulses(); // Draw active pulses on top
    drawSineBreathing();
    
    leds.show();
}

#endif // TEENSY_B || TEENSY_C 