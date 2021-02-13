
#ifndef _Buttons_h
#define _Buttons_h

#include "OneButton.h"
#include "pinout.h"
#include "SharedData.h"
#include "Settings.h"
#include "BLE/BluetoothHandler.h"

#include <Arduino.h>

// BUTTONS
#define BUTTON_LONG_PRESS_TICK 300

class Buttons
{
public:
    Buttons();
    static void setup(SharedData *shrd_p, BluetoothHandler *blh_p, Settings *settings_p);
    static void processTicks();

private:
    static SharedData *shrd;
    static BluetoothHandler *blh;
    static Settings *settings;

    static void processButton1Click();
    static void processButton1LpStart();
    static void processButton1LpDuring();
    static void processButton1LpStop();
    static void processButton1();

    static void processButton2Click();
    static void processButton2LpStart();
    static void processButton2LpDuring();
    static void processButton2LpStop();
    static void processButton2();

    static void processAuxEvent(uint8_t buttonId, bool isLongPress);
    static void processSpeedLimiterEvent(uint8_t buttonId, bool isLongPress);
    static void processLockEvent(uint8_t buttonId, bool isLongPress);
    static void processModeEvent(uint8_t buttonId, bool isLongPress);
};
#endif