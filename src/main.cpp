//////////////////////////////////////////
// TODO : current loop
// TODO : speed loop
// TODO : speed adjustment
// TODO : LCD error indicators
// TODO : buttons management
// TODO : BT pin code
// TODO : beacon MAC
// TODO : mode Z
// TODO : brake disable with high voltage
// TODO : auto mode shift on low battery
//////////////////////////////////////////

//////------------------------------------
////// Inludes

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <EEPROM.h>

#include "main.h"
#include "OTA_softap.h"
#include "MedianFilter.h"
#include "dht_nonblocking.h"
#include "Settings.h"

//////------------------------------------
////// Defines

#define DEBUG_BLE_DISPLAY_VOLTAGE 1
#define DEBUG_BLE_DISPLAY_FRAME 0

#define DEBUG_BLE_SCAN 0
#define DEBUG_BLE_NOTIFY 0
#define DEBUG_DISPLAY_FRAME_LCD_TO_CNTRL 0
#define DEBUG_DISPLAY_FRAME_CNTRL_TO_LCD 0
#define DEBUG_DISPLAY_DECODED_FRAME_CNTRL_TO_LCD 0
#define DEBUG_DISPLAY_SPEED 0
#define DEBUG_DISPLAY_MODE 0
#define DEBUG_DISPLAY_BRAKE 0
#define DEBUG_DISPLAY_ANALOG_BRAKE 0
#define DEBUG_DISPLAY_ECO 0
#define DEBUG_DISPLAY_ACCEL 0
#define DEBUG_DISPLAY_BUTTON1 0
#define DEBUG_DISPLAY_BUTTON2 0
#define DEBUG_DISPLAY_VOLTAGE 0
#define DEBUG_DISPLAY_CURRENT 0
#define DEBUG_DISPLAY_DHT 0
#define DEBUG_SERIAL_CHECKSUM_LCD_TO_CNTRL 0
#define DEBUG_SERIAL_CHECKSUM_CNTRL_TO_LCD 0

#define ALLOW_LCD_TO_CNTRL_MODIFICATIONS 1
#define ALLOW_CNTRL_TO_LCD_MODIFICATIONS 1

#define PIN_SERIAL_LCD_TO_ESP 25
#define PIN_SERIAL_ESP_TO_CNTRL 32
#define PIN_SERIAL_CNTRL_TO_ESP 26
#define PIN_SERIAL_ESP_TO_LCD 13
#define PIN_OUT_RELAY 12
#define PIN_IN_VOLTAGE 33
#define PIN_IN_CURRENT 35
#define PIN_IN_BUTTON1 9
#define PIN_IN_BUTTON2 10
#define PIN_OUT_LED_BUTTON1 14
#define PIN_OUT_LED_BUTTON2 5
#define PIN_OUT_BRAKE 16
#define PIN_IN_OUT_DHT 27
#define PIN_IN_BRAKE 34

#define MODE_LCD_TO_CNTRL 0
#define MODE_CNTRL_TO_LCD 1

#define DATA_BUFFER_SIZE 30
#define BAUD_RATE 1200

#define ANALOG_TO_VOLTS 39.15
#define ANALOG_TO_CURRENT 35
#define NB_CURRENT_CALIB 100

#define ANALOG_BRAKE_MIN_VALUE 850
#define ANALOG_BRAKE_MAX_VALUE 2300

#define EEPROM_SIZE 1024
#define EEPROM_ADDRESS_SETTINGS 0
#define EEPROM_ADDRESS_BLE_LOCK_FORCED 100

#define BLE_MTU 128

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

//////------------------------------------
////// Variables

// Time
uint32_t timeLastNotifyBle = 0;
uint32_t timeLastBrake = 0;
uint32_t timeLoop = 0;

// Settings

int begin_soft = 0;
int begin_hard = 0;

char data_buffer_lcd_mod[DATA_BUFFER_SIZE];
char data_buffer_cntrl_mod[DATA_BUFFER_SIZE];
char data_buffer_lcd_ori[DATA_BUFFER_SIZE];
char data_buffer_cntrl_ori[DATA_BUFFER_SIZE];

char bleLog[50] = "";

HardwareSerial hwSerCntrlToLcd(1);
HardwareSerial hwSerLcdToCntrl(2);

DHT_nonblocking dht_sensor(PIN_IN_OUT_DHT, DHT_TYPE_22);

int i_loop = 0;

boolean inOtaMode = false;

int8_t bleLockStatus = 0;
int8_t blePicclyVisible = 0;
int8_t blePicclyRSSI = 0;
int8_t bleLockForced = 0;

int8_t fastUpdate = 0;

uint8_t speedLimiter = 1;

float currentHumidity = 0.0;
float currentTemperature = 0.0;

int i_LcdToCntrl = 0;
int i_CntrlToLcd = 0;
int begin_LcdToCntrl = 1;
int begin_CntrlToLcd = 1;

int isModified_LcdToCntrl = 0;
int isModified_CntrlToLcd = 0;

uint8_t speedCurrent = 0;
uint8_t speedOld = 0;
uint8_t speedFake = 25;

uint8_t powerReduction = 0;

uint8_t modeOrder = 3;
uint8_t modeLcd = 0;
uint8_t modeLcdOld = 0;

uint8_t accelOrder = 0;
uint8_t accelLcd = 0;
uint8_t accelLcdOld = 0;

uint8_t ecoOrder = 3;
uint8_t ecoLcd = 3;
uint8_t ecoLcdOld = 3;

uint8_t brakeStatus = 0;
uint8_t brakeStatusOld = 0;
int8_t breakeSentOrder = -1;
uint8_t brakeLcd = 0;
int8_t brakeLcdOld = -1;
int8_t breakeSentOrderOld = -1;
uint16_t brakeAnalogValue = 0;
uint8_t button1 = 0;

uint8_t currentCalibOrder = 1;
uint32_t iCurrentCalibOrder = 0;

uint8_t button1Status = 0;
uint8_t button2Status = 0;

uint16_t voltageStatus = 0;
uint32_t voltageInMilliVolts = 0;
MedianFilter voltageFilter(100, 30000);
MedianFilter voltageRawFilter(100, 2000);
MedianFilter currentFilter(100, 1830);
MedianFilter currentFilterInit(100, 1830);

Settings settings;

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristicSpeed = NULL;
BLECharacteristic *pCharacteristicMode = NULL;
BLECharacteristic *pCharacteristicBrakeSentOrder = NULL;
BLECharacteristic *pCharacteristicVoltageStatus = NULL;
BLECharacteristic *pCharacteristicCurrentStatus = NULL;
BLECharacteristic *pCharacteristicPowerStatus = NULL;
BLECharacteristic *pCharacteristicBtlockStatus = NULL;
BLECharacteristic *pCharacteristicTemperatureStatus = NULL;
BLECharacteristic *pCharacteristicHumidityStatus = NULL;
BLECharacteristic *pCharacteristicSettings1 = NULL;
BLECharacteristic *pCharacteristicSpeedLimiter = NULL;
BLECharacteristic *pCharacteristicEco = NULL;
BLECharacteristic *pCharacteristicAccel = NULL;
BLECharacteristic *pCharacteristicCurrentCalib = NULL;
BLECharacteristic *pCharacteristicOtaSwitch = NULL;
BLECharacteristic *pCharacteristicLogs = NULL;
BLECharacteristic *pCharacteristicFastUpdate = NULL;
BLECharacteristic *pCharacteristicSettings2 = NULL;
BLECharacteristic *pCharacteristicSettings3 = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
BLEScan *pBLEScan;

// BYTE 3 FROM LCD TO CONTRL ... for each sequence number // brute force
const byte modeLcd0[256] = {0x80, 0x05, 0x06, 0x2b, 0x34, 0x29, 0x2a, 0x2f, 0x28, 0x2d, 0x2e, 0x53, 0x7c, 0x51, 0x52, 0x57, 0x50, 0x55, 0x56, 0x7b, 0x84, 0x79, 0x7a, 0x7f, 0x78,
                            0x7d, 0x7e, 0x63, 0x0c, 0x61, 0x62, 0x67, 0x60, 0x65, 0x66, 0x0b, 0x14, 0x09, 0x0a, 0x0f, 0x08, 0x0d, 0x0e, 0x33, 0x5c, 0x31, 0x32, 0x37, 0x30, 0x35, 0x36, 0x5b, 0x64, 0x59, 0x5a,
                            0x5f, 0x58, 0x5d, 0x5e, 0x43, 0x6c, 0x41, 0x42, 0x47, 0x40, 0x45, 0x46, 0x6b, 0x74, 0x69, 0x6a, 0x6f, 0x68, 0x6d, 0x6e, 0x13, 0x3c, 0x11, 0x12, 0x17, 0x10, 0x15, 0x16, 0x3b,
                            0x44, 0x39, 0x3a, 0x3f, 0x38, 0x3d, 0x3e, 0x23, 0x4c, 0x21, 0x22, 0x27, 0x20, 0x25, 0x26, 0x4b, 0x54, 0x49, 0x4a, 0x4f, 0x48, 0x4d, 0x4e, 0x73, 0x1c, 0x71, 0x72, 0x77, 0x70,
                            0x75, 0x76, 0x1b, 0x24, 0x19, 0x1a, 0x1f, 0x18, 0x1d, 0x1e, 0x83, 0x2c, 0x81, 0x82, 0x07, 0x80, 0x05, 0x06, 0x2b, 0x34, 0x29, 0x2a, 0x2f, 0x28, 0x2d, 0x2e, 0x53, 0x7c, 0x51, 0x52,
                            0x57, 0x50, 0x55, 0x56, 0x7b, 0x84, 0x79, 0x7a, 0x7f, 0x78, 0x7d, 0x7e, 0x63, 0x0c, 0x61, 0x62, 0x67, 0x60, 0x65, 0x66, 0x0b, 0x14, 0x09, 0x0a, 0x0f, 0x08, 0x0d, 0x0e, 0x33, 0x5c,
                            0x31, 0x32, 0x37, 0x30, 0x35, 0x36, 0x5b, 0x64, 0x59, 0x5a, 0x5f, 0x58, 0x5d, 0x5e, 0x43, 0x6c, 0x41, 0x42, 0x47, 0x40, 0x45, 0x46, 0x6b, 0x74, 0x69, 0x6a, 0x6f, 0x68, 0x6d,
                            0x6e, 0x13, 0x3c, 0x11, 0x12, 0x17, 0x10, 0x15, 0x16, 0x3b, 0x44, 0x39, 0x3a, 0x3f, 0x38, 0x3d, 0x3e, 0x23, 0x4c, 0x21, 0x22, 0x27, 0x20, 0x25, 0x26, 0x4b, 0x54, 0x49, 0x4a,
                            0x4f, 0x48, 0x4d, 0x4e, 0x73, 0x1c, 0x71, 0x72, 0x77, 0x70, 0x75, 0x76, 0x1b, 0x24, 0x19, 0x1a, 0x1f, 0x18, 0x1d, 0x1e, 0x83, 0x2c, 0x81, 0x82, 0x7};
const byte modeLcd1[256] = {0x85, 0x0a, 0x0b, 0x30, 0x39, 0x2e, 0x2f, 0x34, 0x2d, 0x32, 0x33, 0x58, 0x81, 0x56, 0x57, 0x5c, 0x55, 0x5a, 0x5b, 0x80, 0x89, 0x7e, 0x7f, 0x84,
                            0x7d, 0x82, 0x83, 0x68, 0x11, 0x66, 0x67, 0x6c, 0x65, 0x6a, 0x6b, 0x10, 0x19, 0x0e, 0x0f, 0x14, 0x0d, 0x12, 0x13, 0x38, 0x61, 0x36, 0x37, 0x3c, 0x35, 0x3a, 0x3b, 0x60, 0x69,
                            0x5e, 0x5f, 0x64, 0x5d, 0x62, 0x63, 0x48, 0x71, 0x46, 0x47, 0x4c, 0x45, 0x4a, 0x4b, 0x70, 0x79, 0x6e, 0x6f, 0x74, 0x6d, 0x72, 0x73, 0x18, 0x41, 0x16, 0x17, 0x1c, 0x15, 0x1a,
                            0x1b, 0x40, 0x49, 0x3e, 0x3f, 0x44, 0x3d, 0x42, 0x43, 0x28, 0x51, 0x26, 0x27, 0x2c, 0x25, 0x2a, 0x2b, 0x50, 0x59, 0x4e, 0x4f, 0x54, 0x4d, 0x52, 0x53, 0x78, 0x21, 0x76, 0x77,
                            0x7c, 0x75, 0x7a, 0x7b, 0x20, 0x29, 0x1e, 0x1f, 0x24, 0x1d, 0x22, 0x23, 0x88, 0x31, 0x86, 0x87, 0x0c, 0x85, 0x0a, 0x0b, 0x30, 0x39, 0x2e, 0x2f, 0x34, 0x2d, 0x32, 0x33, 0x58,
                            0x81, 0x56, 0x57, 0x5c, 0x55, 0x5a, 0x5b, 0x80, 0x89, 0x7e, 0x7f, 0x84, 0x7d, 0x82, 0x83, 0x68, 0x11, 0x66, 0x67, 0x6c, 0x65, 0x6a, 0x6b, 0x10, 0x19, 0x0e, 0x0f, 0x14, 0x0d,
                            0x12, 0x13, 0x38, 0x61, 0x36, 0x37, 0x3c, 0x35, 0x3a, 0x3b, 0x60, 0x69, 0x5e, 0x5f, 0x64, 0x5d, 0x62, 0x63, 0x48, 0x71, 0x46, 0x47, 0x4c, 0x45, 0x4a, 0x4b, 0x70, 0x79, 0x6e,
                            0x6f, 0x74, 0x6d, 0x72, 0x73, 0x18, 0x41, 0x16, 0x17, 0x1c, 0x15, 0x1a, 0x1b, 0x40, 0x49, 0x3e, 0x3f, 0x44, 0x3d, 0x42, 0x43, 0x28, 0x51, 0x26, 0x27, 0x2c, 0x25, 0x2a, 0x2b,
                            0x50, 0x59, 0x4e, 0x4f, 0x54, 0x4d, 0x52, 0x53, 0x78, 0x21, 0x76, 0x77, 0x7c, 0x75, 0x7a, 0x7b, 0x20, 0x29, 0x1e, 0x1f, 0x24, 0x1d, 0x22, 0x23, 0x88, 0x31, 0x86, 0x87, 0x0c};
const byte modeLcd2[256] = {0x8a, 0x0f, 0x10, 0x35, 0x3e, 0x33, 0x34, 0x39, 0x32, 0x37, 0x38, 0x5d, 0x86, 0x5b, 0x5c, 0x61, 0x5a, 0x5f, 0x60, 0x85, 0x8e, 0x83, 0x84, 0x89, 0x82, 0x87,
                            0x88, 0x6d, 0x16, 0x6b, 0x6c, 0x71, 0x6a, 0x6f, 0x70, 0x15, 0x1e, 0x13, 0x14, 0x19, 0x12, 0x17, 0x18, 0x3d, 0x66, 0x3b, 0x3c, 0x41, 0x3a, 0x3f, 0x40, 0x65, 0x6e, 0x63, 0x64,
                            0x69, 0x62, 0x67, 0x68, 0x4d, 0x76, 0x4b, 0x4c, 0x51, 0x4a, 0x4f, 0x50, 0x75, 0x7e, 0x73, 0x74, 0x79, 0x72, 0x77, 0x78, 0x1d, 0x46, 0x1b, 0x1c, 0x21, 0x1a, 0x1f, 0x20, 0x45,
                            0x4e, 0x43, 0x44, 0x49, 0x42, 0x47, 0x48, 0x2d, 0x56, 0x2b, 0x2c, 0x31, 0x2a, 0x2f, 0x30, 0x55, 0x5e, 0x53, 0x54, 0x59, 0x52, 0x57, 0x58, 0x7d, 0x26, 0x7b, 0x7c, 0x81, 0x7a,
                            0x7f, 0x80, 0x25, 0x2e, 0x23, 0x24, 0x29, 0x22, 0x27, 0x28, 0x8d, 0x36, 0x8b, 0x8c, 0x11, 0x8a, 0x0f, 0x10, 0x35, 0x3e, 0x33, 0x34, 0x39, 0x32, 0x37, 0x38, 0x5d, 0x86, 0x5b,
                            0x5c, 0x61, 0x5a, 0x5f, 0x60, 0x85, 0x8e, 0x83, 0x84, 0x89, 0x82, 0x87, 0x88, 0x6d, 0x16, 0x6b, 0x6c, 0x71, 0x6a, 0x6f, 0x70, 0x15, 0x1e, 0x13, 0x14, 0x19, 0x12, 0x17, 0x18,
                            0x3d, 0x66, 0x3b, 0x3c, 0x41, 0x3a, 0x3f, 0x40, 0x65, 0x6e, 0x63, 0x64, 0x69, 0x62, 0x67, 0x68, 0x4d, 0x76, 0x4b, 0x4c, 0x51, 0x4a, 0x4f, 0x50, 0x75, 0x7e, 0x73, 0x74, 0x79,
                            0x72, 0x77, 0x78, 0x1d, 0x46, 0x1b, 0x1c, 0x21, 0x1a, 0x1f, 0x20, 0x45, 0x4e, 0x43, 0x44, 0x49, 0x42, 0x47, 0x48, 0x2d, 0x56, 0x2b, 0x2c, 0x31, 0x2a, 0x2f, 0x30, 0x55, 0x5e,
                            0x53, 0x54, 0x59, 0x52, 0x57, 0x58, 0x7d, 0x26, 0x7b, 0x7c, 0x81, 0x7a, 0x7f, 0x80, 0x25, 0x2e, 0x23, 0x24, 0x29, 0x22, 0x27, 0x28, 0x8d, 0x36, 0x8b, 0x8c, 0x11};

//////------------------------------------
//////------------------------------------
////// Setups

class BLEServerCallback : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    Serial.println("BLE connected");
    deviceConnected = true;

    if (bleLockForced == 0)
    {
      if (settings.getS1F().Bluetooth_lock_mode == 1)
      {
        bleLockStatus = false;
        Serial.println(" ==> device connected ==> UNLOCK decision");
        Serial.println("-------------------------------------");
      }
      if (settings.getS1F().Bluetooth_lock_mode == 2)
      {
        bleLockStatus = false;
        Serial.println(" ==> device connected ==> UNLOCK decision");
        Serial.println("-------------------------------------");
      }
    }

    // notify of current modes / values (for value not uptate by LCD)
    pCharacteristicSpeedLimiter->setValue((uint8_t *)&speedLimiter, 1);
    pCharacteristicSpeedLimiter->notify();
  };

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("BLE disonnected");
    deviceConnected = false;

    if (bleLockForced == 0)
    {
      if (settings.getS1F().Bluetooth_lock_mode == 1)
      {
        bleLockStatus = true;
        Serial.println(" ==> device disconnected ==> LOCK decision");
        Serial.println("-------------------------------------");
      }
      if (settings.getS1F().Bluetooth_lock_mode == 2)
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
  /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice)
  {
    //Serial.print("BLE Advertised Device found: ");
    //Serial.println(advertisedDevice.toString().c_str());
    /*
    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.getServiceUUID().equals(SERVICE_UUID)) {

      //
      Serial.print("Found our device!  address: ");
      advertisedDevice.getScan()->stop();

      pServerAddress = new BLEAddress(advertisedDevice.getAddress());
      doConnect = true;

    } // Found our server
    */
  } // onResult
};  // MyAdvertisedDeviceCallbacks

class BLECharacteristicCallback : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    if (pCharacteristic->getUUID().toString() == MODE_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();
      modeOrder = rxValue[0];

      char print_buffer[500];
      sprintf(print_buffer, "%02x", modeOrder);
      Serial.print("Write mode : ");
      Serial.println(print_buffer);
    }
    if (pCharacteristic->getUUID().toString() == BRAKE_STATUS_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();
      breakeSentOrder = rxValue[0];

      char print_buffer[500];
      sprintf(print_buffer, "%02x", breakeSentOrder);
      Serial.print("Write breakeSentOrder : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == SETTINGS1_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();

      for (int i = 0; i < rxValue.length(); i++)
      {
        settings.getS1B()[i] = rxValue[i];
      }

      //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

      Serial.print("Settings1 len : ");
      Serial.println(rxValue.length());
      Serial.print("Settings1 size : ");
      Serial.println(rxValue.size());

      Serial.print("Settings1 : ");
      for (int i = 0; i < rxValue.length(); i++)
      {
        char print_buffer[5];
        sprintf(print_buffer, "%02x ", rxValue[i]);
        Serial.print(print_buffer);
      }
      Serial.println("");

      settings.displaySettings1();
    }
    else if (pCharacteristic->getUUID().toString() == SETTINGS2_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();

      for (int i = 0; i < rxValue.length(); i++)
      {
        settings.getS2B()[i] = rxValue[i];
      }

      //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

      Serial.print("Settings2 len : ");
      Serial.println(rxValue.length());
      Serial.print("Settings2 size : ");
      Serial.println(rxValue.size());

      Serial.print("Settings2 : ");
      for (int i = 0; i < rxValue.length(); i++)
      {
        char print_buffer[5];
        sprintf(print_buffer, "%02x ", rxValue[i]);
        Serial.print(print_buffer);
      }
      Serial.println("");

      settings.displaySettings2();
    }
    else if (pCharacteristic->getUUID().toString() == SETTINGS3_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();

      for (int i = 0; i < rxValue.length(); i++)
      {
        settings.getS3B()[i] = rxValue[i];
      }

      //memcpy(&settings1.buffer, &rxValue, sizeof(settings1.buffer));

      Serial.print("Settings3 len : ");
      Serial.println(rxValue.length());
      Serial.print("Settings3 size : ");
      Serial.println(rxValue.size());

      Serial.print("Settings3 : ");
      for (int i = 0; i < rxValue.length(); i++)
      {
        char print_buffer[5];
        sprintf(print_buffer, "%02x ", rxValue[i]);
        Serial.print(print_buffer);
      }
      Serial.println("");

      settings.displaySettings3();

      settings.saveSettings(EEPROM_ADDRESS_SETTINGS);
    }
    else if (pCharacteristic->getUUID().toString() == SPEED_LIMITER_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();
      speedLimiter = rxValue[0];

      char print_buffer[500];
      sprintf(print_buffer, "%02x", speedLimiter);
      Serial.print("Write speedLimiter : ");
      Serial.println(print_buffer);

      // notify of current value
      pCharacteristicSpeedLimiter->setValue((uint8_t *)&speedLimiter, 1);
      pCharacteristicSpeedLimiter->notify();
    }
    else if (pCharacteristic->getUUID().toString() == ECO_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();
      ecoOrder = rxValue[0];

      char print_buffer[500];
      sprintf(print_buffer, "%02x", ecoOrder);
      Serial.print("Write eco : ");
      Serial.println(print_buffer);

      // notify of current value
      pCharacteristicEco->setValue((uint8_t *)&ecoOrder, 1);
      pCharacteristicEco->notify();
    }
    else if (pCharacteristic->getUUID().toString() == ACCEL_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();
      accelOrder = rxValue[0];

      char print_buffer[500];
      sprintf(print_buffer, "%02x", accelOrder);
      Serial.print("Write accel : ");
      Serial.println(print_buffer);

      // notify of current value
      pCharacteristicAccel->setValue((uint8_t *)&accelOrder, 1);
      pCharacteristicAccel->notify();
    }
    else if (pCharacteristic->getUUID().toString() == CURRENT_CALIB_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();
      currentCalibOrder = rxValue[0];

      char print_buffer[500];
      sprintf(print_buffer, "%02x", currentCalibOrder);
      Serial.print("Write currentCalibOrder : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == BTLOCK_STATUS_CHARACTERISTIC_UUID)
    {
      std::string rxValue = pCharacteristic->getValue();
      bleLockForced = rxValue[3];

      char print_buffer[500];
      sprintf(print_buffer, "%02x", bleLockForced);
      Serial.print("Write bleLockForced : ");
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
      inOtaMode = true;
    }
    else if (pCharacteristic->getUUID().toString() == FAST_UPDATE_CHARACTERISTIC_UUID)
    {
      Serial.println("Write FAST_UPDATE_CHARACTERISTIC_UUID");

      std::string rxValue = pCharacteristic->getValue();
      fastUpdate = rxValue[0];

      Serial.print("Fast update = ");
      Serial.println(fastUpdate);
    }
  }

  void onRead(BLECharacteristic *pCharacteristic)
  {

    if (pCharacteristic->getUUID().toString() == MODE_CHARACTERISTIC_UUID)
    {
      pCharacteristicMode->setValue((uint8_t *)&modeOrder, 1);

      char print_buffer[500];
      sprintf(print_buffer, "%02x", modeOrder);
      Serial.print("Read mode : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == SPEED_CHARACTERISTIC_UUID)
    {
      pCharacteristicSpeed->setValue((uint8_t *)&speedCurrent, 1);

      char print_buffer[500];
      sprintf(print_buffer, "%02x", speedCurrent);
      Serial.print("Read speed : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == BRAKE_STATUS_CHARACTERISTIC_UUID)
    {
      pCharacteristicBrakeSentOrder->setValue((uint8_t *)&breakeSentOrder, 1);

      char print_buffer[500];
      sprintf(print_buffer, "%02x", breakeSentOrder);
      Serial.print("Read breakeSentOrder : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == VOLTAGE_STATUS_CHARACTERISTIC_UUID)
    {
      uint32_t voltage = voltageFilter.getMean();
      pCharacteristicVoltageStatus->setValue((uint8_t *)&voltage, 4);

      char print_buffer[500];
      sprintf(print_buffer, "%02f", voltage / 1000.0);
      Serial.print("Read voltage : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == CURRENT_STATUS_CHARACTERISTIC_UUID)
    {
      int32_t current = currentFilter.getMean();
      pCharacteristicCurrentStatus->setValue((uint8_t *)&current, 4);

      char print_buffer[500];
      sprintf(print_buffer, "%02d", current);
      Serial.print("Read current : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == POWER_STATUS_CHARACTERISTIC_UUID)
    {
      int32_t current = currentFilter.getMean();
      uint32_t voltage = voltageFilter.getMean();
      int32_t power = (current / 1000.0) * (voltage / 1000.0);
      pCharacteristicPowerStatus->setValue((uint8_t *)&power, 4);

      char print_buffer[500];
      sprintf(print_buffer, "%04d", power);
      Serial.print("Read power : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == BTLOCK_STATUS_CHARACTERISTIC_UUID)
    {

      notifyBleLock();
      /*
        byte value[4];
        value[0] = bleLockStatus;
        value[1] = blePicclyVisible;
        value[2] = blePicclyRSSI;
        value[3] = bleLockForced;
        pCharacteristicBtlockStatus->setValue((uint8_t *)&value, 4);
        */

      char print_buffer[500];
      sprintf(print_buffer, "%02x", bleLockStatus);
      Serial.print("Read bleLock : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == TEMPERATURE_STATUS_CHARACTERISTIC_UUID)
    {
      int32_t temp = currentTemperature * 1000.0;
      pCharacteristicTemperatureStatus->setValue((uint8_t *)&temp, 4);

      char print_buffer[500];
      sprintf(print_buffer, "%f", currentTemperature);
      Serial.print("Read currentTemperature : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == HUMIDITY_STATUS_CHARACTERISTIC_UUID)
    {
      int32_t temp = currentHumidity * 1000.0;
      pCharacteristicHumidityStatus->setValue((uint8_t *)&temp, 4);

      char print_buffer[500];
      sprintf(print_buffer, "%f", currentHumidity);
      Serial.print("Read currentHumidity : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == SPEED_LIMITER_CHARACTERISTIC_UUID)
    {
      pCharacteristicHumidityStatus->setValue((uint8_t *)&speedLimiter, 1);

      char print_buffer[500];
      sprintf(print_buffer, "%d", speedLimiter);
      Serial.print("Read speedLimiter : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == ECO_CHARACTERISTIC_UUID)
    {
      pCharacteristicEco->setValue((uint8_t *)&ecoOrder, 1);

      char print_buffer[500];
      sprintf(print_buffer, "%d", ecoOrder);
      Serial.print("Read eco : ");
      Serial.println(print_buffer);
    }
    else if (pCharacteristic->getUUID().toString() == ACCEL_CHARACTERISTIC_UUID)
    {
      pCharacteristicAccel->setValue((uint8_t *)&accelOrder, 1);

      char print_buffer[500];
      sprintf(print_buffer, "%d", accelOrder);
      Serial.print("Read accel : ");
      Serial.println(print_buffer);
    }
  }
};

void bleOnScanResults(BLEScanResults scanResults)
{
#if DEBUG_BLE_SCAN
  Serial.print("BLE Scan Device found: ");
  Serial.println(scanResults.getCount());
#endif

  bool newBlePicclyVisible = false;

  for (int i = 0; i < scanResults.getCount(); i++)
  {
    String name = scanResults.getDevice(i).getName().c_str();
    blePicclyRSSI = scanResults.getDevice(i).getRSSI();
    std::string address = scanResults.getDevice(i).getAddress().toString();
    String addressStr = address.c_str();

#if DEBUG_BLE_SCAN
    Serial.print("BLE device : ");
    Serial.print(name);
    Serial.print(" / adress : ");
    Serial.print(addressStr);
    Serial.print(" / name : ");
    Serial.print(name);
    Serial.print(" / rssi ");
    Serial.println(blePicclyRSSI);
#endif

    String addressPicclySettings = settings.getS2F().Beacon_Mac_Address;

    if (addressPicclySettings.equalsIgnoreCase(addressStr))
    {
      if (blePicclyRSSI < settings.getS1F().Beacon_range)
      {
#if DEBUG_BLE_SCAN
        Serial.print(" ==> PICC-LY found ... but too far away / RSSI = ");
        Serial.print(blePicclyRSSI);
        Serial.print(" / min RSSI required = ");
        Serial.print(settings.getS1F().Beacon_range);
        Serial.println(" ==> lock from scan");
#endif
        newBlePicclyVisible = false;
      }
      else
      {
#if DEBUG_BLE_SCAN
        Serial.print(" ==> PICC-LY found  / RSSI = ");
        Serial.print(blePicclyRSSI);
        Serial.print(" / min RSSI required = ");
        Serial.print(settings.getS1F().Beacon_range);
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
    if (settings.getS1F().Bluetooth_lock_mode == 2)
    {
      if ((!blePicclyVisible) && (!deviceConnected))
      {
        bleLockStatus = 1;

#if DEBUG_BLE_SCAN
        Serial.println(" ==> PICLLY not visible // smartphone not connected ==> LOCK decision");
        Serial.println("-------------------------------------");
#endif
      }
      else if ((!blePicclyVisible) && (deviceConnected))
      {
        bleLockStatus = 0;

#if DEBUG_BLE_SCAN
        Serial.println(" ==> PICLLY visible // smartphone connected ==> UNLOCK decision");
        Serial.println("-------------------------------------");
#endif
      }
      else
      {
      }
    }
    if (settings.getS1F().Bluetooth_lock_mode == 3)
    {
      if (!blePicclyVisible)
      {
        bleLockStatus = 1;

#if DEBUG_BLE_SCAN
        Serial.println(" ==> PICLLY not visible ==> LOCK decision");
        Serial.println("-------------------------------------");
#endif
      }
      else if (blePicclyVisible)
      {
        bleLockStatus = 0;

#if DEBUG_BLE_SCAN
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
  /*
    if ((!deviceConnected))
    {
      if (!blePicclyVisible)
      {
        bleLockStatus = true;
        Serial.println(" ==> no device connected and PICC-LY no found ==> LOCK decision");
      }
      else
      {
        bleLockStatus = false;
        Serial.println(" ==> no device connected and PICC-LY found ==> UNLOCK decision");
      }
    }
    */
}

void notifyBleLock()
{
  byte value[4];
  value[0] = bleLockStatus;
  value[1] = blePicclyVisible;
  value[2] = blePicclyRSSI;
  value[3] = bleLockForced;
  pCharacteristicBtlockStatus->setValue((uint8_t *)&value, 4);
  pCharacteristicBtlockStatus->notify();

#if DEBUG_BLE_NOTIFY
  Serial.print("notifyBleLock : bleLockStatus = ");
  Serial.print(bleLockStatus);
  Serial.print(" / blePicclyVisible = ");
  Serial.print(blePicclyVisible);
  Serial.print(" / blePicclyRSSI = ");
  Serial.print(blePicclyRSSI);

  Serial.print(" / bleLockForced = ");
  Serial.print(bleLockForced);
  Serial.println("");
#endif
}

void notifyBleLogs(char *txt)
{

  // notify of new log
  pCharacteristicLogs->setValue((uint8_t *)txt, strlen(txt));
  pCharacteristicLogs->notify();
}

void setupPins()
{

  pinMode(PIN_IN_OUT_DHT, INPUT_PULLUP);
  pinMode(PIN_IN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_IN_BUTTON2, INPUT_PULLUP);
  pinMode(PIN_IN_VOLTAGE, INPUT);
  pinMode(PIN_IN_CURRENT, INPUT);
  pinMode(PIN_IN_BRAKE, INPUT);
  pinMode(PIN_OUT_RELAY, OUTPUT);
  pinMode(PIN_OUT_BRAKE, OUTPUT);
  pinMode(PIN_OUT_LED_BUTTON1, OUTPUT);
  pinMode(PIN_OUT_LED_BUTTON2, OUTPUT);

  analogSetClockDiv(255); // 1338mS
}

void setupBLE()
{

  // Create the BLE Device
  Serial.println("init");
  BLEDevice::init("SmartLCD");
  BLEDevice::setMTU(BLE_MTU);

  int mtu = BLEDevice::getMTU();
  Serial.print("MTU : ");
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

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");

  // Start scanning
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new BLEAdvertisedDeviceCallback());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10, &bleOnScanResults, false);
}

void setupSerial()
{

  hwSerLcdToCntrl.begin(BAUD_RATE, SERIAL_8N1, PIN_SERIAL_LCD_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);

  hwSerCntrlToLcd.begin(BAUD_RATE, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_LCD);
}

void setupEPROMM()
{
  EEPROM.begin(EEPROM_SIZE);
}

void initDataWithSettings()
{
  speedLimiter = (settings.getS1F().Speed_limiter_at_startup == 1);
}

////// Setup
void setup()
{

  // Initialize the Serial (use only in setup codes)
  Serial.begin(115200);
  Serial.println(PSTR("\n\nsetup --- begin"));

  timeLastNotifyBle = millis();

  Serial.println(PSTR("   serial ..."));
  setupSerial();
  delay(10);

  Serial.println(PSTR("   BLE ..."));
  setupBLE();
  delay(10);

  Serial.println(PSTR("   pins ..."));
  setupPins();
  delay(10);

  Serial.println(PSTR("   eeprom ..."));
  setupEPROMM();
  restoreBleLockForced();

  // force locking
  if (bleLockForced == 1)
    bleLockStatus = 1;

  Serial.println(PSTR("   settings ..."));
  settings.restoreSettings(EEPROM_ADDRESS_SETTINGS);
  settings.displaySettings();

  Serial.println(PSTR("   init data with settings ..."));
  initDataWithSettings();

  // End off setup
  Serial.println("setup --- end");
}

//////------------------------------------
//////------------------------------------
////// EEPROM functions

void saveBleLockForced()
{
  Serial.print("save bleLockForced : ");
  Serial.print(sizeof(bleLockForced));
  Serial.println(" bytes");

  EEPROM.writeBytes(EEPROM_ADDRESS_BLE_LOCK_FORCED, &bleLockForced, sizeof(bleLockForced));
  EEPROM.commit();

  Serial.print("save bleLockForced value : ");
  Serial.println(bleLockForced);
}

void restoreBleLockForced()
{

  Serial.print("restore BleLockForced");
  Serial.println(sizeof(bleLockForced));

  EEPROM.readBytes(EEPROM_ADDRESS_BLE_LOCK_FORCED, &bleLockForced, sizeof(bleLockForced));

  Serial.print("restore bleLockForced value : ");
  Serial.println(bleLockForced);
}

//////------------------------------------
//////------------------------------------
////// Various functions

uint8_t getCheckSum(char *string)
{
  byte rtn = 0;

  for (byte i = 0; i < 14; i++)
  {
    rtn ^= string[i];
  }

  return rtn;
}

void displayFrame(int mode, char data_buffer[], byte checksum)
{

  char print_buffer[500];

  // for excel
  sprintf(print_buffer, "(%d) %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x / %02x",
          mode,
          data_buffer[0],
          data_buffer[1],
          data_buffer[2],
          data_buffer[3],
          data_buffer[4],
          data_buffer[5],
          data_buffer[6],
          data_buffer[7],
          data_buffer[8],
          data_buffer[9],
          data_buffer[10],
          data_buffer[11],
          data_buffer[12],
          data_buffer[13],
          data_buffer[14],
          checksum);

  Serial.println(print_buffer);
}

void displayDecodedFrame(int mode, char data_buffer[], byte checksum)
{

  char print_buffer[500];

  // for excel
  sprintf(print_buffer, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
          data_buffer[1],
          (data_buffer[4] - data_buffer[3]) & 0xff,
          (data_buffer[5] - data_buffer[3]) & 0xff,
          (data_buffer[7] - data_buffer[3]) & 0xff,
          (data_buffer[8] - data_buffer[3]) & 0xff,
          (data_buffer[9] - data_buffer[3]) & 0xff,
          (data_buffer[10] - data_buffer[3]) & 0xff,
          (data_buffer[11] - data_buffer[3]) & 0xff,
          (data_buffer[12] - data_buffer[3]) & 0xff,
          (data_buffer[13] - data_buffer[3]) & 0xff);

  Serial.println(print_buffer);
}

void notifyBleLogFrame(int mode, char data_buffer[], byte checksum)
{

  char print_buffer[500];

  // for excel
  sprintf(print_buffer, "(%d) %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x / %02x",
          mode,
          data_buffer[0],
          data_buffer[1],
          data_buffer[2],
          data_buffer[3],
          data_buffer[4],
          data_buffer[5],
          data_buffer[6],
          data_buffer[7],
          data_buffer[8],
          data_buffer[9],
          data_buffer[10],
          data_buffer[11],
          data_buffer[12],
          data_buffer[13],
          data_buffer[14],
          checksum);

  notifyBleLogs(print_buffer);
}

void displaySpeed()
{

  Serial.print("speedCurrent : ");
  Serial.print(speedCurrent);
  Serial.println("");
}

void displayBrake()
{
  Serial.print("Brake : ");
  Serial.println(brakeStatus);
}

void displayButton1()
{
  Serial.print("Button1 : ");
  Serial.println(button1Status);
}

void displayButton2()
{
  Serial.print("Button2 : ");
  Serial.println(button2Status);
}

void displayMode(char data_buffer[])
{

  uint32_t byteDiff = (data_buffer[5] - data_buffer[2]);
  uint8_t modeLcd = byteDiff & 0x03;
  uint8_t modeLcd2 = (byteDiff >> 2) & 0x1;
  uint8_t modeLcd3 = (byteDiff >> 3) & 0x1;
  uint8_t modeLcd4 = (byteDiff >> 4) & 0x1;
  uint8_t modeLcd5 = (byteDiff >> 5) & 0x1;
  uint8_t modeLcd6 = (byteDiff >> 6) & 0x1;
  uint8_t modeLcd7 = (byteDiff >> 7) & 0x1;

  char print_buffer[500];
  sprintf(print_buffer, "%02x %02x / %02x / %02x / %02x / %02x / %02x / %02x / %02x", data_buffer[2], data_buffer[5], modeLcd, modeLcd2, modeLcd3, modeLcd4, modeLcd5, modeLcd6, modeLcd7);

#if DEBUG_DISPLAY_MODE
  Serial.print("LCD mode : ");
  Serial.print(print_buffer);
  Serial.println("");
#endif
}

uint8_t modifyModeOld(char var, char data_buffer[])
{
  uint32_t byteDiff = (var - data_buffer[2]);
  uint8_t modeLcd = byteDiff & 0x03;
  uint8_t modeLcdMask = byteDiff & 0xfc;
  uint8_t newmodeLcd2 = modeOrder | modeLcdMask;
  uint32_t newmodeLcd3 = (newmodeLcd2 + data_buffer[2]) & 0xff;

  char print_buffer[500];
  /*
  sprintf(print_buffer, "%02x %02x / %s %02x / %s %02x / %s %02x / %s %02x  / %s %02x  / %s %02x ",
          data_buffer[2],
          var,
          "byteDiff",
          byteDiff,
          "lcd",
          modeLcd,
          "mask",
          modeLcdMask,
          "order",
          orderMode,
          "newmodeLcd2",
          newmodeLcd2,
          "newmodeLcd3",
          newmodeLcd3);
          */

  sprintf(print_buffer, "%s %02x / %s %02x",
          "lcd",
          modeLcd,
          "order",
          modeOrder);

  Serial.print("LCD mode : ");
  Serial.print(print_buffer);
  Serial.println("");

  return newmodeLcd3;
}

void getBrakeFromAnalog()
{
  brakeAnalogValue = analogRead(PIN_IN_BRAKE);
}

uint8_t getMode(char var, char data_buffer[])
{
  uint32_t byteDiff = (var - data_buffer[2]);
  uint8_t modeLcd = (byteDiff & 0x03) + 1;

  char print_buffer[500];
  sprintf(print_buffer, "%s %02x / %s %02x",
          "lcd",
          modeLcd,
          "order",
          modeOrder);

#if DEBUG_DISPLAY_MODE
  Serial.print("LCD mode : ");
  Serial.print(print_buffer);
  Serial.println("");
#endif

  return modeLcd;
}

uint8_t modifyMode(char var, char data_buffer[])
{
  uint8_t newmodeLcd3;

  uint32_t byteDiff = (var - data_buffer[2]);
  uint8_t modeLcd = (byteDiff & 0x03) + 1;

  // override Smartphone mode with LCD mode
  if (modeLcdOld != modeLcd)
  {
    modeOrder = modeLcd;
    modeLcdOld = modeLcd;

    // notify bluetooth
    pCharacteristicMode->setValue((uint8_t *)&modeOrder, 1);
    pCharacteristicMode->notify();
  }

  if (modeOrder == 1)
    newmodeLcd3 = modeLcd0[(uint8_t)(data_buffer[2])];
  else if (modeOrder == 2)
    newmodeLcd3 = modeLcd1[(uint8_t)(data_buffer[2])];
  else if (modeOrder == 3)
    newmodeLcd3 = modeLcd2[(uint8_t)(data_buffer[2])];
  else
    newmodeLcd3 = modeLcd2[(uint8_t)(data_buffer[2])];

  return newmodeLcd3;
}

uint8_t modifyPower(char var, char data_buffer[])
{
  uint8_t newPower;

  // lock escooter by reducing power to 5%
  if (bleLockStatus == true)
  {
    // REMINDER : never put bellow 5
    newPower = 5;
  }
  else if (speedLimiter == 1)
  {
    newPower = 37;

    /*
    Serial.print("Speed : ");
    Serial.print(speedCurrent);
    Serial.print(" / Power reduction : ");
    Serial.print(powerReduction);
    Serial.print(" / newPower : ");
    Serial.println(newPower);
    */
  }
  else
  {
    newPower = var;
  }

  return newPower;
}

uint8_t getBrakeFromLCD(char var, char data_buffer[])
{

  uint8_t brake = (var - data_buffer[3]) & 0x20;
  uint8_t brakeStatusNew = brake >> 5;

  brakeLcd = var;

  //uint8_t brakeStatusNew = brakeStatus;
  if ((brakeStatusNew == 1) && (brakeStatusOld == 0))
  {
    brakeStatus = brakeStatusNew;
    timeLastBrake = millis();

#if DEBUG_DISPLAY_BRAKE
    Serial.print("Brake pressed at : ");
    Serial.println(timeLastBrake);
#endif

    // notify bluetooth
    pCharacteristicBrakeSentOrder->setValue((uint8_t *)&breakeSentOrder, 1);
    pCharacteristicBrakeSentOrder->notify();
  }
  else if ((brakeStatusNew == 0) && (brakeStatusOld == 1))
  {
    brakeStatus = brakeStatusNew;

    // reset to min
    breakeSentOrder = settings.getS1F().Electric_brake_min_value;

#if DEBUG_DISPLAY_BRAKE
    Serial.print("Brake released at : ");
    Serial.println(millis());
#endif

    // notify bluetooth
    pCharacteristicBrakeSentOrder->setValue((uint8_t *)&breakeSentOrder, 1);
    pCharacteristicBrakeSentOrder->notify();
  }

  brakeStatusOld = brakeStatusNew;
  brakeStatus = brakeStatusNew;

  /*
  char print_buffer[500];
  sprintf(print_buffer, "%s %02x / %s %02x / %s %02x",
          "var",
          var,
          "data_buffer[3]",
          data_buffer[3],
          "brake",
          brake);

  Serial.print("Brake : ");
  Serial.print(print_buffer);
  Serial.println("");
*/

  return brake;
}

uint8_t modifyBrakeFromLCD(char var, char data_buffer[])
{

  uint32_t currentTime = millis();

  // init from LCD brake mode
  if (breakeSentOrder == -1)
    breakeSentOrder = var;

  // progressive mode
  if (settings.getS1F().Electric_brake_progressive_mode == 1)
  {
    if (brakeStatus == 1)
    {
      if (breakeSentOrder < settings.getS1F().Electric_brake_max_value)
      {
        if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 5)
        {
          breakeSentOrder = settings.getS1F().Electric_brake_min_value + 5;
        }
        else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 4)
        {
          breakeSentOrder = settings.getS1F().Electric_brake_min_value + 4;
        }
        else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 3)
        {
          breakeSentOrder = settings.getS1F().Electric_brake_min_value + 3;
        }
        else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 2)
        {
          breakeSentOrder = settings.getS1F().Electric_brake_min_value + 2;
        }
        else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 1)
        {
          breakeSentOrder = settings.getS1F().Electric_brake_min_value + 1;
        }
      }

      // notify bluetooth
      pCharacteristicBrakeSentOrder->setValue((uint8_t *)&breakeSentOrder, 1);
      pCharacteristicBrakeSentOrder->notify();
    }
    else
    // progressive brake enabled but brake released
    {
      breakeSentOrder = settings.getS1F().Electric_brake_min_value;
    }
  }
  else
  // progressive brake disabled
  {

    // notify brake LCD value
    if (breakeSentOrder != breakeSentOrderOld)
    {
      pCharacteristicBrakeSentOrder->setValue((uint8_t *)&breakeSentOrder, 1);
      pCharacteristicBrakeSentOrder->notify();
    }

    breakeSentOrderOld = brakeLcd;
  }

#if DEBUG_DISPLAY_BRAKE
  char print_buffer[500];
  sprintf(print_buffer, "%s %02x %s %02x %s %02x %s %d %s %d %s %d",
          "Brake Status : ",
          brakeStatus,
          " / breakeSentOrder  : ",
          breakeSentOrder,
          " / Current LCD brake  : ",
          var,
          " / timeLastBrake  : ",
          timeLastBrake,
          " / currentTime  : ",
          currentTime,
          " / timeDiff  : ",
          currentTime - timeLastBrake);

  Serial.println(print_buffer);
#endif

  return breakeSentOrder;
}

uint8_t modifyEco(char var, char data_buffer[])
{

  ecoLcd = var;
  var = ecoOrder;

  // override Smartphone mode with LCD mode
  if (ecoLcd != ecoLcdOld)
  {
    ecoOrder = ecoLcd;
    ecoLcdOld = ecoLcd;

    // notify bluetooth
    pCharacteristicEco->setValue((uint8_t *)&ecoOrder, 1);
    pCharacteristicEco->notify();
  }

#if DEBUG_DISPLAY_ECO
  char print_buffer[500];
  sprintf(print_buffer, "%s %02x",
          "Eco Status : ",
          var);

  Serial.println(print_buffer);
#endif

  return var;
}

uint8_t modifyAccel(char var, char data_buffer[])
{

  accelLcd = var;
  var = accelOrder;

  // override Smartphone mode with LCD mode
  if (accelLcd != accelLcdOld)
  {
    accelOrder = accelLcd;
    accelLcdOld = accelLcd;

    // notify bluetooth
    pCharacteristicAccel->setValue((uint8_t *)&accelOrder, 1);
    pCharacteristicAccel->notify();

    /*
    Serial.print("Accel ==> notify new accelOrder : ");
    Serial.println(accelOrder);
*/
  }

#if DEBUG_DISPLAY_ACCEL
  char print_buffer[500];
  sprintf(print_buffer, "%s %02x",
          "Accel Status : ",
          var);

  Serial.println(print_buffer);
#endif

  return var;
}

uint8_t getSpeed()
{
  uint8_t high1 = (data_buffer_cntrl_ori[7] - data_buffer_cntrl_ori[3]) & 0xff;
  uint8_t offset_regul = (data_buffer_cntrl_ori[5] - data_buffer_cntrl_ori[3]) & 0xff;
  uint8_t high2 = (high1 - offset_regul) & 0xff;
  uint8_t low = (data_buffer_cntrl_ori[8] - data_buffer_cntrl_ori[3]);

  int speed = (((int)high2 * 256) + (low));
  speed = speed * (settings.getS1F().Wheel_size / 10.0) / settings.getS1F().Motor_pole_number / 10.5;

  // eject error values
  if (speed > 150)
    return speedOld;

  return speed;
}

uint16_t generateSpeedRawValue(int fakeSpeed)
{
  uint16_t rawValue;
  rawValue = fakeSpeed / (settings.getS1F().Wheel_size / 10.0) * settings.getS1F().Motor_pole_number * 10.5;

  return rawValue;
}

uint8_t modifySpeed(char var, char data_buffer[], int speedFake)
{

  uint8_t isModified = 0;

  // LCD Speed adjustement
  if (settings.getS1F().LCD_Speed_adjustement != 0)
  {
    uint16_t rawSpeed = generateSpeedRawValue(speedCurrent * ((settings.getS1F().LCD_Speed_adjustement + 100) / 100.0));

    uint8_t low = rawSpeed & 0xff;
    uint8_t high = (rawSpeed >> 8) & 0xff;

    uint8_t regulatorOffset = data_buffer[5] - data_buffer[3];
    //uint8_t regulatorOffset = 0;

    data_buffer[7] = (((high + regulatorOffset) & 0xff) + data_buffer[3]) & 0xff;
    data_buffer[8] = (low + data_buffer[3]) & 0xff;

    isModified = 1;
  }

  return isModified;
}

int readHardSerial(int i, HardwareSerial *ss, int serialMode, char data_buffer_ori[], char data_buffer_mod[])
{

  byte var;

  if (ss->available() > 0)
  {

    var = ss->read();
    data_buffer_ori[i] = var;

    // LCD -> CNTRL
    if (serialMode == MODE_LCD_TO_CNTRL)
    {
      if ((var == 0xAA) && (begin_LcdToCntrl == 1))
      {
        begin_LcdToCntrl = 0;

        char log[] = PSTR(" ===> detect begin 0xAA / LCD_TO_ESP");
        Serial.println(log);
        notifyBleLogs(log);

        i = 0;
      }
    }
    else
    // CNTRL -> LCD
    {
      if ((var == 0x36) && (begin_CntrlToLcd == 1))
      {

        begin_CntrlToLcd = 0;

        char log[] = PSTR(" ===> detect begin 0x36 / CNTRL_TO_ESP");
        Serial.println(log);
        notifyBleLogs(log);

        i = 0;
      }
    }

    //---------------------
    // MODIFY LCD_TO_CNTRL
#if ALLOW_LCD_TO_CNTRL_MODIFICATIONS
    if ((!begin_LcdToCntrl) && (serialMode == MODE_LCD_TO_CNTRL))
    {
      if (i == 5)
      {

        var = modifyMode(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (i == 7)
      {
        var = modifyPower(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (i == 10)
      {
        //        var = modifyBrakeFromLCD(var, data_buffer_ori);
        var = modifyBrakeFromAnalog(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (i == 11)
      {
        var = modifyEco(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }

      if (i == 12)
      {
        var = modifyAccel(var, data_buffer_ori);
        isModified_LcdToCntrl = 1;
      }
    }
#endif

    //---------------------
    // MODIFY CNTRL_TO_LCD

    if ((!begin_CntrlToLcd) && (serialMode == MODE_CNTRL_TO_LCD))
    {
      if (i == 4)
      {
        //getBrakeFromLCD(var, data_buffer_ori);
      }

      // modify speed
      if (i == 8)
      {
#if ALLOW_CNTRL_TO_LCD_MODIFICATIONS
        speedCurrent = getSpeed();
        isModified_CntrlToLcd = modifySpeed(var, data_buffer_mod, speedFake);
#endif

        speedOld = speedCurrent;
      }
    }

    // GENERATE CHECKSUM
    if (i == 14)
    {

      if ((isModified_LcdToCntrl == 1) && (serialMode == MODE_LCD_TO_CNTRL))
      {
        var = getCheckSum(data_buffer_mod);

#if DEBUG_SERIAL_CHECKSUM_LCD_TO_CNTRL
        char print_buffer[500];
        sprintf(print_buffer, "%02x %02x",
                oldChecksum,
                var);

        Serial.print(" ===> modified checksum LCD_TO_CNTRL : ");
        Serial.println(print_buffer);
#endif

        isModified_LcdToCntrl = 0;
      }
      else if (((isModified_CntrlToLcd) == 1) && (i == 14) && (serialMode == MODE_CNTRL_TO_LCD))
      {
        var = getCheckSum(data_buffer_mod);

#if DEBUG_SERIAL_CHECKSUM_CNTRL_TO_LCD
        char print_buffer[500];
        sprintf(print_buffer, "%02x %02x",
                oldChecksum,
                var);

        Serial.print(" ===> modified checksum CNTRL_TO_LCD : ");
        Serial.println(print_buffer);
#endif

        isModified_CntrlToLcd = 0;
      }
    }

    data_buffer_mod[i] = var;

    ss->write(var);

    // display
    if (i == 14)
    {

      // Check original checksum
      uint8_t checksum = getCheckSum(data_buffer_ori);

      if (serialMode == MODE_CNTRL_TO_LCD)
      {
#if DEBUG_BLE_DISPLAY_FRAME
        notifyBleLogFrame(serialMode, data_buffer_mod, checksum);
#endif
#if DEBUG_DISPLAY_FRAME_CNTRL_TO_LCD
        displayFrame(serialMode, data_buffer_mod, checksum);
        displayFrame(serialMode, data_buffer_ori, checksum);
#endif
#if DEBUG_DISPLAY_DECODED_FRAME_CNTRL_TO_LCD
        displayDecodedFrame(serialMode, data_buffer_mod, checksum);
//        displayDecodedFrame(serialMode, data_buffer_ori, checksum);
#endif
#if DEBUG_DISPLAY_SPEED
        displaySpeed();
#endif
      }
      else
      {
#if DEBUG_BLE_DISPLAY_FRAME
        notifyBleLogFrame(serialMode, data_buffer_mod, checksum);
#endif
#if DEBUG_DISPLAY_FRAME_LCD_TO_CNTRL
        displayFrame(serialMode, data_buffer_mod, checksum);
#endif
#if DEBUG_DISPLAY_MODE
        displayMode(data_buffer_mod);
#endif
      }

      if (checksum != data_buffer_ori[14])
      {
        char log[] = "====> CHECKSUM error ==> reset";
        Serial.println(log);
        notifyBleLogs(log);

        if (serialMode == MODE_LCD_TO_CNTRL)
          begin_LcdToCntrl = 1;
        else
          begin_CntrlToLcd = 1;
      }

      i = 0;
    }
    else
    {
      i++;
    }
  }

  return i;
}

void processSerial()
{
  // read/write LCD -> CNTRL
  //  i_LcdToCntrl = readSoftSerial(i_LcdToCntrl, &swSerLcdToCntrl, MODE_LCD_TO_CNTRL, data_buffer_lcd_mod);
  i_LcdToCntrl = readHardSerial(i_LcdToCntrl, &hwSerLcdToCntrl, MODE_LCD_TO_CNTRL, data_buffer_lcd_ori, data_buffer_lcd_mod);

  //i_CntrlToLcd = readSoftSerial(i_CntrlToLcd, &swSerCntrlToLcd, MODE_CNTRL_TO_LCD, data_buffer_cntrl_mod);
  i_CntrlToLcd = readHardSerial(i_CntrlToLcd, &hwSerCntrlToLcd, MODE_CNTRL_TO_LCD, data_buffer_cntrl_ori, data_buffer_cntrl_mod);
}

void processBLE()
{
  // notify changed value
  if (deviceConnected)
  {

    uint16_t period = 500;
    if (fastUpdate)
      period = 100;

    if (millis() > timeLastNotifyBle + period)
    {

      pCharacteristicSpeed->setValue((uint8_t *)&speedCurrent, 1);
      pCharacteristicSpeed->notify();

#if DEBUG_BLE_NOTIFY
      Serial.print("Notify speed : ");
      Serial.println(speedCurrent);
#endif

      // notify bluetooth
      uint32_t voltage = voltageFilter.getMean();
      pCharacteristicVoltageStatus->setValue((uint8_t *)&voltage, 4);
      pCharacteristicVoltageStatus->notify();

#if DEBUG_BLE_NOTIFY
      Serial.print("Notify voltage : ");
      Serial.println(voltage);
#endif

      int32_t current = currentFilter.getMean();
      pCharacteristicCurrentStatus->setValue((uint8_t *)&current, 4);
      pCharacteristicCurrentStatus->notify();

#if DEBUG_BLE_NOTIFY
      Serial.print("Notify current : ");
      Serial.println(current);
#endif

      int32_t power = (current / 1000.0) * (voltage / 1000.0);
      if (power < 0)
        power = 0;
      pCharacteristicPowerStatus->setValue((uint8_t *)&power, 4);
      pCharacteristicPowerStatus->notify();

#if DEBUG_BLE_NOTIFY
      Serial.print("Notify power : ");
      Serial.println(power);
#endif

      notifyBleLock();

#if DEBUG_BLE_NOTIFY
      Serial.print("Notify bleLock : ");
      Serial.println(bleLock);
#endif

      timeLastNotifyBle = millis();
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

uint8_t modifyBrakeFromAnalog(char var, char data_buffer[])
{

  breakeSentOrder = settings.getS1F().Electric_brake_min_value;

  // alarm controler from braking
  if (brakeAnalogValue > ANALOG_BRAKE_MIN_VALUE)
  {
    digitalWrite(PIN_OUT_BRAKE, 1);
  }
  else
  {
    digitalWrite(PIN_OUT_BRAKE, 0);
  }

  if (settings.getS1F().Electric_brake_progressive_mode == 1)
  {

    uint32_t step = 5000;
    uint32_t diff = 0;
    uint32_t diffStep = 0;

    if (settings.getS1F().Electric_brake_max_value - settings.getS1F().Electric_brake_min_value > 0)
    {
      step = (ANALOG_BRAKE_MAX_VALUE - ANALOG_BRAKE_MIN_VALUE) / (settings.getS1F().Electric_brake_max_value - settings.getS1F().Electric_brake_min_value);
      if (brakeAnalogValue > ANALOG_BRAKE_MIN_VALUE)
      {
        if (brakeAnalogValue > ANALOG_BRAKE_MAX_VALUE)
          brakeAnalogValue = ANALOG_BRAKE_MAX_VALUE;

        diff = (brakeAnalogValue - ANALOG_BRAKE_MIN_VALUE);
        diffStep = diff / step;
        breakeSentOrder = diffStep + settings.getS1F().Electric_brake_min_value;
      }
    }

#if DEBUG_DISPLAY_ANALOG_BRAKE
    Serial.print("breakeSentOrderOld : ");
    Serial.print(breakeSentOrderOld);
#endif

    // notify brake LCD value
    if (breakeSentOrder != breakeSentOrderOld)
    {
      pCharacteristicBrakeSentOrder->setValue((uint8_t *)&breakeSentOrder, 1);
      pCharacteristicBrakeSentOrder->notify();
#if DEBUG_DISPLAY_ANALOG_BRAKE
      Serial.print(" / brake notify => ");
#endif
    }
    breakeSentOrderOld = breakeSentOrder;

#if DEBUG_DISPLAY_ANALOG_BRAKE
    Serial.print(" / brakeAnalogValue : ");
    Serial.print(brakeAnalogValue);
    Serial.print(" / step : ");
    Serial.print(step);
    Serial.print(" / diff : ");
    Serial.print(diff);
    Serial.print(" / diffStep : ");
    Serial.print(diffStep);
    Serial.print(" / breakeSentOrder : ");
    Serial.println(breakeSentOrder);
#endif
  }

  return breakeSentOrder;
}

void processButton1()
{
  button1Status = digitalRead(PIN_IN_BUTTON1);
  digitalWrite(PIN_OUT_LED_BUTTON1, button1Status);
}

void processButton2()
{
  button2Status = digitalRead(PIN_IN_BUTTON2);
  digitalWrite(PIN_OUT_LED_BUTTON2, button2Status);
}

void processDHT()
{
  static unsigned long measurement_timestamp = millis();

  /* Measure once every four seconds. */
  if (millis() - measurement_timestamp > 5000ul)
  {

    float temperature;
    float humidity;

    if (dht_sensor.measure(&temperature, &humidity) == true)
    {
      measurement_timestamp = millis();

#if DEBUG_DISPLAY_DHT
      Serial.print("T = ");
      Serial.print(temperature, 1);
      Serial.print(" deg. C, H = ");
      Serial.print(humidity, 1);
      Serial.println("%");
#endif

      currentTemperature = temperature;
      currentHumidity = humidity;

      uint32_t temp = temperature * 1000;
      pCharacteristicTemperatureStatus->setValue((uint8_t *)&temp, 4);
      pCharacteristicTemperatureStatus->notify();

      temp = humidity * 1000;
      pCharacteristicHumidityStatus->setValue((uint8_t *)&temp, 4);
      pCharacteristicHumidityStatus->notify();
    }
  }
}

void processVoltage()
{

  voltageStatus = analogRead(PIN_IN_VOLTAGE);

  // eject false reading
  if (voltageStatus == 4095)
  {
    Serial.print("Voltage read : ");
    Serial.print(voltageStatus);
    Serial.println(" => eject ");

#if DEBUG_BLE_DISPLAY_VOLTAGE
    char print_buffer[500];
    sprintf(print_buffer, "Voltage read 4095 ==> eject");
    notifyBleLogs(print_buffer);
#endif

    return;
  }
  if (voltageStatus < 900)
  {
    Serial.print("Voltage read : ");
    Serial.print(voltageStatus);
    Serial.println(" => eject ");

#if DEBUG_BLE_DISPLAY_VOLTAGE
    char print_buffer[500];
    sprintf(print_buffer, "Voltage read <900 ==> eject");
    notifyBleLogs(print_buffer);
#endif

    return;
  }

  voltageInMilliVolts = (voltageStatus * 1000.0) / ANALOG_TO_VOLTS;

  //double correctedValue = -0.000000000000016 * pow(voltageStatus, 4) + 0.000000000118171 * pow(voltageStatus, 3) - 0.000000301211691 * pow(voltageStatus, 2) + 0.001109019271794 * voltageStatus + 0.034143524634089;
  //voltageInMilliVolts = correctedValue * 25.27 * 1000;

  voltageFilter.in(voltageInMilliVolts);
  voltageRawFilter.in(voltageStatus);

#if DEBUG_DISPLAY_VOLTAGE
  Serial.print("Voltage read : ");
  Serial.print(voltageStatus);
  Serial.print(" / in voltage mean : ");
  Serial.print(voltageRawFilter.getMean());
  Serial.print(" / in volts : ");
  Serial.print(voltageInMilliVolts / 1000.0);
  Serial.print(" / in volts mean : ");
  Serial.print(voltageFilter.getMean() / 1000.0);
  Serial.print(" / iloop : ");
  Serial.println(i_loop);

  /*
  Serial.print(" / in correctedValue : ");
  Serial.print(correctedValue); 
  Serial.print(" / in volts2 : ");
  Serial.println(correctedValue * 25.27); 
  */
#endif
#if DEBUG_BLE_DISPLAY_VOLTAGE
  char print_buffer[500];
  sprintf(print_buffer, "Voltage / read : %d / mean : %d / mean volts : %0.1f ", voltageStatus, voltageRawFilter.getMean(), voltageFilter.getMean() / 1000.0);
  notifyBleLogs(print_buffer);
#endif
}

void processCurrent()
{
  int curerntRead = analogRead(PIN_IN_CURRENT);
  int currentInMillamps = (curerntRead - currentFilterInit.getMean()) * 1000 / ANALOG_TO_CURRENT;

  // current rest value
  currentFilter.in(currentInMillamps);
  if ((speedCurrent == 0) && (currentCalibOrder == 1))
  {

    currentFilterInit.in(curerntRead);

    iCurrentCalibOrder++;
    if (iCurrentCalibOrder > NB_CURRENT_CALIB)
    {
      iCurrentCalibOrder = 0;
      currentCalibOrder = 0;
    }

#if DEBUG_DISPLAY_CURRENT
    Serial.print("Current calibration ... ");
#endif
  }

#if DEBUG_DISPLAY_CURRENT
  Serial.print("Current read : ");
  Serial.print(curerntRead);
  Serial.print(" / currentFilterInit mean : ");
  Serial.print(currentFilterInit.getMean());
  Serial.print(" / in amperes : ");
  Serial.println(currentInMillamps / 1000.0);
#endif
}

//////------------------------------------
//////------------------------------------
////// Main loop

void loop()
{

  processSerial();
  processBLE();

  processButton1();
#if DEBUG_DISPLAY_BUTTON1
  displayButton1();
#endif

  processButton2();
#if DEBUG_DISPLAY_BUTTON2
  displayButton2();
#endif

  if (i_loop % 100 == 0)
  {
    processVoltage();
  }

  if (i_loop % 10 == 2)
  {
    //modifyBrakeFromAnalog();
    //displayBrake();
    getBrakeFromAnalog();
  }

  if (i_loop % 10 == 6)
  {
    processCurrent();
  }

  // keep it fast (/100 not working)
  if (i_loop % 5 == 1)
  {
    processDHT();
  }

  // handle OTA
  if (inOtaMode)
  {
    BLEDevice::deinit(true);
    OTA_loop();
  }

  // Give a time for ESP
  delay(1);
  i_loop++;

  timeLoop = millis();
}

/////////// End