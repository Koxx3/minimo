#ifndef _ioprocessing_h
#define _ioprocessing_h

#include <Arduino.h>
#include "pinout.h"

// I2C
#define I2C_FREQ 1000000

// ADC
#define ANALOG_TO_VOLTS_5V 0.0012207 // 4096 = 5V
#define ANALOG_TO_VOLTS_A 0.0213
#define ANALOG_TO_VOLTS_B 5.4225
#define ANALOG_TO_CURRENT 35
#define ANALOG_CURRENT_MIN_RAW_READING 500
#define NB_CURRENT_FILTER_DATA 20
#define NB_CURRENT_FILTER_CALIB_DATA 20
#define NB_VOLTAGE_FILTER_DATA 10
#define NB_THROTTLE_FILTER_DATAS 10


#define ANALOG_BRAKE_MIN_ERR_VALUE 400
#define ANALOG_BRAKE_MAX_ERR_VALUE 3500
#define ANALOG_BRAKE_MIN_VALUE 920
#define ANALOG_BRAKE_MIN_OFFSET 100
#define ANALOG_BRAKE_MAX_VALUE 2300
#define ANALOG_BRAKE_DIGITIAL_MIN_OFFSET 2000

#define TEMP_SENSOR_NONE -1
#define TEMP_SENSOR_SHTC3 0
#define TEMP_SENSOR_DS9990 1
#define TEMP_SENSOR_VESC 2


void setupI2C();
void setupDac();
void setupShtc3();
void setupBattery();
void setupVoltage();
void setupAutonomy();

void processDacOutput();
void processDHT();
void processSHTC3(bool requestRead);
void processVoltage();
void processVoltageLongFilter();
void processVoltageTooHighForBrake();
void processAutonomy();
void processCurrent();
void processRelay();

void checkAndPowerOff();
void powerOff();

void getBrakeFromAnalog();
void getThrottleFromAnalog();
bool isElectricBrakeForbiden();
void changeBrakeIOState();


#endif