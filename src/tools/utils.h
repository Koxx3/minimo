#ifndef _Utils_h

#define _Utils_h

#include "Arduino.h"
#include "Settings.h"

uint16_t RpmToKmh(Settings *settings, uint16_t rpm);
uint16_t KmhToRpm(Settings *settings, float kmh);
uint16_t ErpmToKmh2(Settings *settings, uint16_t rpm);
uint16_t KmhToErpm2(Settings *settings, uint16_t kmh);

int getValueFromString(String data, char separator, int index);

#endif