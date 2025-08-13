#ifndef BUTTONS_H
#define BUTTONS_H

#ifdef TEENSY_A

#include <Arduino.h>

class Bounce;
extern Bounce button1;

void setupButtons();
void loopButtons();

#endif // TEENSY_A

#endif // BUTTONS_H 