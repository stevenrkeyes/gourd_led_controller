#include <OctoWS2811.h>
#include "pins.h"
#include "led_strips.h"

DMAMEM int displayMemoryLeds[LED_STRIP_NUM_LEDS];
int drawingMemoryLeds[LED_STRIP_NUM_LEDS];
const int config = WS2811_RGBW | WS2811_800kHz;
OctoWS2811 leds(LED_STRIP_NUM_LEDS, displayMemoryLeds, drawingMemoryLeds, config, LED_STRIP_1_PIN);

void setupLedStrips() {
    leds.begin();
    leds.show(); // Clear LEDs to off

    // Set all LEDs to white using the white channel
    for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
        leds.setPixel(i, 0x00AAAA00); // W=0, G=170, R=0, B=0
    }
    leds.show();
}

void loopLedStrips() {
    // TODO: implement
} 