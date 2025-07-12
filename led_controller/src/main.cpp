#include <OctoWS2811.h>
#include <Bounce2.h>
#include <FastLED.h>

#define LOOP_DELAY_MS 2
#define LED_CHIPSET WS2811
#define LED_COLOR_ORDER WGRB

constexpr int BOARD_LED_PIN = 13;

constexpr int BUTTON_COLUMN_1_PIN = 0;
constexpr int BUTTON_ROW_1_PIN = 1;

constexpr int LED_STRIP_1_PIN = 2;
constexpr int LED_STRIP_NUM_LEDS = 15;

constexpr int EYE_LEDS_1_PIN = 22;  // Try a different pin
constexpr int NUM_EYES = 3;  // Try with just 1 eye first
constexpr int OUTER_EYE_NUM_LEDS = 24;
constexpr int INNER_EYE_NUM_LEDS = 8;
constexpr int EYE_NUM_LEDS = OUTER_EYE_NUM_LEDS + INNER_EYE_NUM_LEDS;

// OctoWS2811 for main LED strip
DMAMEM int displayMemoryLeds[LED_STRIP_NUM_LEDS];
int drawingMemoryLeds[LED_STRIP_NUM_LEDS];
const int config = WS2811_RGBW | WS2811_800kHz;
OctoWS2811 leds(LED_STRIP_NUM_LEDS, displayMemoryLeds, drawingMemoryLeds, config, LED_STRIP_1_PIN);

// FastLED for eye LEDs
CRGB eyeLeds[NUM_EYES * EYE_NUM_LEDS];

Bounce button1(BUTTON_COLUMN_1_PIN, 50);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  
  pinMode(BOARD_LED_PIN, OUTPUT);
  pinMode(BUTTON_COLUMN_1_PIN, INPUT_PULLUP);

  leds.begin();
  leds.show(); // Clear LEDs to off

  // Set all LEDs to white using the white channel
  for (int i = 0; i < LED_STRIP_NUM_LEDS; i++) {
    leds.setPixel(i, 0x00AAAA00); // W=0, G=170, R=0, B=0
  }
  leds.show();

  // Initialize FastLED for eye LEDs
  FastLED.addLeds<WS2811, EYE_LEDS_1_PIN, GRB>(eyeLeds, NUM_EYES * EYE_NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  // Set eye LED pattern
  for (int i = 0; i < NUM_EYES; i++) {
    int eye_offset = i * EYE_NUM_LEDS;
    int inner_eye_offset = eye_offset + OUTER_EYE_NUM_LEDS;
    for (int j = 0; j < OUTER_EYE_NUM_LEDS; j++) {
      eyeLeds[eye_offset + j] = CRGB::Red; // Red outer ring
    }
    for (int j = 0; j < INNER_EYE_NUM_LEDS; j++) {
      eyeLeds[inner_eye_offset + j] = CRGB::Red; // Green inner ring
    }
  }
  FastLED.show();
  Serial.println("Setup complete");
}

void loop() {
  // Simple test - just flash the LED
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(500);
  digitalWrite(BOARD_LED_PIN, LOW);
  delay(500);

  // Set eye LED pattern
  for (int i = 0; i < NUM_EYES; i++) {
    int eye_offset = i * EYE_NUM_LEDS;
    int inner_eye_offset = eye_offset + OUTER_EYE_NUM_LEDS;
    for (int j = 0; j < OUTER_EYE_NUM_LEDS; j++) {
      eyeLeds[eye_offset + j] = CRGB::Red; // Red outer ring
    }
    for (int j = 0; j < INNER_EYE_NUM_LEDS; j++) {
      eyeLeds[inner_eye_offset + j] = CRGB::Red; // Green inner ring
    }
  }
  FastLED.show();
}
