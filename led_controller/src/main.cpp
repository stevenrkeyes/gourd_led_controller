#include <Arduino.h>
#include "led_strips.h"
#include "eyes.h"
#include "buttons.h"
#include "pins.h"

#define LOOP_DELAY_MS 2
// Do we need these?
#define LED_CHIPSET WS2811
#define LED_COLOR_ORDER WGRB

unsigned long previousMillis = 0;
const unsigned long interval = 500;  // 500ms interval
bool ledState = false;

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
  // flash LED and printout to confirm the program is running.
  unsigned long currentTime = millis();
  if (currentTime - previousMillis > 500) {
    previousMillis = currentTime;
    ledState = !ledState;
    digitalWrite(BOARD_LED_PIN, ledState);
    Serial.println("test printout");
  }

  loopLedStrips();
  loopEyes();
  loopButtons();
  delay(LOOP_DELAY_MS);
}
