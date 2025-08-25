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
#define FIRE_UPDATE_INTERVAL 100 // ms between fire updates

static LedPulse activePulses[MAX_ACTIVE_PULSES];
static uint8_t fireHeat[8][LED_STRIP_NUM_LEDS]; // Simplified: just heat values
static const unsigned long pulseDuration = 400; // ms for pulse to travel full strip
static const uint32_t pulseColor = 0x00FFFFFF; // White
static const uint32_t backgroundColor = 0x00000000; // Black background

// Simplified fire colors - just 4 main colors instead of 256
const uint32_t fireColors[4] = {
    0x00000000, // Black
    0x00400000, // Dark red
    0x00800000, // Red  
    0x00400000  // Orange
};

void setupLedStrips() {
    leds.begin();
    leds.show(); // Clear LEDs to off

    // Initialize all pulses as inactive
    for (int i = 0; i < MAX_ACTIVE_PULSES; i++) {
        activePulses[i].active = false;
    }

    // Initialize fire heat for each strip
    for (int strip = 0; strip < 8; strip++) {
        for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
            fireHeat[strip][i] = random(0, 3); // Simple 0-3 heat levels
        }
    }

    Serial.println("OctoWS2811 LED strips initialized with simplified fire effect");
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

void drawFire() {
    for (int strip = 0; strip < 8; strip++) {
        for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
            // Simplified: just use the heat value directly
            uint8_t heat = fireHeat[strip][i];
            
            // Get color from fire palette
            uint32_t color = fireColors[heat];
            
            // Set the pixel
            int pixelIndex = strip * LED_STRIP_NUM_LEDS + i;
            leds.setPixel(pixelIndex, color);
        }
    }
}

void updateFire() {
    unsigned long now = millis();
    
    for (int strip = 0; strip < 8; strip++) {
        if (now - fireHeat[strip][0] < FIRE_UPDATE_INTERVAL) {
            continue; // Skip this strip if not time to update
        }
        
        // Simple fire physics: heat rises and cools
        for (int i = LED_STRIP_NUM_LEDS - 1; i > 0; i--) {
            fireHeat[strip][i] = fireHeat[strip][i-1];
        }
        
        // Bottom pixel gets new heat
        fireHeat[strip][0] = random(2, 4);
        
        // Add some randomness for flickering
        for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
            if (random(100) < 30) { // 30% chance to flicker
                fireHeat[strip][i] = constrain(fireHeat[strip][i] + random(-1, 2), 0, 3);
            }
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
    drawFire(); // Draw the fire effect
    drawAllPulses(); // Draw active pulses on top
    updateFire(); // Update fire physics
    
    leds.show();
}

#endif // TEENSY_B || TEENSY_C 