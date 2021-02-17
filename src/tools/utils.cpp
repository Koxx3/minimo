#include "utils.h"

uint16_t RpmToKmh(Settings *settings, uint16_t rpm)
{
    float coef = (settings->getS1F().Wheel_size / 10.0) / settings->getS1F().Motor_pole_number / 10.5;
    return rpm * coef;
}

uint16_t KmhToRpm(Settings *settings, float kmh)
{
    float coef = (settings->getS1F().Wheel_size / 10.0) / settings->getS1F().Motor_pole_number / 10.5;
    return kmh / coef;
}

uint16_t ErpmToKmh2(Settings *settings, uint16_t rpm)
{
    float coef = ((settings->getS1F().Wheel_size / 10.0) * 2.54 * 0.001885) / settings->getS1F().Motor_pole_number;
    return rpm * coef;
}

uint16_t KmhToErpm2(Settings *settings, uint16_t kmh)
{
    float coef = ((settings->getS1F().Wheel_size / 10.0) * 2.54 * 0.001885) / settings->getS1F().Motor_pole_number;
    return kmh / coef;
}