//////////////////////////////////////////
// TODO : check button long press processing
// TODO : current loop
// TODO : LCD error indicators
// TODO : mode Z button/settings
// TODO : auto mode shift on low battery
// TODO : original regulator perturbation
// TODO : exponential throttle
// TODO : change OTA to AWS
// BUG : mode Z / android
// BUG : push button make brake analog read wrong
//////////////////////////////////////////

//////------------------------------------
////// Inludes

#include <Arduino.h>
#include <EEPROM.h>
#include "main.h"
#include "Settings.h"
#include "SharedData.h"
#include "debug.h"
#include "OneButton.h"
#include "EEPROM_storage.h"
#include "EEPROM_storage.h"
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <PID_v1.h>

#include "esp32-hal-uart.h"

#include "BLE/BluetoothHandler.h"
#include "OTA/OTA_wifi.h"
#include "filters/MedianFilter.h"
#include "DHT/dht_nonblocking.h"
#include "TFT/tft_main.h"
#include "Battery/Battery.h"

#include "Controllers/VescUart.h"
#include "Controllers/KellyUart.h"
#include "Controllers/MinimoUart.h"
#include "Controllers/SmartEsc.h"
#include "Controllers/ControllerType.h"

#include "app_version.h"

//////------------------------------------
////// Defines

// SMART CONFIGURATION
#define CONTROLLER_TYPE CONTROLLER_SMART_ESC
#define TFT_ENABLED 0
#define READ_THROTTLE 0
#define DEBUG_ESP_HTTP_UPDATE 1
#define TEST_ADC_DAC_REFRESH 0
#define TEMPERATURE_EXT_READ 1
#define VOLTAGE_EXT_READ 0
#define BRAKE_ANALOG_EXT_READ 1

// PINOUT
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
#define PIN_IN_THROTTLE 33
#define PIN_OUT_BACKLIGHT 5
#define PIN_I2C_SDA 32
#define PIN_I2C_SCL 33

// I2C
//#define I2C_FREQ 400000
#define I2C_FREQ 1000000

// UART
#define BAUD_RATE_CONSOLE 921600

// ADC
#define ANALOG_TO_VOLTS_A 0.0213
#define ANALOG_TO_VOLTS_B 5.4225
#define ANALOG_TO_CURRENT 35

#define ANALOG_CURRENT_MIN_RAW_READING 100

#define NB_CURRENT_FILTER_DATA 200
#define NB_CURRENT_FILTER_CALIB_DATA 200
#define NB_BRAKE_CALIB_DATA 100

#define NB_AUTONOMY_FILTER_DATA 120
#define NB_VOLTAGE_FILTER_DATA 100

#define BRAKE_TYPE_ANALOG 1
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

// BUTTONS
#define BUTTON_LONG_PRESS_TICK 300

#define WATCHDOG_TIMEOUT 1000000 //time in ms to trigger the watchdog

//////------------------------------------
////// Variables

// Time
unsigned long timeLoop = 0;

// Watchdog
hw_timer_t *timer = NULL;

// Serial
float serialRcvValue = 0;

// Settings
int begin_soft = 0;
int begin_hard = 0;

char bleLog[50] = "";

HardwareSerial hwSerCntrl(1);
HardwareSerial hwSerLcd(2);

VescUart vescCntrl;
MinimoUart minomoCntrl;
KellyUart kellyCntrl;
SmartEsc smartEscCntrl;

DHT_nonblocking dht_sensor(PIN_IN_OUT_DHT, DHT_TYPE_22);

OneButton button1(PIN_IN_BUTTON1, true, true);
OneButton button2(PIN_IN_BUTTON2, true, true);

TwoWire I2Cone = TwoWire(0);
Adafruit_MCP4725 dac;

SharedData shrd;

int i_loop = 0;

uint32_t iBrakeCalibOrder = 0;

uint16_t voltageRaw = 0;
uint32_t voltageInMilliVolts = 0;
Battery batt = Battery(36000, 54000);

uint16_t brakeAnalogValue = 0;
uint16_t throttleAnalogValue = 0;

MedianFilter voltageFilter(NB_VOLTAGE_FILTER_DATA, 2000);
MedianFilter voltageLongFilter(NB_VOLTAGE_FILTER_DATA, 50000);
MedianFilter voltageRawFilter(NB_VOLTAGE_FILTER_DATA, 2000);
MedianFilter currentFilter(NB_CURRENT_FILTER_DATA, 1830);
MedianFilter currentFilterInit(NB_CURRENT_FILTER_CALIB_DATA, 1830);
MedianFilter brakeFilter(10 /* 20 */, 900);
MedianFilter brakeFilterInit(NB_BRAKE_CALIB_DATA, 900);
MedianFilter autonomyLeftFilter(NB_AUTONOMY_FILTER_DATA, 0);

Settings settings;

BluetoothHandler blh;

PID pidSpeed(&shrd.pidInput, &shrd.pidOutput, &shrd.pidSetpoint, shrd.speedPidKp, shrd.speedPidKi, shrd.speedPidKd, DIRECT);

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

  if (shrd.distanceOdo == 0xffffffff)
  {
    shrd.distanceOdo = 0;
    shrd.distanceOdoBoot = 0;
    shrd.distanceOdoInFlash = 0;
    saveOdo();
    Serial.print("==> ODO init at 0");
  }
}

void saveBatteryCalib()
{
  // Read EEPROM
  int EEAddr = EEPROM_ADDRESS_BATTERY_CALIB;
  EEPROM.put(EEAddr, shrd.batteryMaxVoltageCalibUser);
  EEAddr += sizeof(shrd.batteryMaxVoltageCalibUser);
  EEPROM.put(EEAddr, shrd.batteryMaxVoltageCalibRaw);
  EEAddr += sizeof(shrd.batteryMaxVoltageCalibRaw);
  EEPROM.put(EEAddr, shrd.batteryMinVoltageCalibUser);
  EEAddr += sizeof(shrd.batteryMinVoltageCalibUser);
  EEPROM.put(EEAddr, shrd.batteryMinVoltageCalibRaw);
  EEAddr += sizeof(shrd.batteryMinVoltageCalibRaw);

  EEPROM.commit();

  Serial.println("save BatteryCalib value : ");
  Serial.print("  batteryMaxVoltageCalibUser : ");
  Serial.println(shrd.batteryMaxVoltageCalibUser);
  Serial.print("  batteryMaxVoltageCalibRaw : ");
  Serial.println(shrd.batteryMaxVoltageCalibRaw);
  Serial.print("  batteryMinVoltageCalibUser : ");
  Serial.println(shrd.batteryMinVoltageCalibUser);
  Serial.print("  batteryMinVoltageCalibRaw : ");
  Serial.println(shrd.batteryMinVoltageCalibRaw);
}

void restoreBatteryCalib()
{
  // Read EEPROM
  int EEAddr = EEPROM_ADDRESS_BATTERY_CALIB;
  EEPROM.get(EEAddr, shrd.batteryMaxVoltageCalibUser);
  EEAddr += sizeof(shrd.batteryMaxVoltageCalibUser);
  EEPROM.get(EEAddr, shrd.batteryMaxVoltageCalibRaw);
  EEAddr += sizeof(shrd.batteryMaxVoltageCalibRaw);
  EEPROM.get(EEAddr, shrd.batteryMinVoltageCalibUser);
  EEAddr += sizeof(shrd.batteryMinVoltageCalibUser);
  EEPROM.get(EEAddr, shrd.batteryMinVoltageCalibRaw);
  EEAddr += sizeof(shrd.batteryMinVoltageCalibRaw);

  Serial.println("restore BatteryCalib value : ");
  Serial.print("  batteryMaxVoltageCalibUser : ");
  Serial.println(shrd.batteryMaxVoltageCalibUser);
  Serial.print("  batteryMaxVoltageCalibRaw : ");
  Serial.println(shrd.batteryMaxVoltageCalibRaw);
  Serial.print("  batteryMinVoltageCalibUser : ");
  Serial.println(shrd.batteryMinVoltageCalibUser);
  Serial.print("  batteryMinVoltageCalibRaw : ");
  Serial.println(shrd.batteryMinVoltageCalibRaw);
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

void setupI2C()
{

  I2Cone.begin(PIN_I2C_SDA, PIN_I2C_SCL, I2C_FREQ);
}

void setupDac()
{
  // call GENERAL CALL RESET
  // https://www.sparkfun.com/datasheets/BreakoutBoards/MCP4725.pdf
  // page 22

  dac.begin(0x60, &I2Cone);
}

void setupSerial()
{

#if CONTROLLER_TYPE == CONTROLLER_MINIMOTORS

  minomoCntrl.setSettings(&settings);
  minomoCntrl.setSharedData(&shrd);
  minomoCntrl.setBluetoothHandler(&blh);
  minomoCntrl.setPID(&pidSpeed);

  // minimotor controller
  hwSerCntrl.begin(BAUD_RATE_MINIMOTORS, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  minomoCntrl.setControllerSerialPort(&hwSerCntrl);

  // minimotor display
  hwSerLcd.begin(BAUD_RATE_MINIMOTORS, SERIAL_8N1, PIN_SERIAL_LCD_TO_ESP, PIN_SERIAL_ESP_TO_LCD);
  minomoCntrl.setLcdSerialPort(&hwSerLcd);

  hwSerCntrl.setUartIrqIdleTrigger(0);
  hwSerLcd.setUartIrqIdleTrigger(0);

#elif CONTROLLER_TYPE == CONTROLLER_VESC
  hwSerCntrl.begin(BAUD_RATE_VESC, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  vescCntrl.setSerialPort(&hwSerCntrl);
  //vescCntrl.setDebugPort(&Serial);

  hwSerCntrl.setUartIrqIdleTrigger(1);

#elif CONTROLLER_TYPE == CONTROLLER_KELLY

  hwSerCntrl.begin(BAUD_RATE_KELLY, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  kellyCntrl.setSerialPort(&hwSerCntrl);
  kellyCntrl.setDebugPort(&Serial);

  hwSerCntrl.setUartIrqIdleTrigger(1);

#elif CONTROLLER_TYPE == CONTROLLER_SMART_ESC

  hwSerCntrl.begin(BAUD_RATE_SMARTESC, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  smartEscCntrl.setSerialPort(&hwSerCntrl);
  //smartEscCntrl.setDebugPort(&Serial);
  smartEscCntrl.setSettings(&settings);
  smartEscCntrl.setSharedData(&shrd);

  hwSerCntrl.setUartIrqIdleTrigger(1);

#endif
}

void setupEPROMM()
{
  EEPROM.begin(EEPROM_SIZE);
}

void setupPID()
{

  shrd.pidSetpoint = settings.getS1F().Speed_limiter_max_speed;

  //turn the PID on
  pidSpeed.SetMode(AUTOMATIC);
  //myPID.SetSampleTime(10);
  //myPID.SetOutputLimits(0,100);
}

void setupBattery()
{
  batt = Battery(settings.getS3F().Battery_min_voltage * 100, settings.getS3F().Battery_max_voltage * 100);
  batt.begin(&sigmoidal);

  Serial.println("Battery : min = " + (String)settings.getS3F().Battery_min_voltage + " / max = " + settings.getS3F().Battery_max_voltage);
}

void setupVoltage()
{

  voltageRaw = analogRead(PIN_IN_VOLTAGE);
  voltageInMilliVolts = ((voltageRaw * ANALOG_TO_VOLTS_A) + ANALOG_TO_VOLTS_B) * 1000;
  shrd.voltageActual = voltageInMilliVolts;

  // filter reinit
  for (int i = 0; i < NB_VOLTAGE_FILTER_DATA; i++)
  {
    voltageFilter.in(voltageInMilliVolts);
    voltageLongFilter.in(voltageInMilliVolts);
    voltageRawFilter.in(voltageRaw);
  }
}

void setupAutonomy()
{

  shrd.batteryLevel = batt.level(shrd.voltageActual);

  uint8_t autonomyLeft = (settings.getS3F().Battery_max_distance / 10) * (shrd.batteryLevel) / 100.0;

  // filter reinit
  for (int i = 0; i < NB_AUTONOMY_FILTER_DATA; i++)
    autonomyLeftFilter.in(autonomyLeft);
}

void resetPid()
{

  shrd.pidSetpoint = settings.getS1F().Speed_limiter_max_speed;
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

void taskUpdateTFT(void *parameter)
{
  int i = 0;
  for (;;)
  { // infinite loop

    //Serial.println(">>>>> update TFT");

    tftUpdateData(i);
    // Pause the task again for 50ms
    //vTaskDelay(10 / portTICK_PERIOD_MS);
    i++;

    if (i >= 20)
      i = 0;
  }
}

////// Setup
void setup()
{
#if TFT_ENABLED
  tftSetupBacklight();
#endif

  // Initialize the Serial (use only in setup codes)
  Serial.begin(BAUD_RATE_CONSOLE);
  Serial.println(PSTR("\n\nsetup --- begin"));

  Serial.print("version : ");
  Serial.println(Version);

  shrd.timeLastNotifyBle = millis();

  Serial.println(PSTR("   serial ..."));
  setupSerial();

  Serial.println(PSTR("   i2c ..."));
  setupI2C();

  Serial.println(PSTR("   dac ..."));
  setupDac();

  Serial.println(PSTR("   eeprom ..."));
  setupEPROMM();
  restoreBleLockForced();
  restoreBrakeMaxPressure();
  restoreOdo();
  restoreBatteryCalib();

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
  shrd.currentCalibOrder = NB_CURRENT_FILTER_CALIB_DATA;

  Serial.println(PSTR("   init data with settings ..."));
  initDataWithSettings();

  Serial.println(PSTR("   watchdog ..."));
  setupWatchdog();

  setupVoltage();
  setupBattery();
  setupAutonomy();

  batt.begin(&sigmoidal);

#if TFT_ENABLED
  xTaskCreatePinnedToCore(
      taskUpdateTFT,   // Function that should be called
      "taskUpdateTFT", // Name of the task (for debugging)
      10000,           // Stack size (bytes)
      NULL,            // Parameter to pass
      0,               // Task priority
      NULL,            // Task handle,
      1);
#endif

  // End off setup
  Serial.println("setup --- end");
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
  Serial.println(shrd.button1ClickStatus);
}

void displayButton2()
{
  Serial.print("Button2 : ");
  Serial.println(shrd.button1ClickStatus);
}

void computeDistance(float speed)
{

  // calculate distance
  uint32_t distanceCurTime = millis();
  uint32_t distanceDiffTime = distanceCurTime - shrd.distancePrevTime;
  shrd.distanceTrip = shrd.distanceTrip + ((speed * (distanceDiffTime)) / 360);
  shrd.distancePrevTime = millis();

  shrd.distanceOdo = shrd.distanceOdoBoot + (shrd.distanceTrip / 10000);

  if ((shrd.speedOld != 0) && (speed == 0) && (shrd.distanceOdoInFlash != shrd.distanceOdo))
  {
    shrd.distanceOdoInFlash = shrd.distanceOdo;

    /*
    Serial.print("saveOdo : distanceOdoInFlash ");
    Serial.print(shrd.distanceOdoInFlash);
    Serial.print(" / distanceOdoBoot : ");
    Serial.print(shrd.distanceOdo);
*/

    saveOdo();
  }

#if DEBUG_DISPLAY_DISTANCE
  Serial.println("distanceTrip = " + (String)(shrd.distanceTrip / 10) +
                 " / distanceDiffTime = " + (String)distanceDiffTime +
                 " / speed = " + (String)speed);
#endif
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

      /*
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
*/
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

    if ((brakeAnalogValue < 1000) && (shrd.brakeCalibOrder >= 1))
    {
      brakeFilterInit.in(brakeAnalogValue);
      shrd.brakeFilterInitMean = brakeFilterInit.getMean();
    }

    iBrakeCalibOrder++;
    if (iBrakeCalibOrder > NB_BRAKE_CALIB_DATA)
    {
      iBrakeCalibOrder = 0;
      shrd.brakeCalibOrder = 0;
    }

    if (settings.getS1F().Electric_brake_progressive_mode == 1)
    {
      brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);
      brakeFilterMean = brakeFilter.getMean();

      shrd.brakeFordidenHighVoltage = isElectricBrakeForbiden();

      // alarm controler from braking
      if ((brakeFilterMeanErr > shrd.brakeFilterInitMean + ANALOG_BRAKE_MIN_OFFSET) && (!shrd.brakeFordidenHighVoltage))
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
        blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakeStatus, shrd.brakeFordidenHighVoltage);

#if DEBUG_DISPLAY_ANALOG_BRAKE
        Serial.print("brake notify : ");
        Serial.println(shrd.brakeSentOrder);
#endif

        char print_buffer[500];
        sprintf(print_buffer, ">> brakeNotify = f1 : %d / f2 : %d / brakeFilterInitMean : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d / init : %d / forbid : %d",
                brakeFilterMean,
                brakeFilterMeanErr,
                shrd.brakeFilterInitMean,
                brakeAnalogValue,
                shrd.brakeSentOrder,
                shrd.brakeSentOrderOld,
                shrd.brakeStatus,
                brakeFilterInit.getMean(),
                shrd.brakeFordidenHighVoltage);
        blh.notifyBleLogs(print_buffer);
      }

      shrd.brakeStatusOld = shrd.brakeStatus;
      shrd.brakeSentOrderOld = shrd.brakeSentOrder;

#if DEBUG_BLE_DISPLAY_ANALOG_BRAKE

      if ((brakeFilterMeanErr > shrd.brakeFilterInitMean + ANALOG_BRAKE_MIN_OFFSET))
      {

        char print_buffer[500];
        sprintf(print_buffer, "brake = f1 : %d / f2 : %d / brakeFilterInitMean : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d / init : %d / forbid : %d",
                brakeFilterMean,
                brakeFilterMeanErr,
                shrd.brakeFilterInitMean,
                brakeAnalogValue,
                shrd.brakeSentOrder,
                shrd.brakeSentOrderOld,
                shrd.brakeStatus,
                brakeFilterInit.getMean(),
                shrd.brakeFordidenHighVoltage);

        Serial.println(print_buffer);

        blh.notifyBleLogs(print_buffer);
      }

#endif
    }
  }
}

bool isElectricBrakeForbiden()
{
  if (settings.getS1F().Electric_brake_disabled_on_high_voltage == 0)
  {
#if DEBUG_DISPLAY_BRAKE_FORBIDEN
    Serial.println("electric brake not disabled on battery high voltage");
#endif
    return false;
  }

  float voltage = shrd.voltageFilterMean / 1000.0;
  float bat_min = settings.getS3F().Battery_min_voltage / 10.0;
  float bat_max = settings.getS3F().Battery_max_voltage / 10.0;
  float maxVoltage = bat_min + (settings.getS1F().Electric_brake_disabled_percent_limit * (bat_max - bat_min) / 100.0);

#if DEBUG_DISPLAY_BRAKE_FORBIDEN
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

void processVescSerial()
{

  String command;

  if (vescCntrl.readVescValues())
  {
    /*
    Serial.print("rpm : ");
    Serial.print(vescCntrl.data.rpm);
    Serial.print(" / tachometerAbs : ");
    Serial.print(vescCntrl.data.tachometerAbs);
    Serial.print(" / tachometer : ");
    Serial.print(vescCntrl.data.tachometer);
    Serial.print(" / ampHours : ");
    Serial.print(vescCntrl.data.ampHours);
    Serial.print(" / avgInputCurrent : ");
    Serial.print(vescCntrl.data.avgInputCurrent);
    */

    float speedCompute = vescCntrl.data.rpm * (settings.getS1F().Wheel_size / 10.0) / settings.getS1F().Motor_pole_number / 120.0;
    if (speedCompute < 0)
      speedCompute = 0;
    if (speedCompute > 999)
      speedCompute = 999;

    if (speedCompute > shrd.speedMax)
      shrd.speedMax = speedCompute;

    shrd.speedCurrent = speedCompute;

    /*
    Serial.print(" / speedCompute : ");
    Serial.println(speedCompute);
    Serial.println(vescCntrl.data.inpVoltage);
    Serial.println(vescCntrl.data.ampHours);
    */

    shrd.voltageFilterMean = vescCntrl.data.inpVoltage * 1000;
    shrd.currentFilterMean = vescCntrl.data.avgInputCurrent * 1000;
  }

  /*
  Serial.print("throttleAnalogValue : ");
  Serial.println(throttleAnalogValue);
*/

  if (throttleAnalogValue < 900)
    throttleAnalogValue = 0;

  float duty = (throttleAnalogValue - 900) / 2000.0;

  if (duty > 1)
    duty = 1.0;
  if (duty < 0)
    duty = 0.0;
  /*
  Serial.print("duty : ");
  Serial.println(duty);
*/
  vescCntrl.setDuty(duty);
}

void processKellySerial1()
{

  shrd.voltageFilterMean = kellyCntrl.data1.B_Voltage * 1000;
  shrd.currentTemperature = kellyCntrl.data1.Controller_temperature;

  shrd.brakeStatus = kellyCntrl.data1.BRK_SW;

  // notify brake LCD value
  if ((shrd.brakeSentOrder != shrd.brakeSentOrderOld) || (shrd.brakeStatus != shrd.brakeStatusOld))
  {
    blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakeStatus, shrd.brakeFordidenHighVoltage);
  }

  shrd.brakeStatusOld = shrd.brakeStatus;
}

void processKellySerial2()
{

  // 532 RPM = 25 km/h
  // 1814 =
  // TODO : check calculation - if motor poles numbers
  float speedCompute = kellyCntrl.data2.Mechanical_speed_in_RPM * (settings.getS1F().Wheel_size / 10.0) / settings.getS1F().Motor_pole_number / 14.0;
  if (speedCompute < 0)
    speedCompute = 0;
  if (speedCompute > 999)
    speedCompute = 999;

  if (speedCompute > shrd.speedMax)
    shrd.speedMax = speedCompute;

  shrd.speedCurrent = speedCompute;
}

void processSmartEscSerial()
{

  shrd.voltageFilterMean = (uint32_t)(smartEscCntrl.data.Controller_Voltage) * 10;
  shrd.currentFilterMean = (uint32_t)(smartEscCntrl.data.Controller_Current) * 10 / 3.5;
  shrd.currentTemperature = smartEscCntrl.data.MOSFET_temperature;
  if (smartEscCntrl.data.ERPM < 0)
    smartEscCntrl.data.ERPM = 0;
  shrd.speedCurrent = smartEscCntrl.data.ERPM * (settings.getS1F().Wheel_size / 10.0) / settings.getS1F().Motor_pole_number / 10.5;

  //Serial.println("voltageFilterMean " + (String)shrd.voltageFilterMean + " / shrd.currentFilterMean = " + (String)shrd.currentFilterMean + " / currentTemperature = " + (String)shrd.currentTemperature + " / ERPM = " + (String)smartEscCntrl.data.ERPM + " / speedCurrent = " + (String)shrd.speedCurrent + " / Throttle = " + (String)smartEscCntrl.data.Throttle);

  // notify brake LCD value
  if ((shrd.brakeSentOrder != shrd.brakeSentOrderOld) || (shrd.brakeStatus != shrd.brakeStatusOld))
  {
    blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakeStatus, shrd.brakeFordidenHighVoltage);
  }

  shrd.brakeStatusOld = shrd.brakeStatus;
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
      step = (shrd.brakeMaxPressureRaw - shrd.brakeFilterInitMean) / (settings.getS1F().Electric_brake_max_value - settings.getS1F().Electric_brake_min_value);

      int brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);
      if (brakeFilterMeanErr > shrd.brakeFilterInitMean)
      {

        diff = brakeFilterMeanErr - shrd.brakeFilterInitMean;
        diffStep = diff / step;
        shrd.brakeSentOrder = diffStep + settings.getS1F().Electric_brake_min_value;
      }
    }

#if DEBUG_DISPLAY_ANALOG_BRAKE

    char print_buffer[500];
    sprintf(print_buffer, "brakeFilter : %d / brakeAnalogValue : %d / brakeSentOrder : %d  / brakeSentOrderOld : %d / shrd.brakeStatus : %d / brakeFilterInit : %d / step : %d ",
            brakeFilter.getMean(),
            brakeAnalogValue,
            shrd.brakeSentOrder,
            shrd.brakeSentOrderOld,
            shrd.brakeStatus,
            brakeFilterInit.getMean(),
            step);
    blh.notifyBleLogs(print_buffer);

    Serial.println(print_buffer);

#endif
  }

  return shrd.brakeSentOrder;
}

void processButton1Click()
{
  if (shrd.button1ClickStatus == ACTION_OFF)
  {
    shrd.button1ClickStatus = ACTION_ON;
  }
  else
  {
    shrd.button1ClickStatus = ACTION_OFF;
  }

  processAuxEvent(1, false);
  processSpeedLimiterEvent(1, false);
  processLockEvent(1, false);

  Serial.print("processButton1Click : ");
  Serial.println(shrd.button1ClickStatus);

  char print_buffer[500];
  sprintf(print_buffer, "processButton1Click : %d", shrd.button1ClickStatus);
  blh.notifyBleLogs(print_buffer);
}

void processButton1LpStart()
{
  shrd.button1LpDuration = button1.getPressedTicks();
  Serial.print("processButton1LpStart : ");
  Serial.println(shrd.button1LpDuration);

  char print_buffer[500];
  sprintf(print_buffer, "processButton1LpStart : %d", shrd.button1LpDuration);
  blh.notifyBleLogs(print_buffer);
}

void processButton1LpDuring()
{
  shrd.button1LpDuration = button1.getPressedTicks();

  if ((shrd.button1LpDuration > settings.getS3F().Button_long_press_duration * 1000) && (!shrd.button1LpProcessed))
  {

    char print_buffer[500];
    sprintf(print_buffer, "processButton1LpDuring : %d =>> process", shrd.button1LpDuration);
    blh.notifyBleLogs(print_buffer);

    processAuxEvent(1, true);
    processSpeedLimiterEvent(1, true);
    processLockEvent(1, true);
    shrd.button1LpProcessed = true;
  }
}

void processButton1LpStop()
{
  Serial.print("processButton1LpStop : ");
  Serial.println(shrd.button1LpDuration);

  char print_buffer[500];
  sprintf(print_buffer, "processButton1LpStop : %d", shrd.button1LpDuration);
  blh.notifyBleLogs(print_buffer);

  shrd.button1LpProcessed = false;
  shrd.button1LpDuration = 0;
}

void processButton1()
{
  if (shrd.button1ClickStatus == ACTION_ON)
  {
    digitalWrite(PIN_OUT_LED_BUTTON1, HIGH);
  }
  else if (shrd.button1ClickStatus == ACTION_OFF)
  {
    digitalWrite(PIN_OUT_LED_BUTTON1, LOW);
  }
}

////////////////////////////////////////////

void processButton2Click()
{
  if (shrd.button2ClickStatus == ACTION_OFF)
  {
    shrd.button2ClickStatus = ACTION_ON;
  }
  else
  {
    shrd.button2ClickStatus = ACTION_OFF;
  }

  processAuxEvent(2, false);
  processSpeedLimiterEvent(2, false);
  processLockEvent(2, false);

  Serial.print("processButton2Click : ");
  Serial.println(shrd.button2ClickStatus);

  char print_buffer[500];
  sprintf(print_buffer, "processButton2Click : %d", shrd.button2ClickStatus);
  blh.notifyBleLogs(print_buffer);
}

void processButton2LpStart()
{
  shrd.button2LpDuration = button2.getPressedTicks();
  Serial.print("processButton2LpStart : ");
  Serial.println(shrd.button2LpDuration);

  char print_buffer[500];
  sprintf(print_buffer, "processButton2LpStart : %d", shrd.button2LpDuration);
  blh.notifyBleLogs(print_buffer);
}

void processButton2LpDuring()
{
  shrd.button2LpDuration = button2.getPressedTicks();

  if ((shrd.button2LpDuration > settings.getS3F().Button_long_press_duration * 1000) && (!shrd.button2LpProcessed))
  {

    char print_buffer[500];
    sprintf(print_buffer, "processButton2LpDuring : %d ==> process", shrd.button2LpDuration);
    blh.notifyBleLogs(print_buffer);

    processAuxEvent(2, true);
    processSpeedLimiterEvent(2, true);
    processLockEvent(2, true);
    shrd.button2LpProcessed = true;
  }
}

void processButton2LpStop()
{
  Serial.print("processButton2LpStop : ");
  Serial.println(shrd.button2LpDuration);

  char print_buffer[500];
  sprintf(print_buffer, "processButton2LpStop : %d", shrd.button2LpDuration);
  blh.notifyBleLogs(print_buffer);

  shrd.button2LpProcessed = false;
  shrd.button2LpDuration = 0;
}

void processButton2()
{
  if (shrd.button2ClickStatus == ACTION_ON)
  {
    digitalWrite(PIN_OUT_LED_BUTTON2, HIGH);
  }
  else if (shrd.button2ClickStatus == ACTION_OFF)
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

void processRelay()
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

  voltageRaw = analogRead(PIN_IN_VOLTAGE);

  // eject false reading
  if (voltageRaw == 4095)
  {
    /*
    Serial.print("Voltage read : ");
    Serial.print(voltageRaw);
    Serial.println(" => eject ");
*/

    char print_buffer[500];
    sprintf(print_buffer, "Voltage read 4095 ==> eject");
    blh.notifyBleLogs(print_buffer);

    return;
  }
  if (voltageRaw < 900)
  {
    /*
    Serial.print("Voltage read : ");
    Serial.print(voltageRaw);
    Serial.println(" => eject ");
    */

    char print_buffer[500];
    sprintf(print_buffer, "Voltage read <900 ==> eject");
    blh.notifyBleLogs(print_buffer);

    return;
  }

  //double correctedValue = -0.000000000000016 * pow(voltageRaw, 4) + 0.000000000118171 * pow(voltageRaw, 3) - 0.000000301211691 * pow(voltageRaw, 2) + 0.001109019271794 * voltageRaw + 0.034143524634089;

  if ((shrd.batteryMaxVoltageCalibRaw != 0xffffffff) && ((shrd.batteryMinVoltageCalibRaw == 0xffffffff)))
  {
    // single point calibration
    voltageInMilliVolts = voltageRaw * shrd.batteryMaxVoltageCalibUser / shrd.batteryMaxVoltageCalibRaw * 1000.0;

    //    Serial.println("algo A / voltageRaw = " + (String)voltageRaw + " / voltageInMilliVolts = " + (String)voltageInMilliVolts);
  }
  else if ((shrd.batteryMaxVoltageCalibRaw != 0xffffffff) && ((shrd.batteryMinVoltageCalibRaw != 0xffffffff)))
  {
    // dual point calibration
    double a = (shrd.batteryMaxVoltageCalibUser - shrd.batteryMinVoltageCalibUser) / (shrd.batteryMaxVoltageCalibRaw - shrd.batteryMinVoltageCalibRaw);
    double b = shrd.batteryMaxVoltageCalibUser - (a * shrd.batteryMaxVoltageCalibRaw);

    voltageInMilliVolts = ((voltageRaw * a) + b) * 1000;

    //    Serial.println("algo B / voltageRaw = " + (String)voltageRaw + " / a = " + (String)a + " / b = " + (String)b + " / voltageInMilliVolts = " + (String)voltageInMilliVolts);
  }
  else
  {
    // standard internal calibration
    voltageInMilliVolts = ((voltageRaw * ANALOG_TO_VOLTS_A) + ANALOG_TO_VOLTS_B) * 1000;
    //    Serial.println("algo C / voltageRaw = " + (String)voltageRaw + " / voltageInMilliVolts = " + (String)voltageInMilliVolts);
  }

  shrd.voltageActual = voltageInMilliVolts;
  voltageFilter.in(voltageInMilliVolts);
  voltageRawFilter.in(voltageRaw);
  shrd.voltageFilterMean = voltageFilter.getMean();
  shrd.voltageRawFilterMean = voltageRawFilter.getMean();

#if DEBUG_DISPLAY_VOLTAGE
  Serial.print("Voltage read : ");
  Serial.print(voltageRaw);
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
  sprintf(print_buffer, "Voltage / read : %d / mean : %d / mean volts : %0.1f ", voltageRaw, voltageRawFilter.getMean(), voltageFilter.getMean() / 1000.0);
  blh.notifyBleLogs(print_buffer);
#endif
}

void processVoltageLongFilter()
{
  voltageLongFilter.in(shrd.voltageFilterMean);
  shrd.voltageFilterLongMean = voltageLongFilter.getMean();
}

void processAutonomy()
{

  uint8_t autonomyLeft;
  uint32_t correctedVoltage = 0;
  float correction = 0.0;
  int32_t voltageDiff = 0;
  float currentFactor = 0.0;

  // Compute battery level with or without current sensor
  if (!shrd.currentSensorPresent)
  {
    // without current sensing ... directly compute from voltage reading
    shrd.batteryLevel = batt.level(shrd.voltageFilterMean);
  }
  else
  {
    // compute with fake battery voltage based on mean voltage value corrected with realtime current
    if (shrd.currentActual > 1000) // 1A
    {
      voltageDiff = (shrd.voltageFilterLongMean - shrd.voltageActual);
      currentFactor = (1.0 + (shrd.currentActual / 1000.0));
      correction = voltageDiff / currentFactor;
      correctedVoltage = shrd.voltageFilterLongMean - correction;
      shrd.batteryLevel = batt.level(correctedVoltage);
    }
    else
    {
      shrd.batteryLevel = batt.level(shrd.voltageActual);
    }
  }

  autonomyLeft = (settings.getS3F().Battery_max_distance / 10) * (shrd.batteryLevel) / 100.0;

  autonomyLeftFilter.in(autonomyLeft);
  shrd.autonomyFilterMean = autonomyLeftFilter.getMean();

#if DEBUG_DISPLAY_AUTONOMY
  Serial.println("bat level : " + (String)shrd.batteryLevel +
                 " / voltageInMilliVolts = " + voltageInMilliVolts +
                 " / autonomy = " + (String)autonomyLeft +
                 " / autonomyFilterMean = " + (String)shrd.autonomyFilterMean +
                 " / bat dst = " + (String)(settings.getS3F().Battery_max_distance / 10) +
                 " / voltageFilterLongMean = " + (String)shrd.voltageFilterLongMean +
                 " / voltageActual = " + (String)shrd.voltageActual +
                 " / currentActual = " + (String)shrd.currentActual +
                 " / voltageDiff = " + (String)voltageDiff +
                 " / currentFactor = " + (String)currentFactor +
                 " / correction = " + (String)correction +
                 " / correctedVoltage = " + (String)correctedVoltage);
#endif
}

void processCurrent()
{

  if (shrd.currentSensorPresent == 1)
  {
    int currentRead = analogRead(PIN_IN_CURRENT);
    int currentInMillamps = (currentRead - currentFilterInit.getMean()) * (1000.0 / ANALOG_TO_CURRENT);
    shrd.currentActual = currentInMillamps;

    if ((shrd.speedCurrent == 0) && (shrd.currentCalibOrder > 0))
    {

      if (currentRead < ANALOG_CURRENT_MIN_RAW_READING)
      {
        shrd.currentSensorPresent = false;
        Serial.println("Current sensor is not detected -> disable reading");

        // saturate filter with 0 values
        for (int i = 0; i < NB_CURRENT_FILTER_DATA; i++)
          currentFilter.in(0);
      }
      else
      {

        shrd.currentCalibOrder--;
        currentFilterInit.in(currentRead);

#if DEBUG_DISPLAY_CURRENT
        if (shrd.currentCalibOrder == 1)
          Serial.println("Current calibration end ... ");
#endif
      }
    }

    if (shrd.currentSensorPresent)
    {
      // current rest value
      currentFilter.in(currentInMillamps);
      shrd.currentFilterMean = currentFilter.getMeanWithoutExtremes(10);
    }

#if DEBUG_DISPLAY_CURRENT
    Serial.print("Current read : ");
    Serial.print(currentRead);
    Serial.print(" / currentFilterInit mean : ");
    Serial.print(currentFilterInit.getMean());
    Serial.print(" / in amperes : ");
    Serial.println(currentInMillamps / 1000.0);
#endif
  }
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

#if CONTROLLER_TYPE == CONTROLLER_MINIMOTORS
  minomoCntrl.processMinimotorsSerial();
#elif CONTROLLER_TYPE == CONTROLLER_VESC
  if (i_loop % 10 == 1)
  {
    //Serial.println(">>>>>>>>>>> readVescValues");

    vescCntrl.requestVescValues();
  }

  if (i_loop % 10 == 9)
  {
    //Serial.println(">>>>>>>>>>> processVescSerial");
    processVescSerial();
  }
#elif CONTROLLER_TYPE == CONTROLLER_KELLY
  if (i_loop % 100 == 1)
  {
    //Serial.println(">>>>>>>>>>> readVescValues");

    kellyCntrl.getKellyValues2();
  }

  if (i_loop % 100 == 12)
  {
    //Serial.println(">>>>>>>>>>> processKellySerial2");
    processKellySerial2();
  }

  if (i_loop % 100 == 20)
  {
    //Serial.println(">>>>>>>>>>> readVescValues");

    kellyCntrl.getKellyValues1();
  }

  if (i_loop % 100 == 22)
  {
    //Serial.println(">>>>>>>>>>> processKellySerial1");
    processKellySerial1();
  }

#elif CONTROLLER_TYPE == CONTROLLER_SMART_ESC

  if (i_loop % 10 == 0)
  {
    //Serial.println(">>>>>>>>>>> readSmartEscValues");

    smartEscCntrl.readSmartEscValues();

    //Serial.println(">>>>>>>>>>> processSmartEscSerial");
    processSmartEscSerial();
  }

  if (i_loop % 10 == 1)
  {
    //Serial.println(">>>>>>>>>>> sendPayload");

    smartEscCntrl.sendPayload();
  }

#endif

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

  processRelay();

#if VOLTAGE_EXT_READ
  if (i_loop % 10 == 0)
  {
    processVoltage();
  }
  if (i_loop % 1000 == 0)
  {
    processVoltageLongFilter();
  }
#endif

  if (i_loop % 1000 == 0)
  {
    processAutonomy();
  }

#if BRAKE_ANALOG_EXT_READ
  if ((i_loop % 10 == 2) || (i_loop % 10 == 7))
  {
    //modifyBrakeFromLCD();
    //displayBrake();
    getBrakeFromAnalog();
  }
#endif

#if CONTROLLER_TYPE == CONTROLLER_MINIMOTORS
  if (i_loop % 10 == 4)
  {
    processCurrent();
  }
#endif

  // keep it fast (/100 not working)
#if TEMPERATURE_EXT_READ
  if (i_loop % 10 == 6)
  {
    processDHT();
  }
#endif

#if TEST_ADC_DAC_REFRESH

  if ((i_loop % 10 == 3) || (i_loop % 10 == 9))
  {
    uint32_t dacOutput = shrd.brakeAnalogValue * 1.2;
    if (dacOutput > 4095)
      dacOutput = 4095;

    //dacOutput = (i_loop / 10) % 4096;
    dac.setVoltage(dacOutput, false);

    char print_buffer[500];
    sprintf(print_buffer, "brake raw : %d / dacOutput : %d",
            brakeAnalogValue,
            dacOutput);
    Serial.println(print_buffer);
  }
#endif

#if READ_THROTTLE
  throttleAnalogValue = analogRead(PIN_IN_THROTTLE);
#endif

#if TFT_ENABLED
  //tftUpdateData(i_loop);
#endif

  // Give a time for ESP
  delay(1);
  //delayMicroseconds(1000);
  //yield();

#if DEBUG_TIMELOOP_NS
  Serial.print("> ");
  Serial.print(micros() - timeLoop - 1000);
  Serial.print(" / i_loop : ");
  Serial.println(i_loop);
  timeLoop = micros();
#endif

#if DEBUG_TIMELOOP_MS
  Serial.print("> ");
  Serial.print(millis() - timeLoop - 1);
  Serial.print(" / i_loop : ");
  Serial.println(i_loop);
  timeLoop = millis();
#endif

  i_loop++;

  resetWatchdog();
}

/////////// End