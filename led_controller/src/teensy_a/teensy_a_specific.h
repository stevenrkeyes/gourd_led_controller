#ifndef TEENSY_A_SPECIFIC_H
#define TEENSY_A_SPECIFIC_H

#ifdef TEENSY_A

#include <Arduino.h>
#include "shared/communication.h"

void setupTeensyA();
void loopTeensyA();

void handleButtonLedCommand(const CommandPacket& packet);
// Not used anymore.
// void handleRingLedCommand(const CommandPacket& packet); 
void readAndSendSensorData();

#endif // TEENSY_A

#endif // TEENSY_A_SPECIFIC_H 