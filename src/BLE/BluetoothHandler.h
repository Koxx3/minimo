//////------------------------------------
////// Manage bluetooth

#ifndef _BluetoothHandler_h
#define _BluetoothHandler_h

#include "Arduino.h"
#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLEUtils.h>
#include "Settings.h"
#include "SharedData.h"
#include "Settings2.h"

typedef enum
{
    BLE_STATUS_DISABLED,
    BLE_STATUS_DISCONNECTED,
    BLE_STATUS_CONNECTED_AND_AUTHENTIFYING,
    BLE_STATUS_CONNECTED_AND_AUTHENTIFIED,
} BleStatus;

class BluetoothHandler
{
public:
    BluetoothHandler();
    static void init();
    static void deinit();
    static void setSettings(Settings *);
    static void setSharedData(SharedData *);
    static void bleOnScanResults(BLEScanResults);
    static void notifyBleLock();
    static void notifyBleLogs(char *);
    static void setBleLock(bool);
    static void processBLE();
    static void notifyMeasurements();
    static void notifyCommandsFeedback();
    static void notifySettingsChanged();
    static uint8_t setMeasurementsDataPacket();
    static uint8_t setCommandsDataPacket();
    static void getCommandsDataPacket(uint8_t *);
    static void sendSettingValueDataPacket(uint8_t *);
    static void switchToOta(uint8_t *);
    
    static BLEScan *pBLEScan;
    static BLEServer *pServer;
    static BLESecurity *pSecurity;

    static BLECharacteristic *pCharacteristicMeasurements;
    static BLECharacteristic *pCharacteristicBtlockStatus;
    static BLECharacteristic *pCharacteristicCalibOrder;
    static BLECharacteristic *pCharacteristicOtaSwitch;
    static BLECharacteristic *pCharacteristicLogs;
    static BLECharacteristic *pCharacteristicDistanceRst;
    static BLECharacteristic *pCharacteristicCommands;

    static BLECharacteristic *pCharacteristicSettingsGen;
    static BLECharacteristic *pCharacteristicSettingsAction;

    static BLECharacteristic *pCharacteristicFirmware;

//    static int8_t bleLockStatus;
    static int8_t bleBeaconVisible;
    static int8_t bleLockForced;
    static int8_t fastUpdate;

    static BleStatus deviceStatus;
    static BleStatus oldDeviceStatus;

    static SharedData *shrd;
    static Settings *settings;
};

#endif