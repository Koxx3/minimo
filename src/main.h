#ifndef _main_h
#define _main_h

#include <Arduino.h>


uint8_t modifyBrakeFromAnalog(char, char *);

void processButton1Click();
void processButton1LpStart();
void processButton1LpDuring();
void processButton1LpStop();

void processButton2Click();
void processButton2LpStart();
void processButton2LpDuring();
void processButton2LpStop();

//void processVoltage();
void processSpeedLimiterEvent(uint8_t, bool);
void processLockEvent(uint8_t, bool);
void processAuxEvent(uint8_t, bool);
void processModeEvent(uint8_t, bool);

void resetPid();

bool isElectricBrakeForbiden();

void disableWatchdog();

void saveBleLockForced();
void restoreBleLockForced();
void saveBrakeMinPressure();
void saveBrakeMaxPressure();
void saveOdo();
void saveBatteryCalib();
void saveSettings();

void notifySettingsChangedWithBle();

void displaySpeed();

boolean restoreSettings();

//void setupAutonomy();
//void setupBattery();

void computeDistance(float);
void checkAndSaveOdo();

void processWifi();


#endif