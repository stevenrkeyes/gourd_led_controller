#include <Arduino.h>
#include "config.h"
#include "shared/communication.h"

// Device-specific includes
#ifdef TEENSY_A
  #include "pins_teensy_a.h"
  #include "teensy_a/teensy_a_specific.h"
  #ifdef HAS_BUTTONS
    #include "input_devices/buttons.h"
  #endif
  #ifdef HAS_BUTTON_LEDS
    #include "input_devices/button_leds.h"
  #endif
#endif

#ifdef TEENSY_B
  #include "pins_teensy_b.h"
  #include "teensy_b/teensy_b_specific.h"
  #ifdef HAS_OCTO_LED_STRIPS
    #include "led_controllers/octo_led_strips.h"
    #include "led_controllers/effects.h"
  #endif
#endif

#ifdef TEENSY_C
  #include "pins_teensy_c.h"
  #include "teensy_c/teensy_c_specific.h"
  #ifdef HAS_OCTO_LED_STRIPS
    #include "led_controllers/octo_led_strips.h"
    #include "led_controllers/effects.h"
  #endif
#endif

unsigned long previousMillis = 0;
const unsigned long interval = 500;
bool ledState = false;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  
#ifdef TEENSY_A
  Serial.println("Starting Teensy A - Input Controller with Button LEDs");
#elif defined(TEENSY_B)
  Serial.println("Starting Teensy B - LED Strip Controller");
#elif defined(TEENSY_C)
  Serial.println("Starting Teensy C - LED Strip Controller");
#endif

  pinMode(BOARD_LED_PIN, OUTPUT);
  initCommunication();

  // Initialize based on capabilities, not just device
#ifdef HAS_BUTTONS
  setupButtons();
#endif

#ifdef HAS_BUTTON_LEDS
  setupButtonLeds();
#endif

#ifdef HAS_OCTO_LED_STRIPS
  setupLedStrips();
  setupEffects();
#endif

  // Device-specific initialization
#ifdef TEENSY_A
  setupTeensyA();
#elif defined(TEENSY_B)
  setupTeensyB();
#elif defined(TEENSY_C)
  setupTeensyC();
#endif

  Serial.println("Setup complete");
}

void loop() {
  // Heartbeat LED
  unsigned long currentTime = millis();
  if (currentTime - previousMillis > interval) {
    previousMillis = currentTime;
    ledState = !ledState;
    digitalWrite(BOARD_LED_PIN, ledState);
    sendHeartbeat();
  }

  // Run based on capabilities
#ifdef HAS_BUTTONS
  loopButtons();
#endif

#ifdef HAS_BUTTON_LEDS
  loopButtonLeds();
#endif

#ifdef HAS_OCTO_LED_STRIPS
  loopLedStrips();
  loopEffects();
#endif

  // Device-specific loop code
#ifdef TEENSY_A
  loopTeensyA();
#elif defined(TEENSY_B)
  loopTeensyB();
#elif defined(TEENSY_C)
  loopTeensyC();
#endif

  delay(LOOP_DELAY_MS);
}
