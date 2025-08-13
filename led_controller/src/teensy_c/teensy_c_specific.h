#ifndef TEENSY_C_SPECIFIC_H
#define TEENSY_C_SPECIFIC_H

#ifdef TEENSY_C

#include <Arduino.h>
#include "shared/communication.h"

void setupTeensyC();
void loopTeensyC();

#endif // TEENSY_C

#endif // TEENSY_C_SPECIFIC_H 