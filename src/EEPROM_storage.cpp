//////------------------------------------
////// EEPROM functions

#include <EEPROM.h>
#include "EEPROM_storage.h"
#include "main.h"
#include <Preferences.h>

#define APP_STORAGE "APP_STORAGE"

Preferences preferences;

EEPROMM_storage::EEPROMM_storage()
{
}

void EEPROMM_storage::init()
{
  EEPROM.begin(EEPROM_SIZE);

}

void EEPROMM_storage::test()
{
  int32_t test = 10;
  EEPROM.writeBytes(EEPROM_ADDRESS_TEST, (void *)&test, sizeof(test));
  EEPROM.commit();

  Serial.printf("1 - test = %d\n", test);
  test = 0;
  EEPROM.readBytes(EEPROM_ADDRESS_TEST, (void *)&test, sizeof(test));
  Serial.printf("2 - test = %d\n", test);
}

void EEPROMM_storage::setSettings(Settings *data)
{
  settings = data;
}

void EEPROMM_storage::setSharedData(SharedData *data)
{
  shrd = data;
}

void EEPROMM_storage::saveBleLockForced(int8_t *bleLockForced)
{
  EEPROM.writeBytes(EEPROM_ADDRESS_BLE_LOCK_FORCED, (void *)bleLockForced, sizeof(bleLockForced));
  EEPROM.commit();

  Serial.print("save bleLockForced value : ");
  Serial.println(*bleLockForced);
}

void EEPROMM_storage::restoreBleLockForced(int8_t *bleLockForced)
{
  EEPROM.readBytes(EEPROM_ADDRESS_BLE_LOCK_FORCED, (void *)bleLockForced, sizeof(bleLockForced));

  Serial.print("restore bleLockForced value : ");
  Serial.println(*bleLockForced);
}

void EEPROMM_storage::saveBrakeMinPressure()
{
  EEPROM.writeBytes(EEPROM_ADDRESS_BRAKE_MIN_PRESSURE, &shrd->brakeMinPressureRaw, sizeof(shrd->brakeMinPressureRaw));
  EEPROM.commit();

  Serial.print("save saveBrakeMinPressure value : ");
  Serial.println(shrd->brakeMinPressureRaw);
}

void EEPROMM_storage::restoreBrakeMinPressure()
{
  EEPROM.readBytes(EEPROM_ADDRESS_BRAKE_MIN_PRESSURE, &shrd->brakeMinPressureRaw, sizeof(shrd->brakeMinPressureRaw));

  Serial.print("restore restoreBrakeMinPressure value : ");
  Serial.println(shrd->brakeMinPressureRaw);

  if (shrd->brakeMinPressureRaw == -1)
    shrd->brakeMinPressureRaw = ANALOG_BRAKE_MIN_VALUE;
}

void EEPROMM_storage::saveBrakeMaxPressure()
{
  EEPROM.writeBytes(EEPROM_ADDRESS_BRAKE_MAX_PRESSURE, &shrd->brakeMaxPressureRaw, sizeof(shrd->brakeMaxPressureRaw));
  EEPROM.commit();

  Serial.print("save saveBrakeMaxPressure value : ");
  Serial.println(shrd->brakeMaxPressureRaw);
}

void EEPROMM_storage::restoreBrakeMaxPressure()
{
  EEPROM.readBytes(EEPROM_ADDRESS_BRAKE_MAX_PRESSURE, &shrd->brakeMaxPressureRaw, sizeof(shrd->brakeMaxPressureRaw));

  Serial.print("restore restoreBrakeMaxPressure value : ");
  Serial.println(shrd->brakeMaxPressureRaw);

  if (shrd->brakeMaxPressureRaw == -1)
    shrd->brakeMaxPressureRaw = ANALOG_BRAKE_MAX_VALUE;
}

void EEPROMM_storage::saveOdo()
{
  uint32_t time = micros();

  //  EEPROM.writeBytes(EEPROM_ADDRESS_ODO, &shrd->distanceOdo, sizeof(shrd->distanceOdo));
  //  EEPROM.commit();

  preferences.begin(APP_STORAGE, false);
  preferences.putUInt("distanceOdo", shrd->distanceOdo);
  preferences.end();

  Serial.print("time write : ");
  Serial.println(micros() - time);

  //Serial.print("save saveOdo value : ");
  //Serial.println(shrd->distanceOdo);
}

void EEPROMM_storage::restoreOdo()
{
  //EEPROM.readBytes(EEPROM_ADDRESS_ODO, &shrd->distanceOdo, sizeof(shrd->distanceOdo));

  preferences.begin(APP_STORAGE, false);
  shrd->distanceOdo = preferences.getUInt("distanceOdo", 0xffffffff);
  preferences.end();

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

void EEPROMM_storage::saveBatteryCalib()
{

  uint32_t time = millis();

  // Read EEPROM
  int EEAddr = EEPROM_ADDRESS_BATTERY_CALIB;
  EEPROM.put(EEAddr, shrd->batteryMaxVoltageCalibUser);
  EEAddr += sizeof(shrd->batteryMaxVoltageCalibUser);
  EEPROM.put(EEAddr, shrd->batteryMaxVoltageCalibRaw);
  EEAddr += sizeof(shrd->batteryMaxVoltageCalibRaw);
  EEPROM.put(EEAddr, shrd->batteryMinVoltageCalibUser);
  EEAddr += sizeof(shrd->batteryMinVoltageCalibUser);
  EEPROM.put(EEAddr, shrd->batteryMinVoltageCalibRaw);
  EEAddr += sizeof(shrd->batteryMinVoltageCalibRaw);

  EEPROM.commit();

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

void EEPROMM_storage::restoreBatteryCalib()
{
  // Read EEPROM
  int EEAddr = EEPROM_ADDRESS_BATTERY_CALIB;
  EEPROM.get(EEAddr, shrd->batteryMaxVoltageCalibUser);
  EEAddr += sizeof(shrd->batteryMaxVoltageCalibUser);
  EEPROM.get(EEAddr, shrd->batteryMaxVoltageCalibRaw);
  EEAddr += sizeof(shrd->batteryMaxVoltageCalibRaw);
  EEPROM.get(EEAddr, shrd->batteryMinVoltageCalibUser);
  EEAddr += sizeof(shrd->batteryMinVoltageCalibUser);
  EEPROM.get(EEAddr, shrd->batteryMinVoltageCalibRaw);
  EEAddr += sizeof(shrd->batteryMinVoltageCalibRaw);

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

void EEPROMM_storage::saveSettings()
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
  Serial.println(" bytes");

  EEPROM.put(EEPROM_ADDRESS_SETTINGS1, settings->settings1.buffer);
  EEPROM.put(EEPROM_ADDRESS_SETTINGS2, settings->settings2.buffer);
  EEPROM.put(EEPROM_ADDRESS_SETTINGS3, settings->settings3.buffer);
  EEPROM.put(EEPROM_ADDRESS_SETTINGS4, settings->settings4.buffer);
  EEPROM.put(EEPROM_ADDRESS_SETTINGS5, settings->settings5.buffer);
  EEPROM.commit();
}

boolean EEPROMM_storage::restoreSettings()
{

  Serial.print("Settings::restore : ");
  Serial.print(sizeof(settings->settings1));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings2));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings3));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings4));
  Serial.print(" / ");
  Serial.print(sizeof(settings->settings5));
  Serial.println(" bytes");

  EEPROM.get(EEPROM_ADDRESS_SETTINGS1, settings->settings1.buffer);
  EEPROM.get(EEPROM_ADDRESS_SETTINGS2, settings->settings2.buffer);
  EEPROM.get(EEPROM_ADDRESS_SETTINGS3, settings->settings3.buffer);
  EEPROM.get(EEPROM_ADDRESS_SETTINGS4, settings->settings4.buffer);
  EEPROM.get(EEPROM_ADDRESS_SETTINGS5, settings->settings5.buffer);

  if (settings->settings1.buffer[0] == 0xff || settings->settings1.buffer[1] == 0xff)
    return false;

  return true;
}
