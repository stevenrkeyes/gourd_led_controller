#include "led_strips.h"
#include <random>

DMAMEM int displayMemoryLeds[TOTAL_NUM_LEDS];
int drawingMemoryLeds[TOTAL_NUM_LEDS];
const int config = WS2811_RGBW | WS2811_800kHz;
OctoWS2811 leds(LED_STRIP_NUM_LEDS, displayMemoryLeds, drawingMemoryLeds, config, NUM_STRIPS);

std::mt19937 generator(std::random_device{}());

int getRandomColor() {
    std::uniform_int_distribution<std::size_t> distribution(0, LED_COLOR_LIST.size() - 1);
    std::size_t random_color_index = distribution(generator);
    return LED_COLOR_LIST[random_color_index];
}

void setupLedStrips() {
    leds.begin();
    leds.show(); // Clear LEDs to off
    for (int strip_num = 0; strip_num < NUM_STRIPS; strip_num++) {
        // Pick a random color
        int strip_color = getRandomColor();
        // Example: set all LEDs on both strips to different colors
        const int strip_offset = strip_num * LED_STRIP_NUM_LEDS;
        for (int led_index = 0; led_index < LED_STRIP_NUM_LEDS; led_index++) {
            leds.setPixel(led_index + strip_offset, strip_color);
        }
    }
    leds.show();
}

void loopLedStrips() {
    // Does nothing, at the moment.
} 