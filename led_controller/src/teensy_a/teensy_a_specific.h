#ifndef TEENSY_A_SPECIFIC_H
#define TEENSY_A_SPECIFIC_H

#define RING_LED_PIN 22
#define NUM_RINGS 4            // Number of ring LEDs
#define NUM_LEDS_PER_RING 24 
#define TOTAL_RING_LED_COUNT (NUM_RINGS * NUM_LEDS_PER_RING)  // Total LEDs across all rings 

#ifdef TEENSY_A

#include <Arduino.h>
#include "shared/communication.h"

void setupTeensyA();
void loopTeensyA();
void handleButtonLedCommand(const CommandPacket& packet);
void handleRingLedCommand(const CommandPacket& packet); 
void readAndSendSensorData();

#endif // TEENSY_A

#endif // TEENSY_A_SPECIFIC_H 