#ifndef _prefs_storage_h
#define _prefs_storage_h

#include "SharedData.h"
#include "BLE/BluetoothHandler.h"
#include <Preferences.h>


class preferences
{
public:

    void init();
    
    void setSharedData(SharedData *);
    
    void restore();

    void displaySettings1();

    void saveBleLockForced();
    void saveBrakeMaxPressure();
    void saveBrakeMinPressure();
    void saveOdo();
    void saveBatteryCalib();

private:
    SharedData *shrd;
    Preferences prefs;

    void restoreMode();
    void restoreBleLockForced();
    void restoreBrakeMaxPressure();
    void restoreBrakeMinPressure();
    void restoreOdo();
    void restoreBatteryCalib();

};

#endif