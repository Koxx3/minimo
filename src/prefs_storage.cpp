//////------------------------------------
////// PREFERENCES functions

#include "main.h"
#include <prefs_storage.h>
#define APP_STORAGE "APP_STORAGE"

void preferences::test()
{
  int8_t test = 10;

  prefs.begin(APP_STORAGE, false);
  prefs.putInt("test", test);
  prefs.end();

  Serial.printf("1 - test = %d\n", test);
  test = 0;

  prefs.begin(APP_STORAGE, false);
  test = prefs.getInt("test");
  prefs.end();

  Serial.printf("2 - test = %d\n", test);
}

void preferences::setSettings(Settings *data)
{
  settings = data;
}

void preferences::setSharedData(SharedData *data)
{
  shrd = data;
}

void preferences::saveBleLockForced(int8_t *bleLockForced)
{
  prefs.begin(APP_STORAGE, false);
  prefs.putInt("bleLockForced", *bleLockForced);
  prefs.end();

  Serial.print("save bleLockForced value : ");
  Serial.println(*bleLockForced);
}

void preferences::restoreBleLockForced(int8_t *bleLockForced)
{
  prefs.begin(APP_STORAGE, false);
  *bleLockForced = prefs.getInt("bleLockForced", 0);
  prefs.end();

  Serial.print("restore bleLockForced value : ");
  Serial.println(*bleLockForced);
}

void preferences::saveBrakeMinPressure()
{
  prefs.begin(APP_STORAGE, false);
  prefs.putInt("brakeMinPressureRaw", shrd->brakeMinPressureRaw);
  prefs.end();

  Serial.print("save saveBrakeMinPressure value : ");
  Serial.println(shrd->brakeMinPressureRaw);
}

void preferences::restoreBrakeMinPressure()
{
  prefs.begin(APP_STORAGE, false);
  shrd->brakeMinPressureRaw = prefs.getInt("brakeMinPressureRaw", 900);
  prefs.end();

  Serial.print("restore restoreBrakeMinPressure value : ");
  Serial.println(shrd->brakeMinPressureRaw);

  if (shrd->brakeMinPressureRaw == -1)                                          //A REVOIR. TJR NECESSAIRE AVEC PREFS ?
    shrd->brakeMinPressureRaw = ANALOG_BRAKE_MIN_VALUE;
}

void preferences::saveBrakeMaxPressure()
{
  prefs.begin(APP_STORAGE, false);
  prefs.putInt("brakeMaxPressureRaw", shrd->brakeMaxPressureRaw);
  prefs.end();

  Serial.print("save saveBrakeMaxPressure value : ");
  Serial.println(shrd->brakeMaxPressureRaw);
}

void preferences::restoreBrakeMaxPressure()
{
  prefs.begin(APP_STORAGE, false);
  shrd->brakeMaxPressureRaw = prefs.getInt("brakeMaxPressureRaw", 2000);
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
  prefs.putUInt("distanceOdo", shrd->distanceOdo);
  prefs.end();

  //Serial.print("time write : ");
  //Serial.println(micros() - time);

  //Serial.print("save saveOdo value : ");
  //Serial.println(shrd->distanceOdo);
}

void preferences::restoreOdo()
{
  

  prefs.begin(APP_STORAGE, false);
  shrd->distanceOdo = prefs.getUInt("distanceOdo", 0xffffffff);
  prefs.end();

  shrd->distanceOdoInFlash = shrd->distanceOdo;
  shrd->distanceOdoBoot = shrd->distanceOdo;

  Serial.print("restore restoreOdo value : ");
  Serial.println(shrd->distanceOdo);

  if (shrd->distanceOdo == 0xffffffff)
  {
    shrd->distanceOdo = 0;
    shrd->distanceOdoBoot = 0;
    shrd->distanceOdoInFlash = 0;
    saveOdo();
    Serial.print("==> ODO init at 0");
  }
}

void preferences::saveBatteryCalib()
{

  uint32_t time = millis();

  prefs.begin(APP_STORAGE, false);
  prefs.putFloat ("batteryMaxVoltageCalibUser", shrd->batteryMaxVoltageCalibUser);
  prefs.putFloat("batteryMaxVoltageCalibRaw", shrd->batteryMaxVoltageCalibRaw);
  prefs.putFloat("batteryMinVoltageCalibUser", shrd->batteryMinVoltageCalibUser);
  prefs.putFloat("batteryMinVoltageCalibRaw", shrd->batteryMinVoltageCalibRaw);
  prefs.end();

  Serial.println(millis() - time);

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
  shrd->batteryMaxVoltageCalibUser = prefs.getFloat("batteryMaxVoltageCalibUser");
  shrd->batteryMaxVoltageCalibRaw = prefs.getFloat("batteryMaxVoltageCalibRaw");
  shrd->batteryMinVoltageCalibUser = prefs.getFloat("batteryMinVoltageCalibUser");
  shrd->batteryMinVoltageCalibRaw = prefs.getFloat("batteryMinVoltageCalibRaw");
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
  Serial.print("saveSettings : ");
  Serial.print(sizeof(settings->settings1));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings2));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings3));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings4));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings5));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings6));
  
  Serial.println(" bytes");

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

  Serial.print("Settings::restore : ");
  Serial.print(sizeof(settings->settings1));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings2));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings3));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings4));
  Serial.println(" bytes");
    
  prefs.begin(APP_STORAGE, false);
  prefs.getBytes(SETTINGS1, settings->settings1.buffer, prefs.getBytesLength(SETTINGS1));
  prefs.getBytes(SETTINGS2, settings->settings2.buffer, prefs.getBytesLength(SETTINGS2));
  prefs.getBytes(SETTINGS3, settings->settings3.buffer, prefs.getBytesLength(SETTINGS3));
  prefs.getBytes(SETTINGS4, settings->settings4.buffer, prefs.getBytesLength(SETTINGS4));
  prefs.getBytes(SETTINGS5, settings->settings5.buffer, prefs.getBytesLength(SETTINGS5));
  prefs.end();

  if (settings->settings1.buffer[0] == 0xff || settings->settings1.buffer[1] == 0xff)
    return false;

  return true;
}
