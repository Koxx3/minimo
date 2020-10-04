
#include <Arduino.h>
#include <EEPROM.h>
#include "settings.h"

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

unsigned char* Settings::getS1B()
{
  return settings1.buffer;
}
unsigned char* Settings::getS2B()
{
  return settings2.buffer;
}
unsigned char* Settings::getS3B()
{
  return settings3.buffer;
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
  Serial.print("// Electric_brake_disabled_voltage_limit : ");
  Serial.println(settings1.fields.Electric_brake_disabled_voltage_limit);
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
}

void Settings::displaySettings2()
{
  Serial.print("// Beacon_Mac_Address : ");
  Serial.println(settings2.fields.Beacon_Mac_Address);
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
}

void Settings::displaySettings()
{
  displaySettings1();
  displaySettings2();
  displaySettings3();
}

void Settings::saveSettings(uint32_t address)
{
  Serial.print("saveSettings : ");
  Serial.print(sizeof(settings1));
  Serial.print(" / ");
  Serial.print(sizeof(settings2));
  Serial.print(" / ");
  Serial.print(sizeof(settings3));
  Serial.println(" bytes");

  EEPROM.put(address + 00, settings1.buffer);
  EEPROM.put(address + 25, settings2.buffer);
  EEPROM.put(address + 50, settings3.buffer);

  displaySettings2();
}

void Settings::restoreSettings(uint32_t address)
{

  Serial.print("restoreSettings : ");
  Serial.print(sizeof(settings1));
  Serial.print(" / ");
  Serial.print(sizeof(settings2));
  Serial.print(" / ");
  Serial.print(sizeof(settings3));
  Serial.println(" bytes");

  EEPROM.get(address + 00, settings1.buffer);
  EEPROM.get(address + 25, settings2.buffer);
  EEPROM.get(address + 50, settings3.buffer);

  displaySettings2();
}
