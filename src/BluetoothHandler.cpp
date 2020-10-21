
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

// See the following for generating UUIDs: https://www.uuidgenerator.net/
#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define SPEED_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a0"
#define MODE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a1"
#define BRAKE_STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a2"
#define VOLTAGE_STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a3"
#define CURRENT_STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a4"
#define POWER_STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a5"
#define BTLOCK_STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a6"
#define TEMPERATURE_STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a7"
#define HUMIDITY_STATUS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define SETTINGS1_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a9"
#define SPEED_LIMITER_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26aa"
#define ECO_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26ab"
#define ACCEL_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26ac"
#define CURRENT_CALIB_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26ad"
#define SWITCH_TO_OTA_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26ae"
#define LOGS_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26af"
#define FAST_UPDATE_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b0"
#define SETTINGS2_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b1"
#define SETTINGS3_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b2"
#define AUX_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b3"
#define SPEED_PID_CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26b4"

#define BLE_MTU 128

#define BLE_PIN_CODE 147258

BLEScan *BluetoothHandler::pBLEScan;
BLEServer *BluetoothHandler::pServer;
BLESecurity *BluetoothHandler::pSecurity;
BLECharacteristic *BluetoothHandler::pCharacteristicSpeed;
BLECharacteristic *BluetoothHandler::pCharacteristicMode;
BLECharacteristic *BluetoothHandler::pCharacteristicBrakeSentOrder;
BLECharacteristic *BluetoothHandler::pCharacteristicVoltageStatus;
BLECharacteristic *BluetoothHandler::pCharacteristicCurrentStatus;
BLECharacteristic *BluetoothHandler::pCharacteristicPowerStatus;
BLECharacteristic *BluetoothHandler::pCharacteristicBtlockStatus;
BLECharacteristic *BluetoothHandler::pCharacteristicTemperatureStatus;
BLECharacteristic *BluetoothHandler::pCharacteristicHumidityStatus;
BLECharacteristic *BluetoothHandler::pCharacteristicSettings1;
BLECharacteristic *BluetoothHandler::pCharacteristicSpeedLimiter;
BLECharacteristic *BluetoothHandler::pCharacteristicEco;
BLECharacteristic *BluetoothHandler::pCharacteristicAccel;
BLECharacteristic *BluetoothHandler::pCharacteristicCurrentCalib;
BLECharacteristic *BluetoothHandler::pCharacteristicOtaSwitch;
BLECharacteristic *BluetoothHandler::pCharacteristicLogs;
BLECharacteristic *BluetoothHandler::pCharacteristicFastUpdate;
BLECharacteristic *BluetoothHandler::pCharacteristicSettings2;
BLECharacteristic *BluetoothHandler::pCharacteristicSettings3;
BLECharacteristic *BluetoothHandler::pCharacteristicAux;
BLECharacteristic *BluetoothHandler::pCharacteristicSpeedPid;

int8_t BluetoothHandler::bleLockStatus;
int8_t BluetoothHandler::blePicclyVisible;
int8_t BluetoothHandler::blePicclyRSSI;
int8_t BluetoothHandler::bleLockForced;
int8_t BluetoothHandler::fastUpdate;

bool BluetoothHandler::deviceConnected;
bool BluetoothHandler::oldDeviceConnected;

Settings *BluetoothHandler::settings;
SharedData *BluetoothHandler::shrd;

BluetoothHandler::BluetoothHandler()
{
}

void BluetoothHandler::init(Settings *data)
{

    Serial.println("BLH - init");

    class BLEServerCallback : public BLEServerCallbacks
    {
        void onConnect(BLEServer *pServer)
        {
            Serial.println("BLE connected");
            deviceConnected = true;

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

            // notify of current modes / values (for value not uptate by LCD)
            pCharacteristicSpeedLimiter->setValue((uint8_t *)&shrd->speedLimiter, 1);
            pCharacteristicSpeedLimiter->notify();
        };

        void onDisconnect(BLEServer *pServer)
        {
            Serial.println("BLE disonnected");
            deviceConnected = false;

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
                    if (!blePicclyVisible)
                    {
                        bleLockStatus = true;
                        Serial.println(" ==> device disconnected / PICLLY not visible ==> LOCK decision");
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
            uint32_t pinCode = settings->getS3F().Bluetooth_pin_code;
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
            }
            else
            {
                Serial.print("BLH - onAuthenticationComplete : hummm ... failed / reason : ");
                Serial.println(cmpl.fail_reason);
            }
        }
    };

    class BLECharacteristicCallback : public BLECharacteristicCallbacks
    {
        void onWrite(BLECharacteristic *pCharacteristic)
        {
            if (pCharacteristic->getUUID().toString() == MODE_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                shrd->modeOrder = rxValue[0];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->modeOrder);
                Serial.print("BLH - Write mode : ");
                Serial.println(print_buffer);
            }
            if (pCharacteristic->getUUID().toString() == BRAKE_STATUS_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                shrd->brakeSentOrder = rxValue[0];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->brakeSentOrder);
                Serial.print("BLH - Write brakeSentOrder : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == SETTINGS1_CHARACTERISTIC_UUID)
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

                // update BLE PIN code
                pSecurity->setStaticPIN(settings->getS3F().Bluetooth_pin_code);

                // reset speed PID
                resetPid();

                settings->saveSettings();
            }
            else if (pCharacteristic->getUUID().toString() == SPEED_LIMITER_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                shrd->speedLimiter = rxValue[0];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->speedLimiter);
                Serial.print("BLH - Write speedLimiter : ");
                Serial.println(print_buffer);

                // notify of current value
                pCharacteristicSpeedLimiter->setValue((uint8_t *)&shrd->speedLimiter, 1);
                pCharacteristicSpeedLimiter->notify();
            }
            else if (pCharacteristic->getUUID().toString() == ECO_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                shrd->ecoOrder = rxValue[0];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->ecoOrder);
                Serial.print("BLH - Write eco : ");
                Serial.println(print_buffer);

                // notify of current value
                pCharacteristicEco->setValue((uint8_t *)&shrd->ecoOrder, 1);
                pCharacteristicEco->notify();
            }
            else if (pCharacteristic->getUUID().toString() == ACCEL_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                shrd->accelOrder = rxValue[0];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->accelOrder);
                Serial.print("BLH - Write accel : ");
                Serial.println(print_buffer);

                // notify of current value
                pCharacteristicAccel->setValue((uint8_t *)&shrd->accelOrder, 1);
                pCharacteristicAccel->notify();
            }
            else if (pCharacteristic->getUUID().toString() == CURRENT_CALIB_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                shrd->currentCalibOrder = rxValue[0];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->currentCalibOrder);
                Serial.print("BLH - Write currentCalibOrder : ");
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

                // disconnect BT

                Serial.println("BT deinit => done");

                delay(1000);

                // init OTA
                OTA_setup();

                Serial.println("OTA init => done");
                delay(1000);

                // Enable wifi & OTA
                shrd->inOtaMode = true;
            }
            else if (pCharacteristic->getUUID().toString() == FAST_UPDATE_CHARACTERISTIC_UUID)
            {
                Serial.println("Write FAST_UPDATE_CHARACTERISTIC_UUID");

                std::string rxValue = pCharacteristic->getValue();
                fastUpdate = rxValue[0];

                Serial.print("BLH - Fast update = ");
                Serial.println(fastUpdate);
            }
            else if (pCharacteristic->getUUID().toString() == AUX_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();
                shrd->auxOrder = rxValue[0];

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->auxOrder);
                Serial.print("BLH - Write aux : ");
                Serial.println(print_buffer);

                // notify of current value
                pCharacteristicAux->setValue((uint8_t *)&shrd->auxOrder, 1);
                pCharacteristicAux->notify();
            }
            else if (pCharacteristic->getUUID().toString() == SPEED_PID_CHARACTERISTIC_UUID)
            {
                std::string rxValue = pCharacteristic->getValue();

                memcpy(&shrd->speedPidKp, &rxValue[0], 4);
                memcpy(&shrd->speedPidKi, &rxValue[4], 4);
                memcpy(&shrd->speedPidKd, &rxValue[8], 4);
                /*
                shrd->speedPidKp = rxValue[0];
                shrd->speedPidKi = rxValue[1];
                shrd->speedPidKd = rxValue[2];
*/

                char print_buffer[500];
                sprintf(print_buffer, "BLH - Write : speedPidKp = %d / speedPidKi = %d / speedPidKd = %d", shrd->speedPidKp, shrd->speedPidKi, shrd->speedPidKd);
                Serial.println(print_buffer);

                resetPid();
            }
        }

        void onRead(BLECharacteristic *pCharacteristic)
        {

            if (pCharacteristic->getUUID().toString() == MODE_CHARACTERISTIC_UUID)
            {
                pCharacteristicMode->setValue((uint8_t *)&shrd->modeOrder, 1);

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->modeOrder);
                Serial.print("BLH - Read mode : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == SPEED_CHARACTERISTIC_UUID)
            {
                pCharacteristicSpeed->setValue((uint8_t *)&shrd->speedCurrent, 1);

                char print_buffer[500];
                sprintf(print_buffer, "%02x", (int)shrd->speedCurrent);
                Serial.print("BLH - Read speed : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == BRAKE_STATUS_CHARACTERISTIC_UUID)
            {

                byte value[2];
                value[0] = shrd->brakeSentOrder;
                value[1] = shrd->brakeStatus;
                pCharacteristicBrakeSentOrder->setValue((uint8_t *)&value, 2);

                char print_buffer[500];
                sprintf(print_buffer, "%02x", shrd->brakeSentOrder);
                Serial.print("BLH - Read breakeSentOrder : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == VOLTAGE_STATUS_CHARACTERISTIC_UUID)
            {
                uint32_t voltage = shrd->voltageFilterMean;
                pCharacteristicVoltageStatus->setValue((uint8_t *)&voltage, 4);

                char print_buffer[500];
                sprintf(print_buffer, "%02f", voltage / 1000.0);
                Serial.print("BLH - Read voltage : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == CURRENT_STATUS_CHARACTERISTIC_UUID)
            {
                int32_t current = shrd->currentFilterMean;
                pCharacteristicCurrentStatus->setValue((uint8_t *)&current, 4);

                char print_buffer[500];
                sprintf(print_buffer, "%02d", current);
                Serial.print("BLH - Read current : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == POWER_STATUS_CHARACTERISTIC_UUID)
            {
                int32_t current = shrd->currentFilterMean;
                uint32_t voltage = shrd->voltageFilterMean;
                int32_t power = (current / 1000.0) * (voltage / 1000.0);
                pCharacteristicPowerStatus->setValue((uint8_t *)&power, 4);

                char print_buffer[500];
                sprintf(print_buffer, "%04d", power);
                Serial.print("BLH - Read power : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == BTLOCK_STATUS_CHARACTERISTIC_UUID)
            {

                notifyBleLock();

                // byte value[4];
                // value[0] = bleLockStatus;
                // value[1] = blePicclyVisible;
                // value[2] = blePicclyRSSI;
                // value[3] = bleLockForced;
                // pCharacteristicBtlockStatus->setValue((uint8_t *)&value, 4);

                char print_buffer[500];
                sprintf(print_buffer, "%02x", bleLockStatus);
                Serial.print("BLH - Read bleLock : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == TEMPERATURE_STATUS_CHARACTERISTIC_UUID)
            {
                int32_t temp = shrd->currentTemperature * 1000.0;
                pCharacteristicTemperatureStatus->setValue((uint8_t *)&temp, 4);

                char print_buffer[500];
                sprintf(print_buffer, "%f", shrd->currentTemperature);
                Serial.print("BLH - Read currentTemperature : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == HUMIDITY_STATUS_CHARACTERISTIC_UUID)
            {
                int32_t temp = shrd->currentHumidity * 1000.0;
                pCharacteristicHumidityStatus->setValue((uint8_t *)&temp, 4);

                char print_buffer[500];
                sprintf(print_buffer, "%f", shrd->currentHumidity);
                Serial.print("BLH - Read currentHumidity : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == SPEED_LIMITER_CHARACTERISTIC_UUID)
            {
                pCharacteristicSpeedLimiter->setValue((uint8_t *)&shrd->speedLimiter, 1);

                char print_buffer[500];
                sprintf(print_buffer, "%d", shrd->speedLimiter);
                Serial.print("BLH - Read speedLimiter : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == ECO_CHARACTERISTIC_UUID)
            {
                pCharacteristicEco->setValue((uint8_t *)&shrd->ecoOrder, 1);

                char print_buffer[500];
                sprintf(print_buffer, "%d", shrd->ecoOrder);
                Serial.print("BLH - Read eco : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == ACCEL_CHARACTERISTIC_UUID)
            {
                pCharacteristicAccel->setValue((uint8_t *)&shrd->accelOrder, 1);

                char print_buffer[500];
                sprintf(print_buffer, "%d", shrd->accelOrder);
                Serial.print("BLH - Read accel : ");
                Serial.println(print_buffer);
            }
            else if (pCharacteristic->getUUID().toString() == AUX_CHARACTERISTIC_UUID)
            {
                pCharacteristicAux->setValue((uint8_t *)&shrd->auxOrder, 1);

                char print_buffer[500];
                sprintf(print_buffer, "%d", shrd->auxOrder);
                Serial.print("BLH - Read aux : ");
                Serial.println(print_buffer);
            }
        }
    };

    // Init settings
    settings = data;

    // Create the BLE Device
    BLEDevice::init("SmartLCD");
    String smartLcdNameComp1 = BLEDevice::getAddress().toString().substr(12, 2).c_str();
    String smartLcdNameComp2 = BLEDevice::getAddress().toString().substr(15, 2).c_str();
    String smartLcdFullName = "SmartLCD-" + smartLcdNameComp1 + smartLcdNameComp2;
    Serial.print("BLH - adress = ");
    Serial.println(BLEDevice::getAddress().toString().c_str());
    Serial.print("BLH - name = ");
    Serial.println(smartLcdFullName);
    esp_ble_gap_set_device_name(smartLcdFullName.c_str());
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
    BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID), 80);

    // Create a BLE Characteristic
    pCharacteristicSpeed = pService->createCharacteristic(
        SPEED_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicMode = pService->createCharacteristic(
        MODE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicBrakeSentOrder = pService->createCharacteristic(
        BRAKE_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicVoltageStatus = pService->createCharacteristic(
        VOLTAGE_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicCurrentStatus = pService->createCharacteristic(
        CURRENT_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicPowerStatus = pService->createCharacteristic(
        POWER_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicBtlockStatus = pService->createCharacteristic(
        BTLOCK_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicTemperatureStatus = pService->createCharacteristic(
        TEMPERATURE_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicHumidityStatus = pService->createCharacteristic(
        HUMIDITY_STATUS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSettings1 = pService->createCharacteristic(
        SETTINGS1_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSpeedLimiter = pService->createCharacteristic(
        SPEED_LIMITER_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicEco = pService->createCharacteristic(
        ECO_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicAccel = pService->createCharacteristic(
        ACCEL_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicCurrentCalib = pService->createCharacteristic(
        CURRENT_CALIB_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicOtaSwitch = pService->createCharacteristic(
        SWITCH_TO_OTA_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristicLogs = pService->createCharacteristic(
        LOGS_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY);

    pCharacteristicFastUpdate = pService->createCharacteristic(
        FAST_UPDATE_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristicSettings2 = pService->createCharacteristic(
        SETTINGS2_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSettings3 = pService->createCharacteristic(
        SETTINGS3_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicAux = pService->createCharacteristic(
        AUX_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_NOTIFY |
            BLECharacteristic::PROPERTY_WRITE |
            BLECharacteristic::PROPERTY_READ);

    pCharacteristicSpeedPid = pService->createCharacteristic(
        SPEED_PID_CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE);

    pCharacteristicSpeed->addDescriptor(new BLE2902());
    pCharacteristicMode->addDescriptor(new BLE2902());
    pCharacteristicBrakeSentOrder->addDescriptor(new BLE2902());
    pCharacteristicVoltageStatus->addDescriptor(new BLE2902());
    pCharacteristicCurrentStatus->addDescriptor(new BLE2902());
    pCharacteristicPowerStatus->addDescriptor(new BLE2902());
    pCharacteristicBtlockStatus->addDescriptor(new BLE2902());
    pCharacteristicTemperatureStatus->addDescriptor(new BLE2902());
    pCharacteristicHumidityStatus->addDescriptor(new BLE2902());
    pCharacteristicSettings1->addDescriptor(new BLE2902());
    pCharacteristicSpeedLimiter->addDescriptor(new BLE2902());
    pCharacteristicEco->addDescriptor(new BLE2902());
    pCharacteristicAccel->addDescriptor(new BLE2902());
    pCharacteristicCurrentCalib->addDescriptor(new BLE2902());
    pCharacteristicOtaSwitch->addDescriptor(new BLE2902());
    pCharacteristicLogs->addDescriptor(new BLE2902());
    pCharacteristicFastUpdate->addDescriptor(new BLE2902());
    pCharacteristicSettings2->addDescriptor(new BLE2902());
    pCharacteristicSettings3->addDescriptor(new BLE2902());
    pCharacteristicAux->addDescriptor(new BLE2902());
    pCharacteristicSpeedPid->addDescriptor(new BLE2902());

    pCharacteristicSpeed->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicMode->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicBrakeSentOrder->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicVoltageStatus->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicCurrentStatus->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicPowerStatus->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicBtlockStatus->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicTemperatureStatus->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicHumidityStatus->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSettings1->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSpeedLimiter->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicEco->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicAccel->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicCurrentCalib->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicOtaSwitch->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicLogs->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicFastUpdate->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSettings2->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSettings3->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicAux->setCallbacks(new BLECharacteristicCallback());
    pCharacteristicSpeedPid->setCallbacks(new BLECharacteristicCallback());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
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
    pBLEScan->start(10, &bleOnScanResults, false);
}

void BluetoothHandler::bleOnScanResults(BLEScanResults scanResults)
{

#if DEBUG_DISPLAY_BLE_SCAN
    Serial.print("BLH - BLE Scan Device found: ");
    Serial.println(scanResults.getCount());
#endif

    bool newBlePicclyVisible = false;

    for (int i = 0; i < scanResults.getCount(); i++)
    {
        String name = scanResults.getDevice(i).getName().c_str();
        blePicclyRSSI = scanResults.getDevice(i).getRSSI();
        std::string address = scanResults.getDevice(i).getAddress().toString();
        String addressStr = address.c_str();

#if DEBUG_DISPLAY_BLE_SCAN
        Serial.print("BLH - BLE device : ");
        Serial.print(name);
        Serial.print("BLH -  / adress : ");
        Serial.print(addressStr);
        Serial.print("BLH -  / name : ");
        Serial.print(name);
        Serial.print("BLH -  / rssi ");
        Serial.println(blePicclyRSSI);
#endif

        String addressPicclySettings = settings->getS2F().Beacon_Mac_Address;

        if (addressPicclySettings.equalsIgnoreCase(addressStr))
        {
            if (blePicclyRSSI < settings->getS1F().Beacon_range)
            {
#if DEBUG_DISPLAY_BLE_SCAN
                Serial.print("BLH -  ==> PICC-LY found ... but too far away / RSSI = ");
                Serial.print(blePicclyRSSI);
                Serial.print("BLH -  / min RSSI required = ");
                Serial.print(settings->getS1F().Beacon_range);
                Serial.println(" ==> lock from scan");
#endif
                newBlePicclyVisible = false;
            }
            else
            {
#if DEBUG_DISPLAY_BLE_SCAN
                Serial.print("BLH -  ==> PICC-LY found  / RSSI = ");
                Serial.print(blePicclyRSSI);
                Serial.print("BLH -  / min RSSI required = ");
                Serial.print(settings->getS1F().Beacon_range);
                Serial.println(" ==> unlock from scan");
#endif
                newBlePicclyVisible = true;
            }
        }
    }

    // store piclyy status
    blePicclyVisible = newBlePicclyVisible;

    if (bleLockForced == 0)
    {
        if (settings->getS1F().Bluetooth_lock_mode == 2)
        {
            if ((!blePicclyVisible) && (!deviceConnected))
            {
                bleLockStatus = 1;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.println(" ==> PICLLY not visible // smartphone not connected ==> LOCK decision");
                Serial.println("-------------------------------------");
#endif
            }
            else if ((!blePicclyVisible) && (deviceConnected))
            {
                bleLockStatus = 0;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.println(" ==> PICLLY visible // smartphone connected ==> UNLOCK decision");
                Serial.println("-------------------------------------");
#endif
            }
            else
            {
            }
        }
        if (settings->getS1F().Bluetooth_lock_mode == 3)
        {
            if (!blePicclyVisible)
            {
                bleLockStatus = 1;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.println(" ==> PICLLY not visible ==> LOCK decision");
                Serial.println("-------------------------------------");
#endif
            }
            else if (blePicclyVisible)
            {
                bleLockStatus = 0;

#if DEBUG_DISPLAY_BLE_SCAN
                Serial.println(" ==> PICLLY visible ==> UNLOCK decision");
                Serial.println("-------------------------------------");
#endif
            }
        }
    }

    notifyBleLock();

    // launch new scan
    pBLEScan->start(5, &bleOnScanResults, false);

    // set BT lock
    // if ((!deviceConnected))
    // {
    //   if (!blePicclyVisible)
    //   {
    //     bleLockStatus = true;
    //     Serial.println(" ==> no device connected and PICC-LY no found ==> LOCK decision");
    //   }
    //   else
    //   {
    //     bleLockStatus = false;
    //     Serial.println(" ==> no device connected and PICC-LY found ==> UNLOCK decision");
    //   }
    // }
}
void BluetoothHandler::notifyBleLogs(char *txt)
{
    // notify of new log
    pCharacteristicLogs->setValue((uint8_t *)txt, strlen(txt));
    pCharacteristicLogs->notify();
}

void BluetoothHandler::notifyModeOrder(uint8_t val)
{
    pCharacteristicMode->setValue((uint8_t *)&val, 1);
    pCharacteristicMode->notify();
}

void BluetoothHandler::notifyBreakeSentOrder(uint8_t order, uint8_t isPressed)
{
    byte value[2];
    value[0] = order;
    value[1] = isPressed;
    pCharacteristicBrakeSentOrder->setValue((uint8_t *)&value, 2);
    pCharacteristicBrakeSentOrder->notify();
}

void BluetoothHandler::notifyEcoOrder(uint8_t val)
{
    pCharacteristicEco->setValue((uint8_t *)&val, 1);
    pCharacteristicEco->notify();
}

void BluetoothHandler::notifyAccelOrder(uint8_t val)
{
    pCharacteristicAccel->setValue((uint8_t *)&val, 1);
    pCharacteristicAccel->notify();
}

void BluetoothHandler::notifyTemperatureStatus(uint32_t val)
{
    pCharacteristicTemperatureStatus->setValue((uint8_t *)&val, 4);
    pCharacteristicTemperatureStatus->notify();
}

void BluetoothHandler::notifyHumidityStatus(uint32_t val)
{
    pCharacteristicHumidityStatus->setValue((uint8_t *)&val, 4);
    pCharacteristicHumidityStatus->notify();
}

void BluetoothHandler::notifySpeedLimiterStatus(uint8_t val)
{
    pCharacteristicSpeedLimiter->setValue((uint8_t *)&val, 1);
    pCharacteristicSpeedLimiter->notify();
}

void BluetoothHandler::notifyAuxOrder(uint8_t val)
{
    pCharacteristicAux->setValue((uint8_t *)&val, 1);
    pCharacteristicAux->notify();
}

void BluetoothHandler::notifyBleLock()
{
    byte value[4];
    value[0] = bleLockStatus;
    value[1] = blePicclyVisible;
    value[2] = blePicclyRSSI;
    value[3] = bleLockForced;
    pCharacteristicBtlockStatus->setValue((uint8_t *)&value, 4);
    pCharacteristicBtlockStatus->notify();

#if DEBUG_DISPLAY_BLE_NOTIFY
    Serial.print("BLH - notifyBleLock : bleLockStatus = ");
    Serial.print(bleLockStatus);
    Serial.print("BLH -  / blePicclyVisible = ");
    Serial.print(blePicclyVisible);
    Serial.print("BLH -  / blePicclyRSSI = ");
    Serial.print(blePicclyRSSI);

    Serial.print("BLH -  / bleLockForced = ");
    Serial.print(bleLockForced);
    Serial.println("");
#endif
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

    // notify changed value
    if (deviceConnected)
    {
        uint16_t period = 500;
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
            uint8_t speedInt = shrd->speedCurrent;
            pCharacteristicSpeed->setValue((uint8_t *)&speedInt, 1);
            pCharacteristicSpeed->notify();

#if DEBUG_DISPLAY_BLE_NOTIFY
            Serial.print("BLH - Notify speed : ");
            Serial.println(shrd->speedCurrent);
#endif

            // notify bluetooth
            uint32_t voltage = shrd->voltageFilterMean;
            pCharacteristicVoltageStatus->setValue((uint8_t *)&voltage, 4);
            pCharacteristicVoltageStatus->notify();

#if DEBUG_DISPLAY_BLE_NOTIFY
            Serial.print("BLH - Notify voltage : ");
            Serial.println(voltage);
#endif

            int32_t current = shrd->currentFilterMean;
            pCharacteristicCurrentStatus->setValue((uint8_t *)&current, 4);
            pCharacteristicCurrentStatus->notify();

#if DEBUG_DISPLAY_BLE_NOTIFY
            Serial.print("BLH - Notify current : ");
            Serial.println(current);
#endif

            int32_t power = (current / 1000.0) * (voltage / 1000.0);
            if (power < 0)
                power = 0;
            pCharacteristicPowerStatus->setValue((uint8_t *)&power, 4);
            pCharacteristicPowerStatus->notify();

#if DEBUG_DISPLAY_BLE_NOTIFY
            Serial.print("BLH - Notify power : ");
            Serial.println(power);
#endif

            notifyBleLock();

            /*
#if DEBUG_DISPLAY_BLE_NOTIFY
            Serial.print("BLH - Notify bleLock : ");
            Serial.println(shrd->bleLock);
#endif
*/

            shrd->timeLastNotifyBle = millis();
        }
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected)
    {
        //delay(500);                  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected)
    {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}

void BluetoothHandler::setSharedData(SharedData *data)
{
    shrd = data;
}
