#ifndef _prefs_storage_h
#define _prefs_storage_h

#include "Settings.h"
#include "SharedData.h"
#include "BLE/BluetoothHandler.h"
#include <Preferences.h>

#define SETTINGS1 "SETTINGS1"
#define SETTINGS2 "SETTINGS2"
#define SETTINGS3 "SETTINGS3"
#define SETTINGS4 "SETTINGS4"
#define SETTINGS5 "SETTINGS5"
#define SETTINGS6 "SETTINGS6"

class preferences
{
public:

    void init();
    
    void test();

    void setSettings(Settings *);
    void setSharedData(SharedData *);

    void displaySettings1();

    void saveBleLockForced(int8_t *bleLockForced);
    void restoreBleLockForced(int8_t *bleLockForced);
    void saveBrakeMaxPressure();
    void saveBrakeMinPressure();
    void restoreBrakeMaxPressure();
    void restoreBrakeMinPressure();
    void saveOdo();
    void restoreOdo();
    void saveBatteryCalib();
    void restoreBatteryCalib();
    void saveSettings();
    boolean restoreSettings();

private:
    SharedData *shrd;
    Settings *settings;
    Preferences prefs;
};

#endif