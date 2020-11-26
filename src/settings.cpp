#include <Arduino.h>
#include <EEPROM.h>
#include "settings.h"
#include "EEPROM_storage.h"

Settings::Settings()
{
}

Settings::field_s1 Settings::getS1F()
{
  return settings1.fields;
}
Settings::field_s2 Settings::getS2F()
{
  return settings2.fields;
}
Settings::field_s3 Settings::getS3F()
{
  return settings3.fields;
}
Settings::field_s4 Settings::getS4F()
{
  return settings4.fields;
}
Settings::field_s5 Settings::getS5F()
{
  return settings5.fields;
}

unsigned char *Settings::getS1B()
{
  return settings1.buffer;
}
unsigned char *Settings::getS2B()
{
  return settings2.buffer;
}
unsigned char *Settings::getS3B()
{
  return settings3.buffer;
}
unsigned char *Settings::getS4B()
{
  return settings4.buffer;
}
unsigned char *Settings::getS5B()
{
  return settings5.buffer;
}

void Settings::displaySettings1()
{
  Serial.print("// Beacon_range : ");
  Serial.println(settings1.fields.Beacon_range);
  Serial.print("// Mode_Z_Power_limitation : ");
  Serial.println(settings1.fields.Mode_Z_Power_limitation);
  Serial.print("// Mode_Z_Eco_mode : ");
  Serial.println(settings1.fields.Mode_Z_Eco_mode);
  Serial.print("// Mode_Z_Acceleration : ");
  Serial.println(settings1.fields.Mode_Z_Acceleration);
  Serial.print("// Electric_brake_progressive_mode : ");
  Serial.println(settings1.fields.Electric_brake_progressive_mode);
  Serial.print("// Electric_brake_min_value : ");
  Serial.println(settings1.fields.Electric_brake_min_value);
  Serial.print("// Electric_brake_max_value : ");
  Serial.println(settings1.fields.Electric_brake_max_value);
  Serial.print("// Electric_brake_time_between_mode_shift : ");
  Serial.println(settings1.fields.Electric_brake_time_between_mode_shift);
  Serial.print("// Electric_brake_disabled_condition : ");
  Serial.println(settings1.fields.Electric_brake_disabled_condition);
  Serial.print("// Electric_brake_disabled_percent_limit : ");
  Serial.println(settings1.fields.Electric_brake_disabled_percent_limit);
  Serial.print("// Current_loop_mode : ");
  Serial.println(settings1.fields.Current_loop_mode);
  Serial.print("// Current_loop_max_current : ");
  Serial.println(settings1.fields.Current_loop_max_current);
  Serial.print("// Speed_loop_mode : ");
  Serial.println(settings1.fields.Speed_loop_mode);
  Serial.print("// Speed_limiter_at_startup : ");
  Serial.println(settings1.fields.Speed_limiter_at_startup);
  Serial.print("// Wheel_size : ");
  Serial.println(settings1.fields.Wheel_size);
  Serial.print("// Motor_pole_number : ");
  Serial.println(settings1.fields.Motor_pole_number);
  Serial.print("// Bluetooth_lock_mode : ");
  Serial.println(settings1.fields.Bluetooth_lock_mode);
  Serial.print("// LCD_Speed_adjustement : ");
  Serial.println(settings1.fields.LCD_Speed_adjustement);
  Serial.print("// Speed_limiter_max_speed : ");
  Serial.println(settings1.fields.Speed_limiter_max_speed);
  

}

void Settings::displaySettings2()
{
  Serial.print("// Beacon_Mac_Address : ");
  Serial.println(settings2.fields.Beacon_Mac_Address);
  Serial.print("// Electric_brake_type : ");
  Serial.println(settings2.fields.Electric_brake_type);
}

void Settings::displaySettings3()
{
  Serial.print("// Button_1_short_press_action : ");
  Serial.println(settings3.fields.Button_1_short_press_action);
  Serial.print("// Button_1_long_press_action : ");
  Serial.println(settings3.fields.Button_1_long_press_action);
  Serial.print("// Button_2_short_press_action : ");
  Serial.println(settings3.fields.Button_2_short_press_action);
  Serial.print("// Button_2_long_press_action : ");
  Serial.println(settings3.fields.Button_2_long_press_action);
  Serial.print("// Button_long_press_duration : ");
  Serial.println(settings3.fields.Button_long_press_duration);
  Serial.print("// Battery_min_voltage : ");
  Serial.println(settings3.fields.Battery_min_voltage);
  Serial.print("// Battery_max_voltage : ");
  Serial.println(settings3.fields.Battery_max_voltage);
  Serial.print("// Battery_saving_medium_voltage : ");
  Serial.println(settings3.fields.Battery_saving_medium_voltage);
  Serial.print("// Battery_saving_strong_voltage : ");
  Serial.println(settings3.fields.Battery_saving_strong_voltage);
  Serial.print("// Bluetooth_pin_code : ");
  Serial.println(settings3.fields.Bluetooth_pin_code);
  Serial.print("// Battery_capacity : ");
  Serial.println(settings3.fields.Battery_capacity);
  Serial.print("// Battery_discharge_rate : ");
  Serial.println(settings3.fields.Battery_discharge_rate);

}

void Settings::displaySettings4()
{
  Serial.print("// Wifi_ssid : ");
  Serial.println(settings4.fields.Wifi_ssid);
}

void Settings::displaySettings5()
{
  Serial.print("// Wifi_pwd : ");
  Serial.println(settings5.fields.Wifi_pwd);
}

void Settings::displaySettings()
{
  displaySettings1();
  displaySettings2();
  displaySettings3();
  displaySettings4();
  displaySettings5();
}

void Settings::saveSettings()
{
  Serial.print("saveSettings : ");
  Serial.print(sizeof(settings1));
  Serial.print(" / ");
  Serial.print(sizeof(settings2));
  Serial.print(" / ");
  Serial.print(sizeof(settings3));
  Serial.print(" / ");
  Serial.print(sizeof(settings4));
  Serial.print(" / ");
  Serial.print(sizeof(settings5));
  Serial.println(" bytes");

  EEPROM.put(EEPROM_ADDRESS_SETTINGS1, settings1.buffer);
  EEPROM.put(EEPROM_ADDRESS_SETTINGS2, settings2.buffer);
  EEPROM.put(EEPROM_ADDRESS_SETTINGS3, settings3.buffer);
  EEPROM.put(EEPROM_ADDRESS_SETTINGS4, settings4.buffer);
  EEPROM.put(EEPROM_ADDRESS_SETTINGS5, settings5.buffer);
  EEPROM.commit();
}

boolean Settings::restoreSettings()
{

  Serial.print("restoreSettings : ");
  Serial.print(sizeof(settings1));
  Serial.print(" / ");
  Serial.print(sizeof(settings2));
  Serial.print(" / ");
  Serial.print(sizeof(settings3));
  Serial.print(" / ");
  Serial.print(sizeof(settings4));
  Serial.print(" / ");
  Serial.print(sizeof(settings5));
  Serial.println(" bytes");

  EEPROM.get(EEPROM_ADDRESS_SETTINGS1, settings1.buffer);
  EEPROM.get(EEPROM_ADDRESS_SETTINGS2, settings2.buffer);
  EEPROM.get(EEPROM_ADDRESS_SETTINGS3, settings3.buffer);
  EEPROM.get(EEPROM_ADDRESS_SETTINGS4, settings4.buffer);
  EEPROM.get(EEPROM_ADDRESS_SETTINGS5, settings5.buffer);

  if (settings1.buffer[0] == 0xff ||settings1.buffer[1] == 0xff)
  return false;

  return true;
}


void Settings::initSettings()
{
  Serial.print("initSettings : ");
  settings3.fields.Bluetooth_pin_code = 147258;

}
