#include <Arduino.h>
#include "settings.h"
#include "prefs_storage.h"

#define BLE_PIN_CODE 147258

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
Settings::field_s6 Settings::getS6F()
{
  return settings6.fields;
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
unsigned char *Settings::getS6B()
{
  return settings6.buffer;
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
  Serial.print("// Electric_brake_disabled_on_high_voltage : ");
  Serial.println(settings1.fields.Electric_brake_disabled_on_high_voltage);
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
  Serial.print("// Battery_max_distance : ");
  Serial.println(settings3.fields.Battery_max_distance);
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

void Settings::displaySettings6()
{
  Serial.print("// Throttle_input_min : ");
  Serial.println(settings6.fields.Throttle_input_min);
  Serial.print("// Throttle_input_max : ");
  Serial.println(settings6.fields.Throttle_input_max);
  Serial.print("// Throttle_output_min : ");
  Serial.println(settings6.fields.Throttle_output_min);
  Serial.print("// Throttle_output_max : ");
  Serial.println(settings6.fields.Throttle_output_max);
  Serial.print("// Throttle_output_curve : ");
  Serial.println(settings6.fields.Throttle_output_curve);
  Serial.print("// Pas_enabled : ");
  Serial.println(settings6.fields.Pas_enabled);
  Serial.print("// Abs_enabled : ");
  Serial.println(settings6.fields.Abs_enabled);
  Serial.print("// Display_brightness : ");
  Serial.println(settings6.fields.Display_brightness);
  Serial.print("// Temperature_warning : ");
  Serial.println(settings6.fields.Temperature_warning);
  Serial.print("// Humidity_warning : ");
  Serial.println(settings6.fields.Humidity_warning);
  Serial.print("// Throttle_regeneration : ");
  Serial.println(settings6.fields.Throttle_regeneration);
  
}

void Settings::displaySettings()
{
  displaySettings1();
  displaySettings2();
  displaySettings3();
  displaySettings4();
  displaySettings5();
  displaySettings6();
}

void Settings::init()
{
  Serial.print("Settings::init : ");
  settings3.fields.Bluetooth_pin_code = BLE_PIN_CODE;
  settings6.fields.Display_brightness = 100;
}
