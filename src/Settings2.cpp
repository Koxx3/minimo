
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

////// Manage settings exchanged in BLE and stored in EEPOM

#include "Arduino.h"
#include "Settings2.h"
#include "tools/buffer.h"

#define SETTINGS_STORAGE "SETTINGS"

Settings2::Settings2()
{
}

void Settings2::restore() {

    Serial.println(" > restore settings");
    prefs.begin(SETTINGS_STORAGE, false);
    wheel_size = prefs.getFloat(SETTINGS_WHEEL_SIZE_ID_STR, 10.0);
    Serial.println("  >> wheel_size = " + (String)wheel_size);
    number_of_poles_pairs = prefs.getInt(SETTINGS_NUMBER_OF_POLES_PAIRS_ID_STR, 15);
    Serial.println("  >> number_of_poles_pairs = " + (String)number_of_poles_pairs);
    battery_minimum_voltage = prefs.getFloat(SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID_STR, 42.0);
    Serial.println("  >> battery_minimum_voltage = " + (String)battery_minimum_voltage);
    battery_maximum_voltage = prefs.getFloat(SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID_STR, 58.8);
    Serial.println("  >> battery_maximum_voltage = " + (String)battery_maximum_voltage);
    battery_maximum_distance = prefs.getInt(SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID_STR, 40);
    Serial.println("  >> battery_maximum_distance = " + (String)battery_maximum_distance);
    speed_adjustment = prefs.getInt(SETTINGS_SPEED_ADJUSTMENT_ID_STR, 0);
    Serial.println("  >> speed_adjustment = " + (String)speed_adjustment);
    display_gps_speed_instead_of_escooter_speed = prefs.getInt(SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID_STR, 0);
    Serial.println("  >> display_gps_speed_instead_of_escooter_speed = " + (String)display_gps_speed_instead_of_escooter_speed);
    aux_relay_name = prefs.getInt(SETTINGS_AUX_RELAY_NAME_ID_STR, 0);
    Serial.println("  >> aux_relay_name = " + (String)aux_relay_name);
    default_mode_at_startup = prefs.getInt(SETTINGS_DEFAULT_MODE_AT_STARTUP_ID_STR, 3);
    Serial.println("  >> default_mode_at_startup = " + (String)default_mode_at_startup);
    default_eco_mode_at_startup = prefs.getInt(SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID_STR, 2);
    Serial.println("  >> default_eco_mode_at_startup = " + (String)default_eco_mode_at_startup);
    default_acceleration = prefs.getInt(SETTINGS_DEFAULT_ACCELERATION_ID_STR, 5);
    Serial.println("  >> default_acceleration = " + (String)default_acceleration);
    default_electric_brake_at_startup = prefs.getInt(SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID_STR, 3);
    Serial.println("  >> default_electric_brake_at_startup = " + (String)default_electric_brake_at_startup);
    pas_enabled = prefs.getInt(SETTINGS_PAS_ENABLED_ID_STR, 0);
    Serial.println("  >> pas_enabled = " + (String)pas_enabled);
    abs_enabled = prefs.getInt(SETTINGS_ABS_ENABLED_ID_STR, 0);
    Serial.println("  >> abs_enabled = " + (String)abs_enabled);
    display_brightness = prefs.getInt(SETTINGS_DISPLAY_BRIGHTNESS_ID_STR, 100);
    Serial.println("  >> display_brightness = " + (String)display_brightness);
    temperature_warning = prefs.getInt(SETTINGS_TEMPERATURE_WARNING_ID_STR, 70);
    Serial.println("  >> temperature_warning = " + (String)temperature_warning);
    humidity_warning = prefs.getInt(SETTINGS_HUMIDITY_WARNING_ID_STR, 80);
    Serial.println("  >> humidity_warning = " + (String)humidity_warning);
    speed_limiter_at_startup = prefs.getInt(SETTINGS_SPEED_LIMITER_AT_STARTUP_ID_STR, 0);
    Serial.println("  >> speed_limiter_at_startup = " + (String)speed_limiter_at_startup);
    speed_limiter_max_speed_in_percent_of_max_speed = prefs.getInt(SETTINGS_SPEED_LIMITER_MAX_SPEED_IN_PERCENT_OF_MAX_SPEED_ID_STR, 37);
    Serial.println("  >> speed_limiter_max_speed_in_percent_of_max_speed = " + (String)speed_limiter_max_speed_in_percent_of_max_speed);
    bluetooth_lock_mode = prefs.getInt(SETTINGS_BLUETOOTH_LOCK_MODE_ID_STR, 0);
    Serial.println("  >> bluetooth_lock_mode = " + (String)bluetooth_lock_mode);
    pin_code_use_6_digits_only = prefs.getInt(SETTINGS_PIN_CODE_USE_6_DIGITS_ONLY_ID_STR, 147258);
    Serial.println("  >> pin_code_use_6_digits_only = " + (String)pin_code_use_6_digits_only);
    beacon_mac_address = prefs.getString(SETTINGS_BEACON_MAC_ADDRESS_ID_STR, "aa:bb:cc:dd:ee:ff");
    Serial.println("  >> beacon_mac_address = " + (String)beacon_mac_address);
    beacon_range = prefs.getInt(SETTINGS_BEACON_RANGE_ID_STR, -80);
    Serial.println("  >> beacon_range = " + (String)beacon_range);
    original_display_speed_adjustment = prefs.getInt(SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID_STR, 0);
    Serial.println("  >> original_display_speed_adjustment = " + (String)original_display_speed_adjustment);
    electric_brake_progressive_mode = prefs.getInt(SETTINGS_ELECTRIC_BRAKE_PROGRESSIVE_MODE_ID_STR, 0);
    Serial.println("  >> electric_brake_progressive_mode = " + (String)electric_brake_progressive_mode);
    electric_brake_type = prefs.getInt(SETTINGS_ELECTRIC_BRAKE_TYPE_ID_STR, 0);
    Serial.println("  >> electric_brake_type = " + (String)electric_brake_type);
    electric_brake_min_value = prefs.getInt(SETTINGS_ELECTRIC_BRAKE_MIN_VALUE_ID_STR, 1);
    Serial.println("  >> electric_brake_min_value = " + (String)electric_brake_min_value);
    electric_brake_max_value = prefs.getInt(SETTINGS_ELECTRIC_BRAKE_MAX_VALUE_ID_STR, 5);
    Serial.println("  >> electric_brake_max_value = " + (String)electric_brake_max_value);
    electric_brake_time_between_mode_shift = prefs.getInt(SETTINGS_ELECTRIC_BRAKE_TIME_BETWEEN_MODE_SHIFT_ID_STR, 500);
    Serial.println("  >> electric_brake_time_between_mode_shift = " + (String)electric_brake_time_between_mode_shift);
    electric_brake_disabled_on_high_battery_voltage = prefs.getInt(SETTINGS_ELECTRIC_BRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID_STR, 0);
    Serial.println("  >> electric_brake_disabled_on_high_battery_voltage = " + (String)electric_brake_disabled_on_high_battery_voltage);
    electric_brake_disabled_percent_limit = prefs.getInt(SETTINGS_ELECTRIC_BRAKE_DISABLED_PERCENT_LIMIT_ID_STR, 100);
    Serial.println("  >> electric_brake_disabled_percent_limit = " + (String)electric_brake_disabled_percent_limit);
    throttle_regeneration = prefs.getInt(SETTINGS_THROTTLE_REGENERATION_ID_STR, 0);
    Serial.println("  >> throttle_regeneration = " + (String)throttle_regeneration);
    throttle_input_min_voltage_in_millivolts = prefs.getInt(SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, 650);
    Serial.println("  >> throttle_input_min_voltage_in_millivolts = " + (String)throttle_input_min_voltage_in_millivolts);
    throttle_input_max_voltage_in_millivolts = prefs.getInt(SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, 3950);
    Serial.println("  >> throttle_input_max_voltage_in_millivolts = " + (String)throttle_input_max_voltage_in_millivolts);
    throttle_output_min_voltage_in_millivolts = prefs.getInt(SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, 800);
    Serial.println("  >> throttle_output_min_voltage_in_millivolts = " + (String)throttle_output_min_voltage_in_millivolts);
    throttle_output_max_voltage_in_millivolts = prefs.getInt(SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, 3350);
    Serial.println("  >> throttle_output_max_voltage_in_millivolts = " + (String)throttle_output_max_voltage_in_millivolts);
    throttle_output_curve = prefs.getInt(SETTINGS_THROTTLE_OUTPUT_CURVE_ID_STR, 2);
    Serial.println("  >> throttle_output_curve = " + (String)throttle_output_curve);
    button_1_short_press_action = prefs.getInt(SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_ID_STR, 0);
    Serial.println("  >> button_1_short_press_action = " + (String)button_1_short_press_action);
    button_1_long_press_action = prefs.getInt(SETTINGS_BUTTON_1_LONG_PRESS_ACTION_ID_STR, 0);
    Serial.println("  >> button_1_long_press_action = " + (String)button_1_long_press_action);
    button_2_short_press_action = prefs.getInt(SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_ID_STR, 0);
    Serial.println("  >> button_2_short_press_action = " + (String)button_2_short_press_action);
    button_long_press_duration = prefs.getInt(SETTINGS_BUTTON_LONG_PRESS_DURATION_ID_STR, 5);
    Serial.println("  >> button_long_press_duration = " + (String)button_long_press_duration);
    wifi_network_name_ssid = prefs.getString(SETTINGS_WIFI_NETWORK_NAME_SSID_ID_STR, "");
    Serial.println("  >> wifi_network_name_ssid = " + (String)wifi_network_name_ssid);
    wifi_password = prefs.getString(SETTINGS_WIFI_PASSWORD_ID_STR, "");
    Serial.println("  >> wifi_password = " + (String)wifi_password);
    prefs.end();
    Serial.println(" < restore settings");
}

void Settings2::unpack_setting_packet(uint8_t* packet, uint8_t length) {
    
    int32_t ind = 0;

    uint16_t settingId = buffer_get_uint16(packet, &ind);
    uint16_t packetNumber = buffer_get_uint16(packet, &ind);

    Serial.println("settingId = " + (String)settingId);
    Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
    case SETTINGS_WHEEL_SIZE_ID :
        set_wheel_size(buffer_get_float32_auto(packet, &ind));
        display_wheel_size();
        break;
    case SETTINGS_NUMBER_OF_POLES_PAIRS_ID :
        set_number_of_poles_pairs(buffer_get_uint8(packet, &ind));
        display_number_of_poles_pairs();
        break;
    case SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID :
        set_battery_minimum_voltage(buffer_get_float32_auto(packet, &ind));
        display_battery_minimum_voltage();
        break;
    case SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID :
        set_battery_maximum_voltage(buffer_get_float32_auto(packet, &ind));
        display_battery_maximum_voltage();
        break;
    case SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID :
        set_battery_maximum_distance(buffer_get_uint8(packet, &ind));
        display_battery_maximum_distance();
        break;
    case SETTINGS_SPEED_ADJUSTMENT_ID :
        set_speed_adjustment(buffer_get_int8(packet, &ind));
        display_speed_adjustment();
        break;
    case SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID :
        set_display_gps_speed_instead_of_escooter_speed(buffer_get_uint8(packet, &ind));
        display_display_gps_speed_instead_of_escooter_speed();
        break;
    case SETTINGS_AUX_RELAY_NAME_ID :
        set_aux_relay_name(buffer_get_uint8(packet, &ind));
        display_aux_relay_name();
        break;
    case SETTINGS_DEFAULT_MODE_AT_STARTUP_ID :
        set_default_mode_at_startup(buffer_get_uint8(packet, &ind));
        display_default_mode_at_startup();
        break;
    case SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID :
        set_default_eco_mode_at_startup(buffer_get_uint8(packet, &ind));
        display_default_eco_mode_at_startup();
        break;
    case SETTINGS_DEFAULT_ACCELERATION_ID :
        set_default_acceleration(buffer_get_uint8(packet, &ind));
        display_default_acceleration();
        break;
    case SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID :
        set_default_electric_brake_at_startup(buffer_get_uint8(packet, &ind));
        display_default_electric_brake_at_startup();
        break;
    case SETTINGS_PAS_ENABLED_ID :
        set_pas_enabled(buffer_get_uint8(packet, &ind));
        display_pas_enabled();
        break;
    case SETTINGS_ABS_ENABLED_ID :
        set_abs_enabled(buffer_get_uint8(packet, &ind));
        display_abs_enabled();
        break;
    case SETTINGS_DISPLAY_BRIGHTNESS_ID :
        set_display_brightness(buffer_get_uint8(packet, &ind));
        display_display_brightness();
        break;
    case SETTINGS_TEMPERATURE_WARNING_ID :
        set_temperature_warning(buffer_get_uint8(packet, &ind));
        display_temperature_warning();
        break;
    case SETTINGS_HUMIDITY_WARNING_ID :
        set_humidity_warning(buffer_get_uint8(packet, &ind));
        display_humidity_warning();
        break;
    case SETTINGS_SPEED_LIMITER_AT_STARTUP_ID :
        set_speed_limiter_at_startup(buffer_get_uint8(packet, &ind));
        display_speed_limiter_at_startup();
        break;
    case SETTINGS_SPEED_LIMITER_MAX_SPEED_IN_PERCENT_OF_MAX_SPEED_ID :
        set_speed_limiter_max_speed_in_percent_of_max_speed(buffer_get_uint8(packet, &ind));
        display_speed_limiter_max_speed_in_percent_of_max_speed();
        break;
    case SETTINGS_BLUETOOTH_LOCK_MODE_ID :
        set_bluetooth_lock_mode(buffer_get_uint8(packet, &ind));
        display_bluetooth_lock_mode();
        break;
    case SETTINGS_PIN_CODE_USE_6_DIGITS_ONLY_ID :
        set_pin_code_use_6_digits_only(buffer_get_uint32(packet, &ind));
        display_pin_code_use_6_digits_only();
        break;
    case SETTINGS_BEACON_MAC_ADDRESS_ID :
        char beacon_mac_address_part[17];
        memset(beacon_mac_address_part, 0, 17 );
        if (packetNumber == 0) {
            beacon_mac_address = "";
        }
        memcpy(beacon_mac_address_part, &packet[ind], length  - 4);
        beacon_mac_address =  beacon_mac_address + beacon_mac_address_part;
        set_beacon_mac_address( beacon_mac_address);
        display_beacon_mac_address();
        break;
    case SETTINGS_BEACON_RANGE_ID :
        set_beacon_range(buffer_get_int8(packet, &ind));
        display_beacon_range();
        break;
    case SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID :
        set_original_display_speed_adjustment(buffer_get_int8(packet, &ind));
        display_original_display_speed_adjustment();
        break;
    case SETTINGS_ELECTRIC_BRAKE_PROGRESSIVE_MODE_ID :
        set_electric_brake_progressive_mode(buffer_get_uint8(packet, &ind));
        display_electric_brake_progressive_mode();
        break;
    case SETTINGS_ELECTRIC_BRAKE_TYPE_ID :
        set_electric_brake_type(buffer_get_uint8(packet, &ind));
        display_electric_brake_type();
        break;
    case SETTINGS_ELECTRIC_BRAKE_MIN_VALUE_ID :
        set_electric_brake_min_value(buffer_get_uint8(packet, &ind));
        display_electric_brake_min_value();
        break;
    case SETTINGS_ELECTRIC_BRAKE_MAX_VALUE_ID :
        set_electric_brake_max_value(buffer_get_uint8(packet, &ind));
        display_electric_brake_max_value();
        break;
    case SETTINGS_ELECTRIC_BRAKE_TIME_BETWEEN_MODE_SHIFT_ID :
        set_electric_brake_time_between_mode_shift(buffer_get_uint8(packet, &ind));
        display_electric_brake_time_between_mode_shift();
        break;
    case SETTINGS_ELECTRIC_BRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID :
        set_electric_brake_disabled_on_high_battery_voltage(buffer_get_uint8(packet, &ind));
        display_electric_brake_disabled_on_high_battery_voltage();
        break;
    case SETTINGS_ELECTRIC_BRAKE_DISABLED_PERCENT_LIMIT_ID :
        set_electric_brake_disabled_percent_limit(buffer_get_uint8(packet, &ind));
        display_electric_brake_disabled_percent_limit();
        break;
    case SETTINGS_THROTTLE_REGENERATION_ID :
        set_throttle_regeneration(buffer_get_uint8(packet, &ind));
        display_throttle_regeneration();
        break;
    case SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID :
        set_throttle_input_min_voltage_in_millivolts(buffer_get_uint32(packet, &ind));
        display_throttle_input_min_voltage_in_millivolts();
        break;
    case SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID :
        set_throttle_input_max_voltage_in_millivolts(buffer_get_uint32(packet, &ind));
        display_throttle_input_max_voltage_in_millivolts();
        break;
    case SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID :
        set_throttle_output_min_voltage_in_millivolts(buffer_get_uint32(packet, &ind));
        display_throttle_output_min_voltage_in_millivolts();
        break;
    case SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID :
        set_throttle_output_max_voltage_in_millivolts(buffer_get_uint32(packet, &ind));
        display_throttle_output_max_voltage_in_millivolts();
        break;
    case SETTINGS_THROTTLE_OUTPUT_CURVE_ID :
        set_throttle_output_curve(buffer_get_uint8(packet, &ind));
        display_throttle_output_curve();
        break;
    case SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_ID :
        set_button_1_short_press_action(buffer_get_uint8(packet, &ind));
        display_button_1_short_press_action();
        break;
    case SETTINGS_BUTTON_1_LONG_PRESS_ACTION_ID :
        set_button_1_long_press_action(buffer_get_uint8(packet, &ind));
        display_button_1_long_press_action();
        break;
    case SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_ID :
        set_button_2_short_press_action(buffer_get_uint8(packet, &ind));
        display_button_2_short_press_action();
        break;
    case SETTINGS_BUTTON_LONG_PRESS_DURATION_ID :
        set_button_long_press_duration(buffer_get_uint8(packet, &ind));
        display_button_long_press_duration();
        break;
    case SETTINGS_WIFI_NETWORK_NAME_SSID_ID :
        char wifi_network_name_ssid_part[17];
        memset(wifi_network_name_ssid_part, 0, 17 );
        if (packetNumber == 0) {
            wifi_network_name_ssid = "";
        }
        memcpy(wifi_network_name_ssid_part, &packet[ind], length  - 4);
        wifi_network_name_ssid =  wifi_network_name_ssid + wifi_network_name_ssid_part;
        set_wifi_network_name_ssid( wifi_network_name_ssid);
        display_wifi_network_name_ssid();
        break;
    case SETTINGS_WIFI_PASSWORD_ID :
        char wifi_password_part[17];
        memset(wifi_password_part, 0, 17 );
        if (packetNumber == 0) {
            wifi_password = "";
        }
        memcpy(wifi_password_part, &packet[ind], length  - 4);
        wifi_password =  wifi_password + wifi_password_part;
        set_wifi_password( wifi_password);
        display_wifi_password();
        break;
    default:
        Serial.println("unpack_setting_packet : ID error");
        break;
    }
}

void Settings2::pack_setting_packet(uint16_t setting_id, uint8_t* packet,uint8_t* length) {

}
                

/*-------------------------------------------------------*/

void Settings2::set_wheel_size(float value) {

    wheel_size = value;
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_WHEEL_SIZE_ID_STR, wheel_size);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_NUMBER_OF_POLES_PAIRS_ID_STR, number_of_poles_pairs);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID_STR, battery_minimum_voltage);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID_STR, battery_maximum_voltage);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID_STR, battery_maximum_distance);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SPEED_ADJUSTMENT_ID_STR, speed_adjustment);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID_STR, display_gps_speed_instead_of_escooter_speed);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_AUX_RELAY_NAME_ID_STR, aux_relay_name);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_MODE_AT_STARTUP_ID_STR, default_mode_at_startup);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID_STR, default_eco_mode_at_startup);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ACCELERATION_ID_STR, default_acceleration);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID_STR, default_electric_brake_at_startup);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_PAS_ENABLED_ID_STR, pas_enabled);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ABS_ENABLED_ID_STR, abs_enabled);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISPLAY_BRIGHTNESS_ID_STR, display_brightness);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_TEMPERATURE_WARNING_ID_STR, temperature_warning);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_HUMIDITY_WARNING_ID_STR, humidity_warning);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SPEED_LIMITER_AT_STARTUP_ID_STR, speed_limiter_at_startup);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SPEED_LIMITER_MAX_SPEED_IN_PERCENT_OF_MAX_SPEED_ID_STR, speed_limiter_max_speed_in_percent_of_max_speed);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BLUETOOTH_LOCK_MODE_ID_STR, bluetooth_lock_mode);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_PIN_CODE_USE_6_DIGITS_ONLY_ID_STR, pin_code_use_6_digits_only);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_BEACON_MAC_ADDRESS_ID_STR, beacon_mac_address);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BEACON_RANGE_ID_STR, beacon_range);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID_STR, original_display_speed_adjustment);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ELECTRIC_BRAKE_PROGRESSIVE_MODE_ID_STR, electric_brake_progressive_mode);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ELECTRIC_BRAKE_TYPE_ID_STR, electric_brake_type);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ELECTRIC_BRAKE_MIN_VALUE_ID_STR, electric_brake_min_value);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ELECTRIC_BRAKE_MAX_VALUE_ID_STR, electric_brake_max_value);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ELECTRIC_BRAKE_TIME_BETWEEN_MODE_SHIFT_ID_STR, electric_brake_time_between_mode_shift);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ELECTRIC_BRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID_STR, electric_brake_disabled_on_high_battery_voltage);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ELECTRIC_BRAKE_DISABLED_PERCENT_LIMIT_ID_STR, electric_brake_disabled_percent_limit);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_REGENERATION_ID_STR, throttle_regeneration);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, throttle_input_min_voltage_in_millivolts);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, throttle_input_max_voltage_in_millivolts);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, throttle_output_min_voltage_in_millivolts);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, throttle_output_max_voltage_in_millivolts);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_CURVE_ID_STR, throttle_output_curve);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_ID_STR, button_1_short_press_action);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_1_LONG_PRESS_ACTION_ID_STR, button_1_long_press_action);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_ID_STR, button_2_short_press_action);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_LONG_PRESS_DURATION_ID_STR, button_long_press_duration);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_WIFI_NETWORK_NAME_SSID_ID_STR, wifi_network_name_ssid);
    prefs.end();
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
    
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_WIFI_PASSWORD_ID_STR, wifi_password);
    prefs.end();
}

String Settings2::get_wifi_password() {
    return wifi_password ;
}

void Settings2::display_wifi_password() {
    Serial.println("  wifi_password = " + (String) wifi_password);
}


env = jinja2.Environment()
env.filters['regex_replace'] = regex_replace

tmpl_cpp = env.from_string(template_cpp)
tmpl_h = env.from_string(template_h)
