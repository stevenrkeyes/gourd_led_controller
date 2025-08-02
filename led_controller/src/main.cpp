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
bool is_button_teensy = true;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  pinMode(BOARD_LED_PIN, OUTPUT);

  // TODO: Set pin 23 (which is hardwired to ground on the buttons teensy) to INPUT_PULLUP.
  // Then check its value.
  // If value is LOW (pulled down by ground), we know we're the button teensy.
  // If value is HIGH (unaffected), we know we're the LED teensy (set is_button_teensy to false).
  if (is_button_teensy) {
    setupButtons();
    setupEyes();  
  } else {
    setupLedStrips();
  }

  Serial.println("Setup complete");
}

void loop() {
  // flash LED and printout to confirm the program is running.
  unsigned long currentTime = millis();
  if (currentTime - previousMillis > 500) {
    previousMillis = currentTime;
    ledState = !ledState;
    digitalWrite(BOARD_LED_PIN, ledState);
  }

  if (is_button_teensy) {
    loopButtons();
    loopEyes();
  } else {
    loopLedStrips();
  }
  delay(LOOP_DELAY_MS);
}
