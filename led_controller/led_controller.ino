#include <FastLED.h>
#include <Bounce2.h>

#define LOOP_DELAY_MS 2

constexpr int BOARD_LED_PIN = 13;

constexpr int BUTTON_COLUMN_1_PIN = 0;
constexpr int BUTTON_ROW_1_PIN = 1;

constexpr int LED_STRIP_1_PIN = 2;

#define LED_CHIPSET WS2811
#define LED_COLOR_ORDER GRB
#define LED_STRIP_NUM_LEDS 50

CRGB leds[LED_STRIP_NUM_LEDS];

Bounce button1(BUTTON_COLUMN_1_PIN, 50);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");

  pinMode(BUTTON_COLUMN_1_PIN, INPUT_PULLUP);

  FastLED.addLeds<LED_CHIPSET, LED_STRIP_1_PIN, LED_COLOR_ORDER>(leds, LED_STRIP_NUM_LEDS).setCorrection(TypicalLEDStrip);

  pinMode(BOARD_LED_PIN, OUTPUT);
}

void loop() {
  // read buttons
  button1.update();
  // send buttons to computer
  if (button1.fell()) {
    Serial.println("button pressed");
  }
  // update LED pattern
  delay(LOOP_DELAY_MS);
}
