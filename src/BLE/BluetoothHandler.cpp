#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "BluetoothHandler.h"
#include "SharedData.h"
#include "main.h"
#include "debug.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "OTA/OTA_wifi.h"
#include "tools/buffer.h"

// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define SERVICE_MAIN_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define SERVICE_FIRMWARE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914c"
#define SERVICE_SETTINGS_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914d"

#define MEASUREMENTS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a0"
//#define xxx "beb5483e-36e1-4688-b7f5-ea07361b26a1"
//#define xxx "beb5483e-36e1-4688-b7f5-ea07361b26a2"
#define FIRMWARE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a3"
#define KEEP_ALIVE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a4"
#define COMMANDS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a5"
#define BTLOCK_STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a6"
#define SETTINGS4_WIFI_SSID_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a7"
#define SETTINGS5_WIFI_PWD_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SETTINGS1_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a9"
#define SETTINGS6_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26aa"
//#define xxx "beb5483e-36e1-4688-b7f5-ea07361b26ab"
//#define xxx "beb5483e-36e1-4688-b7f5-ea07361b26ac"
#define CALIB_ORDER_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26ad"
#define SWITCH_TO_OTA_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26ae"
#define LOGS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26af"
#define FAST_UPDATE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b0"
#define SETTINGS2_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b1"
#define SETTINGS3_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b2"
//#define xxxx "beb5483e-36e1-4688-b7f5-ea07361b26b3"
//#define xxxx "beb5483e-36e1-4688-b7f5-ea07361b26b4"
#define DISTANCE_RST_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b5"

#define BLE_MTU 23

#define MAX_BEACON_INVISIBLE_COUNT 1

#define BEACON_SCAN_PERIOD_IN_SECONDS 10

BLEScan *BluetoothHandler::pBLEScan;
BLEServer *BluetoothHandler::pServer;
BLESecurity *BluetoothHandler::pSecurity;

// Main services
BLECharacteristic *BluetoothHandler::pCharacteristicMeasurements;
BLECharacteristic *BluetoothHandler::pCharacteristicBtlockStatus;
BLECharacteristic *BluetoothHandler::pCharacteristicCalibOrder;
BLECharacteristic *BluetoothHandler::pCharacteristicOtaSwitch;
BLECharacteristic *BluetoothHandler::pCharacteristicLogs;
BLECharacteristic *BluetoothHandler::pCharacteristicDistanceRst;
BLECharacteristic *BluetoothHandler::pCharacteristicKeepAlive;
BLECharacteristic *BluetoothHandler::pCharacteristicCommands;

// Settings services
BLECharacteristic *BluetoothHandler::pCharacteristicSettings1;
BLECharacteristic *BluetoothHandler::pCharacteristicSettings2;
BLECharacteristic *BluetoothHandler::pCharacteristicSettings3;
BLECharacteristic *BluetoothHandler::pCharacteristicSettings4;
BLECharacteristic *BluetoothHandler::pCharacteristicSettings5;
BLECharacteristic *BluetoothHandler::pCharacteristicSettings6;

// firmware services
BLECharacteristic *BluetoothHandler::pCharacteristicFirmware;

int8_t BluetoothHandler::bleLockStatus;
int8_t BluetoothHandler::bleBeaconVisible;
int8_t BluetoothHandler::bleBeaconRSSI;
int8_t BluetoothHandler::bleLockForced;
int8_t BluetoothHandler::fastUpdate;

BleStatus BluetoothHandler::deviceStatus;
BleStatus BluetoothHandler::oldDeviceStatus;

Settings *BluetoothHandler::settings;
SharedData *BluetoothHandler::shrd;

uint32_t bleBeaconInvisibleCount = 0;
uint32_t errCounter = 0;

BluetoothHandler::BluetoothHandler()
{
}

void BluetoothHandler::setSettings(Settings *data)
{

    Serial.println("BLH - init");

    class BLEServerCallback : public BLEServerCallbacks
    {
        void onConnect(BLEServer *pServer)
        {
            Serial.println("BLE connecting");

            deviceStatus = BLE_STATUS_CONNECTED_AND_AUTHENTIFYING;

            if (bleLockForced == 0)
            {
                if (settings->getS1F().Bluetooth_lock_mode == 1)
                {
                    bleLockStatus = false;
                    Serial.println(" ==> device connected ==> UNLOCK decision");
                    Serial.println("-------------------------------------");
                }
                if (settings->getS1F().Bluetooth_lock_mode == 2)
                {
                    bleLockStatus = false;
                    Serial.println(" ==> device connected ==> UNLOCK decision");
                    Serial.println("-------------------------------------");
                }
            }

            // notify commands feedback
            // notify of current modes / values (for value not uptate by LCD)
            notifyCommandsFeedback();
        };

        void onDisconnect(BLEServer *pServer)
        {
            Serial.println("BLE disconnected");
            deviceStatus = BLE_STATUS_DISCONNECTED;

            if (bleLockForced == 0)
            {
                if (settings->getS1F().Bluetooth_lock_mode == 1)
                {
                    bleLockStatus = true;
                    Serial.println(" ==> device disconnected ==> LOCK decision");
                    Serial.println("-------------------------------------");
                }
                if (settings->getS1F().Bluetooth_lock_mode == 2)
                {
                    if (!bleBeaconVisible)
                    {
                        bleLockStatus = true;
                        Serial.println(" ==> device disconnected / Beacon not visible ==> LOCK decision");
                        Serial.println("-------------------------------------");
                    }
                }
            }
        }
    };

    class BLEAdvertisedDeviceCallback : public BLEAdvertisedDeviceCallbacks
    {
        void onResult(BLEAdvertisedDevice advertisedDevice)
        {
            //Serial.print("BLH - BLE Advertised Device found: ");
            //Serial.println(advertisedDevice.toString().c_str());
        } // onResult
    };    // MyAdvertisedDeviceCallbacks

    class BLESecurityCallback : public BLESecurityCallbacks
    {

        bool onConfirmPIN(uint32_t pin)
        {
            return false;
        }

        uint32_t onPassKeyRequest()
        {
            Serial.print("BLH - onPassKeyRequest : ");
            uint32_t pinCode;
            pinCode = settings->getS3F().Bluetooth_pin_code;
            Serial.println(pinCode);
            return pinCode;
        }

        void onPassKeyNotify(uint32_t pass_key)
        {
            Serial.print("BLH - onPassKeyNotify : ");
            Serial.println(pass_key);
        }

        bool onSecurityRequest()
        {
            Serial.println("onSecurityRequest");
            return true;
        }

        void onAuthenticationComplete(esp_ble_auth_cmpl_t cmpl)
        {
            if (cmpl.success)
            {
                uint16_t length;
                esp_ble_gap_get_whitelist_size(&length);
                Serial.println("onAuthenticationComplete : success");
                deviceStatus = BLE_STATUS_CONNECTED_AND_AUTHENTIFIED;
            }
            else
            {
                Serial.print("BLH - onAuthenticationComplete : hummm ... failed / reason : ");
                Serial.println(cmpl.fail_reason);

                deviceStatus = BLE_STATUS_DISCONNECTED;
            }
        }
    };

    class BLECharacteristicCallback : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *pCharacteristic)
        {

            //const char *uuid = pCharacteristic->getUUID().toString().data();
            //Serial.println("onWrite : " + (String)(uuid));

            /* if (pCharacteristic->getUUID().toString() == BRAKE_STATUS_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                shrd->brakeSentOrder = rxValue[0];
                shrd->brakeSentOrderFromBLE = rxValue[0];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->brakeSentOrder);
                Serial.print("BLH - Write brakeSentOrder : ");
                Serial.println(print_buffer);
            }
            else 
            */
            if (pCharacteristic->getUUID().toString() == SETTINGS1_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();

                for (int i = 0; i < rxValue.length(); i++)
                {
                    settings->getS1B()[i] = rxValue[i];
                }

                //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

                Serial.print("BLH - Settings1 len : ");
                Serial.println(rxValue.length());
                Serial.print("BLH - Settings1 size : ");
                Serial.println(rxValue.size());

                Serial.print("BLH - Settings1 : ");
                for (int i = 0; i < rxValue.length(); i++)
                {
                    char print_buffer[5];
                    sprintf(print_buffer, "%02x ", rxValue[i]);
                    Serial.print(print_buffer);
                }
                Serial.println("");

                settings->displaySettings1();
            }
            else if (pCharacteristic->getUUID().toString() == SETTINGS2_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();

                for (int i = 0; i < rxValue.length(); i++)
                {
                    settings->getS2B()[i] = rxValue[i];
                }

                //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

                Serial.print("BLH - Settings2 len : ");
                Serial.println(rxValue.length());
                Serial.print("BLH - Settings2 size : ");
                Serial.println(rxValue.size());

                Serial.print("BLH - Settings2 : ");
                for (int i = 0; i < rxValue.length(); i++)
                {
                    char print_buffer[5];
                    sprintf(print_buffer, "%02x ", rxValue[i]);
                    Serial.print(print_buffer);
                }
                Serial.println("");

                settings->displaySettings2();
            }
            else if (pCharacteristic->getUUID().toString() == SETTINGS3_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();

                for (int i = 0; i < rxValue.length(); i++)
                {
                    settings->getS3B()[i] = rxValue[i];
                }

                //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

                Serial.print("BLH - Settings3 len : ");
                Serial.println(rxValue.length());
                Serial.print("BLH - Settings3 size : ");
                Serial.println(rxValue.size());

                Serial.print("BLH - Settings3 : ");
                for (int i = 0; i < rxValue.length(); i++)
                {
                    char print_buffer[5];
                    sprintf(print_buffer, "%02x ", rxValue[i]);
                    Serial.print(print_buffer);
                }
                Serial.println("");

                settings->displaySettings3();

                // reset battery min/max datas
                setupBattery();

                // update BLE PIN code
                pSecurity->setStaticPIN(settings->getS3F().Bluetooth_pin_code);
            }
            else if (pCharacteristic->getUUID().toString() == SETTINGS4_WIFI_SSID_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();

                memset(settings->getS4B(), 0, 20);
                for (int i = 0; i < rxValue.length(); i++)
                {
                    settings->getS4B()[i] = rxValue[i];
                }

                //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

                Serial.print("BLH - Settings4 len : ");
                Serial.println(rxValue.length());
                Serial.print("BLH - Settings4 size : ");
                Serial.println(rxValue.size());

                Serial.print("BLH - Settings4 : ");
                for (int i = 0; i < rxValue.length(); i++)
                {
                    char print_buffer[5];
                    sprintf(print_buffer, "%02x ", rxValue[i]);
                    Serial.print(print_buffer);
                }
                Serial.println("");

                settings->displaySettings4();
            }

            else if (pCharacteristic->getUUID().toString() == SETTINGS5_WIFI_PWD_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();

                memset(settings->getS5B(), 0, 20);

                for (int i = 0; i < rxValue.length(); i++)
                {
                    settings->getS5B()[i] = rxValue[i];
                }

                //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

                Serial.print("BLH - Settings5 len : ");
                Serial.println(rxValue.length());
                Serial.print("BLH - Settings5 size : ");
                Serial.println(rxValue.size());

                Serial.print("BLH - Settings5 : ");
                for (int i = 0; i < rxValue.length(); i++)
                {
                    char print_buffer[5];
                    sprintf(print_buffer, "%02x ", rxValue[i]);
                    Serial.print(print_buffer);
                }
                Serial.println("");

                settings->displaySettings5();

                // update BLE PIN code
                pSecurity->setStaticPIN(settings->getS3F().Bluetooth_pin_code);

            }
            else if (pCharacteristic->getUUID().toString() == SETTINGS6_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();

                for (int i = 0; i < rxValue.length(); i++)
                {
                    settings->getS6B()[i] = rxValue[i];
                }

                //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

                Serial.print("BLH - Settings6 len : ");
                Serial.println(rxValue.length());
                Serial.print("BLH - Settings6 size : ");
                Serial.println(rxValue.size());

                Serial.print("BLH - Settings6 : ");
                for (int i = 0; i < rxValue.length(); i++)
                {
                    char print_buffer[5];
                    sprintf(print_buffer, "%02x ", rxValue[i]);
                    Serial.print(print_buffer);
                }
                Serial.println("");

                settings->displaySettings6();
                
                saveSettings();
            }
            else if (pCharacteristic->getUUID().toString() == CALIB_ORDER_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();

                int valueInt;
                memcpy(&valueInt, &rxValue[1], 4);

                //BrakeMaxPressure(0),
                if (rxValue[0] == 0)
                {
                    shrd->brakeMaxPressureRaw = shrd->brakeAnalogValue;
                    saveBrakeMaxPressure();

                    Serial.print("BLH - brake max current raw value :");
                    Serial.println(shrd->brakeAnalogValue);
                }
                //BatMaxVoltage(1),
                else if (rxValue[0] == 1)
                {
                    shrd->batteryMaxVoltageCalibUser = valueInt / 10.0;
                    shrd->batteryMaxVoltageCalibRaw = shrd->voltageRawFilterMean;
                    saveBatteryCalib();

                    Serial.print("BLH - batteryMaxVoltageCalibUser : ");
                    Serial.print(shrd->batteryMaxVoltageCalibUser);
                    Serial.print(" / batteryMaxVoltageCalibRaw : ");
                    Serial.println(shrd->batteryMaxVoltageCalibRaw);
                }
                //BatMinVoltage(2),
                else if (rxValue[0] == 2)
                {
                    shrd->batteryMinVoltageCalibUser = valueInt / 10.0;
                    shrd->batteryMinVoltageCalibRaw = shrd->voltageRawFilterMean;
                    saveBatteryCalib();

                    Serial.print("BLH - batteryMinVoltageCalibUser : ");
                    Serial.print(shrd->batteryMinVoltageCalibUser);
                    Serial.print(" / batteryMinVoltageCalibRaw : ");
                    Serial.println(shrd->batteryMinVoltageCalibRaw);
                }
                //CurrentZero(3);
                else if (rxValue[0] == 3)
                {
                    //shrd->currentCalibOrder = valueInt;
                }
                //BrakeMinPressure(4),
                else if (rxValue[0] == 4)
                {
                    shrd->brakeMinPressureRaw = shrd->brakeAnalogValue;
                    saveBrakeMinPressure();

                    Serial.print("BLH - brake min current raw value :");
                    Serial.println(shrd->brakeAnalogValue);
                }

                char print_buffer[500];
                sprintf(print_buffer, "type %d / %02x / %02x / %02x / %02x / %02x / value %d", rxValue[0], rxValue[1], rxValue[2], rxValue[3], rxValue[4], rxValue[5], valueInt);
                Serial.print("BLH - Write calib order : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == BTLOCK_STATUS_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                bleLockForced = rxValue[3];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", bleLockForced);
                Serial.print("BLH - Write bleLockForced : ");
                Serial.println(print_buffer);

                bleLockStatus = bleLockForced;

                notifyBleLock();
                saveBleLockForced();
            }
            else if (pCharacteristic->getUUID().toString() == SWITCH_TO_OTA_CHARACTERISTIC_UUID)
            {
                Serial.println("Write SWITCH_TO_OTA_CHARACTERISTIC_UUID");

                std::string rxValue = pCharacteristic->getValue();
                shrd->inOtaMode = rxValue[0]; // Enable http OTA mode
            }
            else
                /* if (pCharacteristic->getUUID().toString() == FAST_UPDATE_CHARACTERISTIC_UUID)
            {
                Serial.println("Write FAST_UPDATE_CHARACTERISTIC_UUID");

                std::string rxValue = pCharacteristic->getValue();
                fastUpdate = rxValue[0];

                Serial.print("BLH - Fast update = ");
                Serial.println(fastUpdate);
            }
            else
            */
                if (pCharacteristic->getUUID().toString() == DISTANCE_RST_CHARACTERISTIC_UUID)
            {
                shrd->distanceTrip = 0;
            }
            else if (pCharacteristic->getUUID().toString() == KEEP_ALIVE_CHARACTERISTIC_UUID)
            {
                //Serial.println("BLH - Write : KEEP_ALIVE_CHARACTERISTIC_UUID");
                checkAndSaveOdo();
            }
            else if (pCharacteristic->getUUID().toString() == COMMANDS_CHARACTERISTIC_UUID)
            {
                Serial.println("BLH - Write : COMMANDS_CHARACTERISTIC_UUID");
                getCommandsDataPacket((uint8_t *)pCharacteristic->getValue().data());

                notifyCommandsFeedback();
            }
            else
            {
                const char *uuid = pCharacteristic->getUUID().toString().data();
                Serial.println("BLH - Write : unknown " + (String)(uuid));
            }
        }

        void onRead(BLECharacteristic *pCharacteristic)
        {

            const char *uuid = pCharacteristic->getUUID().toString().data();
            Serial.println("onRead : " + (String)(uuid));

            if (pCharacteristic->getUUID().toString() == FIRMWARE_CHARACTERISTIC_UUID)
            {
                String finalString;
                String firmwareVersion = (String)FIRMWARE_VERSION;
                String firmwareType = (String)FIRMWARE_TYPE;
                firmwareType.replace("smartcontroller_smartdisplay_", "");
                finalString = firmwareType + " " + firmwareVersion;
                pCharacteristicFirmware->setValue(finalString.c_str());
                Serial.print("BLH - Read firmware : ");
                Serial.println(finalString.c_str());
            }
            else if (pCharacteristic->getUUID().toString() == MEASUREMENTS_CHARACTERISTIC_UUID)
            {
                int nb_bytes = setMeasurementsDataPacket();
                Serial.print("BLH - Read measurement : nb bytes");
                Serial.println(nb_bytes);
            }
            else if (pCharacteristic->getUUID().toString() == COMMANDS_CHARACTERISTIC_UUID)
            {
                int nb_bytes = setCommandsDataPacket();
                Serial.print("BLH - Read commands : nb bytes");
                Serial.println(nb_bytes);
            }
            else
                /*
             if (pCharacteristic->getUUID().toString() == BRAKE_STATUS_CHARACTERISTIC_UUID)
            {

                byte value[3];
                value[0] = shrd->brakeSentOrder;
                value[1] = shrd->brakePressedStatus;
                value[2] = shrd->brakeFordidenHighVoltage;
                pCharacteristicBrakeSentOrder->setValue((uint8_t *)&value, 3);

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->brakeSentOrder);
                Serial.print("BLH - Read breakeSentOrder : ");
                Serial.println(print_buffer);
            }
            else 
            */
                if (pCharacteristic->getUUID().toString() == BTLOCK_STATUS_CHARACTERISTIC_UUID)
            {

                notifyBleLock();

                // byte value[4];
                // value[0] = bleLockStatus;
                // value[1] = bleBeaconVisible;
                // value[2] = bleBeaconRSSI;
                // value[3] = bleLockForced;
                // pCharacteristicBtlockStatus->setValue((uint8_t *)&value, 4);

                char print_buffer[500];
                sprintf(print_buffer, "%02x", bleLockStatus);
                Serial.print("BLH - Read bleLock : ");
                Serial.println(print_buffer);
            }
        }

        void onStatus(BLECharacteristic *pCharacteristic, Status s, uint32_t code)
        {
            //Serial.printf("BLH - onStatus: status = %d / code = %d / errCounter = %d\n", s, code, errCounter);

            if (code == 0xffffffff)
            {
                Serial.printf("BLH - >>>> onStatus: status = %d / code = %d / errCounter = %d\n", s, code, errCounter);
                errCounter++;

                // reset connexion if too many errors
                if (errCounter > 1000)
                {
                    pServer->disconnect(pServer->getConnId());
                    errCounter = 0;
                }
            }
        }
    };

    // Init settings
    settings = data;

    // Create the BLE Device
    BLEDevice::init("SmartCntrl");
    String smartCntrlNameComp1 = BLEDevice::getAddress().toString().substr(12, 2).c_str();
    String smartCntrlNameComp2 = BLEDevice::getAddress().toString().substr(15, 2).c_str();
    String smartCntrlFullName = "SmartCntrl-" + smartCntrlNameComp1 + smartCntrlNameComp2;
    Serial.print("BLH - adress = ");
    Serial.println(BLEDevice::getAddress().toString().c_str());
    Serial.print("BLH - name = ");
    Serial.println(smartCntrlFullName);
    esp_ble_gap_set_device_name(smartCntrlFullName.c_str());
    BLEDevice::setMTU(BLE_MTU);

    /////
    BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
    BLEDevice::setSecurityCallbacks(new BLESecurityCallback());
    /////

    int mtu = BLEDevice::getMTU();
    Serial.print("BLH - MTU : ");
    Serial.println(mtu);

    // Create the BLE Server
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new BLEServerCallback());

    // Create the BLE Service
    BLEService *pServiceMain = pServer->createService(BLEUUID(SERVICE_MAIN_UUID), 40);
    BLEService *pServiceFirmware = pServer->createService(BLEUUID(SERVICE_FIRMWARE_UUID), 20);
    BLEService *pServiceSettings = pServer->createService(BLEUUID(SERVICE_SETTINGS_UUID), 20);

    // Create a BLE Characteristic

    //-------------------
    // services main

    pCharacteristicMeasurements = pServiceMain->createCharacteristic(
        MEASUREMENTS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicBtlockStatus = pServiceMain->createCharacteristic(
        BTLOCK_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicCalibOrder = pServiceMain->createCharacteristic(
        CALIB_ORDER_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristicOtaSwitch = pServiceMain->createCharacteristic(
        SWITCH_TO_OTA_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristicLogs = pServiceMain->createCharacteristic(
        LOGS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristicDistanceRst = pServiceMain->createCharacteristic(
        DISTANCE_RST_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristicKeepAlive = pServiceMain->createCharacteristic(
        KEEP_ALIVE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristicCommands = pServiceMain->createCharacteristic(
        COMMANDS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    //-------------------
    // services firmware

    pCharacteristicFirmware = pServiceFirmware->createCharacteristic(
        FIRMWARE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ);

    //-------------------
    // services settings

    pCharacteristicSettings1 = pServiceSettings->createCharacteristic(
        SETTINGS1_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSettings2 = pServiceSettings->createCharacteristic(
        SETTINGS2_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSettings3 = pServiceSettings->createCharacteristic(
        SETTINGS3_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSettings4 = pServiceSettings->createCharacteristic(
        SETTINGS4_WIFI_SSID_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSettings5 = pServiceSettings->createCharacteristic(
        SETTINGS5_WIFI_PWD_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSettings6 = pServiceSettings->createCharacteristic(
        SETTINGS6_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    //////////////
    pCharacteristicMeasurements->addDescriptor(new BLE2902());
    pCharacteristicBtlockStatus->addDescriptor(new BLE2902());
    pCharacteristicCalibOrder->addDescriptor(new BLE2902());
    pCharacteristicOtaSwitch->addDescriptor(new BLE2902());
    pCharacteristicLogs->addDescriptor(new BLE2902());
    pCharacteristicDistanceRst->addDescriptor(new BLE2902());
    pCharacteristicKeepAlive->addDescriptor(new BLE2902());
    pCharacteristicCommands->addDescriptor(new BLE2902());

    pCharacteristicSettings1->addDescriptor(new BLE2902());
    pCharacteristicSettings2->addDescriptor(new BLE2902());
    pCharacteristicSettings3->addDescriptor(new BLE2902());
    pCharacteristicSettings4->addDescriptor(new BLE2902());
    pCharacteristicSettings5->addDescriptor(new BLE2902());
    pCharacteristicSettings6->addDescriptor(new BLE2902());

    pCharacteristicFirmware->addDescriptor(new BLE2902());

    //////////////
    pCharacteristicMeasurements->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicBtlockStatus->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicCalibOrder->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicOtaSwitch->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicLogs->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicDistanceRst->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicKeepAlive->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicCommands->setCallbacks(new BLECharacteristicCallback());

    pCharacteristicSettings1->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSettings2->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSettings3->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSettings4->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSettings5->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSettings6->setCallbacks(new BLECharacteristicCallback());

    pCharacteristicFirmware->setCallbacks(new BLECharacteristicCallback());

    // Start the service
    pServiceMain->start();
    pServiceFirmware->start();
    pServiceSettings->start();

    deviceStatus = BLE_STATUS_DISCONNECTED;

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_MAIN_UUID);
    pAdvertising->addServiceUUID(SERVICE_FIRMWARE_UUID);
    pAdvertising->addServiceUUID(SERVICE_SETTINGS_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");

    // Security
    pSecurity = new BLESecurity();
    Serial.print("BLH - pin code : ");
    uint32_t pinCode = settings->getS3F().Bluetooth_pin_code;
    Serial.println(pinCode);
    pSecurity->setStaticPIN(pinCode);
    pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
    pSecurity->setAuthenticationMode(ESP_LE_AUTH_BOND);

    // Start scanning
    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new BLEAdvertisedDeviceCallback());
    pBLEScan->setActiveScan(true);
    pBLEScan->start(BEACON_SCAN_PERIOD_IN_SECONDS, &bleOnScanResults, false);
}

void BluetoothHandler::bleOnScanResults(BLEScanResults scanResults)
{

#if DEBUG_DISPLAY_BLE_SCAN
    Serial.print("BLH - BLE Scan Device found: ");
    Serial.println(scanResults.getCount());
#endif

    bool newBleBeaconVisible = false;
    int8_t RSSI = -1;
    bool beaconFound = false;

    for (int i = 0; i < scanResults.getCount(); i++)
    {
        String name = scanResults.getDevice(i).getName().c_str();
        RSSI = scanResults.getDevice(i).getRSSI();
        std::string address = scanResults.getDevice(i).getAddress().toString();
        String addressStr = address.c_str();

        String addressBeaconSettings = settings->getS2F().Beacon_Mac_Address;
        addressBeaconSettings = addressBeaconSettings.substring(0, 17);

#if DEBUG_DISPLAY_BLE_SCAN
        Serial.print("BLH - BLE device : address : ");
        Serial.print(addressStr);
        Serial.print(" / name : ");
        Serial.print(name);
        Serial.print(" / RSSI : ");
        Serial.print(RSSI);
        Serial.print(" / search for : ");
        Serial.println(addressBeaconSettings);
#endif

        if (addressBeaconSettings.equals(addressStr))
        {
            beaconFound = true;
            bleBeaconRSSI = RSSI;

            if (bleBeaconRSSI < settings->getS1F().Beacon_range)
            {

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.print("BLH -  ==> Beacon found ... but too far away / RSSI = ");
                Serial.print(bleBeaconRSSI);
                Serial.print(" / min RSSI required = ");
                Serial.print(settings->getS1F().Beacon_range);
                Serial.println(" ==> lock from scan");
                Serial.println();
#endif
            }
            else
            {

                bleBeaconInvisibleCount = 0;
                newBleBeaconVisible = true;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.print("BLH -  ==> Beacon found  / RSSI = ");
                Serial.print(bleBeaconRSSI);
                Serial.print(" / min RSSI required = ");
                Serial.print(settings->getS1F().Beacon_range);
                Serial.println(" ==> unlock from scan");
                Serial.println();
#endif
            }
        }
    }

    // reset RSSI
    if (!beaconFound)
        bleBeaconRSSI = -1;

    // count beacon invible times
    if (!newBleBeaconVisible)
    {
        bleBeaconInvisibleCount++;
        if (bleBeaconInvisibleCount >= MAX_BEACON_INVISIBLE_COUNT)
        {
            newBleBeaconVisible = false;
#if DEBUG_DISPLAY_BLE_SCAN
            Serial.printf("BLH -  ==> Beacon not found  / bleBeaconInvisibleCount = %d / newBleBeaconVisible = %d\n", bleBeaconInvisibleCount, newBleBeaconVisible);
#endif
        }
        else
        {
            newBleBeaconVisible = true;
#if DEBUG_DISPLAY_BLE_SCAN
            Serial.printf("BLH -  ==> Beacon not found  / bleBeaconInvisibleCount = %d / newBleBeaconVisible = %d ... until lock\n", bleBeaconInvisibleCount, newBleBeaconVisible);
#endif
        }
    }

    // store beacon status
    bleBeaconVisible = newBleBeaconVisible;

#if DEBUG_DISPLAY_BLE_SCAN
    Serial.printf("BLH - bleLockForced = %d / settings->getS1F().Bluetooth_lock_mode = %d / bleBeaconVisible = %d / deviceConnected = %d \n", bleLockForced, settings->getS1F().Bluetooth_lock_mode, bleBeaconVisible, deviceConnected);
#endif

    if (bleLockForced <= 0)
    {
        if (settings->getS1F().Bluetooth_lock_mode == 2)
        {
            if ((!bleBeaconVisible) && (deviceStatus != BLE_STATUS_DISCONNECTED))
            {
                bleLockStatus = 1;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.println(" ==> Beacon not visible // smartphone not connected ==> LOCK decision");
                Serial.println("-------------------------------------");
                Serial.println();
#endif
            }
            else if ((!bleBeaconVisible) && (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED))
            {
                bleLockStatus = 0;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.println(" ==> Beacon visible // smartphone connected ==> UNLOCK decision");
                Serial.println("-------------------------------------");
                Serial.println();
#endif
            }
            else
            {
                Serial.println(" ==> Beacon visible // smartphone connected ==> .... unchecked condition");
                Serial.println("-------------------------------------");
                Serial.println();
            }
        }
        if (settings->getS1F().Bluetooth_lock_mode == 3)
        {
            if (!bleBeaconVisible)
            {
                bleLockStatus = 1;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.println(" ==> Beacon not visible ==> LOCK decision");
                Serial.println("-------------------------------------");
#endif
            }
            else if (bleBeaconVisible)
            {
                bleLockStatus = 0;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.println(" ==> Beacon visible ==> UNLOCK decision");
                Serial.println("-------------------------------------");
#endif
            }
        }
    }

    notifyBleLock();

    // launch new scan
    pBLEScan->start(BEACON_SCAN_PERIOD_IN_SECONDS, &bleOnScanResults, false);

    // set BT lock
    // if ((!deviceConnected))
    // {
    //   if (!bleBeaconVisible)
    //   {
    //     bleLockStatus = true;
    //     Serial.println(" ==> no device connected and Beacon no found ==> LOCK decision");
    //   }
    //   else
    //   {
    //     bleLockStatus = false;
    //     Serial.println(" ==> no device connected and Beacon found ==> UNLOCK decision");
    //   }
    // }
}

uint8_t BluetoothHandler::setMeasurementsDataPacket()
{
    //Serial.println("setMeasurementsDataPacket");

    int32_t ind = 0;

    if (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED)
    {

        uint8_t txValue[20];

        buffer_append_uint8(txValue, shrd->speedCurrent, &ind);
        buffer_append_uint16_inv(txValue, (ceil(shrd->voltageFilterMean / 100.0)), &ind);
        buffer_append_int16_inv(txValue, (shrd->currentActual / 100), &ind);
        buffer_append_int16_inv(txValue, (shrd->currentActual / 100) * (shrd->voltageActual / 100), &ind);
        buffer_append_int16_inv(txValue, (shrd->currentTemperature * 10.0), &ind);
        buffer_append_uint16_inv(txValue, (shrd->currentHumidity * 10.0), &ind);
        buffer_append_uint16_inv(txValue, (shrd->distanceTrip / 100), &ind);
        buffer_append_uint32_inv(txValue, (shrd->distanceOdo), &ind);
        buffer_append_uint8(txValue, (shrd->batteryLevel), &ind);
        buffer_append_uint8(txValue, (shrd->autonomyLeft), &ind);

        pCharacteristicMeasurements->setValue((uint8_t *)&txValue[0], ind);

        //Serial.println("setMeasurementsDataPacket : sent " + (String)ind);
    }
    return ind;
}

uint8_t BluetoothHandler::setCommandsDataPacket()
{

    Serial.println("setCommandsDataPacket");

    int32_t ind = 0;

    if (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED)
    {
        uint8_t txValue[20];

        buffer_append_uint8(txValue, shrd->modeOrder, &ind);
        buffer_append_uint8(txValue, shrd->speedLimiter, &ind);
        buffer_append_uint8(txValue, shrd->ecoOrder, &ind);
        buffer_append_uint8(txValue, shrd->accelOrder, &ind);
        buffer_append_uint8(txValue, shrd->auxOrder, &ind);
        buffer_append_uint8(txValue, shrd->brakeSentOrder, &ind);
        buffer_append_uint8(txValue, shrd->brakeSentOrderFromBLE, &ind);
        buffer_append_uint8(txValue, shrd->brakePressedStatus, &ind);
        buffer_append_uint8(txValue, shrd->brakeFordidenHighVoltage, &ind);
        buffer_append_uint8(txValue, fastUpdate, &ind);

        // copy values
        shrd->brakeSentOrderFromBLE = shrd->brakeSentOrder;

        pCharacteristicCommands->setValue((uint8_t *)&txValue[0], ind);

        buffer_display("setCommandsDataPacket : ", txValue, ind);
    }
    return ind;
}

void BluetoothHandler::getCommandsDataPacket(uint8_t *rxValue)
{
    Serial.println("getCommandsDataPacket");

    int32_t ind = 0;

    shrd->modeOrder = buffer_get_uint8(rxValue, &ind);
    shrd->speedLimiter = buffer_get_uint8(rxValue, &ind);
    shrd->ecoOrder = buffer_get_uint8(rxValue, &ind);
    shrd->accelOrder = buffer_get_uint8(rxValue, &ind);
    shrd->auxOrder = buffer_get_uint8(rxValue, &ind);
    shrd->brakeSentOrder = buffer_get_uint8(rxValue, &ind);
    shrd->brakeSentOrderFromBLE = buffer_get_uint8(rxValue, &ind);
    buffer_get_uint8(rxValue, &ind); /*shrd->brakePressedStatus*/
    buffer_get_uint8(rxValue, &ind); /*shrd->brakeFordidenHighVoltage*/
    fastUpdate = buffer_get_uint8(rxValue, &ind);

    buffer_display("getCommandsDataPacket : ", rxValue, ind);
}

void BluetoothHandler::notifyCommandsFeedback()
{
    Serial.println("notifyCommandsFeedback");

    if (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED)
    {
        // notify of new log
        setCommandsDataPacket();
        pCharacteristicCommands->notify();
    }
}

void BluetoothHandler::notifyMeasurements()
{
    // Serial.println("notifyMeasurements");

    if (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED)
    {
        // notify of new log
        setMeasurementsDataPacket();
        pCharacteristicMeasurements->notify();
    }
}

void BluetoothHandler::notifyBleLogs(char *txt)
{

    if (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED)
    {
        char bufferSend[21];

        /*
        memcpy(fullStringBufferWithEnd, txt, strlen(txt));
        fullStringBufferWithEnd[strlen(txt)] = '\0';
*/

        bufferSend[20] = '\0';
        int size = 0;

        // notify of new log
        int nbChunks = ceil((strlen(txt) + 1) / 20.0);
        int lastChunkSize = (strlen(txt) + 1) % 20;

        //Serial.printf("BLH - notifyBleLogs : nbChunks = %d / lastChunkSize = %d\n", nbChunks, lastChunkSize);

        for (int i = 0; i < nbChunks; i++)
        {
            //memset(txt, 0, 20);
            if ((i == nbChunks - 1) && (lastChunkSize > 0))
            {
                size = lastChunkSize;
                // bufferSend[lastChunkSize] = '\0';
            }
            else
            {
                size = 20;
            }

            //Serial.printf("BLH - notifyBleLogs : size = %d / i = %d\n", size, i);

            memcpy(&bufferSend, &txt[i * 20], size);

            //Serial.printf("BLH - notifyBleLogs : chunk = %s\n", bufferSend);

            pCharacteristicLogs->setValue((uint8_t *)bufferSend, size);
            pCharacteristicLogs->notify();
        }
    }
}

/*
void BluetoothHandler::notifyBreakeSentOrder(uint8_t order, uint8_t isPressed, uint8_t brakeFordidenHighVoltage)
{

    if (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED)
    {
        byte value[3];
        value[0] = order;
        value[1] = isPressed;
        value[2] = brakeFordidenHighVoltage;
        pCharacteristicBrakeSentOrder->setValue((uint8_t *)&value, 3);
        pCharacteristicBrakeSentOrder->notify();
    }
}
*/

void BluetoothHandler::notifyBleLock()
{

    //Serial.println("notifyBleLock");

    if (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED)
    {
        byte value[4];
        value[0] = bleLockStatus;
        value[1] = bleBeaconVisible;
        value[2] = bleBeaconRSSI;
        value[3] = bleLockForced;
        pCharacteristicBtlockStatus->setValue((uint8_t *)&value, 4);
        pCharacteristicBtlockStatus->notify();

#if DEBUG_DISPLAY_BLE_NOTIFY
        Serial.print("BLH - notifyBleLock : bleLockStatus = ");
        Serial.print(bleLockStatus);
        Serial.print(" / bleBeaconVisible = ");
        Serial.print(bleBeaconVisible);
        Serial.print(" / bleBeaconRSSI = ");
        Serial.print(bleBeaconRSSI);

        Serial.print(" / bleLockForced = ");
        Serial.print(bleLockForced);
        Serial.println("");
#endif
    }
}

void BluetoothHandler::setBleLock(bool force)
{
    // force locking
    if (force)
    {
        bleLockForced = 1;
        saveBleLockForced();
    }

    // update lock status
    if (bleLockForced == 1)
        bleLockStatus = 1;
}

void BluetoothHandler::processBLE()
{

#ifndef DEBUG_NEW

    // notify changed value
    if (deviceStatus == BLE_STATUS_CONNECTED_AND_AUTHENTIFIED)
    {
        uint16_t period = 250;
        if (fastUpdate)
            period = 100;

        /*
        Serial.print(millis());
        Serial.print("BLH -  / ");
        Serial.print(shrd->timeLastNotifyBle);
        Serial.print("BLH -  / ");
        Serial.print(period);
        Serial.println(" / ");
        */

        if (millis() > shrd->timeLastNotifyBle + period)
        {

            notifyMeasurements();

            notifyBleLock();

            shrd->timeLastNotifyBle = millis();
        }
    }
    // disconnecting
    if ((deviceStatus != oldDeviceStatus) && (deviceStatus == BLE_STATUS_DISCONNECTED))
    {
        delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
    }
    oldDeviceStatus = deviceStatus;

#endif
}

void BluetoothHandler::setSharedData(SharedData *data)
{
    shrd = data;
}

void BluetoothHandler::deinit()
{
    if (deviceStatus != BLE_STATUS_DISABLED)
    {
        // stop advertising
        BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
        pAdvertising->stop();

        Serial.println("BLH - stop advertising ... done");
        // stop scanning
        pBLEScan->stop();
        Serial.println("BLH - stop scanning ... done");

        // stop BLE stack
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
        esp_bt_controller_disable();
        esp_bt_controller_deinit();
        Serial.println("BLH - stop scanning ... done");

        deviceStatus = BLE_STATUS_DISABLED;
    }
}
