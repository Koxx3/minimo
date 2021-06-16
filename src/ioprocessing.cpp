#include "ioprocessing.h"
#include "SharedData.h"
#include "Settings.h"
#include "debug.h"
#include <MCP4725/Adafruit_MCP4725.h>
#include <Wire.h>
#include "SHTC3/SparkFun_SHTC3.h"

#include "filters/MedianFilter.h"
#include "BLE/BluetoothHandler.h"
#include "Battery/Battery.h"
#include "tools/utils.h"
#include "Buttons/Buttons.h"
#include "TFT/tft_main.h"

extern SharedData shrd;
extern Settings settings;
extern Settings2 settings2;
extern BluetoothHandler blh;
extern Buttons btns;
extern uint32_t i_loop;

TwoWire I2Cone = TwoWire(0);
Adafruit_MCP4725 dac;
SHTC3 mySHTC3;

uint32_t timerPowerOff = 0;

uint32_t iBrakeMinCalibOrder = 0;

uint16_t voltageRaw = 0;
uint32_t voltageInMilliVolts = 0;
Battery batt = Battery(NULL);

uint16_t throttleAnalogValue = 0;

MedianFilter voltageFilter(NB_VOLTAGE_FILTER_DATA, 2000);
MedianFilter voltageLongFilter(NB_VOLTAGE_FILTER_DATA, 50000);
MedianFilter voltageRawFilter(NB_VOLTAGE_FILTER_DATA, 2000);
MedianFilter currentRawFilter(NB_CURRENT_FILTER_DATA, 1830);
MedianFilter currentRawFilterInit(NB_CURRENT_FILTER_CALIB_DATA, 1830);
MedianFilter brakeFilter(10 /* 20 */, 900);
//MedianFilter brakeMaxFilterInit(NB_BRAKE_CALIB_DATA, 900);
MedianFilter throttleFilter(NB_THROTTLE_FILTER_DATAS /* 20 */, 900);

void setupBattery()
{
  batt = Battery(&settings);

  Serial.println("Battery : type = " + (String)settings.get_Battery_nominal_voltage());
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
    shrd.shtc3_present = false;
  }
  else
  {
    shrd.shtc3_present = true;

    mySHTC3.setMode(SHTC3_CMD_CSE_TF_LPM);
    mySHTC3.wake(true);
  }
}

#endif

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
    if ((shrd.brakeFilterMeanErr > ((shrd.brakeMaxPressureRaw - shrd.brakeMinPressureRaw) / 2) + shrd.brakeMinPressureRaw) && (i_loop < 100))
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
      Serial.println("brake ANALOG_BRAKE_MIN_ERR_VALUE : " + (String)shrd.brakeAnalogValue);
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
      Serial.println("brake ANALOG_BRAKE_MAX_ERR_VALUE : " + (String)shrd.brakeAnalogValue);
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

    if ((shrd.brakeAnalogValue > shrd.brakeMaxPressureRaw) && (shrd.inSettingsMenu == SETTINGS_MENU_STATE_OUT))
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

      //Serial.println("brakePercent = " + (String)shrd.brakePercent);

      // alarm controler from braking throught IO output
      changeBrakeIOState();

      // notify brake LCD value
      notifyBrakeWithBle();
    }
  }
}

void getThrottleFromAnalog()
{

#define TMIN_MARGIN 0.5
#define TMAX_MARGIN 1.25

  uint32_t tInMin = settings.get_Throttle_input_min_voltage();
  uint32_t tInMax = settings.get_Throttle_input_max_voltage();

  // Read and filter ADC
  throttleAnalogValue = analogRead(PIN_IN_ATHROTTLE);
  shrd.throttleAnalogValue = throttleAnalogValue;

  // init filter
  if (i_loop == 0)
  {
    for (int i = 0; i < NB_THROTTLE_FILTER_DATAS; i++)
    {
      throttleFilter.in(throttleAnalogValue);
    }
  }
  else
  {
    throttleFilter.in(throttleAnalogValue);
  }

  // Compute throttle voltage in 0-5V range
  uint32_t throttleInMillv = throttleAnalogValue * ANALOG_TO_VOLTS_5V * 1000;

  // ignore out of range datas ... and notify
  if (throttleInMillv < tInMin * TMIN_MARGIN)
  {
    char print_buffer[500];
    sprintf(print_buffer, "throttle : value too low / tAnalogValue : %d / throttleFilter.getMean() : %d / tInMin : %d / tInMin with margin : %d",
            throttleAnalogValue,
            throttleFilter.getMean(),
            tInMin,
            (uint32_t)(tInMin * TMIN_MARGIN));
    blh.notifyBleLogs(print_buffer);
    /*
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
    /*
    Serial.println(print_buffer);
*/

    shrd.errorThrottle = true;

    return;
  }
  else
  {
    shrd.errorThrottle = false;
  }

#if DEBUG_DISPLAY_THROTTLE
  if (i_loop % 10 == 0)
  {
    char print_buffer[500];
    sprintf(print_buffer, "throttle : tAnalogValue : %d / throttleFilter.getMean() : %d / tInMin : %d  / tInMax : %d / tInMax with margin : %d",
            throttleAnalogValue,
            throttleFilter.getMean(),
            tInMin,
            tInMax,
            (uint32_t)(tInMax * TMAX_MARGIN));
    Serial.println(print_buffer);
  }
#endif
}

void processDacOutput()
{
  uint32_t filteredThrottleIn = throttleFilter.getMeanWithoutExtremes(2);
  uint32_t throttleInMillv = filteredThrottleIn * ANALOG_TO_VOLTS_5V * 1000;

  uint32_t tInMin = settings.get_Throttle_input_min_voltage();
  uint32_t tInMax = settings.get_Throttle_input_max_voltage();

  uint32_t rangeInMilliv = tInMax - tInMin;

  // map to percent range
  float throttlePercent = map(throttleInMillv - tInMin, 0, rangeInMilliv, 0, 10000) / 100.0;
  throttlePercent = constrain(throttlePercent, 0, 100);

  // apply exponential curve
  if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Exponential_1)
  {
    throttlePercent = (exp(throttlePercent / 100.0) - 1) / (exp(1) - 1) * 100;
  }
  else if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Exponential_2)
  {
    throttlePercent = (exp(throttlePercent / 100.0 * 2) - 1) / (exp(2) - 1) * 100;
  }
  else if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Exponential_3)
  {
    throttlePercent = (exp(throttlePercent / 100.0 * 3) - 1) / (exp(3) - 1) * 100;
  }
  else if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Exponential_4)
  {
    throttlePercent = (exp(throttlePercent / 100.0 * 4) - 1) / (exp(4) - 1) * 100;
  }
  else if (settings.get_Throttle_output_curve() == settings.LIST_Throttle_output_curve_Custom_6_points)
  {
    int8_t index = 4;
    uint16_t low = 0;
    uint16_t high = 10000;
    uint16_t throttlePercentInt = throttlePercent * 100;
    if ((throttlePercentInt > 0) && (throttlePercentInt < 10000))
    {
      index = (throttlePercentInt / 2000) - 1;
      if (index >= 0)
        low = getValueFromString(settings.get_Throttle_output_curve_custom4(), ',', index) * 100;
      if (index < 3)
        high = getValueFromString(settings.get_Throttle_output_curve_custom4(), ',', index + 1) * 100;
      float throttlePercentNew = map(throttlePercentInt, (index + 1) * 2000, (index + 2) * 2000, low, high) / 100.0;
      //Serial.println("throttlePercentInt = " + (String)throttlePercentInt + " / index = " + (String)index + " / low = " + (String)low + " / high = " + (String)high + " / throttlePercentNew = " + (String)throttlePercentNew);
      throttlePercent = throttlePercentNew;
    }
  }

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
      minBrakeVoltage = map(settings.get_Ebrake_max_power_value(), 0, 5, 3300 / 2, 0);
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
    outputMilliv = tOutMin;
  }
  else if (((shrd.pasEnabled) && (shrd.speedCurrent > 1)) || (!shrd.pasEnabled)) // normal condition
  {
    outputMilliv = map(throttlePercent * 100, 0, 10000, tOutMin, tOutMax);
  }
  else // souldn't occur
  {
    outputMilliv = tOutMin;
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

#if ENABLE_TEMPERATURE_EXT_READ
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
      if (shrd.currentTemperature > shrd.maxTemperature)
        shrd.maxTemperature = shrd.currentTemperature;

      shrd.currentHumidity = humidity;
    }
  }
}
#endif

void processSHTC3(bool requestRead)
{

  if (requestRead)
  {
    mySHTC3.requestDatas(); // Call "update()" to command a measurement, wait for measurement to complete, and update the RH and T members of the object
  }
  else
  {
    SHTC3_Status_TypeDef status = mySHTC3.readDatas();
#if DEBUG_DISPLAY_SHTC3
    Serial.print(mySHTC3.toPercent()); // "toPercent" returns the percent humidity as a floating point number
    Serial.print("% / ");
    Serial.print("T = ");
    Serial.print(mySHTC3.toDegC()); // "toDegF" and "toDegC" return the temperature as a flaoting point number in deg F and deg C respectively
    Serial.print(" deg C\n");
#endif

    if (status == SHTC3_Status_Nominal)
    {
#if BUILD_CONTROLLER_MINIMOTORS || BUILD_CONTROLLER_ZERO
      shrd.currentTemperature = mySHTC3.toDegC();
      if (shrd.currentTemperature > shrd.maxTemperature)
        shrd.maxTemperature = shrd.currentTemperature;
#endif

      shrd.currentHumidity = mySHTC3.toPercent();
    }
    else
    {
    }
  }

#if DEBUG_FAKE_TEMPERATURE
  shrd.currentTemperature = 26;
#endif
}

void processVoltage()
{

  voltageRaw = analogRead(PIN_IN_VOLTAGE);

#if DEBUG_FAKE_VOLTAGE
  voltageRaw = 2500;
#endif

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
  int currentRead = analogRead(PIN_IN_CURRENT);

#if DEBUG_FAKE_CURRENT
  currentRead = 2048;
#endif

  // detect current sensor at startup
  if (i_loop < 100 * NB_CURRENT_FILTER_CALIB_DATA)
  {
    if (currentRead > ANALOG_CURRENT_MIN_RAW_READING)
    {
      shrd.currentSensorPresent = 1;
      Serial.println("Current sensor is detected -> enable reading : currentRead = " + (String)currentRead);

      if (shrd.speedCurrent == 0)
        currentRawFilterInit.in(currentRead);
    }
    else
    {
      Serial.println("Current sensor is not detected -> disable reading : currentRead = " + (String)currentRead);
      shrd.currentSensorPresent = 0;
    }

    return;
  }

  if (shrd.currentSensorPresent == 1)
  {
    currentRawFilter.in(currentRead);

    // current rest value
    int currentRawFilter2 = currentRawFilter.getMeanWithoutExtremes(2);
    int currentRawFilterInit2 = currentRawFilterInit.getMeanWithoutExtremes(5);
    int currentInMillamps = (currentRawFilter2 - currentRawFilterInit2) * (1000.0 / ANALOG_TO_CURRENT);
    shrd.currentActual = currentInMillamps;
  }
  else if (shrd.currentSensorPresent == -1)
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
#if USE_CURRENT_FROM_MINIMO_CONTROLLER
  // TEST // TEST // TEST // TEST // TEST //
  shrd.currentSensorPresent = 2;
  shrd.currentActual = shrd.currentFromController;
  // TEST // TEST // TEST // TEST // TEST //
#endif
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

void powerOff()
{
  Serial.println("SHUTDOWN");
  digitalWrite(PIN_OUT_POWER_LATCH, 1);
  btns.ledsOff();
  tftBacklightLow(true);
}

void checkAndPowerOff()
{

  if ((shrd.speedCurrent > 0) || (shrd.inOtaMode != OTA_OFF))
  {
    timerPowerOff = millis();
  }
  else
  {

    bool shouldPowerOff = false;
    if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_2_min) && (millis() > timerPowerOff + (60000 * 2)))
    {
      shouldPowerOff = true;
    }
    else if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_3_min) && (millis() > timerPowerOff + (60000 * 3)))
    {
      shouldPowerOff = true;
    }
    else if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_4_min) && (millis() > timerPowerOff + (60000 * 4)))
    {
      shouldPowerOff = true;
    }
    else if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_5_min) && (millis() > timerPowerOff + (60000 * 5)))
    {
      shouldPowerOff = true;
    }
    else if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_10_min) && (millis() > timerPowerOff + (60000 * 10)))
    {
      shouldPowerOff = true;
    }
    else if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_15_min) && (millis() > timerPowerOff + (60000 * 15)))
    {
      shouldPowerOff = true;
    }
    else if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_20_min) && (millis() > timerPowerOff + (60000 * 20)))
    {
      shouldPowerOff = true;
    }
    else if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_30_min) && (millis() > timerPowerOff + (60000 * 30)))
    {
      shouldPowerOff = true;
    }
    else if ((settings.get_Auto_power_off() == settings.LIST_Auto_power_off_60_min) && (millis() > timerPowerOff + (60000 * 60)))
    {
      shouldPowerOff = true;
    }

    if (shouldPowerOff)
    {
      powerOff();
    }
  }
}
