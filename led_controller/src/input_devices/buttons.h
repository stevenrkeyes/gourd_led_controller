#ifndef BUTTONS_H
#define BUTTONS_H

#ifdef HAS_BUTTONS

#include <Arduino.h>

class Bounce;
extern Bounce button1;

void setupButtons();
void loopButtons();

#endif // HAS_BUTTONS

#endif // BUTTONS_H 