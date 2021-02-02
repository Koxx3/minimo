#ifndef _EEPROM_storage_h
#define _EEPROM_storage_h

#include "Settings.h"
#include "SharedData.h"
#include "BLE/BluetoothHandler.h"

#define EEPROM_SIZE 0x001000

#define EEPROM_ADDRESS_SETTINGS1 00
#define EEPROM_ADDRESS_SETTINGS2 25
#define EEPROM_ADDRESS_SETTINGS3 50
#define EEPROM_ADDRESS_SETTINGS4 75
#define EEPROM_ADDRESS_SETTINGS5 100
#define EEPROM_ADDRESS_ODO 460
#define EEPROM_ADDRESS_BATTERY_CALIB 150
#define EEPROM_ADDRESS_BLE_LOCK_FORCED 200
#define EEPROM_ADDRESS_BRAKE_MAX_PRESSURE 225
#define EEPROM_ADDRESS_BRAKE_MIN_PRESSURE 250
#define EEPROM_ADDRESS_TEST 300

class EEPROMM_storage
{
public:
    EEPROMM_storage();

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
};

#endif