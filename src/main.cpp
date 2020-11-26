//////////////////////////////////////////
// TODO : check button long press processing
// TODO : current loop
// TODO : LCD error indicators
// TODO : mode Z button/settings
// TODO : auto mode shift on low battery
// TODO : original regulator perturbation
// TODO : exponential throttle
// BUG : mode Z / android
// BUG : push button make brake analog read wrong
//////////////////////////////////////////

//////------------------------------------
////// Inludes

#include <Arduino.h>
#include <EEPROM.h>
#include "main.h"
#include "OTA_wifi.h"
#include "MedianFilter.h"
#include "dht_nonblocking.h"
#include "Settings.h"
#include "BluetoothHandler.h"
#include "SharedData.h"
#include "debug.h"
#include "OneButton.h"
#include "EEPROM_storage.h"

#include "app_version.h"

#include <PID_v1.h>

#include "TFT/tft_main.h"

//////------------------------------------
////// Defines

#define TFT_ENABLED 1

#define DEBUG_ESP_HTTP_UPDATE 1

#define ALLOW_LCD_TO_CNTRL_MODIFICATIONS true
#define ALLOW_CNTRL_TO_LCD_MODIFICATIONS true

#define PIN_SERIAL_ESP_TO_LCD 26
#define PIN_SERIAL_ESP_TO_CNTRL 27
#define PIN_SERIAL_LCD_TO_ESP 25
#define PIN_SERIAL_CNTRL_TO_ESP 14
//#define PIN_OUT_RELAY xx
#define PIN_IN_VOLTAGE 32
#define PIN_IN_CURRENT 35
#define PIN_IN_BUTTON1 22
#define PIN_IN_BUTTON2 15 // PB was TX
#define PIN_OUT_LED_BUTTON1 3
#define PIN_OUT_LED_BUTTON2 21
#define PIN_OUT_BRAKE 13
#define PIN_IN_OUT_DHT 12
#define PIN_IN_ABRAKE 34
//#define PIN_IN_DBRAKE 4

#define MODE_LCD_TO_CNTRL 0
#define MODE_CNTRL_TO_LCD 1
#define MODE_LCD_TO_CNTRL_START_BYTE 0xAA
#define MODE_CNTRL_TO_LCD_START_BYTE 0x36

#define DATA_BUFFER_SIZE 30
#define BAUD_RATE 1200

#define ANALOG_TO_VOLTS_A 0.0213
#define ANALOG_TO_VOLTS_B 5.4225

#define ANALOG_TO_CURRENT 35
#define NB_CURRENT_CALIB 200
#define NB_BRAKE_CALIB 100

#define BRAKE_TYPE_ANALOG 0
#if BRAKE_TYPE_ANALOG
#define ANALOG_BRAKE_MIN_ERR_VALUE 500
#define ANALOG_BRAKE_MAX_ERR_VALUE 3500
#else
#define ANALOG_BRAKE_MIN_ERR_VALUE 0
#define ANALOG_BRAKE_MAX_ERR_VALUE 4095
#endif
#define ANALOG_BRAKE_MIN_VALUE 920
#define ANALOG_BRAKE_MIN_OFFSET 100
#define ANALOG_BRAKE_MAX_VALUE 2300

#define BUTTON_LONG_PRESS_TICK 300

#define WATCHDOG_TIMEOUT 1000000 //time in ms to trigger the watchdog

//////------------------------------------
////// Variables

// Time
uint32_t timeLastBrake = 0;
uint32_t timeLoop = 0;

// Watchdog
hw_timer_t *timer = NULL;

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

OneButton button1(PIN_IN_BUTTON1, true, true);
OneButton button2(PIN_IN_BUTTON2, true, true);

// The actions I ca do...
typedef enum
{
  ACTION_OFF, // set LED "OFF".
  ACTION_ON   // set LED "ON"
} MyActions;

MyActions button1ClickStatus = ACTION_OFF;
uint32_t button1LpDuration = 0;
MyActions button2ClickStatus = ACTION_OFF;
uint32_t button2LpDuration = 0;
boolean button1LpProcessed = false;
boolean button2LpProcessed = false;

SharedData shrd;

int i_loop = 0;

uint32_t iBrakeCalibOrder = 0;

uint16_t voltageStatus = 0;
uint32_t voltageInMilliVolts = 0;

uint16_t brakeAnalogValue = 0;

int8_t modeOrderBeforeNitro = -1;

uint32_t distancePrevTime = 0;

int i_LcdToCntrl = 0;
int i_CntrlToLcd = 0;
int begin_LcdToCntrl = 1;
int begin_CntrlToLcd = 1;

int isModified_LcdToCntrl = 0;
int isModified_CntrlToLcd = 0;

MedianFilter voltageFilter(100, 2000);
MedianFilter voltageRawFilter(100, 2000);
MedianFilter currentFilter(200, 1830);
MedianFilter currentFilterInit(NB_CURRENT_CALIB, 1830);
MedianFilter brakeFilter(10 /* 20 */, 900);
MedianFilter brakeFilterInit(NB_BRAKE_CALIB, 900);

Settings settings;

BluetoothHandler blh;

double pidSetpoint, pidInput, pidOutput;
PID pidSpeed(&pidInput, &pidOutput, &pidSetpoint, shrd.speedPidKp, shrd.speedPidKi, shrd.speedPidKd, DIRECT);

//////------------------------------------

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
////// EEPROM functions

void saveBleLockForced()
{
  EEPROM.writeBytes(EEPROM_ADDRESS_BLE_LOCK_FORCED, &blh.bleLockForced, sizeof(blh.bleLockForced));
  EEPROM.commit();

  Serial.print("save bleLockForced value : ");
  Serial.println(blh.bleLockForced);
}

void restoreBleLockForced()
{
  EEPROM.readBytes(EEPROM_ADDRESS_BLE_LOCK_FORCED, &blh.bleLockForced, sizeof(blh.bleLockForced));

  Serial.print("restore bleLockForced value : ");
  Serial.println(blh.bleLockForced);
}

void saveBrakeMaxPressure()
{
  EEPROM.writeBytes(EEPROM_ADDRESS_BRAKE_MAX_PRESSURE, &shrd.brakeMaxPressureRaw, sizeof(shrd.brakeMaxPressureRaw));
  EEPROM.commit();

  Serial.print("save saveBrakeMaxPressure value : ");
  Serial.println(shrd.brakeMaxPressureRaw);
}

void restoreBrakeMaxPressure()
{
  EEPROM.readBytes(EEPROM_ADDRESS_BRAKE_MAX_PRESSURE, &shrd.brakeMaxPressureRaw, sizeof(shrd.brakeMaxPressureRaw));

  Serial.print("restore restoreBrakeMaxPressure value : ");
  Serial.println(shrd.brakeMaxPressureRaw);

  if (shrd.brakeMaxPressureRaw == -1)
    shrd.brakeMaxPressureRaw = ANALOG_BRAKE_MAX_VALUE;
}

void saveOdo()
{
  EEPROM.writeBytes(EEPROM_ADDRESS_ODO, &shrd.distanceOdo, sizeof(shrd.distanceOdo));
  EEPROM.commit();

  Serial.print("save saveOdo value : ");
  Serial.println(shrd.distanceOdo);
}

void restoreOdo()
{
  EEPROM.readBytes(EEPROM_ADDRESS_ODO, &shrd.distanceOdo, sizeof(shrd.distanceOdo));

  shrd.distanceOdoInFlash = shrd.distanceOdo;
  shrd.distanceOdoBoot = shrd.distanceOdo;

  Serial.print("restore restoreOdo value : ");
  Serial.println(shrd.distanceOdo);

  if (shrd.distanceOdo == -1)
    shrd.distanceOdo = 0;
}

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
  //pinMode(PIN_IN_DBRAKE, INPUT_PULLUP);
  //  pinMode(PIN_OUT_RELAY, OUTPUT);
  pinMode(PIN_OUT_BRAKE, OUTPUT);
  pinMode(PIN_OUT_LED_BUTTON1, OUTPUT);
  pinMode(PIN_OUT_LED_BUTTON2, OUTPUT);
}

/*
#include "soc/efuse_reg.h"
#include "esp_efuse.h" // for programming eFuse.

void setupEFuse()
{

  // INITIAL SETUP...BURN THE EFUSE IF NECESSARY FOR PROPER OPERATION.
  // Force the FLASH voltage regulator to 3.3v, disabling "MTDI" strapping check at startup
  if ((REG_READ(EFUSE_BLK0_RDATA4_REG) & EFUSE_RD_SDIO_TIEH) == 0)
  {
    esp_efuse_reset();
    REG_WRITE(EFUSE_BLK0_WDATA4_REG, EFUSE_RD_SDIO_TIEH);
    esp_efuse_burn_new_values();
  } //burning SDIO_TIEH -> sets SDIO voltage regulator to pass-thru 3.3v from VDD
  if ((REG_READ(EFUSE_BLK0_RDATA4_REG) & EFUSE_RD_XPD_SDIO_REG) == 0)
  {
    esp_efuse_reset();
    REG_WRITE(EFUSE_BLK0_WDATA4_REG, EFUSE_RD_XPD_SDIO_REG);
    esp_efuse_burn_new_values();
  } //burning SDIO_REG -> enables SDIO voltage regulator (otherwise user must hardwire power to SDIO)
  if ((REG_READ(EFUSE_BLK0_RDATA4_REG) & EFUSE_RD_SDIO_FORCE) == 0)
  {
    esp_efuse_reset();
    REG_WRITE(EFUSE_BLK0_WDATA4_REG, EFUSE_RD_SDIO_FORCE);
    esp_efuse_burn_new_values();
  } //burning SDIO_FORCE -> enables SDIO_REG and SDIO_TIEH
}
*/

void setupTft()
{
}

void setupDac()
{
  // call GENERAL CALL RESET
  // https://www.sparkfun.com/datasheets/BreakoutBoards/MCP4725.pdf
  // page 22
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

void setupPID()
{

  pidSetpoint = settings.getS1F().Speed_limiter_max_speed;

  //turn the PID on
  pidSpeed.SetMode(AUTOMATIC);
  //myPID.SetSampleTime(10);
  //myPID.SetOutputLimits(0,100);
}

void resetPid()
{

  pidSetpoint = settings.getS1F().Speed_limiter_max_speed;
  pidSpeed.SetTunings(shrd.speedPidKp, shrd.speedPidKi, shrd.speedPidKd);
  Serial.println("set PID tunings");
}

void initDataWithSettings()
{
  shrd.speedLimiter = (settings.getS1F().Speed_limiter_at_startup == 1);
}

void setupButtons()
{

  button1.attachClick(processButton1Click);
  button1.attachLongPressStart(processButton1LpStart);
  button1.attachDuringLongPress(processButton1LpDuring);
  button1.attachLongPressStop(processButton1LpStop);
  button1.setDebounceTicks(50);
  button1.setPressTicks(BUTTON_LONG_PRESS_TICK);

  button2.attachClick(processButton2Click);
  button2.attachLongPressStart(processButton2LpStart);
  button2.attachDuringLongPress(processButton2LpDuring);
  button2.attachLongPressStop(processButton2LpStop);
  button2.setDebounceTicks(50);
  button2.setPressTicks(BUTTON_LONG_PRESS_TICK);
}

void IRAM_ATTR triggerWatchdog()
{
  ets_printf("watchdog => reboot\n");
  esp_restart();
}

void setupWatchdog()
{
  timer = timerBegin(0, 80, true);                        //timer 0, div 80
  timerAttachInterrupt(timer, &triggerWatchdog, true);    //attach callback
  timerAlarmWrite(timer, WATCHDOG_TIMEOUT * 1000, false); //set time in us
  timerAlarmEnable(timer);                                //enable interrupt
}

void resetWatchdog()
{
  timerWrite(timer, 0); //reset timer (feed watchdog)
}

void disableWatchdog()
{
  timerAlarmDisable(timer);
}

////// Setup
void setup()
{

  // Initialize the Serial (use only in setup codes)
  Serial.begin(115200);
  Serial.println(PSTR("\n\nsetup --- begin"));

  Serial.print("version : ");
  Serial.println(Version);

  shrd.timeLastNotifyBle = millis();

  Serial.println(PSTR("   serial ..."));
  setupSerial();

  Serial.println(PSTR("   dac ..."));
  setupDac();

  Serial.println(PSTR("   eeprom ..."));
  setupEPROMM();
  restoreBleLockForced();
  restoreBrakeMaxPressure();
  restoreOdo();

  Serial.println(PSTR("   settings ..."));
  bool settingsStatusOk = settings.restoreSettings();
  if (!settingsStatusOk)
  {
    settings.initSettings();
  }
  settings.displaySettings();

  Serial.println(PSTR("   BLE ..."));
  //setupBLE();
  blh.init(&settings);
  blh.setSharedData(&shrd);

  Serial.println(PSTR("   pins ..."));
  setupPins();

  Serial.println(PSTR("   buttons ..."));
  setupButtons();

  Serial.println(PSTR("   PID ..."));
  setupPID();

#if TFT_ENABLED
  Serial.println(PSTR("   TFT ..."));
  tftSetup(&shrd, &settings);
#endif

  // force BLE lock mode
  blh.setBleLock(false);

  // setup shared datas
  shrd.currentCalibOrder = NB_CURRENT_CALIB;

  Serial.println(PSTR("   init data with settings ..."));
  initDataWithSettings();

  Serial.println(PSTR("   watchdog ..."));
  setupWatchdog();

  // End off setup
  Serial.println("setup --- end");
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
  sprintf(print_buffer, "(%d) %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x  %02x /  %2d /  %2d",
          mode,
          data_buffer[0],
          data_buffer[1],
          data_buffer[2],
          data_buffer[3],
          (data_buffer[4] - data_buffer[3]) & 0xff,
          (data_buffer[5] - data_buffer[3]) & 0xff,
          (data_buffer[7] - data_buffer[3]) & 0xff,
          (data_buffer[8] - data_buffer[3]) & 0xff,
          (data_buffer[9] - data_buffer[3]) & 0xff,
          (data_buffer[10] - data_buffer[3]) & 0xff,
          (data_buffer[11] - data_buffer[3]) & 0xff,
          (data_buffer[12] - data_buffer[3]) & 0xff,
          (data_buffer[13] - data_buffer[3]) & 0xff,
          data_buffer[14],
          ((data_buffer[10] - data_buffer[3]) & 0xff) >> 1,
          ((data_buffer[10] - data_buffer[3]) & 0xff) >> 2);

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
  Serial.print(" / speedOld : ");
  Serial.print(shrd.speedOld);
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
  Serial.println(button1ClickStatus);
}

void displayButton2()
{
  Serial.print("Button2 : ");
  Serial.println(button1ClickStatus);
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
  brakeAnalogValue = analogRead(PIN_IN_ABRAKE);
  shrd.brakeAnalogValue = brakeAnalogValue;

  if (settings.getS2F().Electric_brake_type == settings.LIST_Electric_brake_type_analog)
  {

    int brakeFilterMean = brakeFilter.getMean();
    int brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);

    // ignore out of range datas ... and notify
    if (brakeAnalogValue < ANALOG_BRAKE_MIN_ERR_VALUE)
    {
#if DEBUG_DISPLAY_ANALOG_BRAKE
      Serial.println("brake ANALOG_BRAKE_MIN_ERR_VALUE");
#endif
      char print_buffer[500];
      sprintf(print_buffer, "brake ANALOG_BRAKE_MIN_ERR_VALUE / f1 : %d / f2 : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d / init : %d",
              brakeFilterMean,
              brakeFilterMeanErr,
              brakeAnalogValue,
              shrd.brakeSentOrder,
              shrd.brakeSentOrderOld,
              shrd.brakeStatus,
              brakeFilterInit.getMean());
      blh.notifyBleLogs(print_buffer);
      Serial.println(print_buffer);
      return;
    }

    // ignore out of range datas ... and notify
    if (brakeAnalogValue > ANALOG_BRAKE_MAX_ERR_VALUE)
    {
#if DEBUG_DISPLAY_ANALOG_BRAKE
      Serial.println("brake ANALOG_BRAKE_MAX_ERR_VALUE");
      char print_buffer[500];
      sprintf(print_buffer, "brake ANALOG_BRAKE_MAX_ERR_VALUE / f1 : %d / f2 : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d / init : %d",
              brakeFilterMean,
              brakeFilterMeanErr,
              brakeAnalogValue,
              shrd.brakeSentOrder,
              shrd.brakeSentOrderOld,
              shrd.brakeStatus,
              brakeFilterInit.getMean());
      blh.notifyBleLogs(print_buffer);
      Serial.println(print_buffer);
#endif
      return;
    }

    if (brakeAnalogValue > shrd.brakeMaxPressureRaw)
      brakeAnalogValue = shrd.brakeMaxPressureRaw;

    brakeFilter.in(brakeAnalogValue);

    if ((brakeAnalogValue < 1000) && (shrd.currentCalibOrder == 1))

      brakeFilterInit.in(brakeAnalogValue);

    iBrakeCalibOrder++;
    if (iBrakeCalibOrder > NB_BRAKE_CALIB)
    {
      iBrakeCalibOrder = 0;
      shrd.brakeCalibOrder = 0;
    }

    if (settings.getS1F().Electric_brake_progressive_mode == 1)
    {
      brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);
      brakeFilterMean = brakeFilter.getMean();

      // alarm controler from braking
      if ((brakeFilterMeanErr > brakeFilterInit.getMean() + ANALOG_BRAKE_MIN_OFFSET) && (!isElectricBrakeForbiden()))
      {
        digitalWrite(PIN_OUT_BRAKE, 1);

        if (shrd.brakeStatus == 0)
        {
          char print_buffer[500];
          sprintf(print_buffer, ">>>> brake IO ON");

          Serial.println(print_buffer);

          blh.notifyBleLogs(print_buffer);
        }

        shrd.brakeStatus = 1;
      }
      else
      {
        digitalWrite(PIN_OUT_BRAKE, 0);

        if (shrd.brakeStatus == 1)
        {
          char print_buffer[500];
          sprintf(print_buffer, ">>>> brake IO OFF");

          Serial.println(print_buffer);

          blh.notifyBleLogs(print_buffer);
        }

        shrd.brakeStatus = 0;
      }

      // notify brake LCD value
      if ((shrd.brakeSentOrder != shrd.brakeSentOrderOld) || (shrd.brakeStatus != shrd.brakeStatusOld))
      {
        blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakeStatus);

#if DEBUG_DISPLAY_ANALOG_BRAKE
        Serial.print("brake notify : ");
        Serial.println(shrd.brakeSentOrder);
#endif

        char print_buffer[500];
        sprintf(print_buffer, ">> brakeNotify = f1 : %d / f2 : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d / init : %d",
                brakeFilterMean,
                brakeFilterMeanErr,
                brakeAnalogValue,
                shrd.brakeSentOrder,
                shrd.brakeSentOrderOld,
                shrd.brakeStatus,
                brakeFilterInit.getMean());
        blh.notifyBleLogs(print_buffer);
      }

      shrd.brakeStatusOld = shrd.brakeStatus;
      shrd.brakeSentOrderOld = shrd.brakeSentOrder;

#if DEBUG_BLE_DISPLAY_ANALOG_BRAKE

      if ((brakeFilterMeanErr > brakeFilterInit.getMean() + ANALOG_BRAKE_MIN_OFFSET))
      {

        char print_buffer[500];
        sprintf(print_buffer, "brake = f1 : %d / f2 : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d / init : %d",
                brakeFilterMean,
                brakeFilterMeanErr,
                brakeAnalogValue,
                shrd.brakeSentOrder,
                shrd.brakeSentOrderOld,
                shrd.brakeStatus,
                brakeFilterInit.getMean());

        Serial.println(print_buffer);

        blh.notifyBleLogs(print_buffer);
      }

#endif
    }
  }
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
  uint8_t newModeLcd;

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

  /*
  #if DEBUG_DISPLAY_NITRO
  Serial.print("button1LpDuration = ");
  Serial.print(button1LpDuration);
  Serial.print(" / settings.getS3F().Button_1_long_press_action = ");
  Serial.print(settings.getS3F().Button_1_long_press_action);
  Serial.print(" / settings.LIST_Button_press_action_Nitro_boost = ");
  Serial.println(settings.LIST_Button_press_action_Nitro_boost);
#endif
*/

  // Nitro boost
  if (((settings.getS3F().Button_1_short_press_action == settings.LIST_Button_press_action_Nitro_boost_on_off) && (button1ClickStatus == ACTION_ON)) ||
      ((settings.getS3F().Button_1_long_press_action == settings.LIST_Button_press_action_Nitro_boost_cont) && (button1LpDuration > 0)) ||
      ((settings.getS3F().Button_2_short_press_action == settings.LIST_Button_press_action_Nitro_boost_on_off) && (button2ClickStatus == ACTION_ON)) ||
      ((settings.getS3F().Button_2_long_press_action == settings.LIST_Button_press_action_Nitro_boost_cont) && (button2LpDuration > 0)))
  {
    if (modeOrderBeforeNitro < 0)
    {
      modeOrderBeforeNitro = shrd.modeOrder;
    }
    shrd.modeOrder = 3;
    blh.notifyModeOrder(shrd.modeOrder);

#if DEBUG_DISPLAY_NITRO
    Serial.print(" !!!!!!!!!!!!! LIST_Button_press_action_Nitro_boost in PROGRESS !!!!!!!!!!!!! ");
    Serial.println(shrd.modeOrder);
#endif
  }
  else
  {
    if (modeOrderBeforeNitro > 0)
    {
      shrd.modeOrder = modeOrderBeforeNitro;

      blh.notifyModeOrder(shrd.modeOrder);

#if DEBUG_DISPLAY_NITRO
      Serial.print(" !!!!!!!!!!!!! LIST_Button_press_action_Nitro_boost STOPPED !!!!!!!!!!!!! ");
      Serial.println(shrd.modeOrder);
#endif

      modeOrderBeforeNitro = -1;
    }
  }

  if (shrd.modeOrder == 1)
    newModeLcd = modeLcd0[(uint8_t)(data_buffer[2])];
  else if (shrd.modeOrder == 2)
    newModeLcd = modeLcd1[(uint8_t)(data_buffer[2])];
  else if (shrd.modeOrder == 3)
    newModeLcd = modeLcd2[(uint8_t)(data_buffer[2])];
  else
    newModeLcd = modeLcd2[(uint8_t)(data_buffer[2])];

  return newModeLcd;
}

uint8_t modifyPower(char var, char data_buffer[])
{
  uint8_t newPower = 100;

  float voltage = shrd.voltageFilterMean / 1000.0;
  float bat_min = settings.getS3F().Battery_min_voltage / 10.0;
  float bat_max = settings.getS3F().Battery_max_voltage / 10.0;
  //float bat_med_save = settings.getS3F().Battery_saving_medium_voltage;

  float bat_med_save_voltage = ((bat_max - bat_min) * settings.getS3F().Battery_saving_medium_voltage / 100.0) + bat_min;

  /*
  Serial.print("voltage : ");
  Serial.print(voltage);
  Serial.print("V / bat_min : ");
  Serial.print(bat_min);
  Serial.print("V / bat_max : ");
  Serial.print(bat_max);
  Serial.print("V / bat_med_save_pourcent : ");
  Serial.print(bat_med_save);
  Serial.print("% / bat_med_save_voltage : ");
  Serial.print(bat_med_save_voltage);
  Serial.print("V");
  */

  // lock escooter by reducing power to 5%
  if (blh.bleLockStatus == true)
  {
    // REMINDER : never put bellow 5
    newPower = 5;
  }
  else if (shrd.speedLimiter == 1)
  {
    //    newPower = 37;

    // PID regulation for 25 km/h
    newPower = pidOutput / 2.55;
    if (newPower < 5)
    {
      newPower = 5;
    }

#if DEBUG_DISPLAY_SPEED_PID
    Serial.print(" / output = ");
    Serial.print(pidOutput);
#endif

#if DEBUG_DISPLAY_SPEED_PID
    Serial.print(" / new_power = ");
    Serial.print(newPower);
    Serial.println("%");
#endif
  }
  else
  {

    // override with battery status
    int min_power = 20;
    if (voltage < bat_min)
    {
      if (min_power < var)
      {
        newPower = min_power;
      }
    }
    else if (voltage < bat_med_save_voltage)
    {
      float factor = ((min_power - 100) / (bat_min - bat_med_save_voltage));

      float origin = 100 - (factor * bat_med_save_voltage);

      /*
      Serial.print(" / factor = ");
      Serial.print(factor);
      Serial.print(" / origin = ");
      Serial.print(origin);
*/

      if (newPower < var)
      {
        newPower = (voltage * factor) + origin;
      }
    }
    else
    {
      newPower = var;
    }
  }

  return newPower;
}

uint8_t getBrakeFromLCD(char var, char data_buffer[])
{

  uint8_t brake = (var - data_buffer[3]) & 0x20;
  uint8_t brakeStatusFromLcdNew = brake >> 5;

  shrd.brakeLcd = var;

  //uint8_t brakeStatusFromLcdNew = brakeStatus;
  if ((brakeStatusFromLcdNew == 1) && (shrd.brakeStatusOld == 0))
  {
    shrd.brakeStatus = brakeStatusFromLcdNew;
    timeLastBrake = millis();

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    Serial.print("Brake pressed at : ");
    Serial.println(timeLastBrake);
#endif

    // notify bluetooth
    blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakeStatus);
  }
  else if ((brakeStatusFromLcdNew == 0) && (shrd.brakeStatusOld == 1))
  {
    shrd.brakeStatus = brakeStatusFromLcdNew;

    // reset to min
    shrd.brakeSentOrder = settings.getS1F().Electric_brake_min_value;

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    Serial.print("Brake released at : ");
    Serial.println(millis());
#endif

    // notify bluetooth
    blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakeStatus);
  }

  shrd.brakeStatusOld = brakeStatusFromLcdNew;
  shrd.brakeStatus = brakeStatusFromLcdNew;

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

bool isElectricBrakeForbiden()
{

  float voltage = shrd.voltageFilterMean / 1000.0;
  float bat_min = settings.getS3F().Battery_min_voltage / 10.0;
  float bat_max = settings.getS3F().Battery_max_voltage / 10.0;
  float maxVoltage = bat_min + (settings.getS1F().Electric_brake_disabled_percent_limit * (bat_max - bat_min) / 100.0);

#if DEBUG_BRAKE_FORBIDEN
  Serial.print("bat_min ");
  Serial.print(bat_min);
  Serial.print(" / bat_max ");
  Serial.print(bat_max);
  Serial.print(" / voltage ");
  Serial.print(voltage);
  Serial.print(" / maxVoltage ");
  Serial.print(maxVoltage);
  Serial.print(" / settings.getS1F().Electric_brake_disabled_percent_limit ");
  Serial.println(settings.getS1F().Electric_brake_disabled_percent_limit);
#endif

  return (voltage > maxVoltage);
}

uint8_t modifyBrakeFromLCD(char var, char data_buffer[])
{

  uint32_t currentTime = millis();

  // init from LCD brake mode
  if (shrd.brakeSentOrder == -1)
    shrd.brakeSentOrder = var;

  if (settings.getS2F().Electric_brake_type == settings.LIST_Electric_brake_type_digital)
  {

    // progressive mode
    if (settings.getS1F().Electric_brake_progressive_mode == 1)
    {
      if (shrd.brakeStatus == 1)
      {
        if (shrd.brakeSentOrder < settings.getS1F().Electric_brake_max_value)
        {
          if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 5)
          {
            shrd.brakeSentOrder = settings.getS1F().Electric_brake_min_value + 5;
          }
          else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 4)
          {
            shrd.brakeSentOrder = settings.getS1F().Electric_brake_min_value + 4;
          }
          else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 3)
          {
            shrd.brakeSentOrder = settings.getS1F().Electric_brake_min_value + 3;
          }
          else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 2)
          {
            shrd.brakeSentOrder = settings.getS1F().Electric_brake_min_value + 2;
          }
          else if (currentTime - timeLastBrake > settings.getS1F().Electric_brake_time_between_mode_shift * 1)
          {
            shrd.brakeSentOrder = settings.getS1F().Electric_brake_min_value + 1;
          }
        }

        // notify bluetooth
        blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakeStatus);
      }
      else
      // progressive brake enabled but brake released
      {
        shrd.brakeSentOrder = settings.getS1F().Electric_brake_min_value;
      }
    }
    else
    // progressive brake disabled
    {

      // notify brake LCD value
      if (shrd.brakeSentOrder != shrd.brakeSentOrderOld)
      {
        // notify bluetooth
        blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakeStatus);
      }

      shrd.brakeSentOrderOld = shrd.brakeSentOrder;
    }

#if DEBUG_DISPLAY_DIGITAL_BRAKE
    char print_buffer[500];
    sprintf(print_buffer, "%s %02x %s %02x %s %02x %s %d %s %d %s %d",
            "Brake Status : ",
            shrd.brakeStatus,
            " / brakeSentOrder  : ",
            shrd.brakeSentOrder,
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
  }

  return shrd.brakeSentOrder;
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

double getSpeed()
{
  uint8_t high1 = (data_buffer_cntrl_ori[7] - data_buffer_cntrl_ori[3]) & 0xff;
  uint8_t offset_regul = (data_buffer_cntrl_ori[5] - data_buffer_cntrl_ori[3]) & 0xff;
  uint8_t high2 = (high1 - offset_regul) & 0xff;
  uint8_t low = (data_buffer_cntrl_ori[8] - data_buffer_cntrl_ori[3]);

  double speed = (((int)high2 * 256) + (low));
  speed = speed * (settings.getS1F().Wheel_size / 10.0) / settings.getS1F().Motor_pole_number / 10.5;

  // calculate distance
  uint32_t distanceCurTime = millis();
  uint32_t distanceDiffTime = distanceCurTime - distancePrevTime;
  shrd.distanceTrip = shrd.distanceTrip + ((speed * (distanceDiffTime)) / 360);
  distancePrevTime = millis();

  shrd.distanceOdo = shrd.distanceOdoBoot + (shrd.distanceTrip / 10000);

  if ((shrd.speedOld != 0) && (speed == 0) && (shrd.distanceOdoInFlash != shrd.distanceOdo))
  {
    shrd.distanceOdoInFlash = shrd.distanceOdo;

    Serial.print("saveOdo : distanceOdoInFlash ");
    Serial.print(shrd.distanceOdoInFlash);
    Serial.print(" / distanceOdoBoot : ");
    Serial.print(shrd.distanceOdoBoot);

    saveOdo();
  }

  /*
  Serial.print("distance = ");
  Serial.print(shrd.distance / 10);
  Serial.print(" / distanceDiffTime = ");
  Serial.println(distanceDiffTime);
*/

  // eject error values
  if (speed > 150)
    return shrd.speedOld;

  return speed;
}

uint16_t generateSpeedRawValue(double speed)
{
  uint16_t rawValue;
  double wheelFactor = (settings.getS1F().Wheel_size / 10.0);
  double polesFactor = settings.getS1F().Motor_pole_number * 10.5;
  rawValue = (uint16_t)(speed / wheelFactor * polesFactor);

  rawValue = rawValue;

  return rawValue;
}

uint8_t modifySpeed(char var, char data_buffer[], uint8_t byte)
{

  // LCD Speed adjustement
  if ((settings.getS1F().LCD_Speed_adjustement != 0) || (shrd.speedLimiter == 1))
  {
    double speedToProcess = shrd.speedOld * ((settings.getS1F().LCD_Speed_adjustement + 100) / 100.0);

    if ((shrd.speedLimiter == 1) && (speedToProcess > settings.getS1F().Speed_limiter_max_speed))
    {
      speedToProcess = settings.getS1F().Speed_limiter_max_speed;
    }

    uint16_t rawSpeed = generateSpeedRawValue(speedToProcess);

    uint8_t low = rawSpeed & 0xff;
    uint8_t high = (rawSpeed >> 8) & 0xff;

    uint8_t regulatorOffset = (data_buffer_cntrl_ori[5] - data_buffer_cntrl_ori[3]) & 0xff;

    /*
    char print_buffer[500];
    sprintf(print_buffer, "speed : %.2f / reg %02x / mc %02x %02x / oc %02x %02x / md %02x %02x / od %02x %02x %02x %02x %02x %02x ",
            speedToProcess,

            regulatorOffset,

            (((high + regulatorOffset) & 0xff) + data_buffer_cntrl_ori[3]) & 0xff,
            (low + data_buffer_cntrl_ori[3]) & 0xff,

            data_buffer_cntrl_ori[7],
            data_buffer_cntrl_ori[8],

            high,
            low,

            (data_buffer_cntrl_ori[3] - data_buffer_cntrl_ori[3]) & 0xff,
            (data_buffer_cntrl_ori[4] - data_buffer_cntrl_ori[3]) & 0xff,
            (data_buffer_cntrl_ori[5] - data_buffer_cntrl_ori[3]) & 0xff,
            (data_buffer_cntrl_ori[6]) & 0xff,
            (data_buffer_cntrl_ori[7] - data_buffer_cntrl_ori[3]) & 0xff,
            (data_buffer_cntrl_ori[8] - data_buffer_cntrl_ori[3]) & 0xff);
    Serial.print(print_buffer);
    */

    if (byte == 0)
      return (((high + regulatorOffset) & 0xff) + data_buffer_cntrl_ori[3]) & 0xff;
    else
      return (low + data_buffer_cntrl_ori[3]) & 0xff;
  }
  return var;
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
      if ((var == MODE_LCD_TO_CNTRL_START_BYTE) && (begin_LcdToCntrl == 1))
      {
        begin_LcdToCntrl = 0;

        char log[] = PSTR(" ===> detect begin 0xAA / MODE_LCD_TO_CNTRL");
        Serial.println(log);
        blh.notifyBleLogs(log);

        i = 0;
      }
    }
    else
    // CNTRL -> LCD
    {
      if ((var == MODE_CNTRL_TO_LCD_START_BYTE) && (begin_CntrlToLcd == 1))
      {

        begin_CntrlToLcd = 0;

        char log[] = PSTR(" ===> detect begin 0x36 / MODE_CNTRL_TO_LCD");
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
      if (i == 7)
      {
        var = modifySpeed(var, data_buffer_mod, 0);

        isModified_CntrlToLcd = 1;
      }

      // modify speed
      if (i == 8)
      {
#if ALLOW_CNTRL_TO_LCD_MODIFICATIONS
        shrd.speedCurrent = getSpeed();

        pidInput = shrd.speedCurrent;
        pidSpeed.Compute();

#if DEBUG_DISPLAY_SPEED_PID
        Serial.print("Input = ");
        Serial.print(pidInput);
#endif

        var = modifySpeed(var, data_buffer_mod, 1);
#endif

        shrd.speedOld = shrd.speedCurrent;

        if (shrd.speedCurrent > shrd.speedMax)
          shrd.speedMax = shrd.speedCurrent;

        isModified_CntrlToLcd = 1;
      }

      if (i == 10)
      {
#if ALLOW_CNTRL_TO_LCD_MODIFICATIONS
//        var = data_buffer_cntrl_ori[3];
//        isModified_CntrlToLcd = true;
#endif
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
      else if ((isModified_CntrlToLcd == 1) && (serialMode == MODE_CNTRL_TO_LCD))
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
        //        displayFrame(serialMode, data_buffer_mod, checksum);
        displayFrame(serialMode, data_buffer_ori, checksum);
#endif
#if DEBUG_DISPLAY_DECODED_FRAME_CNTRL_TO_LCD
        //displayDecodedFrame(serialMode, data_buffer_ori, checksum);
        displayDecodedFrame(serialMode, data_buffer_mod, checksum);
//        Serial.println("");
#endif
#if DEBUG_DISPLAY_SPEED
        displaySpeed();
#endif

        /*
        char print_buffer[500];
        sprintf(print_buffer, " %02x %02x %02x %02x %02x ",

                (data_buffer_cntrl_ori[9] - data_buffer_cntrl_ori[3]) & 0xff,
                (data_buffer_cntrl_ori[10] - data_buffer_cntrl_ori[3]) & 0xff,
                (data_buffer_cntrl_ori[11] - data_buffer_cntrl_ori[3]) & 0xff,
                (data_buffer_cntrl_ori[12] - data_buffer_cntrl_ori[3]) & 0xff,
                (data_buffer_cntrl_ori[13] - data_buffer_cntrl_ori[3]) & 0xff);
        Serial.println(print_buffer);
        */
      }
      else
      {
#if DEBUG_BLE_DISPLAY_FRAME
        notifyBleLogFrame(serialMode, data_buffer_mod, checksum);
#endif
#if DEBUG_DISPLAY_FRAME_LCD_TO_CNTRL
        displayFrame(serialMode, data_buffer_ori, checksum);
#endif
#if DEBUG_DISPLAY_MODE
        displayMode(data_buffer_mod);
#endif
      }

      if (checksum != data_buffer_ori[14])
      {

        if (serialMode == MODE_CNTRL_TO_LCD)
        {
          char log[] = "====> CHECKSUM error MODE_CNTRL_TO_LCD ==> reset ";
          Serial.println(log);
          blh.notifyBleLogs(log);

          begin_CntrlToLcd = 1;
        }
        else
        {
          char log[] = "====> CHECKSUM error MODE_LCD_TO_CNTRL ==> reset ";
          Serial.println(log);
          blh.notifyBleLogs(log);

          begin_LcdToCntrl = 1;
        }
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

  //*********************************
  // shrd.brakeSentOrder = var;
  // BUG TO FIX ???

  shrd.brakeSentOrder = settings.getS1F().Electric_brake_min_value;

  if (settings.getS1F().Electric_brake_progressive_mode == 1)
  {

    uint32_t step = 0;
    uint32_t diff = 0;
    uint32_t diffStep = 0;

    if (settings.getS1F().Electric_brake_max_value - settings.getS1F().Electric_brake_min_value > 0)
    {
      step = (shrd.brakeMaxPressureRaw - ANALOG_BRAKE_MIN_VALUE) / (settings.getS1F().Electric_brake_max_value - settings.getS1F().Electric_brake_min_value);

      int brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);
      if (brakeFilterMeanErr > ANALOG_BRAKE_MIN_VALUE)
      {

        diff = brakeFilterMeanErr - ANALOG_BRAKE_MIN_VALUE;
        diffStep = diff / step;
        shrd.brakeSentOrder = diffStep + settings.getS1F().Electric_brake_min_value;
      }
    }

#if DEBUG_DISPLAY_ANALOG_BRAKE

    char print_buffer[500];
    sprintf(print_buffer, "brakeFilter : %d / brakeAnalogValue : %d / brakeSentOrder : %d  / brakeSentOrderOld : %d / shrd.brakeStatus : %d / brakeFilterInit : %d ",
            brakeFilter.getMean(),
            brakeAnalogValue,
            shrd.brakeSentOrder,
            shrd.brakeSentOrderOld,
            shrd.brakeStatus,
            brakeFilterInit.getMean());
    blh.notifyBleLogs(print_buffer);

    Serial.println(print_buffer);

#endif
  }

  return shrd.brakeSentOrder;
}

void processButton1Click()
{
  if (button1ClickStatus == ACTION_OFF)
  {
    button1ClickStatus = ACTION_ON;
  }
  else
  {
    button1ClickStatus = ACTION_OFF;
  }

  processAuxEvent(1, false);
  processSpeedLimiterEvent(1, false);
  processLockEvent(1, false);

  Serial.print("processButton1Click : ");
  Serial.println(button1ClickStatus);

  char print_buffer[500];
  sprintf(print_buffer, "processButton1Click : %d", button1ClickStatus);
  blh.notifyBleLogs(print_buffer);
}

void processButton1LpStart()
{
  button1LpDuration = button1.getPressedTicks();
  Serial.print("processButton1LpStart : ");
  Serial.println(button1LpDuration);

  char print_buffer[500];
  sprintf(print_buffer, "processButton1LpStart : %d", button1LpDuration);
  blh.notifyBleLogs(print_buffer);
}

void processButton1LpDuring()
{
  button1LpDuration = button1.getPressedTicks();

  if ((button1LpDuration > settings.getS3F().Button_long_press_duration * 1000) && (!button1LpProcessed))
  {

    char print_buffer[500];
    sprintf(print_buffer, "processButton1LpDuring : %d =>> process", button1LpDuration);
    blh.notifyBleLogs(print_buffer);

    processAuxEvent(1, true);
    processSpeedLimiterEvent(1, true);
    processLockEvent(1, true);
    button1LpProcessed = true;
  }
}

void processButton1LpStop()
{
  Serial.print("processButton1LpStop : ");
  Serial.println(button1LpDuration);

  char print_buffer[500];
  sprintf(print_buffer, "processButton1LpStop : %d", button1LpDuration);
  blh.notifyBleLogs(print_buffer);

  button1LpProcessed = false;
  button1LpDuration = 0;
}

void processButton1()
{
  if (button1ClickStatus == ACTION_ON)
  {
    digitalWrite(PIN_OUT_LED_BUTTON1, HIGH);
  }
  else if (button1ClickStatus == ACTION_OFF)
  {
    digitalWrite(PIN_OUT_LED_BUTTON1, LOW);
  }
}

////////////////////////////////////////////

void processButton2Click()
{
  if (button2ClickStatus == ACTION_OFF)
  {
    button2ClickStatus = ACTION_ON;
  }
  else
  {
    button2ClickStatus = ACTION_OFF;
  }

  processAuxEvent(2, false);
  processSpeedLimiterEvent(2, false);
  processLockEvent(2, false);

  Serial.print("processButton2Click : ");
  Serial.println(button2ClickStatus);

  char print_buffer[500];
  sprintf(print_buffer, "processButton2Click : %d", button2ClickStatus);
  blh.notifyBleLogs(print_buffer);
}

void processButton2LpStart()
{
  button2LpDuration = button2.getPressedTicks();
  Serial.print("processButton2LpStart : ");
  Serial.println(button2LpDuration);

  char print_buffer[500];
  sprintf(print_buffer, "processButton2LpStart : %d", button2LpDuration);
  blh.notifyBleLogs(print_buffer);
}

void processButton2LpDuring()
{
  button2LpDuration = button2.getPressedTicks();

  if ((button2LpDuration > settings.getS3F().Button_long_press_duration * 1000) && (!button2LpProcessed))
  {

    char print_buffer[500];
    sprintf(print_buffer, "processButton2LpDuring : %d ==> process", button2LpDuration);
    blh.notifyBleLogs(print_buffer);

    processAuxEvent(2, true);
    processSpeedLimiterEvent(2, true);
    processLockEvent(2, true);
    button2LpProcessed = true;
  }
}

void processButton2LpStop()
{
  Serial.print("processButton2LpStop : ");
  Serial.println(button2LpDuration);

  char print_buffer[500];
  sprintf(print_buffer, "processButton2LpStop : %d", button2LpDuration);
  blh.notifyBleLogs(print_buffer);

  button2LpProcessed = false;
  button2LpDuration = 0;
}

void processButton2()
{
  if (button2ClickStatus == ACTION_ON)
  {
    digitalWrite(PIN_OUT_LED_BUTTON2, HIGH);
  }
  else if (button2ClickStatus == ACTION_OFF)
  {
    digitalWrite(PIN_OUT_LED_BUTTON2, LOW);
  }
}
//////////////////////////

void processAuxEvent(uint8_t buttonId, bool isLongPress)
{

  // process AUX order -- button 1
  if (((buttonId == 1) && (!isLongPress) && (settings.getS3F().Button_1_short_press_action == settings.LIST_Button_press_action_Aux_on_off)) ||
      ((buttonId == 1) && (isLongPress) && (settings.getS3F().Button_1_long_press_action == settings.LIST_Button_press_action_Aux_on_off)) ||
      ((buttonId == 2) && (!isLongPress) && (settings.getS3F().Button_2_short_press_action == settings.LIST_Button_press_action_Aux_on_off)) ||
      ((buttonId == 2) && (isLongPress) && (settings.getS3F().Button_2_long_press_action == settings.LIST_Button_press_action_Aux_on_off)))
  {
    if (shrd.auxOrder == 0)
    {
      shrd.auxOrder = 1;
    }
    else
    {
      shrd.auxOrder = 0;
    }
    blh.notifyAuxOrder(shrd.auxOrder);

    Serial.print("processAuxEvent => ok / ");
    Serial.println(shrd.auxOrder);

    char print_buffer[500];
    sprintf(print_buffer, "processAuxEvent : %d", shrd.auxOrder);
    blh.notifyBleLogs(print_buffer);
  }
}

void processSpeedLimiterEvent(uint8_t buttonId, bool isLongPress)
{

  // process SpeedLimiter
  if (((buttonId == 1) && (!isLongPress) && (settings.getS3F().Button_1_short_press_action == settings.LIST_Button_press_action_Startup_speed_limitation_on_off)) ||
      ((buttonId == 1) && (isLongPress) && (settings.getS3F().Button_1_long_press_action == settings.LIST_Button_press_action_Startup_speed_limitation_on_off)) ||
      ((buttonId == 2) && (!isLongPress) && (settings.getS3F().Button_2_short_press_action == settings.LIST_Button_press_action_Startup_speed_limitation_on_off)) ||
      ((buttonId == 2) && (isLongPress) && (settings.getS3F().Button_2_long_press_action == settings.LIST_Button_press_action_Startup_speed_limitation_on_off)))
  {
    if (shrd.speedLimiter == 0)
    {
      shrd.speedLimiter = 1;
    }
    else
    {
      shrd.speedLimiter = 0;
    }
    blh.notifySpeedLimiterStatus(shrd.speedLimiter);

    Serial.print("notifySpeedLimiterStatus => ok / ");
    Serial.println(shrd.speedLimiter);

    char print_buffer[500];
    sprintf(print_buffer, "notifySpeedLimiterStatus : %d", shrd.speedLimiter);
    blh.notifyBleLogs(print_buffer);
  }
}

void processLockEvent(uint8_t buttonId, bool isLongPress)
{

  // process SpeedLimiter
  if (((buttonId == 1) && (!isLongPress) && (settings.getS3F().Button_1_short_press_action == settings.LIST_Button_press_action_Anti_theft_manual_lock_on)) ||
      ((buttonId == 1) && (isLongPress) && (settings.getS3F().Button_1_long_press_action == settings.LIST_Button_press_action_Anti_theft_manual_lock_on)) ||
      ((buttonId == 2) && (!isLongPress) && (settings.getS3F().Button_2_short_press_action == settings.LIST_Button_press_action_Anti_theft_manual_lock_on)) ||
      ((buttonId == 2) && (isLongPress) && (settings.getS3F().Button_2_long_press_action == settings.LIST_Button_press_action_Anti_theft_manual_lock_on)))
  {
    blh.setBleLock(true);
    blh.notifyBleLock();

    Serial.println("processLockEvent => ok / ON");

    char print_buffer[500];
    sprintf(print_buffer, "processLockEvent");
    blh.notifyBleLogs(print_buffer);
  }
}

void processAux()
{
  /*
  if (shrd.auxOrder == 1)
  {
    digitalWrite(PIN_OUT_RELAY, 1);
  }
  else
  {
    digitalWrite(PIN_OUT_RELAY, 0);
  }
  */
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
    }
  }
}

void processVoltage()
{

  voltageStatus = analogRead(PIN_IN_VOLTAGE);

  // eject false reading
  if (voltageStatus == 4095)
  {
    /*
    Serial.print("Voltage read : ");
    Serial.print(voltageStatus);
    Serial.println(" => eject ");
*/

    char print_buffer[500];
    sprintf(print_buffer, "Voltage read 4095 ==> eject");
    blh.notifyBleLogs(print_buffer);

    return;
  }
  if (voltageStatus < 900)
  {
    /*
    Serial.print("Voltage read : ");
    Serial.print(voltageStatus);
    Serial.println(" => eject ");
    */

    char print_buffer[500];
    sprintf(print_buffer, "Voltage read <900 ==> eject");
    blh.notifyBleLogs(print_buffer);

    return;
  }

  voltageInMilliVolts = ((voltageStatus * ANALOG_TO_VOLTS_A) + ANALOG_TO_VOLTS_B) * 1000;

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
  int currentInMillamps = (curerntRead - currentFilterInit.getMean()) * (1000.0 / ANALOG_TO_CURRENT);

  // current rest value
  currentFilter.in(currentInMillamps);
  shrd.currentFilterMean = currentFilter.getMeanWithoutExtremes(10);

  if ((shrd.speedCurrent == 0) && (shrd.currentCalibOrder > 0))
  {

    shrd.currentCalibOrder--;
    currentFilterInit.in(curerntRead);

#if DEBUG_DISPLAY_CURRENT
    if (shrd.currentCalibOrder == 1)
      Serial.println("Current calibration end ... ");
#endif
  }

  /*
#if DEBUG_DISPLAY_CURRENT
  Serial.print("Current read : ");
  Serial.print(curerntRead);
  Serial.print(" / currentFilterInit mean : ");
  Serial.print(currentFilterInit.getMean());
  Serial.print(" / in amperes : ");
  Serial.println(currentInMillamps / 1000.0);
#endif
*/
}

//////------------------------------------
//////------------------------------------
////// Main loop

void loop()
{

  // handle Wifi OTA
  if (shrd.inOtaMode)
  {
    blh.deinit();
    OTA_loop();
    return;
  }

  processSerial();

  blh.processBLE();

  button1.tick();
  button2.tick();
  processButton1();
#if DEBUG_DISPLAY_BUTTON1
  displayButton1();
#endif

  processButton2();
#if DEBUG_DISPLAY_BUTTON2
  displayButton2();
#endif

  processAux();

  if (i_loop % 10 == 0)
  {
    processVoltage();
  }

  if ((i_loop % 10 == 2))
  {
    //modifyBrakeFromLCD();
    //displayBrake();
    getBrakeFromAnalog();
  }

  if (i_loop % 10 == 4)
  {
    processCurrent();
  }

  // keep it fast (/100 not working)
  if (i_loop % 10 == 6)
  {
    processDHT();
  }

#if TFT_ENABLED
  if (i_loop % 100 == 1)
  {
    tftUpdateData();
  }
#endif

  // Give a time for ESP
  delay(1);
  i_loop++;

  //Serial.print("time loop");
  //Serial.println(millis() - timeLoop);

  timeLoop = millis();

  resetWatchdog();

  //digitalWrite(PIN_OUT_BRAKE, i_loop % 2000 > 1000);
}

/////////// End