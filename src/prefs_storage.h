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
    
    void setSettings(Settings *);
    void setSharedData(SharedData *);
    
    void restore();

    void displaySettings1();

    void saveMode();
    void saveBleLockForced();
    void saveBrakeMaxPressure();
    void saveBrakeMinPressure();
    void saveOdo();
    void saveBatteryCalib();

    void saveSettings();
    boolean restoreSettings();

private:
    SharedData *shrd;
    Settings *settings;
    Preferences prefs;

    void restoreMode();
    void restoreBleLockForced();
    void restoreBrakeMaxPressure();
    void restoreBrakeMinPressure();
    void restoreOdo();
    void restoreBatteryCalib();

};

#endif