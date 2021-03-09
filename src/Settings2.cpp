
//////------------------------------------
////// Manage settings exchanged in BLE and stored in EEPOM

#include "Arduino.h"
#include "Settings2.h"
#include "tools/buffer.h"

Settings2::Settings2()
{
}
                

/*-------------------------------------------------------*/

void Settings2::set_wheel_size(float value) {
    wheel_size = value;
}

float Settings2::get_wheel_size() {
    return wheel_size ;
}

void Settings2::display_wheel_size() {
    Serial.println("  wheel_size = " + (String) wheel_size);
}
                

/*-------------------------------------------------------*/

void Settings2::set_number_of_poles_pairs(uint8_t value) {
    number_of_poles_pairs = value;
}

uint8_t Settings2::get_number_of_poles_pairs() {
    return number_of_poles_pairs ;
}

void Settings2::display_number_of_poles_pairs() {
    Serial.println("  number_of_poles_pairs = " + (String) number_of_poles_pairs);
}
                

/*-------------------------------------------------------*/

void Settings2::set_battery_minimum_voltage(float value) {
    battery_minimum_voltage = value;
}

float Settings2::get_battery_minimum_voltage() {
    return battery_minimum_voltage ;
}

void Settings2::display_battery_minimum_voltage() {
    Serial.println("  battery_minimum_voltage = " + (String) battery_minimum_voltage);
}
                

/*-------------------------------------------------------*/

void Settings2::set_battery_maximum_voltage(float value) {
    battery_maximum_voltage = value;
}

float Settings2::get_battery_maximum_voltage() {
    return battery_maximum_voltage ;
}

void Settings2::display_battery_maximum_voltage() {
    Serial.println("  battery_maximum_voltage = " + (String) battery_maximum_voltage);
}
                

/*-------------------------------------------------------*/

void Settings2::set_battery_maximum_distance(uint8_t value) {
    battery_maximum_distance = value;
}

uint8_t Settings2::get_battery_maximum_distance() {
    return battery_maximum_distance ;
}

void Settings2::display_battery_maximum_distance() {
    Serial.println("  battery_maximum_distance = " + (String) battery_maximum_distance);
}
                

/*-------------------------------------------------------*/

void Settings2::set_speed_adjustment(int8_t value) {
    speed_adjustment = value;
}

int8_t Settings2::get_speed_adjustment() {
    return speed_adjustment ;
}

void Settings2::display_speed_adjustment() {
    Serial.println("  speed_adjustment = " + (String) speed_adjustment);
}
                

/*-------------------------------------------------------*/

void Settings2::set_display_gps_speed_instead_of_escooter_speed(uint8_t value) {
    display_gps_speed_instead_of_escooter_speed = value;
}

uint8_t Settings2::get_display_gps_speed_instead_of_escooter_speed() {
    return display_gps_speed_instead_of_escooter_speed ;
}

void Settings2::display_display_gps_speed_instead_of_escooter_speed() {
    Serial.println("  display_gps_speed_instead_of_escooter_speed = " + (String) display_gps_speed_instead_of_escooter_speed);
}
                

/*-------------------------------------------------------*/

void Settings2::set_aux_relay_name(uint8_t value) {
    aux_relay_name = value;
}

uint8_t Settings2::get_aux_relay_name() {
    return aux_relay_name ;
}

void Settings2::display_aux_relay_name() {
    Serial.println("  aux_relay_name = " + (String) aux_relay_name);
}
                

/*-------------------------------------------------------*/

void Settings2::set_default_mode_at_startup(uint8_t value) {
    default_mode_at_startup = value;
}

uint8_t Settings2::get_default_mode_at_startup() {
    return default_mode_at_startup ;
}

void Settings2::display_default_mode_at_startup() {
    Serial.println("  default_mode_at_startup = " + (String) default_mode_at_startup);
}
                

/*-------------------------------------------------------*/

void Settings2::set_default_eco_mode_at_startup(uint8_t value) {
    default_eco_mode_at_startup = value;
}

uint8_t Settings2::get_default_eco_mode_at_startup() {
    return default_eco_mode_at_startup ;
}

void Settings2::display_default_eco_mode_at_startup() {
    Serial.println("  default_eco_mode_at_startup = " + (String) default_eco_mode_at_startup);
}
                

/*-------------------------------------------------------*/

void Settings2::set_default_acceleration(uint8_t value) {
    default_acceleration = value;
}

uint8_t Settings2::get_default_acceleration() {
    return default_acceleration ;
}

void Settings2::display_default_acceleration() {
    Serial.println("  default_acceleration = " + (String) default_acceleration);
}
                

/*-------------------------------------------------------*/

void Settings2::set_default_electric_brake_at_startup(uint8_t value) {
    default_electric_brake_at_startup = value;
}

uint8_t Settings2::get_default_electric_brake_at_startup() {
    return default_electric_brake_at_startup ;
}

void Settings2::display_default_electric_brake_at_startup() {
    Serial.println("  default_electric_brake_at_startup = " + (String) default_electric_brake_at_startup);
}
                

/*-------------------------------------------------------*/

void Settings2::set_pas_enabled(uint8_t value) {
    pas_enabled = value;
}

uint8_t Settings2::get_pas_enabled() {
    return pas_enabled ;
}

void Settings2::display_pas_enabled() {
    Serial.println("  pas_enabled = " + (String) pas_enabled);
}
                

/*-------------------------------------------------------*/

void Settings2::set_abs_enabled(uint8_t value) {
    abs_enabled = value;
}

uint8_t Settings2::get_abs_enabled() {
    return abs_enabled ;
}

void Settings2::display_abs_enabled() {
    Serial.println("  abs_enabled = " + (String) abs_enabled);
}
                

/*-------------------------------------------------------*/

void Settings2::set_display_brightness(uint8_t value) {
    display_brightness = value;
}

uint8_t Settings2::get_display_brightness() {
    return display_brightness ;
}

void Settings2::display_display_brightness() {
    Serial.println("  display_brightness = " + (String) display_brightness);
}
                

/*-------------------------------------------------------*/

void Settings2::set_temperature_warning(uint8_t value) {
    temperature_warning = value;
}

uint8_t Settings2::get_temperature_warning() {
    return temperature_warning ;
}

void Settings2::display_temperature_warning() {
    Serial.println("  temperature_warning = " + (String) temperature_warning);
}
                

/*-------------------------------------------------------*/

void Settings2::set_humidity_warning(uint8_t value) {
    humidity_warning = value;
}

uint8_t Settings2::get_humidity_warning() {
    return humidity_warning ;
}

void Settings2::display_humidity_warning() {
    Serial.println("  humidity_warning = " + (String) humidity_warning);
}
                

/*-------------------------------------------------------*/

void Settings2::set_speed_limiter_at_startup(uint8_t value) {
    speed_limiter_at_startup = value;
}

uint8_t Settings2::get_speed_limiter_at_startup() {
    return speed_limiter_at_startup ;
}

void Settings2::display_speed_limiter_at_startup() {
    Serial.println("  speed_limiter_at_startup = " + (String) speed_limiter_at_startup);
}
                

/*-------------------------------------------------------*/

void Settings2::set_speed_limiter_max_speed_in_percent_of_max_speed(uint8_t value) {
    speed_limiter_max_speed_in_percent_of_max_speed = value;
}

uint8_t Settings2::get_speed_limiter_max_speed_in_percent_of_max_speed() {
    return speed_limiter_max_speed_in_percent_of_max_speed ;
}

void Settings2::display_speed_limiter_max_speed_in_percent_of_max_speed() {
    Serial.println("  speed_limiter_max_speed_in_percent_of_max_speed = " + (String) speed_limiter_max_speed_in_percent_of_max_speed);
}
                

/*-------------------------------------------------------*/

void Settings2::set_bluetooth_lock_mode(uint8_t value) {
    bluetooth_lock_mode = value;
}

uint8_t Settings2::get_bluetooth_lock_mode() {
    return bluetooth_lock_mode ;
}

void Settings2::display_bluetooth_lock_mode() {
    Serial.println("  bluetooth_lock_mode = " + (String) bluetooth_lock_mode);
}
                

/*-------------------------------------------------------*/

void Settings2::set_pin_code_use_6_digits_only(uint32_t value) {
    pin_code_use_6_digits_only = value;
}

uint32_t Settings2::get_pin_code_use_6_digits_only() {
    return pin_code_use_6_digits_only ;
}

void Settings2::display_pin_code_use_6_digits_only() {
    Serial.println("  pin_code_use_6_digits_only = " + (String) pin_code_use_6_digits_only);
}
                

/*-------------------------------------------------------*/

void Settings2::set_beacon_mac_address(String value) {
    beacon_mac_address = value;
}

String Settings2::get_beacon_mac_address() {
    return beacon_mac_address ;
}

void Settings2::display_beacon_mac_address() {
    Serial.println("  beacon_mac_address = " + (String) beacon_mac_address);
}
                

/*-------------------------------------------------------*/

void Settings2::set_beacon_range(int8_t value) {
    beacon_range = value;
}

int8_t Settings2::get_beacon_range() {
    return beacon_range ;
}

void Settings2::display_beacon_range() {
    Serial.println("  beacon_range = " + (String) beacon_range);
}
                

/*-------------------------------------------------------*/

void Settings2::set_original_display_speed_adjustment(int8_t value) {
    original_display_speed_adjustment = value;
}

int8_t Settings2::get_original_display_speed_adjustment() {
    return original_display_speed_adjustment ;
}

void Settings2::display_original_display_speed_adjustment() {
    Serial.println("  original_display_speed_adjustment = " + (String) original_display_speed_adjustment);
}
                

/*-------------------------------------------------------*/

void Settings2::set_electric_brake_progressive_mode(uint8_t value) {
    electric_brake_progressive_mode = value;
}

uint8_t Settings2::get_electric_brake_progressive_mode() {
    return electric_brake_progressive_mode ;
}

void Settings2::display_electric_brake_progressive_mode() {
    Serial.println("  electric_brake_progressive_mode = " + (String) electric_brake_progressive_mode);
}
                

/*-------------------------------------------------------*/

void Settings2::set_electric_brake_type(uint8_t value) {
    electric_brake_type = value;
}

uint8_t Settings2::get_electric_brake_type() {
    return electric_brake_type ;
}

void Settings2::display_electric_brake_type() {
    Serial.println("  electric_brake_type = " + (String) electric_brake_type);
}
                

/*-------------------------------------------------------*/

void Settings2::set_electric_brake_min_value(uint8_t value) {
    electric_brake_min_value = value;
}

uint8_t Settings2::get_electric_brake_min_value() {
    return electric_brake_min_value ;
}

void Settings2::display_electric_brake_min_value() {
    Serial.println("  electric_brake_min_value = " + (String) electric_brake_min_value);
}
                

/*-------------------------------------------------------*/

void Settings2::set_electric_brake_max_value(uint8_t value) {
    electric_brake_max_value = value;
}

uint8_t Settings2::get_electric_brake_max_value() {
    return electric_brake_max_value ;
}

void Settings2::display_electric_brake_max_value() {
    Serial.println("  electric_brake_max_value = " + (String) electric_brake_max_value);
}
                

/*-------------------------------------------------------*/

void Settings2::set_electric_brake_time_between_mode_shift(uint8_t value) {
    electric_brake_time_between_mode_shift = value;
}

uint8_t Settings2::get_electric_brake_time_between_mode_shift() {
    return electric_brake_time_between_mode_shift ;
}

void Settings2::display_electric_brake_time_between_mode_shift() {
    Serial.println("  electric_brake_time_between_mode_shift = " + (String) electric_brake_time_between_mode_shift);
}
                

/*-------------------------------------------------------*/

void Settings2::set_electric_brake_disabled_on_high_battery_voltage(uint8_t value) {
    electric_brake_disabled_on_high_battery_voltage = value;
}

uint8_t Settings2::get_electric_brake_disabled_on_high_battery_voltage() {
    return electric_brake_disabled_on_high_battery_voltage ;
}

void Settings2::display_electric_brake_disabled_on_high_battery_voltage() {
    Serial.println("  electric_brake_disabled_on_high_battery_voltage = " + (String) electric_brake_disabled_on_high_battery_voltage);
}
                

/*-------------------------------------------------------*/

void Settings2::set_electric_brake_disabled_percent_limit(uint8_t value) {
    electric_brake_disabled_percent_limit = value;
}

uint8_t Settings2::get_electric_brake_disabled_percent_limit() {
    return electric_brake_disabled_percent_limit ;
}

void Settings2::display_electric_brake_disabled_percent_limit() {
    Serial.println("  electric_brake_disabled_percent_limit = " + (String) electric_brake_disabled_percent_limit);
}
                

/*-------------------------------------------------------*/

void Settings2::set_throttle_regeneration(uint8_t value) {
    throttle_regeneration = value;
}

uint8_t Settings2::get_throttle_regeneration() {
    return throttle_regeneration ;
}

void Settings2::display_throttle_regeneration() {
    Serial.println("  throttle_regeneration = " + (String) throttle_regeneration);
}
                

/*-------------------------------------------------------*/

void Settings2::set_throttle_input_min_voltage_in_millivolts(uint32_t value) {
    throttle_input_min_voltage_in_millivolts = value;
}

uint32_t Settings2::get_throttle_input_min_voltage_in_millivolts() {
    return throttle_input_min_voltage_in_millivolts ;
}

void Settings2::display_throttle_input_min_voltage_in_millivolts() {
    Serial.println("  throttle_input_min_voltage_in_millivolts = " + (String) throttle_input_min_voltage_in_millivolts);
}
                

/*-------------------------------------------------------*/

void Settings2::set_throttle_input_max_voltage_in_millivolts(uint32_t value) {
    throttle_input_max_voltage_in_millivolts = value;
}

uint32_t Settings2::get_throttle_input_max_voltage_in_millivolts() {
    return throttle_input_max_voltage_in_millivolts ;
}

void Settings2::display_throttle_input_max_voltage_in_millivolts() {
    Serial.println("  throttle_input_max_voltage_in_millivolts = " + (String) throttle_input_max_voltage_in_millivolts);
}
                

/*-------------------------------------------------------*/

void Settings2::set_throttle_output_min_voltage_in_millivolts(uint32_t value) {
    throttle_output_min_voltage_in_millivolts = value;
}

uint32_t Settings2::get_throttle_output_min_voltage_in_millivolts() {
    return throttle_output_min_voltage_in_millivolts ;
}

void Settings2::display_throttle_output_min_voltage_in_millivolts() {
    Serial.println("  throttle_output_min_voltage_in_millivolts = " + (String) throttle_output_min_voltage_in_millivolts);
}
                

/*-------------------------------------------------------*/

void Settings2::set_throttle_output_max_voltage_in_millivolts(uint32_t value) {
    throttle_output_max_voltage_in_millivolts = value;
}

uint32_t Settings2::get_throttle_output_max_voltage_in_millivolts() {
    return throttle_output_max_voltage_in_millivolts ;
}

void Settings2::display_throttle_output_max_voltage_in_millivolts() {
    Serial.println("  throttle_output_max_voltage_in_millivolts = " + (String) throttle_output_max_voltage_in_millivolts);
}
                

/*-------------------------------------------------------*/

void Settings2::set_throttle_output_curve(uint8_t value) {
    throttle_output_curve = value;
}

uint8_t Settings2::get_throttle_output_curve() {
    return throttle_output_curve ;
}

void Settings2::display_throttle_output_curve() {
    Serial.println("  throttle_output_curve = " + (String) throttle_output_curve);
}
                

/*-------------------------------------------------------*/

void Settings2::set_button_1_short_press_action(uint8_t value) {
    button_1_short_press_action = value;
}

uint8_t Settings2::get_button_1_short_press_action() {
    return button_1_short_press_action ;
}

void Settings2::display_button_1_short_press_action() {
    Serial.println("  button_1_short_press_action = " + (String) button_1_short_press_action);
}
                

/*-------------------------------------------------------*/

void Settings2::set_button_1_long_press_action(uint8_t value) {
    button_1_long_press_action = value;
}

uint8_t Settings2::get_button_1_long_press_action() {
    return button_1_long_press_action ;
}

void Settings2::display_button_1_long_press_action() {
    Serial.println("  button_1_long_press_action = " + (String) button_1_long_press_action);
}
                

/*-------------------------------------------------------*/

void Settings2::set_button_2_short_press_action(uint8_t value) {
    button_2_short_press_action = value;
}

uint8_t Settings2::get_button_2_short_press_action() {
    return button_2_short_press_action ;
}

void Settings2::display_button_2_short_press_action() {
    Serial.println("  button_2_short_press_action = " + (String) button_2_short_press_action);
}
                

/*-------------------------------------------------------*/

void Settings2::set_button_long_press_duration(uint8_t value) {
    button_long_press_duration = value;
}

uint8_t Settings2::get_button_long_press_duration() {
    return button_long_press_duration ;
}

void Settings2::display_button_long_press_duration() {
    Serial.println("  button_long_press_duration = " + (String) button_long_press_duration);
}
                

/*-------------------------------------------------------*/

void Settings2::set_wifi_network_name_ssid(String value) {
    wifi_network_name_ssid = value;
}

String Settings2::get_wifi_network_name_ssid() {
    return wifi_network_name_ssid ;
}

void Settings2::display_wifi_network_name_ssid() {
    Serial.println("  wifi_network_name_ssid = " + (String) wifi_network_name_ssid);
}
                

/*-------------------------------------------------------*/

void Settings2::set_wifi_password(String value) {
    wifi_password = value;
}

String Settings2::get_wifi_password() {
    return wifi_password ;
}

void Settings2::display_wifi_password() {
    Serial.println("  wifi_password = " + (String) wifi_password);
}
