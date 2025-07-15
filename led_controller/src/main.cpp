#include <Arduino.h>
#include "led_strips.h"
#include "eyes.h"
#include "buttons.h"

#define LOOP_DELAY_MS 2
// Do we need these?
#define LED_CHIPSET WS2811
#define LED_COLOR_ORDER WGRB

constexpr int BOARD_LED_PIN = 13;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  pinMode(BOARD_LED_PIN, OUTPUT);

  setupLedStrips();
  setupEyes();
  setupButtons();

  Serial.println("Setup complete");
}

void loop() {
  // flash LED to confirm the program is running.
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(500);
  digitalWrite(BOARD_LED_PIN, LOW);
  delay(500);

  loopLedStrips();
  loopEyes();
  loopButtons();
  delay(LOOP_DELAY_MS);
}
