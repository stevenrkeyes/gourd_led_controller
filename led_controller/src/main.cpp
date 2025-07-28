#include <Arduino.h>
#include "led_strips.h"
#include "eyes.h"
#include "buttons.h"
#include "pins.h"

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
  if (currentTime - previousMillis > 2000) {
    previousMillis = currentTime;
    ledState = !ledState;
    digitalWrite(BOARD_LED_PIN, ledState);
    Serial.println("test printout");
  }

  // Check for serial input
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'a') {
      Serial.println("serial 'a' received");
      triggerLedPulse(millis());
    }
  }

  loopLedStrips();
  loopEyes();
  loopButtons();
}
