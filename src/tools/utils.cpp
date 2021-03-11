#include "utils.h"

uint16_t RpmToKmh(Settings *settings, uint16_t rpm)
{
    float coef = settings->get_Wheel_size() / settings->get_Number_of_poles_pairs() / 10.46;
    return rpm * coef;
}

uint16_t KmhToRpm(Settings *settings, float kmh)
{
    float coef = settings->get_Wheel_size() / settings->get_Number_of_poles_pairs() / 10.46;
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