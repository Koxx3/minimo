//////------------------------------------
////// Manage settings exchanged in BLE and stored in EEPOM

#ifndef _SharedData_h
#define _SharedData_h

#include "Arduino.h"
#include "Filters/MedianFilter.h"
// The actions I ca do...
typedef enum
{
    ACTION_OFF, // set LED "OFF".
    ACTION_ON   // set LED "ON"
} MyActions;

typedef enum
{
    OTA_OFF,        // NORMAL MODE
    OTA_IDE,    // IDE OTA MODE
    OTA_SERVER  // OTA CLOUD - SERVER MODE
} OTAmode;

class SharedData
{
public:
    SharedData(){};

    uint32_t timeLastNotifyBle = 0;

    uint8_t inOtaMode = OTA_OFF;

    float currentHumidity = 0.0;
    float currentTemperature = 0.0;

    uint8_t speedLimiter = 1;

    double speedCurrent = 0.0;
    double speedMax = 0.0;
    double speedOld = 0;

    uint32_t distanceTrip = 0; // this TRIP can be reset from/for display // 1 unit = 0.1m ? 
    uint32_t distanceTripForOdo = 0; // this TRIP should never be reset
    uint32_t distanceOdo = 0;  // 1 unit = 100m
    uint32_t distanceOdoBoot = 0;
    uint32_t distanceOdoInFlash = 0;
    uint32_t distancePrevTime = 0;

    uint8_t powerReduction = 0;

    uint8_t modeOrder = 3;
    uint8_t modeLcd = 0;
    uint8_t modeLcdOld = 0;
    int8_t modeOrderBeforeNitro = -1;

    uint8_t accelOrder = 0;
    uint8_t accelLcd = 0;
    uint8_t accelLcdOld = 0;

    uint8_t ecoOrder = 3;
    uint8_t ecoLcd = 3;
    uint8_t ecoLcdOld = 3;

    uint8_t auxOrder = 0;

    uint8_t brakePressedStatus = 0; // is brake pressed
    uint8_t brakePressedStatusOld = 0;
    int8_t brakeSentOrder = -1;  // brake force order sent to controller
    int8_t brakeSentOrderOld = -1; 
    int8_t brakeSentOrderFromBLE = -1; // brake force order from LCD
    uint8_t brakeDisplay = 0;   // brake force order sent from escooter display
    int8_t brakeDisplayOld = -1;

    uint8_t brakeFordidenHighVoltage = 0;

    int32_t brakeMaxPressureRaw = -1;
    int32_t brakeMinPressureRaw = -1;
    uint32_t brakeAnalogValue = 0;

    uint32_t throttleAnalogValue = 0;

    uint32_t brakeMaxCalibOrder = 1;

    uint32_t voltageActual = 0;
    uint32_t voltageFilterMean = 0;
    uint32_t voltageFilterLongMean = 0;
    uint32_t voltageRawFilterMean = 0;
    float batteryMinVoltageCalibUser;
    uint32_t batteryMinVoltageCalibRaw;
    float batteryMaxVoltageCalibUser;
    uint32_t batteryMaxVoltageCalibRaw;

    uint8_t batteryLevel;
    uint8_t autonomyLeft;

    //int32_t currentRawFilterMean = 0;
    int32_t currentActual = 0; // in milliamps
    uint8_t currentSensorPresent = 1;

    MyActions button1ClickStatus = ACTION_OFF;
    uint32_t button1LpDuration = 0;
    MyActions button2ClickStatus = ACTION_OFF;
    uint32_t button2LpDuration = 0;
    boolean button1LpProcessed = false;
    boolean button2LpProcessed = false;
};

#endif
