//////////////////////////////////////////
// TODO : buttons change accel/eco mode
// TODO : LCD error indicators
// TODO : auto mode shift on low battery
// TODO : reduce SHTC3 read time - brake read function in 2 parts
// BUG : Minimo - original regulator perturbation
// BUG : minimo - brake digital lever ?
// BUG : minimo - brake controller ?
// BUG : current init = false max current/power on smartphone ??
//////////////////////////////////////////

//////------------------------------------
////// Inludes

#include "main.h"
#include "BLE/BluetoothHandler.h"
#include "Battery/Battery.h"
#include "Buttons/Buttons.h"
#include "DHT/dht_nonblocking.h"
#include "OTA/OTA_wifi.h"
#include "SHTC3/SparkFun_SHTC3.h"
#include "Settings.h"
#include "SharedData.h"
#include "TFT/tft_main.h"
#include "TFT/tft_settings_menu.h"
#include "debug.h"
#include "esp32-hal-uart.h"
#include "filters/MedianFilter.h"
#include "pinout.h"
#include "tools/utils.h"
#include <MCP4725/Adafruit_MCP4725.h>
#include <Wire.h>
#include "Settings.h"
#include "Settings2.h"

#include "Controllers/ControllerType.h"
#include "Controllers/KellyUart.h"
#include "Controllers/MinimoUart.h"
#include "Controllers/SmartEsc.h"
#include "Controllers/VescUart.h"
#include "Controllers/ZeroUart.h"

//////------------------------------------
////// Defines

// SMART CONFIGURATION
#ifdef BUILD_CONTROLLER_MINIMOTORS
#define CONTROLLER_TYPE CONTROLLER_MINIMOTORS
MinimoUart minomoCntrl;
#endif
#ifdef BUILD_CONTROLLER_VESC
#define CONTROLLER_TYPE CONTROLLER_VESC
VescUart vescCntrl;
#endif
#ifdef BUILD_CONTROLLER_KELLY
#define CONTROLLER_TYPE CONTROLLER_KELLY
KellyUart kellyCntrl;
#endif
#ifdef BUILD_CONTROLLER_SMART_ESC
#define CONTROLLER_TYPE CONTROLLER_SMART_ESC
SmartEsc smartEscCntrl;
#endif
#ifdef BUILD_CONTROLLER_ZERO
#define CONTROLLER_TYPE CONTROLLER_ZERO
ZeroUart zeroCntrl;
#endif

#define DEBUG_ESP_HTTP_UPDATE 1
#define TEMPERATURE_EXT_READ 0
#define TEMPERATURE_INT_READ 1
#define VOLTAGE_EXT_READ 1
#define BRAKE_ANALOG_EXT_READ 1

// I2C
#define I2C_FREQ 1000000

// UART
#define BAUD_RATE_CONSOLE 921600

// ADC
#define ANALOG_TO_VOLTS_5V 0.0012207 // 4096 = 5V
#define ANALOG_TO_VOLTS_A 0.0213
#define ANALOG_TO_VOLTS_B 5.4225
#define ANALOG_TO_CURRENT 35
#define ANALOG_CURRENT_MIN_RAW_READING 100
#define NB_CURRENT_FILTER_DATA 20
#define NB_CURRENT_FILTER_CALIB_DATA 20
#define NB_VOLTAGE_FILTER_DATA 10

// distance
#define SPEED_TO_DISTANCE_CORRECTION_FACTOR 1

#define ENABLE_WATCHDOG 1
#define WATCHDOG_TIMEOUT 1000 // 1s // time in ms to trigger the watchdog

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

DHT_nonblocking dht_sensor(PIN_IN_OUT_DHT, DHT_TYPE_22);

TwoWire I2Cone = TwoWire(0);
Adafruit_MCP4725 dac;
SHTC3 mySHTC3;

SharedData shrd;

int i_loop = 0;

uint32_t iBrakeMinCalibOrder = 0;

uint16_t voltageRaw = 0;
uint32_t voltageInMilliVolts = 0;
Battery batt = Battery(36000, 54000);

uint16_t throttleAnalogValue = 0;

MedianFilter voltageFilter(NB_VOLTAGE_FILTER_DATA, 2000);
MedianFilter voltageLongFilter(NB_VOLTAGE_FILTER_DATA, 50000);
MedianFilter voltageRawFilter(NB_VOLTAGE_FILTER_DATA, 2000);
MedianFilter currentRawFilter(NB_CURRENT_FILTER_DATA, 1830);
MedianFilter currentRawFilterInit(NB_CURRENT_FILTER_CALIB_DATA, 1830);
MedianFilter brakeFilter(10 /* 20 */, 900);
//MedianFilter brakeMaxFilterInit(NB_BRAKE_CALIB_DATA, 900);
MedianFilter throttleFilter(10 /* 20 */, 900);

Buttons btns;

Settings settings;
Settings2 settings2;
BluetoothHandler blh;

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
  pinMode(PIN_OUT_RELAY, OUTPUT);
  pinMode(PIN_OUT_BRAKE, OUTPUT);
  pinMode(PIN_OUT_LED_BUTTON1, OUTPUT);
  pinMode(PIN_IN_ABRAKE, INPUT);
  pinMode(PIN_IN_ATHROTTLE, INPUT);
  pinMode(PIN_OUT_FREE, OUTPUT);
  //digitalWrite(PIN_OUT_FREE, 0);
}

#if HAS_I2C
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
  dac.setVoltage(0, true);
}

void setupShtc3()
{
  SHTC3_Status_TypeDef err = mySHTC3.begin(I2Cone);
  if (err != SHTC3_Status_Nominal)
  {
    Serial.print("SHTC3 error");
  }
  mySHTC3.setMode(SHTC3_CMD_CSE_TF_LPM);
  mySHTC3.wake(true);
}

#endif

void setupSerial()
{

#if CONTROLLER_TYPE == CONTROLLER_MINIMOTORS

  minomoCntrl.setSettings(&settings);
  minomoCntrl.setSharedData(&shrd);
  minomoCntrl.setBluetoothHandler(&blh);

  // minimotor controller
  hwSerCntrl.begin(MINIMO_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  minomoCntrl.setControllerSerialPort(&hwSerCntrl);

  // minimotor display
  hwSerLcd.begin(MINIMO_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_LCD_TO_ESP, PIN_SERIAL_ESP_TO_LCD);
  minomoCntrl.setLcdSerialPort(&hwSerLcd);

  hwSerCntrl.setUartIrqIdleTrigger(0);
  hwSerLcd.setUartIrqIdleTrigger(0);

#elif CONTROLLER_TYPE == CONTROLLER_ZERO

  zeroCntrl.setSettings(&settings);
  zeroCntrl.setSharedData(&shrd);
  zeroCntrl.setBluetoothHandler(&blh);

  // zero controller
  hwSerCntrl.begin(ZERO_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  zeroCntrl.setControllerSerialPort(&hwSerCntrl);

  // zero display
  hwSerLcd.begin(ZERO_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_LCD_TO_ESP, PIN_SERIAL_ESP_TO_LCD);
  zeroCntrl.setLcdSerialPort(&hwSerLcd);

  hwSerCntrl.setUartIrqIdleTrigger(0);
  hwSerLcd.setUartIrqIdleTrigger(0);

#elif CONTROLLER_TYPE == CONTROLLER_VESC
  hwSerCntrl.begin(BAUD_RATE_VESC, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  vescCntrl.setSerialPort(&hwSerCntrl);
  vescCntrl.setup(&shrd, &blh, &settings);

#if DEBUG_DISPLAY_VESC_FRAME
  vescCntrl.setDebugPort(&Serial);
#endif
  //  vescCntrl.requestMotorConfig();
  //vescCntrl.setDebugPort(NULL);

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

void setupBattery()
{
  batt = Battery(settings.get_Battery_minimum_voltage() * 1000, settings.get_Battery_maximum_voltage() * 1000);

  Serial.println("Battery : min = " + (String)settings.get_Battery_minimum_voltage() + " / max = " + settings.get_Battery_maximum_voltage());
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
  shrd.autonomyLeft = (settings.get_Battery_maximum_distance()) * (shrd.batteryLevel) / 100.0;
}

void saveBleLockForced()
{
  settings2.saveBleLockForced();
}
void saveBrakeMinPressure()
{
  settings2.saveBrakeMinPressure();
}
void saveBrakeMaxPressure()
{
  settings2.saveBrakeMaxPressure();
}
void saveOdo()
{
  settings2.saveOdo();
}
void saveBatteryCalib()
{
  settings2.saveBatteryCalib();
}
void initSharedDataWithSettings()
{
  shrd.speedLimiter = (settings.get_Speed_limiter_at_startup() == 1);
  shrd.pasEnabled = (settings.get_Pas_enabled() == 1);
  shrd.modeOrder = settings.get_Default_mode_at_startup();
  shrd.ecoOrder = settings.get_Default_eco_mode_at_startup();
  shrd.accelOrder = settings.get_Default_acceleration();
  shrd.brakeSentOrder = settings.get_Default_electric_brake_at_startup();
  shrd.brakeSentOrderFromBLE = settings.get_Default_electric_brake_at_startup();
}

#if ENABLE_WATCHDOG
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
#endif

void taskUpdateTFT(void *parameter)
{
  int i = -2;

  // infinite loop
  for (;;)
  {
    shrd.oldInSettingsMenu = shrd.inSettingsMenu;
    shrd.inSettingsMenu = settings_menu_enabled();
    //Serial.println("oldInSettingsMenu = " + (String)oldInSettingsMenu + " / inSettingsMenu = " + (String)inSettingsMenu);

    // not currently in settings menu
    if (!shrd.inSettingsMenu)
    {

      // was in settings menu
      if (shrd.oldInSettingsMenu)
      {
        // force main display reset
        i = -1;

        btns.setSlowButtonBehavior(false);
      }
      else
      {
        tftUpdateData(i);
        i++;

        if (i >= 20)
        {
          i = 0;
          vTaskDelay(200);
        }
      }
    }
    else
    {
      // entering in settings menu
      if (!shrd.oldInSettingsMenu)
      {
        Serial.println("settings_menu_setup");
        btns.setSlowButtonBehavior(true);
        settings_menu_setup();
      }
      settings_menu_loop();
      vTaskDelay(100);
    }

    // test
    //vTaskDelay(200);
  }
}

////// Setup
void setup()
{

  esp_bt_sleep_disable();
  esp_ble_scan_dupilcate_list_flush();

  // Initialize the Serial (use only in setup codes)
  Serial.begin(BAUD_RATE_CONSOLE);

  Serial.printf("\n\nfirmware : type = %s / version : %d\n", FIRMWARE_TYPE, FIRMWARE_VERSION);
  Serial.println("\nsetup --- begin :");

#if TFT_ENABLED
  Serial.println("   TFT backligth... ");
  tftSetupBacklight();
#endif

  shrd.timeLastNotifyBle = millis();

  Serial.println("   serial... ");
  setupSerial();

#if HAS_I2C
  Serial.println("   I2C...");
  setupI2C();

  Serial.println("   DAC...");
  setupDac();

  Serial.println("   SHT3...");
  setupShtc3();
#endif

  Serial.println("   pins...");
  setupPins();

  Serial.println("   buttons...");
  btns.setup(&shrd, &blh, &settings);

#if TFT_ENABLED
  Serial.println("   TFT... ");
  tftSetup(&shrd, &settings);
#endif

  // force BLE lock mode
  blh.setBleLock(false);

  Serial.println("   prefs... ");
  settings2.setSharedData(&shrd);
  settings2.restore();

  Serial.println("   settings ...");
  settings.restore();
  settings_menu_set_settings(&settings);

  Serial.println("   init data with settings... ");
  initSharedDataWithSettings();

  Serial.println("   BLE ...");
  blh.setSettings(&settings);
  blh.setSharedData(&shrd);

  setupVoltage();
  setupBattery();
  setupAutonomy();

#if TFT_ENABLED
  xTaskCreatePinnedToCore(
      taskUpdateTFT,   // Function that should be called
      "taskUpdateTFT", // Name of the task (for debugging)
      10000,           // Stack size (bytes)
      NULL,            // Parameter to pass
      0,               // Task priority
      NULL,            // Task handle,
      1);              // Core
#endif

#if ENABLE_WATCHDOG
  Serial.println(PSTR("Watchdog enabled"));
  setupWatchdog();
#endif
  // End of setup
  Serial.println("setup --- end\n");
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
  Serial.println(shrd.brakePressedStatus);
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

void checkAndSaveOdo()
{

  if ((shrd.speedOldForOdo != 0.00) && (shrd.speedCurrent == 0.00)) /* save when speed become 0 */
  {
#if DEBUG_DISPLAY_SAVE_ODO
    Serial.println("Save ODO ... YES - speed condition / speedOldForOdo = " + (String)shrd.speedOldForOdo + " / speedCurrent = " + (String)shrd.speedCurrent);
#endif

    shrd.distanceOdoInFlash = shrd.distanceOdo;
    settings2.saveOdo();
  }
  else if (shrd.distanceOdo > shrd.distanceOdoInFlash + 10) /* save every kilometer */
  {
#if DEBUG_DISPLAY_SAVE_ODO
    Serial.println("Save ODO ... YES - distance condition/ distanceOdo = " + (String)shrd.distanceOdo + " / distanceOdoInFlash = " + (String)shrd.distanceOdoInFlash);
#endif

    shrd.distanceOdoInFlash = shrd.distanceOdo;
    settings2.saveOdo();
  }
  else
  {
#if DEBUG_DISPLAY_SAVE_ODO
    Serial.println("Save ODO ... no / speedOldForOdo = " + (String)shrd.speedOldForOdo + " / speedCurrent = " + (String)shrd.speedCurrent);
#endif
  }

  shrd.speedOldForOdo = shrd.speedCurrent;
}

void computeDistance(float speed)
{

  // calculate distance
  uint32_t distanceCurTime = millis();
  uint32_t distanceDiffTime = distanceCurTime - shrd.distancePrevTime;

#if DEBUG_DISPLAY_DISTANCE
  uint32_t oldDistanceTrip = shrd.distanceTrip;
#endif

  uint32_t tripDiff = ((speed * (distanceDiffTime)) / 360) * SPEED_TO_DISTANCE_CORRECTION_FACTOR;
  shrd.distanceTrip = shrd.distanceTrip + tripDiff;
  shrd.distanceTripForOdo = shrd.distanceTripForOdo + tripDiff;
  shrd.distancePrevTime = millis();

  shrd.distanceOdo = shrd.distanceOdoBoot + (shrd.distanceTripForOdo / 1000);

  // if BLE is not connected, save ODO in the main thread ... else save on BLE keep alive
  if (blh.deviceStatus == BLE_STATUS_DISCONNECTED)
  {
    checkAndSaveOdo();
  }

#if DEBUG_DISPLAY_DISTANCE
  Serial.println("distanceTrip = " + (String)(shrd.distanceTrip) +
                 " / oldDistanceTrip = " + (String)oldDistanceTrip +
                 " / distanceDiffTime = " + (String)distanceDiffTime +
                 " / speed = " + (String)speed +
                 " / shrd.distanceOdoInFlash = " + (String)shrd.distanceOdoInFlash +
                 " / shrd.distanceOdo = " + (String)shrd.distanceOdo);
#endif
}

void changeBrakeIOState()
{
  // alarm controler from braking
  if ((shrd.brakePercent > 0) && (!shrd.brakeFordidenHighVoltage))
  {

    digitalWrite(PIN_OUT_BRAKE, 1);

    if (shrd.brakePressedStatus == 0)
    {
      char print_buffer[500];
      sprintf(print_buffer, ">>>> brake IO ON");

      Serial.println(print_buffer);

      blh.notifyBleLogs(print_buffer);
    }

    shrd.brakePressedStatus = 1;
  }
  else
  {
    digitalWrite(PIN_OUT_BRAKE, 0);

    if (shrd.brakePressedStatus == 1)
    {
      char print_buffer[500];
      sprintf(print_buffer, ">>>> brake IO OFF");

      Serial.println(print_buffer);

      blh.notifyBleLogs(print_buffer);
    }

    shrd.brakePressedStatus = 0;
  }
}

void notifySettingsChangedWithBle()
{
  blh.notifySettingsChanged();
}

void notifyBrakeWithBle()
{

  if ((shrd.brakeSentOrder != shrd.brakeSentOrderOld) || (shrd.brakePressedStatus != shrd.brakePressedStatusOld))
  {
    blh.notifyCommandsFeedback();

#if DEBUG_DISPLAY_ANALOG_BRAKE
    Serial.print("brake notify : ");
    Serial.println(shrd.brakeSentOrder);
#endif

    char print_buffer[500];
    sprintf(print_buffer, ">> brakeNotify = f2 : %d / brakeMinPressureRaw : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d / init : %d / forbid : %d",
            shrd.brakeFilterMeanErr,
            shrd.brakeMinPressureRaw,
            shrd.brakeAnalogValue,
            shrd.brakeSentOrder,
            shrd.brakeSentOrderOld,
            shrd.brakePressedStatus,
            shrd.brakeMaxPressureRaw,
            shrd.brakeFordidenHighVoltage);
    blh.notifyBleLogs(print_buffer);
  }

  shrd.brakePressedStatusOld = shrd.brakePressedStatus;
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
            shrd.brakeMaxPressureRaw,
            shrd.brakeFordidenHighVoltage);

    Serial.println(print_buffer);

    blh.notifyBleLogs(print_buffer);
  }
#endif
}

void getBrakeFromAnalog()
{

  shrd.brakeAnalogValue = analogRead(PIN_IN_ABRAKE);

  if (settings.get_Ebrake_smart_brake_type() == settings.LIST_Ebrake_smart_brake_type_Smart__analog_brake_lever)
  {
    shrd.brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);

    // if brake is pressed at startup, disable speed limiter
    if ((shrd.brakeFilterMeanErr > shrd.brakeMaxPressureRaw - ANALOG_BRAKE_MIN_OFFSET) && (i_loop < 100))
    {
      shrd.speedLimiter = 0;

      Serial.println("getBrakeFromAnalog - speedLimiter = " + (String)shrd.speedLimiter + " / notifySpeedLimiterStatus => disabled by brake / " + (String)shrd.speedLimiter);

      shrd.errorBrake = false;

      return;
    }

    // ignore out of range datas ... and notify
    else if (shrd.brakeAnalogValue < ANALOG_BRAKE_MIN_ERR_VALUE)
    {
#if DEBUG_DISPLAY_ANALOG_BRAKE
      Serial.println("brake ANALOG_BRAKE_MIN_ERR_VALUE");
#endif
      /*
      char print_buffer[500];
      sprintf(print_buffer, "brake ANALOG_BRAKE_MIN_ERR_VALUE / f2 : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d",
              shrd.brakeFilterMeanErr,
              shrd.brakeAnalogValue,
              shrd.brakeSentOrder,
              shrd.brakeSentOrderOld,
              shrd.brakePressedStatus);
      Serial.println(print_buffer);
*/
      shrd.errorBrake = true;

      return;
    }
    else
    {
      shrd.errorBrake = false;
    }

    // ignore out of range datas ... and notify
    if (shrd.brakeAnalogValue > ANALOG_BRAKE_MAX_ERR_VALUE)
    {
#if DEBUG_DISPLAY_ANALOG_BRAKE
      Serial.println("brake ANALOG_BRAKE_MAX_ERR_VALUE");
      char print_buffer[500];
      sprintf(print_buffer, "brake ANALOG_BRAKE_MAX_ERR_VALUE / f2 : %d / raw : %d / sentOrder : %d / sentOrderOld : %d / status : %d",
              shrd.brakeFilterMeanErr,
              shrd.brakeAnalogValue,
              shrd.brakeSentOrder,
              shrd.brakeSentOrderOld,
              shrd.brakePressedStatus);

      // too fast for BLE !!!
      //blh.notifyBleLogs(print_buffer);

      Serial.println(print_buffer);
#endif
      return;
    }

    if (shrd.brakeAnalogValue > shrd.brakeMaxPressureRaw)
      shrd.brakeAnalogValue = shrd.brakeMaxPressureRaw;

    brakeFilter.in(shrd.brakeAnalogValue);

    if (settings.get_Ebrake_progressive_mode() == 1)
    {
      shrd.brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);

      // calculate pressure percentage
      float brakePercent = map(shrd.brakeFilterMeanErr, shrd.brakeMinPressureRaw + ANALOG_BRAKE_MIN_OFFSET, shrd.brakeMaxPressureRaw, 0, 100);
      shrd.brakePercent = constrain(brakePercent, 0, 100);

      // Serial.println("brakePercent = " + (String)shrd.brakePercent);

      // alarm controler from braking throught IO output
      changeBrakeIOState();

      // notify brake LCD value
      notifyBrakeWithBle();
    }
  }
  else if (settings.get_Ebrake_smart_brake_type() == settings.LIST_Ebrake_smart_brake_type_Smart__digital_brake_lever)
  {
    //Serial.println("getBrakeFromAnalog - digital - shrd.brakeFilterMeanErr = " + (String)shrd.brakeFilterMeanErr + " / shrd.brakeAnalogValue = " + (String)shrd.brakeAnalogValue);

    shrd.brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);

    // if brake is pressed at startup, disable speed limiter
    if ((shrd.brakeFilterMeanErr > ANALOG_BRAKE_DIGITIAL_MIN_OFFSET) && (i_loop < 100))
    {
      shrd.speedLimiter = 0;

      Serial.println("getBrakeFromAnalog - speedLimiter = " + (String)shrd.speedLimiter + " / notifySpeedLimiterStatus => disabled by brake / " + (String)shrd.speedLimiter);
      
      shrd.errorBrake = false;

      return;
    }

    brakeFilter.in(shrd.brakeAnalogValue);

    if (settings.get_Ebrake_progressive_mode() == 1)
    {
      shrd.brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);

      if (shrd.brakeFilterMeanErr > ANALOG_BRAKE_DIGITIAL_MIN_OFFSET)
      {
        if (shrd.brakeDigitalTimeStart == 0xffffffff)
        {
          shrd.brakeDigitalTimeStart = millis();
          Serial.println("start braking at = " + (String)shrd.brakeDigitalTimeStart);
        }

        // calculate pressure percentage
        uint8_t nbSteps = (settings.get_Ebrake_max_power_value() - settings.get_Ebrake_min_power_value());
        uint32_t fullBrakeDuration = settings.get_Ebrake_time_between_mode_shift() * nbSteps;
        shrd.brakePercent = map(millis(), shrd.brakeDigitalTimeStart, shrd.brakeDigitalTimeStart + fullBrakeDuration, 0, 100);
        shrd.brakePercent = constrain(shrd.brakePercent, 0, 100);

        // Serial.println("nbSteps = " + (String)nbSteps + " / fullBrakeDuration = " + (String)fullBrakeDuration + " / brakePercent = " + (String)shrd.brakePercent +
        // " / millis() = " + (String)millis() + " / brakeDigitalTimeStart = " + (String)shrd.brakeDigitalTimeStart);
      }
      else if ((shrd.brakeFilterMeanErr < ANALOG_BRAKE_DIGITIAL_MIN_OFFSET) && (shrd.brakeDigitalTimeStart != 0xffffffff))
      {
        shrd.brakeDigitalTimeStart = 0xffffffff;
        Serial.println("stop braking at = " + (String)millis());

        shrd.brakePercent = 0;
      }

      // Serial.println("brakePercent = " + (String)shrd.brakePercent);

      // alarm controler from braking throught IO output
      changeBrakeIOState();

      // notify brake LCD value
      notifyBrakeWithBle();
    }
  }
  else if (settings.get_Ebrake_smart_brake_type() == settings.LIST_Ebrake_smart_brake_type_Smart__digital_brake__throttle_exp)
  {

    shrd.brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);

    //Serial.println("getBrakeFromAnalog - digital - shrd.brakeFilterMeanErr = " + (String)shrd.brakeFilterMeanErr + " / shrd.brakeAnalogValue = " + (String)shrd.brakeAnalogValue);

    // if brake is pressed at startup, disable speed limiter
    if ((shrd.brakeFilterMeanErr > ANALOG_BRAKE_DIGITIAL_MIN_OFFSET) && (i_loop < 100))
    {
      shrd.speedLimiter = 0;

      Serial.println("getBrakeFromAnalog - speedLimiter = " + (String)shrd.speedLimiter + " / notifySpeedLimiterStatus => disabled by brake / " + (String)shrd.speedLimiter);

      shrd.errorBrake = false;

      return;
    }

    brakeFilter.in(shrd.brakeAnalogValue);

    if (settings.get_Ebrake_progressive_mode() == 1)
    {
      shrd.brakeFilterMeanErr = brakeFilter.getMeanWithoutExtremes(1);

      if (shrd.brakeFilterMeanErr > ANALOG_BRAKE_DIGITIAL_MIN_OFFSET)
      {

        // calculate pressure percentage
        shrd.brakePercent = constrain(shrd.throttlePercent + 10, 10, 100);

        // Serial.println("nbSteps = " + (String)nbSteps + " / fullBrakeDuration = " + (String)fullBrakeDuration + " / brakePercent = " + (String)shrd.brakePercent +
        // " / millis() = " + (String)millis() + " / brakeDigitalTimeStart = " + (String)shrd.brakeDigitalTimeStart);
      }
      else if (shrd.brakeFilterMeanErr < ANALOG_BRAKE_DIGITIAL_MIN_OFFSET)
      {
        shrd.brakePercent = 0;
      }

      // Serial.println("brakePercent = " + (String)shrd.brakePercent);

      // alarm controler from braking throught IO output
      changeBrakeIOState();

      // notify brake LCD value
      notifyBrakeWithBle();
    }
  }
}

void getThrottleFromAnalog()
{

#define TMIN_MARGIN 0.75
#define TMAX_MARGIN 1.25

  uint32_t tInMin = settings.get_Throttle_input_min_voltage();
  uint32_t tInMax = settings.get_Throttle_input_max_voltage();

  // Read and filter ADC
  throttleAnalogValue = analogRead(PIN_IN_ATHROTTLE);
  shrd.throttleAnalogValue = throttleAnalogValue;
  throttleFilter.in(throttleAnalogValue);

  // Compute throttle voltage in 0-5V range
  uint32_t throttleInMillv = throttleAnalogValue * ANALOG_TO_VOLTS_5V * 1000;

  // ignore out of range datas ... and notify
  if (throttleInMillv < tInMin * TMIN_MARGIN)
  {
    /*
    char print_buffer[500];
    sprintf(print_buffer, "throttle : value too low / tAnalogValue : %d / throttleFilter.getMean() : %d / tInMin : %d / tInMin with margin : %d",
            throttleAnalogValue,
            throttleFilter.getMean(),
            tInMin,
            (uint32_t) (tInMin * TMIN_MARGIN)
            );
    blh.notifyBleLogs(print_buffer);
    Serial.println(print_buffer);
*/
    shrd.errorThrottle = true;

    return;
  }

  // ignore out of range datas ... and notify
  if (throttleInMillv > tInMax * TMAX_MARGIN)
  {
    char print_buffer[500];
    sprintf(print_buffer, "throttle : value too high / tAnalogValue : %d / throttleFilter.getMean() : %d / tInMax : %d / tInMax with margin : %d",
            throttleAnalogValue,
            throttleFilter.getMean(),
            tInMax,
            (uint32_t)(tInMax * TMAX_MARGIN));
    blh.notifyBleLogs(print_buffer);
    Serial.println(print_buffer);

    shrd.errorThrottle = true;

    return;
  }
  else
  {
    shrd.errorThrottle = false;
  }
}

void processDacOutput()
{
  uint32_t filteredThrottleIn = throttleFilter.getMeanWithoutExtremes(2);
  uint32_t throttleInMillv = filteredThrottleIn * ANALOG_TO_VOLTS_5V * 1000;

  uint32_t tInMin = settings.get_Throttle_input_min_voltage();
  uint32_t tInMax = settings.get_Throttle_input_max_voltage();

  uint32_t rangeInMilliv = tInMax - tInMin;

  // map to percent range
  float throttlePercent = map(throttleInMillv - tInMin, 0, rangeInMilliv, 0, 100);
  throttlePercent = constrain(throttlePercent, 0, 100);

  // apply exponential curve
  if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Exponential_1)
    throttlePercent = (exp(throttlePercent / 100.0) - 1) / (exp(1) - 1) * 100;
  else if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Exponential_2)
    throttlePercent = (exp(throttlePercent / 100.0 * 2) - 1) / (exp(2) - 1) * 100;
  else if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Exponential_3)
    throttlePercent = (exp(throttlePercent / 100.0 * 3) - 1) / (exp(3) - 1) * 100;
  else if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Exponential_4)
    throttlePercent = (exp(throttlePercent / 100.0 * 4) - 1) / (exp(4) - 1) * 100;

  shrd.throttlePercent = throttlePercent;

  shrd.pasEnabled = settings.get_Pas_enabled();

  uint32_t dacOutput = 0;
  uint32_t outputMilliv = 0;

#if CONTROLLER_TYPE == CONTROLLER_VESC
  uint32_t minBrakeVoltage = 0;
  if (shrd.brakePercent > 1)
  {
    // if brake is forbiden simply cut power
    if (shrd.brakeFordidenHighVoltage)
    {
      outputMilliv = 3300 / 2;
    }
    else // ... else apply brake current
    {
      minBrakeVoltage = map(settings.get_Ebrake_maximum_value, 0, 5, 3300 / 2, 0);
      outputMilliv = map(shrd.brakePercent * 16, 100 * 16, 0, minBrakeVoltage, 3300 / 2);
    }
  }
  else
  {
    if (shrd.isLocked) // if locked... 0 current target
    {
      outputMilliv = 3300 / 2;
    }
    else if (((shrd.pasEnabled) && (shrd.speedCurrent > 1)) || (!shrd.pasEnabled)) // normal condition
    {
      outputMilliv = map(throttlePercent * 16, 0, 100 * 16, 3300 / 2, 3300);
    }
    else // souldn't occur
    {
      outputMilliv = 3300 / 2;
    }
  }
#else

  uint32_t tOutMin = settings.get_Throttle_output_min_voltage();
  uint32_t tOutMax = settings.get_Throttle_output_max_voltage();

  if (shrd.isLocked) // if locked... 0 current target
  {
    outputMilliv = 0;
  }
  else if (((shrd.pasEnabled) && (shrd.speedCurrent > 1)) || (!shrd.pasEnabled)) // normal condition
  {
    outputMilliv = map(throttlePercent, 0, 100, tOutMin, tOutMax);
  }
  else // souldn't occur
  {
    outputMilliv = 0;
  }

#endif

  // compute DAC output
  dacOutput = outputMilliv / (ANALOG_TO_VOLTS_5V * 1000);
  dacOutput = constrain(dacOutput, 0, 4095);
  dac.setVoltage(dacOutput, false, I2C_FREQ);

#if DEBUG_DISPLAY_DAC_OUTPUT
  char print_buffer[500];
  sprintf(print_buffer, "filteredThrottleIn : %d / tInMillv : %d / tInMin : %d / tInMax : %d / rangeInMilliv : %d / tPercent = %2.2f / bAnalogValue = %d / bFilterMeanErr = %d / bPercent = %2.2f / outputMilliv = %d / dacOutput = %d",
          filteredThrottleIn,
          throttleInMillv,
          tInMin,
          tInMax,
          rangeInMilliv,
          shrd.throttlePercent,
          shrd.brakeAnalogValue,
          shrd.brakeFilterMeanErr,
          shrd.brakePercent,
          outputMilliv,
          dacOutput);

  if (millis() % 500 == 0)
  {
    blh.notifyBleLogs(print_buffer);
    Serial.println(print_buffer);
  }
#endif
}

bool isElectricBrakeForbiden()
{
  if (settings.get_Ebrake_disabled_on_high_battery_voltage() == 0)
  {
#if DEBUG_DISPLAY_BRAKE_FORBIDEN
    Serial.println("electric brake not disabled on battery high voltage");
#endif
    return false;
  }

  return (shrd.batteryLevel > settings.get_Ebrake_disabled_percent_limit());
}

#if (CONTROLLER_TYPE == CONTROLLER_VESC)
void processVescSerial()
{

  // simulate brake force from 0 to 5 for smartphone feedback
  shrd.brakeSentOrder = map(shrd.brakePercent, 0, 100, 0, settings.get_Ebrake_maximum_value);

  // synchronous - set mode 1/2/3
  vescCntrl.setMaxSpeed(shrd.modeOrder);

  if (vescCntrl.readVescSerialFeedback())
  {

    int32_t rpm = vescCntrl.data.rpm;
    if (rpm < 0)
      rpm = 0;
    float speedCompute = ErpmToKmh2(&settings, rpm);
    // Serial.println("rpm = " + (String) rpm + " / speedCompute = " + (String) speedCompute + " / rpmCompute = " + rpmCompute);

    if (speedCompute > shrd.speedMax)
      shrd.speedMax = speedCompute;

    shrd.speedCurrent = speedCompute;
    shrd.voltageFilterMean = vescCntrl.data.inpVoltage * 1000;
    shrd.voltageActual = vescCntrl.data.inpVoltage * 1000;
    shrd.currentActual = vescCntrl.data.avgInputCurrent * 1000;
    shrd.currentTemperature = vescCntrl.data.tempMosfet;

    //Serial.println("voltageFilterMean = " + (String)shrd.voltageFilterMean + " / currentActual = " + (String)shrd.currentActual);

    computeDistance(speedCompute);
  }
}
#endif

#if (CONTROLLER_TYPE == CONTROLLER_KELLY)
void processKellySerial1()
{

  shrd.voltageFilterMean = kellyCntrl.data1.B_Voltage * 1000;
  shrd.currentTemperature = kellyCntrl.data1.Controller_temperature;

  shrd.brakePressedStatus = kellyCntrl.data1.BRK_SW;

  // notify brake LCD value
  if ((shrd.brakeSentOrder != shrd.brakeSentOrderOld) || (shrd.brakePressedStatus != shrd.brakePressedStatusOld))
  {
    //    blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakePressedStatus, shrd.brakeFordidenHighVoltage);
    blh.notifyCommandsFeedback();
  }

  shrd.brakePressedStatusOld = shrd.brakePressedStatus;
}

void processKellySerial2()
{

  // 532 RPM = 25 km/h
  // 1814 =
  // TODO : check calculation - if motor poles numbers
  float speedCompute = RpmToKmh(&settings, kellyCntrl.data2.Mechanical_speed_in_RPM);
  if (speedCompute < 0)
    speedCompute = 0;
  if (speedCompute > 999)
    speedCompute = 999;

  if (speedCompute > shrd.speedMax)
    shrd.speedMax = speedCompute;

  shrd.speedCurrent = speedCompute;
}
#endif

#if (CONTROLLER_TYPE == CONTROLLER_SMART_ESC)
void processSmartEscSerial()
{

  shrd.voltageFilterMean = (uint32_t)(smartEscCntrl.data.Controller_Voltage) * 10;
  shrd.currentActual = (uint32_t)(smartEscCntrl.data.Controller_Current) * 10 / 3.5;
  shrd.currentTemperature = smartEscCntrl.data.MOSFET_temperature;
  if (smartEscCntrl.data.ERPM < 0)
    smartEscCntrl.data.ERPM = 0;
  shrd.speedCurrent = RpmToKmh(&settings, smartEscCntrl.data.ERPM);

  //Serial.println("voltageFilterMean " + (String)shrd.voltageFilterMean + " / shrd.currentFilterMean = " + (String)shrd.currentFilterMean + " / currentTemperature = " + (String)shrd.currentTemperature + " / ERPM = " + (String)smartEscCntrl.data.ERPM + " / speedCurrent = " + (String)shrd.speedCurrent + " / Throttle = " + (String)smartEscCntrl.data.Throttle);

  // notify brake LCD value
  if ((shrd.brakeSentOrder != shrd.brakeSentOrderOld) || (shrd.brakePressedStatus != shrd.brakePressedStatusOld))
  {
    //    blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakePressedStatus, shrd.brakeFordidenHighVoltage);
    blh.notifyCommandsFeedback();
  }

  shrd.brakePressedStatusOld = shrd.brakePressedStatus;
}
#endif

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

void processSHTC3(bool requestRead)
{

  if (requestRead)
  {
    mySHTC3.requestDatas(); // Call "update()" to command a measurement, wait for measurement to complete, and update the RH and T members of the object
  }
  else
  {
    mySHTC3.readDatas();
#if DEBUG_DISPLAY_SHTC3
    Serial.print(mySHTC3.toPercent()); // "toPercent" returns the percent humidity as a floating point number
    Serial.print("% / ");
    Serial.print("T = ");
    Serial.print(mySHTC3.toDegC()); // "toDegF" and "toDegC" return the temperature as a flaoting point number in deg F and deg C respectively
    Serial.print(" deg C\n");
#endif

#if CONTROLLER_TYPE == CONTROLLER_MINIMOTORS
    shrd.currentTemperature = mySHTC3.toDegC();
#endif
    shrd.currentHumidity = mySHTC3.toPercent();
  }
}

void processVoltage()
{

  voltageRaw = analogRead(PIN_IN_VOLTAGE);

  // eject false reading
  if (voltageRaw == 4095)
  {
    if (i_loop % 1000 == 0)
    {
      char print_buffer[500];
      sprintf(print_buffer, "Voltage read 4095 ==> eject");
      blh.notifyBleLogs(print_buffer);
    }
    return;
  }
  if (voltageRaw < 900)
  {
    if (i_loop % 1000 == 0)
    {
      char print_buffer[500];
      sprintf(print_buffer, "Voltage read < 900 ==> eject");
      blh.notifyBleLogs(print_buffer);
    }

    return;
  }

  // Voltage calibration
  if (shrd.batteryMaxVoltageCalibRaw != 0)
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

void processVoltageTooHighForBrake()
{

  bool isElectricBrakeForbidenNew = isElectricBrakeForbiden();

#if DEBUG_BLE_DISPLAY_VOLTAGE_TOO_HIGH
  Serial.printf("processVoltageTooHighForBrake : isElectricBrakeForbidenNew = %d / brakeFordidenHighVoltage = %d \n", isElectricBrakeForbidenNew, shrd.brakeFordidenHighVoltage);
#endif

  if (shrd.brakeFordidenHighVoltage != isElectricBrakeForbidenNew)
  {

    shrd.brakeFordidenHighVoltage = isElectricBrakeForbidenNew;

    // notify
    //blh.notifyBreakeSentOrder(shrd.brakeSentOrder, shrd.brakePressedStatus, shrd.brakeFordidenHighVoltage);
    blh.notifyCommandsFeedback();

#if DEBUG_BLE_DISPLAY_VOLTAGE_TOO_HIGH
    Serial.printf("processVoltageTooHighForBrake : notify\n");
#endif
  }
}

void processAutonomy()
{

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
      int32_t voltageDiff = (shrd.voltageFilterLongMean - shrd.voltageActual);
      float currentFactor = (1.0 + (shrd.currentActual / 1000.0));
      float correction = voltageDiff / currentFactor;
      uint32_t correctedVoltage = shrd.voltageFilterLongMean - correction;
      shrd.batteryLevel = batt.level(correctedVoltage);

#if DEBUG_DISPLAY_AUTONOMY
      Serial.println("voltageDiff = " + (String)voltageDiff +
                     " / currentFactor = " + (String)currentFactor +
                     " / correction = " + (String)correction +
                     " / correctedVoltage = " + (String)correctedVoltage);
#endif
    }
    else
    {
      shrd.batteryLevel = batt.level(shrd.voltageFilterMean);
    }
  }

  shrd.autonomyLeft = (settings.get_Battery_maximum_distance()) * (shrd.batteryLevel) / 100.0;

#if DEBUG_DISPLAY_AUTONOMY
  Serial.println(" / bat level : " + (String)shrd.batteryLevel +
                 " / voltageInMilliVolts = " + voltageInMilliVolts +
                 " / autonomy = " + (String)shrd.autonomyLeft +
                 " / bat dst = " + (String)(settings.get_Battery_maximum_distance()) +
                 " / voltageFilterLongMean = " + (String)shrd.voltageFilterLongMean +
                 " / voltageActual = " + (String)shrd.voltageActual +
                 " / currentActual = " + (String)shrd.currentActual);
#endif
}

void processCurrent()
{
  if (shrd.currentSensorPresent)
  {
    int currentRead = analogRead(PIN_IN_CURRENT);

    if (shrd.speedCurrent == 0)
    {
      if (currentRead < ANALOG_CURRENT_MIN_RAW_READING)
      {
        shrd.currentSensorPresent = false;
        Serial.println("Current sensor is not detected -> disable reading");
      }
      else
      {
        currentRawFilterInit.in(currentRead);
      }
    }
    currentRawFilter.in(currentRead);

    if (shrd.currentSensorPresent)
    {
      // current rest value
      int currentRawFilter2 = currentRawFilter.getMeanWithoutExtremes(2);
      int currentRawFilterInit2 = currentRawFilterInit.getMeanWithoutExtremes(5);
      int currentInMillamps = (currentRawFilter2 - currentRawFilterInit2) * (1000.0 / ANALOG_TO_CURRENT);
      shrd.currentActual = currentInMillamps;
    }
    else
    {
      shrd.currentActual = 0;
    }

#if DEBUG_DISPLAY_CURRENT
    Serial.print("currentRead : ");
    Serial.print(currentRead);
    Serial.print(" / currentFilterInit getMeanWithoutExtremes : ");
    Serial.print(currentRawFilterInit.getMeanWithoutExtremes(5));
    Serial.print(" / in amperes : ");
    Serial.println(shrd.currentActual / 1000.0);
#endif
  }
}

void processRelay()
{
  if (shrd.auxOrder == 1)
  {
    digitalWrite(PIN_OUT_RELAY, 1);
  }
  else
  {
    digitalWrite(PIN_OUT_RELAY, 0);
  }
}

//////------------------------------------
//////------------------------------------
////// Main loop

void loop()
{

#if DEBUG_FAKE_SPEED
  if (millis() > 5000)
    shrd.speedCurrent = millis() % 2000;
  if (millis() > 5000 && millis() % 20 == 0)
    computeDistance(shrd.speedCurrent);
#endif

  // handle Wifi OTA
  if (shrd.inOtaMode)
  {

#if ENABLE_WATCHDOG
    // stop watchdog / not all OTA updates have interactive loop
    disableWatchdog();
#endif

    // stop BLE
    blh.deinit();

    // convert String to char*
    char Wifi_ssid[settings.get_Wifi_ssid().length()];
    settings.get_Wifi_ssid().toCharArray(Wifi_ssid, settings.get_Wifi_ssid().length());
    char Wifi_password[settings.get_Wifi_password().length()];
    settings.get_Wifi_password().toCharArray(Wifi_password, settings.get_Wifi_password().length());

    // select & launch requested OTA mode
    if (shrd.inOtaMode == OTA_SERVER)
    {
      OTA_server_run(Wifi_ssid, Wifi_password, shrd.inOtaModeVersion);
    }
    else if (shrd.inOtaMode == OTA_IDE)
    {
      while (1)
      {
        OTA_ide_loop(Wifi_ssid, Wifi_password);
        delay(1);
      }
    }

    return;
  }

#if CONTROLLER_TYPE == CONTROLLER_MINIMOTORS
  minomoCntrl.processSerial(i_loop, false);

  // get serial link status ... both directions
  shrd.errorSerialFromContrl = !minomoCntrl.getSerialStatusOkFromContrl();
  shrd.errorSerialFromDisplay = !minomoCntrl.getSerialStatusOkFromLcd();
  shrd.errorContrl = !minomoCntrl.getContrlStatusOk();
#elif CONTROLLER_TYPE == CONTROLLER_ZERO
  zeroCntrl.processSerial(i_loop, false);

  // get serial link status ... both directions
  shrd.errorSerialFromContrl = !zeroCntrl.getSerialStatusOkFromContrl();
  shrd.errorSerialFromDisplay = !zeroCntrl.getSerialStatusOkFromLcd();
  shrd.errorContrl = !zeroCntrl.getContrlStatusOk();
#elif CONTROLLER_TYPE == CONTROLLER_VESC
  if (i_loop % 100 == 1)
  {
    //Serial.println(">>>>>>>>>>> readVescValues");

    if (i_loop < 100)
    {
      vescCntrl.requestMotorConfigTemp();
    }
    else
    {
      vescCntrl.requestVescValues();
    }
  }

  if (i_loop % 100 == 51)
  {
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

  processRelay();

#if VOLTAGE_EXT_READ
  if (i_loop % 10 == 0)
  {
#if CONTROLLER_TYPE != CONTROLLER_VESC
    processVoltage();
#endif
  }
  if (i_loop % 1000 == 0)
  {
    processVoltageLongFilter();
    processVoltageTooHighForBrake();
  }
#endif

  if (i_loop % 1000 == 0)
  {
    processAutonomy();
  }

  if (i_loop % 10 == 1)
  {
    btns.processTicks();
  }
  if (i_loop % 10 == 2)
  {
    //displayBrake();
    getBrakeFromAnalog();
  }

  if (i_loop % 10 == 3)
  {
    blh.processBLE();
  }

#if CONTROLLER_TYPE == CONTROLLER_MINIMOTORS
  if (i_loop % 100 == 5)
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

#if HAS_I2C
  // avoid reading throttle in settings menu
  if ((settings.get_Throttle_regeneration()) && (!shrd.inSettingsMenu))
  {
    // 1000 Hz read loop
    getThrottleFromAnalog();

    // 200 Hz refresh loop
    if ((i_loop % 10 == 7) || (i_loop % 10 == 2))
    {
      // uint32_t timeBefore = micros();
      processDacOutput();
      //Serial.println("led update = " + (String)(micros() - timeBefore));
    }
  }
#endif

#if HAS_I2C && TEMPERATURE_INT_READ
  if (i_loop % 100 == 8)
  {
    processSHTC3(true);
  }
  if (i_loop % 100 == 16)
  {
    processSHTC3(false);
  }
#endif

  if (i_loop % 1000 == 99)
  {
    checkAndSaveOdo();
  }

  // Give a time for ESP
  delay(1);
  //delayMicroseconds(1000);
  //yield();

#if DEBUG_TIMELOOP_NS
  Serial.print("> ");
  Serial.print(micros() - timeLoop);
  Serial.print(" / i_loop : ");
  Serial.println(i_loop);
  timeLoop = micros();
#endif

#if DEBUG_TIMELOOP_MS
  Serial.print("> ");
  Serial.print(millis() - timeLoop);
  Serial.print(" / i_loop : ");
  Serial.println(i_loop);
  timeLoop = millis();
#endif

  i_loop++;

#if ENABLE_WATCHDOG
  resetWatchdog();
#endif
}

/////////// End
