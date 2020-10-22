//////------------------------------------
////// Manage settings exchanged in BLE and stored in EEPOM

#include "Arduino.h"
#include "MedianFilter.h"

#ifndef _SharedData_h

#define _SharedData_h

class SharedData
{
public:
    SharedData(){};

    uint32_t timeLastNotifyBle = 0;

    boolean inOtaMode = false;

    float currentHumidity = 0.0;
    float currentTemperature = 0.0;

    uint8_t speedLimiter = 1;

    double speedCurrent = 0.0;
    double speedOld = 0;
    
    uint32_t speedPidKp = 4;
    uint32_t speedPidKi = 8;
    uint32_t speedPidKd = 1;

    uint8_t powerReduction = 0;

    uint8_t modeOrder = 3;
    uint8_t modeLcd = 0;
    uint8_t modeLcdOld = 0;

    uint8_t accelOrder = 0;
    uint8_t accelLcd = 0;
    uint8_t accelLcdOld = 0;

    uint8_t ecoOrder = 3;
    uint8_t ecoLcd = 3;
    uint8_t ecoLcdOld = 3;

    uint8_t auxOrder = 0;

    uint8_t brakeStatus = 0;
    uint8_t brakeStatusOld = 0;
    int8_t brakeSentOrder = -1;
    uint8_t brakeLcd = 0;
    int8_t brakeLcdOld = -1;
    int8_t brakeSentOrderOld = -1;

    uint8_t currentCalibOrder = 1;
    uint8_t brakeCalibOrder = 1;

    uint32_t voltageFilterMean = 0;
    uint32_t currentFilterMean = 0;
};

#endif
