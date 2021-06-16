//////////////////////////////////////////
// TODO : VESC-EYE => error indicator on EYE
// TODO : auto mode shift on low battery
// TODO : reduce SHTC3 read time - brake read function in 2 parts
// TODO : VESC / configurable speed limits
//           - 20/25
//           - 35/30/35
//           - unlimited speed
// TODO : VESC / configure ECO & BOOST
//           - eco 0.6 multiplier for phase and 0.7 for battery current
//           - normal no change
//           - boost multiplier 1.2 for battery and phase currents
// TODO : VESC / TFT & app warning indicator boost
// TODO : VESC / limit boost with threshold if MOSFET are too high
// BUG : Minimo - original regulator perturbation
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
#include "Settings.h"
#include "SharedData.h"
#include "TFT/tft_main.h"
#include "TFT/tft_settings_menu.h"
#include "debug.h"
#include "esp32-hal-uart.h"
#include "filters/MedianFilter.h"
#include "pinout.h"
#include "tools/utils.h"
#include "Settings.h"
#include "Settings2.h"
#include "WifiSettingsPortal/WifiSettingsPortal.h"
#include "Controllers/KellyUart.h"
#include "Controllers/MinimoUart.h"
#include "Controllers/SmartEsc.h"
#include "Controllers/VescUart.h"
#include "Controllers/ZeroUart.h"
#include "ioprocessing.h"
#include "OWB/owb_process.h"

//////------------------------------------
////// Defines

// SMART CONFIGURATION
#if BUILD_CONTROLLER_MINIMOTORS || BUILD_DISPLAY_EYE
MinimoUart minomoCntrl;
#endif
#if BUILD_CONTROLLER_VESC
VescUart vescCntrl;
#endif
#if BUILD_CONTROLLER_KELLY
KellyUart kellyCntrl;
#endif
#if BUILD_CONTROLLER_SMART_ESC
SmartEsc smartEscCntrl;
#endif
#if BUILD_CONTROLLER_ZERO
ZeroUart zeroCntrl;
#endif

// UART
#define BAUD_RATE_CONSOLE 921600

// distance
#define SPEED_TO_DISTANCE_CORRECTION_FACTOR 1

#define DEBUG_ESP_HTTP_UPDATE 1

#define ENABLE_TEMPERATURE_INT_READ 1
#define ENABLE_VOLTAGE_EXT_READ 1
#define ENABLE_BRAKE_ANALOG_EXT_READ 1
#define ENABLE_ONEWIRE 1
#define ENABLE_WIFI 1
#define ENABLE_WATCHDOG 1

#define WATCHDOG_TIMEOUT 1000 // 1s // time in ms to trigger the watchdog

#define USE_CURRENT_FROM_MINIMO_CONTROLLER 0

//////------------------------------------
////// Variables

// Tasks handlers
TaskHandle_t htaskUpdateTFT;
TaskHandle_t htaskProcessWifiBlocking;
TaskHandle_t htaskProcessButtons;
TaskHandle_t htaskProcessOwb;

// Time
unsigned long timeLoop = 0;
unsigned long timeLoopMax = 0;

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

SharedData shrd;

int i_loop = 0;

Buttons btns;

Settings settings;
Settings2 settings2;
BluetoothHandler blh;

//////------------------------------------
//////------------------------------------
////// Setups

void setupPins()
{

  pinMode(PIN_IN_OUT_ONEWIRE, INPUT_PULLUP);
  pinMode(PIN_IN_BUTTON1, INPUT_PULLUP);
  pinMode(PIN_IN_BUTTON2, INPUT_PULLUP);
  pinMode(PIN_IN_VOLTAGE, INPUT);
  pinMode(PIN_IN_CURRENT, INPUT);
  pinMode(PIN_OUT_RELAY, OUTPUT);
  pinMode(PIN_OUT_BRAKE, OUTPUT);
  pinMode(PIN_OUT_LED_BUTTON1, OUTPUT);
  pinMode(PIN_IN_ABRAKE, INPUT);
  pinMode(PIN_IN_ATHROTTLE, INPUT);
#if (PCB >= 142)
  pinMode(PIN_OUT_POWER_LATCH, OUTPUT);
  pinMode(PIN_IN_BUTTON_PWR, INPUT);
#endif
}

void setupSerial()
{

#if BUILD_CONTROLLER_MINIMOTORS || BUILD_DISPLAY_EYE
  minomoCntrl.setSettings(&settings);
  minomoCntrl.setSharedData(&shrd);
  minomoCntrl.setBluetoothHandler(&blh);
#endif

#if BUILD_CONTROLLER_MINIMOTORS
  // minimotor controller
  hwSerCntrl.begin(MINIMO_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  minomoCntrl.setControllerSerialPort(&hwSerCntrl);
  hwSerCntrl.setUartIrqIdleTrigger(0);
#endif

#if BUILD_DISPLAY_EYE
  // minimotor display
  hwSerLcd.begin(MINIMO_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_LCD_TO_ESP, PIN_SERIAL_ESP_TO_LCD);
  minomoCntrl.setLcdSerialPort(&hwSerLcd);
  hwSerLcd.setUartIrqIdleTrigger(0);
#endif

#if BUILD_CONTROLLER_ZERO
  zeroCntrl.setSettings(&settings);
  zeroCntrl.setSharedData(&shrd);
  zeroCntrl.setBluetoothHandler(&blh);

  // zero controller
  hwSerCntrl.begin(ZERO_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  zeroCntrl.setControllerSerialPort(&hwSerCntrl);
  hwSerCntrl.setUartIrqIdleTrigger(0);
#endif

#if BUILD_DISPLAY_QSS4
  // zero display
  hwSerLcd.begin(ZERO_BAUD_RATE, SERIAL_8N1, PIN_SERIAL_LCD_TO_ESP, PIN_SERIAL_ESP_TO_LCD);
  zeroCntrl.setLcdSerialPort(&hwSerLcd);
  hwSerLcd.setUartIrqIdleTrigger(0);
#endif

#if BUILD_CONTROLLER_VESC
  hwSerCntrl.begin(BAUD_RATE_VESC, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  vescCntrl.setSerialPort(&hwSerCntrl);
  vescCntrl.setup(&shrd, &blh, &settings);

#if DEBUG_DISPLAY_VESC_FRAME
  vescCntrl.setDebugPort(&Serial);
#endif
  //vescCntrl.requestMotorConfig();
  //vescCntrl.setDebugPort(NULL);
#endif

#if BUILD_CONTROLLER_KELLY
  hwSerCntrl.begin(BAUD_RATE_KELLY, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  kellyCntrl.setSerialPort(&hwSerCntrl);
  kellyCntrl.setDebugPort(&Serial);
  hwSerCntrl.setUartIrqIdleTrigger(1);
#endif

  /*
#if BUILD_CONTROLLER_SMART
  hwSerCntrl.begin(BAUD_RATE_SMARTESC, SERIAL_8N1, PIN_SERIAL_CNTRL_TO_ESP, PIN_SERIAL_ESP_TO_CNTRL);
  smartEscCntrl.setSerialPort(&hwSerCntrl);
  //smartEscCntrl.setDebugPort(&Serial);
  smartEscCntrl.setSettings(&settings);
  smartEscCntrl.setSharedData(&shrd);
  hwSerCntrl.setUartIrqIdleTrigger(1);
#endif
*/
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

    // not currently in settings menu
    if (shrd.inSettingsMenu == SETTINGS_MENU_STATE_ENTERING)
    {

      Serial.println("taskUpdateTFT / SETTINGS_MENU_STATE_ENTERING");

      // force main display reset for next time in SETTINGS_MENU_STATE_OUT
      i = -1;

      Serial.println("settings_menu_setup");
      settings_menu_setup();

      shrd.inSettingsMenu = SETTINGS_MENU_STATE_IN;
      Serial.println("taskUpdateTFT ===> SETTINGS_MENU_STATE_IN");
    }
    else if (shrd.inSettingsMenu == SETTINGS_MENU_STATE_IN)
    {

      // Serial.println("taskUpdateTFT / SETTINGS_MENU_STATE_IN");

      settings_menu_loop();

      vTaskDelay(10);

      // Serial.println("taskUpdateTFT ===> SETTINGS_MENU_STATE_IN");
    }
    else if (shrd.inSettingsMenu == SETTINGS_MENU_STATE_OUT)
    {

      //Serial.println("taskUpdateTFT / SETTINGS_MENU_STATE_OUT");

      tftUpdateData(i);
      i++;

      //Serial.println("taskUpdateTFT ===> SETTINGS_MENU_STATE_OUT");

      if (i >= 20)
      {
        i = 0;
        vTaskDelay(200);
        //Serial.println("taskUpdateTFT ===> delay 200");
      }
    }
  }
}

void taskProcessWifiBlocking(void *parameter)
{

  // For for display to init to avoid too much memory allocation
  vTaskDelay(4000);

  // setup
  WifiSettingsPortal_setSettings(&settings);
  WifiSettingsPortal_setSharedData(&shrd);
  WifiSettingsPortal_setBluetoothHandler(&blh);
  WifiSettingsPortal_setup();
  WifiSettingsPortal_begin();

  // infinite loop
  for (;;)
  {

    WifiSettingsPortal_loop();

    vTaskDelay(50);
  }
}

void taskProcessButtons(void *parameter)
{
  // infinite loop
  for (;;)
  {

    btns.processTicks();

    vTaskDelay(10);
  }
}

void taskProcessOwb(void *parameter)
{
  owb_loop();
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

#if ENABLE_TFT
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

#if ENABLE_TFT
  Serial.println("   TFT... ");
  tftSetup(&shrd, &settings);
#endif

  Serial.println("   prefs... ");
  settings2.setSharedData(&shrd);
  settings2.restore();

  Serial.println("   settings ...");
  settings.restore();
  settings_menu_set_settings(&settings);
  settings_menu_set_shared_datas(&shrd);
  Serial.println("   init data with settings... ");
  initSharedDataWithSettings();

  Serial.println("   BLE ...");
  blh.setSettings(&settings);
  blh.setSharedData(&shrd);
  blh.setBleLock(false); // force BLE lock mode
  blh.init();

#if ENABLE_ONEWIRE
  Serial.println("   OneWire ...");
  owb_setup(&shrd);
#endif

  setupVoltage();
  setupBattery();
  setupAutonomy();

  // Setup wifi portal
  Serial.println("   Wifi portal ...");

#if ENABLE_TFT
  xTaskCreatePinnedToCore(
      taskUpdateTFT,    // Function that should be called
      "taskUpdateTFT",  // Name of the task (for debugging)
      30000,            // Stack size (bytes)
      NULL,             // Parameter to pass
      tskIDLE_PRIORITY, // Task priority
      &htaskUpdateTFT,  // Task handle,
      1);               // Core
#endif

#if ENABLE_WIFI
  xTaskCreatePinnedToCore(
      taskProcessWifiBlocking,   // Function that should be called
      "taskProcessWifiBlocking", // Name of the task (for debugging)
      8000,                      // Stack size (bytes)
      NULL,                      // Parameter to pass
      tskIDLE_PRIORITY,          // Task priority
      &htaskProcessWifiBlocking, // Task handle,
      1);                        // Core
#endif

  xTaskCreatePinnedToCore(
      taskProcessButtons,   // Function that should be called
      "taskProcessButtons", // Name of the task (for debugging)
      3000,                 // Stack size (bytes)
      NULL,                 // Parameter to pass
      tskIDLE_PRIORITY,     // Task priority
      &htaskProcessButtons, // Task handle,
      1);                   // Core

#if ENABLE_ONEWIRE
  xTaskCreatePinnedToCore(
      taskProcessOwb,   // Function that should be called
      "taskProcessOwb", // Name of the task (for debugging)
      2000,             // Stack size (bytes)
      NULL,             // Parameter to pass
      1,                // Task priority
      &htaskProcessOwb, // Task handle,
      1);               // Core
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
  Serial.print(shrd.speedOldForCntrl);
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
  else if ((shrd.speedCurrent < 10) && (shrd.distanceOdo > shrd.distanceOdoInFlash + 1)) /* save when speed < 10 km/h and 100m done */
  {
#if DEBUG_DISPLAY_SAVE_ODO
    Serial.println("Save ODO ... YES - low speed condition / speedOldForOdo = " + (String)shrd.speedOldForOdo + " / speedCurrent = " + (String)shrd.speedCurrent);
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

#if BUILD_CONTROLLER_VESC
void processVescValuesFeedback()
{

  int32_t rpm = vescCntrl.data.rpm;
  if (rpm < 0)
    rpm = 0;
  float speedCompute = ErpmToKmh2(&settings, rpm);
  //Serial.println("rpm = " + (String) rpm + " / speedCompute = " + (String) speedCompute);

  if (speedCompute > shrd.speedMax)
    shrd.speedMax = speedCompute;

  shrd.speedCurrent = speedCompute;
  shrd.voltageFilterMean = vescCntrl.data.inpVoltage * 1000;
  shrd.voltageActual = vescCntrl.data.inpVoltage * 1000;
  shrd.currentActual = vescCntrl.data.avgInputCurrent * 1000;
  shrd.currentTemperature = vescCntrl.data.tempMosfet;
  if (shrd.currentTemperature > shrd.maxTemperature)
    shrd.maxTemperature = shrd.currentTemperature;

  //Serial.println("voltageFilterMean = " + (String)shrd.voltageFilterMean + " / currentActual = " + (String)shrd.currentActual);

  computeDistance(speedCompute);
}
#endif

#if BUILD_CONTROLLER_KELLY
void processKellySerial1()
{

  shrd.voltageFilterMean = kellyCntrl.data1.B_Voltage * 1000;
  shrd.currentTemperature = kellyCntrl.data1.Controller_temperature;
  if (shrd.currentTemperature > shrd.maxTemperature)
    shrd.maxTemperature = shrd.currentTemperature;

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

//////------------------------------------
//////------------------------------------
////// Main loop

void loop()
{

#if DEBUG_FAKE_SPEED
  if (millis() % 10000 < 5000)
    shrd.speedCurrent = shrd.speedCurrent + 0.01;
  else if (millis() % 10000 > 5000)
    shrd.speedCurrent = shrd.speedCurrent - 0.01;
  if (shrd.speedCurrent < 0)
    shrd.speedCurrent = 0;
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

    // select & launch requested OTA mode
    if (shrd.inOtaMode == OTA_SERVER)
    {
      // wait for WifiSettingsPortal to reply
      delay(100);

      // close WifiSettingsPortal to reconfigure wifi correctly
      WifiSettingsPortal_close();

      // launch OTA
      OTA_server_run((char *)settings.get_Wifi_ssid().c_str(), (char *)settings.get_Wifi_password().c_str(), shrd.inOtaModeVersion);
    }
    else if (shrd.inOtaMode == OTA_IDE)
    {
      Serial.println("OTA IDE");
      while (1)
      {
        OTA_ide_loop((char *)settings.get_Wifi_ssid().c_str(), (char *)settings.get_Wifi_password().c_str());
        delay(1);
      }
    }

    return;
  }

#if BUILD_CONTROLLER_MINIMOTORS || BUILD_DISPLAY_EYE
  minomoCntrl.processSerial(i_loop, false);

#if BUILD_CONTROLLER_MINIMOTORS
  // get serial link status ... both directions
  shrd.errorSerialFromContrl = !minomoCntrl.getSerialStatusOkFromContrl();
  shrd.errorSerialFromDisplay = !minomoCntrl.getSerialStatusOkFromLcd();
  shrd.errorContrl = !minomoCntrl.getContrlStatusOk();
#endif
#endif

#if BUILD_CONTROLLER_ZERO
  zeroCntrl.processSerial(i_loop, false);

  // get serial link status ... both directions
  shrd.errorSerialFromContrl = !zeroCntrl.getSerialStatusOkFromContrl();
  shrd.errorSerialFromDisplay = !zeroCntrl.getSerialStatusOkFromLcd();
  shrd.errorContrl = !zeroCntrl.getContrlStatusOk();
#endif

#if BUILD_CONTROLLER_VESC
#define LOOP_SPEED_VESC 150
  if (i_loop % LOOP_SPEED_VESC == 0)
  {
    //Serial.println(">>>>>>>>>>> requestMotorConfigTemp");
    vescCntrl.requestMotorConfigTemp();
  }
  if (i_loop % LOOP_SPEED_VESC == 60)
  {
    //Serial.println(">>>>>>>>>>> readMotorConfigTempFeedback");
    vescCntrl.readMotorConfigTempFeedback();

    //Serial.println(">>>>>>>>>>> readVescValues");
    vescCntrl.requestVescValues();
  }

  if ((i_loop % LOOP_SPEED_VESC == 140) && (vescCntrl.motorSettingReceived))
  {
    //Serial.println(">>>>>>>>>>> readVescValuesFeedback");

    if (vescCntrl.readVescValuesFeedback())
    {
      // simulate brake force from 0 to 5 for smartphone feedback
      shrd.brakeSentOrder = map(shrd.brakePercent, 0, 100, 0, settings.get_Ebrake_max_power_value());

      processVescValuesFeedback();
    }

    //Serial.println(">>>>>>>>>>> setMaxSpeed");
    // synchronous - set mode 1/2/3 -- no ACK
    vescCntrl.setMaxSpeed(shrd.modeOrder);
  }

#endif

#if BUILD_CONTROLLER_KELLY
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
#endif

#if BUILD_CONTROLLER_SMART_ESC

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

#if ENABLE_VOLTAGE_EXT_READ
  if (i_loop % 10 == 0)
  {
#ifndef BUILD_CONTROLLER_VESC
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

  if (i_loop % 10 == 2)
  {
    //displayBrake();
    getBrakeFromAnalog();
  }

  if (i_loop % 10 == 3)
  {
    blh.processBLE();
  }

#if (CONTROLLER_TYPE == CONTROLLER_MINIMOTORS) || (CONTROLLER_TYPE == CONTROLLER_ZERO)
  if (i_loop % 100 == 5)
  {
    processCurrent();
  }
#else
  shrd.currentSensorPresent = 2;
#endif

/*
  if (i_loop % 1000 == 0)
    Serial.println("shrd.currentSensorPresent = " + (String)shrd.currentSensorPresent);
*/

#if HAS_I2C
  // avoid reading throttle in settings menu
  if (settings.get_Throttle_regeneration())
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

#if HAS_I2C && ENABLE_TEMPERATURE_INT_READ
  if (!shrd.owb_ds9990_present)
  {
    if (i_loop % 1000 == 8)
    {
      processSHTC3(true);
    }
    if (i_loop % 1000 == 16)
    {
      processSHTC3(false);
    }
  }
#endif

#if PCB > 130
  if (i_loop % 1000 == 97)
  {
    checkAndPowerOff();
  }
#endif

#if ENABLE_WIFI
  if (i_loop % 200 == 98)
  {
    WifiSettingsPortal_sendValues();
  }
#endif

  if (i_loop % 1000 == 99)
  {
    checkAndSaveOdo();
  }

  // push error logs in BLE
  if (i_loop % 100 == 99)
  {
    if (shrd.errorContrl)
    {
      blh.notifyBleLogs((char *)"controller error");
    }
    if (shrd.errorSerialFromDisplay)
    {
      blh.notifyBleLogs((char *)"serial display error");
    }
    if (shrd.errorSerialFromContrl)
    {
      blh.notifyBleLogs((char *)"serial controller error");
    }
  }

  // Give a time for ESP
  delay(1);
  //delayMicroseconds(1000);
  //yield();

#if DEBUG_TIMELOOP_NS
  if (micros() - timeLoop > timeLoopMax)
  {
    timeLoopMax = micros() - timeLoop;
    if (timeLoopMax > 1)
    {
      Serial.print("> timeLoopMax = ");
      Serial.print(timeLoopMax);
      Serial.print(" / i_loop : ");
      Serial.println(i_loop);
    }
  }
  if (i_loop % 10000 == 0)
  {
    timeLoopMax = 0;
  }
  timeLoop = micros();
#endif

#if DEBUG_TIMELOOP_MS
  if (millis() - timeLoop > timeLoopMax)
  {
    timeLoopMax = millis() - timeLoop;
    if (timeLoopMax > 1)
    {
      Serial.print("> timeLoopMax = ");
      Serial.print(timeLoopMax);
      Serial.print(" / i_loop : ");
      Serial.println(i_loop);
    }
  }
  if (i_loop % 10000 == 0)
  {
    timeLoopMax = 0;
  }
  timeLoop = millis();
#endif

  i_loop++;

#if DEBUG_HEAP
  if (i_loop % 10000 == 0)
  {
    uint32_t hm1 = uxTaskGetStackHighWaterMark(NULL);
    uint32_t hm2 = uxTaskGetStackHighWaterMark(htaskUpdateTFT);
    uint32_t hm3 = uxTaskGetStackHighWaterMark(htaskProcessWifiBlocking);
    uint32_t hm4 = uxTaskGetStackHighWaterMark(htaskProcessButtons);
    uint32_t hm5 = uxTaskGetStackHighWaterMark(htaskProcessOwb);
    Serial.printf("RAM left = %d / HM idle = %d / HM tft = %d / HM wifi = %d / HM btn = %d / HM owb = %d\n", esp_get_free_heap_size(), hm1, hm2, hm3, hm4, hm5);
  }
#endif

#if ENABLE_WATCHDOG
  resetWatchdog();
#endif
}

/////////// End
