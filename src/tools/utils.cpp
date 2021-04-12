#include "utils.h"

uint16_t RpmToKmh(Settings *settings, uint16_t rpm)
{
    float coef = settings->get_Wheel_size() / settings->get_Number_of_poles_pairs() / 10;
    return rpm * coef;
}

uint16_t KmhToRpm(Settings *settings, float kmh)
{
    float coef = settings->get_Wheel_size() / settings->get_Number_of_poles_pairs() / 10;
    return kmh / coef;
}

uint16_t ErpmToKmh2(Settings *settings, uint16_t rpm)
{
    float coef = (settings->get_Wheel_size() * 2.54 * 0.001885) / settings->get_Number_of_poles_pairs();
    return rpm * coef;
}

uint16_t KmhToErpm2(Settings *settings, uint16_t kmh)
{
    float coef = (settings->get_Wheel_size() * 2.54 * 0.001885) / settings->get_Number_of_poles_pairs();
    return kmh / coef;
}

int getValueFromString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]).toInt() : -1;
}
