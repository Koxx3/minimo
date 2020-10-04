//////////////////////////////////////////
// TODO : current loop
// TODO : speed loop
// TODO : speed adjustment
// TODO : LCD error indicators
// TODO : buttons management
// TODO : BT pin code
// TODO : mode Z
// TODO : brake disable with high voltage
// TODO : auto mode shift on low battery
//////////////////////////////////////////

//////------------------------------------
////// Inludes

#include <Arduino.h>
#include <EEPROM.h>

#include "main.h"
#include "OTA_softap.h"
#include "MedianFilter.h"
#include "dht_nonblocking.h"
#include "Settings.h"
#include "BluetoothHandler.h"
#include "SharedData.h"
#include "debug.h"

//////------------------------------------
////// Defines

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

//////------------------------------------
////// Variables

// Time
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

SharedData shrd;

int i_loop = 0;

uint32_t iCurrentCalibOrder = 0;

uint16_t brakeAnalogValue = 0;

uint16_t voltageStatus = 0;
uint32_t voltageInMilliVolts = 0;

int i_LcdToCntrl = 0;
int i_CntrlToLcd = 0;
int begin_LcdToCntrl = 1;
int begin_CntrlToLcd = 1;

int isModified_LcdToCntrl = 0;
int isModified_CntrlToLcd = 0;

MedianFilter voltageFilter(100, 30000);
MedianFilter voltageRawFilter(100, 2000);
MedianFilter currentFilter(100, 1830);
MedianFilter currentFilterInit(100, 1830);

Settings settings;

BluetoothHandler blh;

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
  shrd.speedLimiter = (settings.getS1F().Speed_limiter_at_startup == 1);
}

////// Setup
void setup()
{

  // Initialize the Serial (use only in setup codes)
  Serial.begin(115200);
  Serial.println(PSTR("\n\nsetup --- begin"));

  delay(1000);

  shrd.timeLastNotifyBle = millis();

  Serial.println(PSTR("   serial ..."));
  setupSerial();
  delay(10);

  Serial.println(PSTR("   BLE ..."));
  //setupBLE();
  blh.init();
  blh.setSharedData(&shrd);

  delay(10);

  Serial.println(PSTR("   pins ..."));
  setupPins();
  delay(10);

  Serial.println(PSTR("   eeprom ..."));
  setupEPROMM();
  restoreBleLockForced();

  // force BLE lock mode
  blh.forceBleLock();

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
  Serial.print(sizeof(blh.bleLockForced));
  Serial.println(" bytes");

  EEPROM.writeBytes(EEPROM_ADDRESS_BLE_LOCK_FORCED, &blh.bleLockForced, sizeof(blh.bleLockForced));
  EEPROM.commit();

  Serial.print("save bleLockForced value : ");
  Serial.println(blh.bleLockForced);
}

void restoreBleLockForced()
{

  Serial.print("restore BleLockForced");
  Serial.println(sizeof(blh.bleLockForced));

  EEPROM.readBytes(EEPROM_ADDRESS_BLE_LOCK_FORCED, &blh.bleLockForced, sizeof(blh.bleLockForced));

  Serial.print("restore bleLockForced value : ");
  Serial.println(blh.bleLockForced);
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

  blh.notifyBleLogs(print_buffer);
}

void displaySpeed()
{

  Serial.print("speedCurrent : ");
  Serial.print(shrd.speedCurrent);
  Serial.println("");
}

void displayBrake()
{
  Serial.print("Brake : ");
  Serial.println(shrd.brakeStatus);
}

void displayButton1()
{
  Serial.print("Button1 : ");
  Serial.println(shrd.button1Status);
}

void displayButton2()
{
  Serial.print("Button2 : ");
  Serial.println(shrd.button2Status);
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
  uint8_t newmodeLcd2 = shrd.modeOrder | modeLcdMask;
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
          shrd.modeOrder);

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
          shrd.modeOrder);

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
  if (shrd.modeLcdOld != modeLcd)
  {
    shrd.modeOrder = modeLcd;
    shrd.modeLcdOld = modeLcd;

    // notify bluetooth
    blh.notifyModeOrder(shrd.modeOrder);
  }

  if (shrd.modeOrder == 1)
    newmodeLcd3 = modeLcd0[(uint8_t)(data_buffer[2])];
  else if (shrd.modeOrder == 2)
    newmodeLcd3 = modeLcd1[(uint8_t)(data_buffer[2])];
  else if (shrd.modeOrder == 3)
    newmodeLcd3 = modeLcd2[(uint8_t)(data_buffer[2])];
  else
    newmodeLcd3 = modeLcd2[(uint8_t)(data_buffer[2])];

  return newmodeLcd3;
}

uint8_t modifyPower(char var, char data_buffer[])
{
  uint8_t newPower;

  // lock escooter by reducing power to 5%
  if (blh.bleLockStatus == true)
  {
    // REMINDER : never put bellow 5
    newPower = 5;
  }
  else if (shrd.speedLimiter == 1)
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

  shrd.brakeLcd = var;

  //uint8_t brakeStatusNew = brakeStatus;
  if ((brakeStatusNew == 1) && (shrd.brakeStatusOld == 0))
  {
    shrd.brakeStatus = brakeStatusNew;
    timeLastBrake = millis();

#if DEBUG_DISPLAY_BRAKE
    Serial.print("Brake pressed at : ");
    Serial.println(timeLastBrake);
#endif

    // notify bluetooth
    blh.notifyBreakeSentOrder(shrd.breakeSentOrder);
  }
  else if ((brakeStatusNew == 0) && (shrd.brakeStatusOld == 1))
  {
    shrd.brakeStatus = brakeStatusNew;

    // reset to min
    shrd.breakeSentOrder = settings.getS1F().Electric_brake_min_value;

#if DEBUG_DISPLAY_BRAKE
    Serial.print("Brake released at : ");
    Serial.println(millis());
#endif

    // notify bluetooth
    blh.notifyBreakeSentOrder(shrd.breakeSentOrder);
  }

  shrd.brakeStatusOld = brakeStatusNew;
  shrd.brakeStatus = brakeStatusNew;

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
  if (shrd.breakeSentOrder == -1)
    shrd.breakeSentOrder = var;

  // progressive mode
  if (settings.getS1F().Electric_brake_progressive_mode == 1)
  {
    if (shrd.brakeStatus == 1)
    {
      if (shrd.breakeSentOrder < settings.getS1F().Electric_brake_max_value)
      {
        if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 5)
        {
          shrd.breakeSentOrder = settings.getS1F().Electric_brake_min_value + 5;
        }
        else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 4)
        {
          shrd.breakeSentOrder = settings.getS1F().Electric_brake_min_value + 4;
        }
        else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 3)
        {
          shrd.breakeSentOrder = settings.getS1F().Electric_brake_min_value + 3;
        }
        else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 2)
        {
          shrd.breakeSentOrder = settings.getS1F().Electric_brake_min_value + 2;
        }
        else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 1)
        {
          shrd.breakeSentOrder = settings.getS1F().Electric_brake_min_value + 1;
        }
      }

      // notify bluetooth
      blh.notifyBreakeSentOrder(shrd.breakeSentOrder);
    }
    else
    // progressive brake enabled but brake released
    {
      shrd.breakeSentOrder = settings.getS1F().Electric_brake_min_value;
    }
  }
  else
  // progressive brake disabled
  {

    // notify brake LCD value
    if (shrd.breakeSentOrder != shrd.breakeSentOrderOld)
    {
      // notify bluetooth
      blh.notifyBreakeSentOrder(shrd.breakeSentOrder);
    }

    shrd.breakeSentOrderOld = shrd.breakeSentOrder;
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

  return shrd.breakeSentOrder;
}

uint8_t modifyEco(char var, char data_buffer[])
{

  shrd.ecoLcd = var;
  var = shrd.ecoOrder;

  // override Smartphone mode with LCD mode
  if (shrd.ecoLcd != shrd.ecoLcdOld)
  {
    shrd.ecoOrder = shrd.ecoLcd;
    shrd.ecoLcdOld = shrd.ecoLcd;

    // notify bluetooth
    blh.notifyEcoOrder(shrd.ecoOrder);
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

  shrd.accelLcd = var;
  var = shrd.accelOrder;

  // override Smartphone mode with LCD mode
  if (shrd.accelLcd != shrd.accelLcdOld)
  {
    shrd.accelOrder = shrd.accelLcd;
    shrd.accelLcdOld = shrd.accelLcd;

    // notify bluetooth
    blh.notifyAccelOrder(shrd.accelOrder);

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
    return shrd.speedOld;

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
    uint16_t rawSpeed = generateSpeedRawValue(shrd.speedCurrent * ((settings.getS1F().LCD_Speed_adjustement + 100) / 100.0));

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
        blh.notifyBleLogs(log);

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
        blh.notifyBleLogs(log);

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
        shrd.speedCurrent = getSpeed();
        isModified_CntrlToLcd = modifySpeed(var, data_buffer_mod, shrd.speedFake);
#endif

        shrd.speedOld = shrd.speedCurrent;
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
        blh.notifyBleLogs(log);

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

uint8_t modifyBrakeFromAnalog(char var, char data_buffer[])
{

  shrd.breakeSentOrder = settings.getS1F().Electric_brake_min_value;

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
        shrd.breakeSentOrder = diffStep + settings.getS1F().Electric_brake_min_value;
      }
    }

#if DEBUG_DISPLAY_ANALOG_BRAKE
    Serial.print("breakeSentOrderOld : ");
    Serial.print(breakeSentOrderOld);
#endif

    // notify brake LCD value
    if (shrd.breakeSentOrder != shrd.breakeSentOrderOld)
    {
      blh.notifyBreakeSentOrder(shrd.breakeSentOrder);
#if DEBUG_DISPLAY_ANALOG_BRAKE
      Serial.print(" / brake notify => ");
#endif
    }
    shrd.breakeSentOrderOld = shrd.breakeSentOrder;

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

  return shrd.breakeSentOrder;
}

void processButton1()
{
  shrd.button1Status = digitalRead(PIN_IN_BUTTON1);
  digitalWrite(PIN_OUT_LED_BUTTON1, shrd.button1Status);
}

void processButton2()
{
  shrd.button2Status = digitalRead(PIN_IN_BUTTON2);
  digitalWrite(PIN_OUT_LED_BUTTON2, shrd.button2Status);
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

      shrd.currentTemperature = temperature;
      shrd.currentHumidity = humidity;

      uint32_t temp = temperature * 1000;
      blh.notifyTemperatureStatus(temp);

      temp = humidity * 1000;
      blh.notifyHumidityStatus(temp);
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
    blh.notifyBleLogs(print_buffer);
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
    blh.notifyBleLogs(print_buffer);
#endif

    return;
  }

  voltageInMilliVolts = (voltageStatus * 1000.0) / ANALOG_TO_VOLTS;

  //double correctedValue = -0.000000000000016 * pow(voltageStatus, 4) + 0.000000000118171 * pow(voltageStatus, 3) - 0.000000301211691 * pow(voltageStatus, 2) + 0.001109019271794 * voltageStatus + 0.034143524634089;
  //voltageInMilliVolts = correctedValue * 25.27 * 1000;

  voltageFilter.in(voltageInMilliVolts);
  voltageRawFilter.in(voltageStatus);
  shrd.voltageFilterMean = voltageFilter.getMean();

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
  blh.notifyBleLogs(print_buffer);
#endif
}

void processCurrent()
{
  int curerntRead = analogRead(PIN_IN_CURRENT);
  int currentInMillamps = (curerntRead - currentFilterInit.getMean()) * 1000 / ANALOG_TO_CURRENT;

  // current rest value
  currentFilter.in(currentInMillamps);
  shrd.currentFilterMean = currentFilter.getMean();

  if ((shrd.speedCurrent == 0) && (shrd.currentCalibOrder == 1))
  {

    currentFilterInit.in(curerntRead);

    iCurrentCalibOrder++;
    if (iCurrentCalibOrder > NB_CURRENT_CALIB)
    {
      iCurrentCalibOrder = 0;
      shrd.currentCalibOrder = 0;
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
  blh.processBLE();

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
  if (shrd.inOtaMode)
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