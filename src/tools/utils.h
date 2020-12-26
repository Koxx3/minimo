#ifndef _Utils_h

#define _Utils_h

#include "Arduino.h"
#include "Settings.h"

uint16_t RpmToKmh(Settings *settings, uint16_t rpm);
uint16_t KmhToRpm(Settings *settings, uint16_t kmh);

#endif