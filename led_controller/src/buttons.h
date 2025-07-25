#ifndef BUTTONS_H
#define BUTTONS_H

#include "pins.h"

class Bounce;
extern Bounce button1;

// Add your declarations here
void setupButtons();
void loopButtons();

#endif // BUTTONS_H 