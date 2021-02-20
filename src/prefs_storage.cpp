//////------------------------------------
////// PREFERENCES functions

#include "main.h"
#include <prefs_storage.h>
#define APP_STORAGE "APP_STORAGE"

// preferences
#define PREF_bleLockForced "bleLockForced"
#define PREF_brkMinPressRaw "brkMinPressRaw"
#define PREF_brkMaxPressRaw "brkMaxPressRaw"
#define PREF_distanceOdo "distanceOdo"
#define PREF_bMaxCalibUser "bMaxCalibUser"
#define PREF_bMaxCalibRaw "bMaxCalibRaw"
#define PREF_bMinCalibUser "bMinCalibUser"
#define PREF_bMinCalibRaw "bMinCalibRaw"
#define PREF_mode "mode"
#define PREF_accel "accel"
#define PREF_eco "eco"

void preferences::setSettings(Settings *data)
{
  settings = data;
}

void preferences::setSharedData(SharedData *data)
{
  shrd = data;
}

void preferences::restore()
{
  restoreBleLockForced();
  restoreBrakeMinPressure();
  restoreBrakeMaxPressure();
  restoreOdo();
  restoreBatteryCalib();
}

void preferences::saveBleLockForced()
{
  prefs.begin(APP_STORAGE, false);
  prefs.putInt(PREF_bleLockForced, shrd->isLocked);
  prefs.end();

  Serial.print("save bleLockForced value : ");
  Serial.println(shrd->isLocked);
}

void preferences::restoreBleLockForced()
{
  prefs.begin(APP_STORAGE, false);
  shrd->isLocked = prefs.getInt(PREF_bleLockForced, 0);
  prefs.end();

  Serial.print("restore bleLockForced value : ");
  Serial.println(shrd->isLocked);
}

void preferences::saveBrakeMinPressure()
{

  prefs.begin(APP_STORAGE, false);
  prefs.putInt(PREF_brkMinPressRaw, shrd->brakeMinPressureRaw);
  prefs.end();

  Serial.print("save saveBrakeMinPressure value : ");
  Serial.println(shrd->brakeMinPressureRaw);
}

void preferences::restoreBrakeMinPressure()
{
  prefs.begin(APP_STORAGE, false);
  shrd->brakeMinPressureRaw = prefs.getInt(PREF_brkMinPressRaw, 900);
  prefs.end();

  Serial.print("restore restoreBrakeMinPressure value : ");
  Serial.println(shrd->brakeMinPressureRaw);

  if (shrd->brakeMinPressureRaw == -1)
    shrd->brakeMinPressureRaw = ANALOG_BRAKE_MIN_VALUE;
}

void preferences::saveBrakeMaxPressure()
{
  prefs.begin(APP_STORAGE, false);
  prefs.putInt(PREF_brkMaxPressRaw, shrd->brakeMaxPressureRaw);
  prefs.end();

  Serial.print("save saveBrakeMaxPressure value : ");
  Serial.println(shrd->brakeMaxPressureRaw);
}

void preferences::restoreBrakeMaxPressure()
{
  prefs.begin(APP_STORAGE, false);
  shrd->brakeMaxPressureRaw = prefs.getInt(PREF_brkMaxPressRaw, 2000);
  prefs.end();

  Serial.print("restore restoreBrakeMaxPressure value : ");
  Serial.println(shrd->brakeMaxPressureRaw);

  if (shrd->brakeMaxPressureRaw == -1)
    shrd->brakeMaxPressureRaw = ANALOG_BRAKE_MAX_VALUE;
}

void preferences::saveOdo()
{
  //uint32_t time = micros();
  prefs.begin(APP_STORAGE, false);
  prefs.putUInt(PREF_distanceOdo, shrd->distanceOdo);
  prefs.end();

  //Serial.print("time write : ");
  //Serial.println(micros() - time);

  //Serial.print("save saveOdo value : ");
  //Serial.println(shrd->distanceOdo);
}

void preferences::restoreOdo()
{
  prefs.begin(APP_STORAGE, false);
  shrd->distanceOdo = prefs.getUInt(PREF_distanceOdo, 0);
  prefs.end();

  shrd->distanceOdoInFlash = shrd->distanceOdo;
  shrd->distanceOdoBoot = shrd->distanceOdo;

  Serial.print("restore restoreOdo value : ");
  Serial.println(shrd->distanceOdo);
}

void preferences::saveBatteryCalib()
{
  prefs.begin(APP_STORAGE, false);
  prefs.putFloat(PREF_bMaxCalibUser, shrd->batteryMaxVoltageCalibUser);
  prefs.putUInt(PREF_bMaxCalibRaw, shrd->batteryMaxVoltageCalibRaw);
  prefs.putFloat(PREF_bMinCalibUser, shrd->batteryMinVoltageCalibUser);
  prefs.putUInt(PREF_bMinCalibRaw, shrd->batteryMinVoltageCalibRaw);
  prefs.end();

  Serial.println("save BatteryCalib value : ");
  Serial.print("  batteryMaxVoltageCalibUser : ");
  Serial.println(shrd->batteryMaxVoltageCalibUser);
  Serial.print("  batteryMaxVoltageCalibRaw : ");
  Serial.println(shrd->batteryMaxVoltageCalibRaw);
  Serial.print("  batteryMinVoltageCalibUser : ");
  Serial.println(shrd->batteryMinVoltageCalibUser);
  Serial.print("  batteryMinVoltageCalibRaw : ");
  Serial.println(shrd->batteryMinVoltageCalibRaw);
}

void preferences::restoreBatteryCalib()
{
  prefs.begin(APP_STORAGE, false);
  shrd->batteryMaxVoltageCalibUser = prefs.getFloat(PREF_bMaxCalibUser, 66.20);
  shrd->batteryMaxVoltageCalibRaw = prefs.getUInt(PREF_bMaxCalibRaw, 2970);
  shrd->batteryMinVoltageCalibUser = prefs.getFloat(PREF_bMinCalibUser, 0.0);
  shrd->batteryMinVoltageCalibRaw = prefs.getUInt(PREF_bMinCalibRaw, 0);
  prefs.end();

  Serial.println("restore BatteryCalib value : ");
  Serial.print("  batteryMaxVoltageCalibUser : ");
  Serial.println(shrd->batteryMaxVoltageCalibUser);
  Serial.print("  batteryMaxVoltageCalibRaw : ");
  Serial.println(shrd->batteryMaxVoltageCalibRaw);
  Serial.print("  batteryMinVoltageCalibUser : ");
  Serial.println(shrd->batteryMinVoltageCalibUser);
  Serial.print("  batteryMinVoltageCalibRaw : ");
  Serial.println(shrd->batteryMinVoltageCalibRaw);
}

void preferences::saveSettings()
{
  prefs.begin(APP_STORAGE, false);
  prefs.putBytes(SETTINGS1, settings->settings1.buffer, sizeof(settings->settings1));
  prefs.putBytes(SETTINGS2, settings->settings2.buffer, sizeof(settings->settings2));
  prefs.putBytes(SETTINGS3, settings->settings3.buffer, sizeof(settings->settings3));
  prefs.putBytes(SETTINGS4, settings->settings4.buffer, sizeof(settings->settings4));
  prefs.putBytes(SETTINGS5, settings->settings5.buffer, sizeof(settings->settings5));
  prefs.putBytes(SETTINGS6, settings->settings6.buffer, sizeof(settings->settings6));
  prefs.end();
}

boolean preferences::restoreSettings()
{
  prefs.begin(APP_STORAGE, false);
  prefs.getBytes(SETTINGS1, settings->settings1.buffer, prefs.getBytesLength(SETTINGS1));
  prefs.getBytes(SETTINGS2, settings->settings2.buffer, prefs.getBytesLength(SETTINGS2));
  prefs.getBytes(SETTINGS3, settings->settings3.buffer, prefs.getBytesLength(SETTINGS3));
  prefs.getBytes(SETTINGS4, settings->settings4.buffer, prefs.getBytesLength(SETTINGS4));
  prefs.getBytes(SETTINGS5, settings->settings5.buffer, prefs.getBytesLength(SETTINGS5));
  prefs.getBytes(SETTINGS6, settings->settings6.buffer, prefs.getBytesLength(SETTINGS6));
  prefs.end();
  if (settings->settings1.buffer[0] == 0xff || settings->settings1.buffer[1] == 0xff)
    return false;

  return true;
}
