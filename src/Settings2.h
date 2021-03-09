
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

////// Manage settings exchanged in BLE and stored in EEPOM

#include "Arduino.h"
#include <Preferences.h>

#ifndef _Settings2_h
#define _Settings2_h

class Settings2
{
private : 
    Preferences prefs;

public:

    Settings2();

    void restore();

    void unpack_setting_packet(uint8_t* packet, uint8_t length);
    void pack_setting_packet(uint16_t setting_id, uint8_t* packet,uint8_t* length);


    
            
    /*-------------------------------------------------------*/

    #define SETTINGS_WHEEL_SIZE_ID 1
    #define SETTINGS_WHEEL_SIZE_ID_STR "1"
    #define SETTINGS_WHEEL_SIZE_NAME "Wheel size"

    float wheel_size;

    void set_wheel_size (float value);
    float get_wheel_size();
    void display_wheel_size();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_NUMBER_OF_POLES_PAIRS_ID 2
    #define SETTINGS_NUMBER_OF_POLES_PAIRS_ID_STR "2"
    #define SETTINGS_NUMBER_OF_POLES_PAIRS_NAME "Number of poles pairs"

    uint8_t number_of_poles_pairs;

    void set_number_of_poles_pairs (uint8_t value);
    uint8_t get_number_of_poles_pairs();
    void display_number_of_poles_pairs();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID 3
    #define SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID_STR "3"
    #define SETTINGS_BATTERY_MINIMUM_VOLTAGE_NAME "Battery minimum voltage"

    float battery_minimum_voltage;

    void set_battery_minimum_voltage (float value);
    float get_battery_minimum_voltage();
    void display_battery_minimum_voltage();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID 4
    #define SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID_STR "4"
    #define SETTINGS_BATTERY_MAXIMUM_VOLTAGE_NAME "Battery maximum voltage"

    float battery_maximum_voltage;

    void set_battery_maximum_voltage (float value);
    float get_battery_maximum_voltage();
    void display_battery_maximum_voltage();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID 5
    #define SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID_STR "5"
    #define SETTINGS_BATTERY_MAXIMUM_DISTANCE_NAME "Battery maximum distance"

    uint8_t battery_maximum_distance;

    void set_battery_maximum_distance (uint8_t value);
    uint8_t get_battery_maximum_distance();
    void display_battery_maximum_distance();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_SPEED_ADJUSTMENT_ID 13
    #define SETTINGS_SPEED_ADJUSTMENT_ID_STR "13"
    #define SETTINGS_SPEED_ADJUSTMENT_NAME "Speed adjustment"

    int8_t speed_adjustment;

    void set_speed_adjustment (int8_t value);
    int8_t get_speed_adjustment();
    void display_speed_adjustment();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID 14
    #define SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID_STR "14"
    #define SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_NAME "Display GPS speed instead of escooter speed"

    uint8_t display_gps_speed_instead_of_escooter_speed;

    void set_display_gps_speed_instead_of_escooter_speed (uint8_t value);
    uint8_t get_display_gps_speed_instead_of_escooter_speed();
    void display_display_gps_speed_instead_of_escooter_speed();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_AUX_RELAY_NAME_ID 15
    #define SETTINGS_AUX_RELAY_NAME_ID_STR "15"
    #define SETTINGS_AUX_RELAY_NAME_NAME "Aux_relay_name"

    uint8_t aux_relay_name;
    typedef enum LIST_Aux_relay_name {
        LIST_Aux_relay_name_Aux,
        LIST_Aux_relay_name_Dual,
        LIST_Aux_relay_name_Lights,
        LIST_Aux_relay_name_Horn,
        LIST_Aux_relay_name_Relay, 
    } tAux_relay_name;

    void set_aux_relay_name (uint8_t value);
    uint8_t get_aux_relay_name();
    void display_aux_relay_name();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_DEFAULT_MODE_AT_STARTUP_ID 16
    #define SETTINGS_DEFAULT_MODE_AT_STARTUP_ID_STR "16"
    #define SETTINGS_DEFAULT_MODE_AT_STARTUP_NAME "Default mode at startup"

    uint8_t default_mode_at_startup;

    void set_default_mode_at_startup (uint8_t value);
    uint8_t get_default_mode_at_startup();
    void display_default_mode_at_startup();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID 17
    #define SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID_STR "17"
    #define SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_NAME "Default eco mode at startup"

    uint8_t default_eco_mode_at_startup;
    typedef enum LIST_Default_eco_mode_at_startup {
        LIST_Default_eco_mode_at_startup_Max,
        LIST_Default_eco_mode_at_startup_Medium,
        LIST_Default_eco_mode_at_startup_None, 
    } tDefault_eco_mode_at_startup;

    void set_default_eco_mode_at_startup (uint8_t value);
    uint8_t get_default_eco_mode_at_startup();
    void display_default_eco_mode_at_startup();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_DEFAULT_ACCELERATION_ID 18
    #define SETTINGS_DEFAULT_ACCELERATION_ID_STR "18"
    #define SETTINGS_DEFAULT_ACCELERATION_NAME "Default acceleration"

    uint8_t default_acceleration;
    typedef enum LIST_Default_acceleration {
        LIST_Default_acceleration_Very_slow,
        LIST_Default_acceleration_Slow,
        LIST_Default_acceleration_Medium,
        LIST_Default_acceleration_Fast,
        LIST_Default_acceleration_Very_fast,
        LIST_Default_acceleration_Maximum, 
    } tDefault_acceleration;

    void set_default_acceleration (uint8_t value);
    uint8_t get_default_acceleration();
    void display_default_acceleration();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID 19
    #define SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID_STR "19"
    #define SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_NAME "Default electric brake at startup"

    uint8_t default_electric_brake_at_startup;

    void set_default_electric_brake_at_startup (uint8_t value);
    uint8_t get_default_electric_brake_at_startup();
    void display_default_electric_brake_at_startup();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_PAS_ENABLED_ID 20
    #define SETTINGS_PAS_ENABLED_ID_STR "20"
    #define SETTINGS_PAS_ENABLED_NAME "PAS enabled"

    uint8_t pas_enabled;

    void set_pas_enabled (uint8_t value);
    uint8_t get_pas_enabled();
    void display_pas_enabled();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ABS_ENABLED_ID 21
    #define SETTINGS_ABS_ENABLED_ID_STR "21"
    #define SETTINGS_ABS_ENABLED_NAME "Abs enabled"

    uint8_t abs_enabled;

    void set_abs_enabled (uint8_t value);
    uint8_t get_abs_enabled();
    void display_abs_enabled();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_DISPLAY_BRIGHTNESS_ID 22
    #define SETTINGS_DISPLAY_BRIGHTNESS_ID_STR "22"
    #define SETTINGS_DISPLAY_BRIGHTNESS_NAME "Display brightness"

    uint8_t display_brightness;

    void set_display_brightness (uint8_t value);
    uint8_t get_display_brightness();
    void display_display_brightness();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_TEMPERATURE_WARNING_ID 23
    #define SETTINGS_TEMPERATURE_WARNING_ID_STR "23"
    #define SETTINGS_TEMPERATURE_WARNING_NAME "Temperature warning"

    uint8_t temperature_warning;

    void set_temperature_warning (uint8_t value);
    uint8_t get_temperature_warning();
    void display_temperature_warning();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_HUMIDITY_WARNING_ID 24
    #define SETTINGS_HUMIDITY_WARNING_ID_STR "24"
    #define SETTINGS_HUMIDITY_WARNING_NAME "Humidity warning"

    uint8_t humidity_warning;

    void set_humidity_warning (uint8_t value);
    uint8_t get_humidity_warning();
    void display_humidity_warning();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_SPEED_LIMITER_AT_STARTUP_ID 6
    #define SETTINGS_SPEED_LIMITER_AT_STARTUP_ID_STR "6"
    #define SETTINGS_SPEED_LIMITER_AT_STARTUP_NAME "Speed limiter at startup"

    uint8_t speed_limiter_at_startup;

    void set_speed_limiter_at_startup (uint8_t value);
    uint8_t get_speed_limiter_at_startup();
    void display_speed_limiter_at_startup();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_SPEED_LIMITER_MAX_SPEED_IN_PERCENT_OF_MAX_SPEED_ID 7
    #define SETTINGS_SPEED_LIMITER_MAX_SPEED_IN_PERCENT_OF_MAX_SPEED_ID_STR "7"
    #define SETTINGS_SPEED_LIMITER_MAX_SPEED_IN_PERCENT_OF_MAX_SPEED_NAME "Speed limiter max speed (in percent of max speed)"

    uint8_t speed_limiter_max_speed_in_percent_of_max_speed;

    void set_speed_limiter_max_speed_in_percent_of_max_speed (uint8_t value);
    uint8_t get_speed_limiter_max_speed_in_percent_of_max_speed();
    void display_speed_limiter_max_speed_in_percent_of_max_speed();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BLUETOOTH_LOCK_MODE_ID 8
    #define SETTINGS_BLUETOOTH_LOCK_MODE_ID_STR "8"
    #define SETTINGS_BLUETOOTH_LOCK_MODE_NAME "Bluetooth lock mode"

    uint8_t bluetooth_lock_mode;
    typedef enum LIST_Bluetooth_lock_mode {
        LIST_Bluetooth_lock_mode_None,
        LIST_Bluetooth_lock_mode_Smartphone_connected,
        LIST_Bluetooth_lock_mode_Smartphone_connected_or_beacon_visible,
        LIST_Bluetooth_lock_mode_Beacon_visible, 
    } tBluetooth_lock_mode;

    void set_bluetooth_lock_mode (uint8_t value);
    uint8_t get_bluetooth_lock_mode();
    void display_bluetooth_lock_mode();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_PIN_CODE_USE_6_DIGITS_ONLY_ID 9
    #define SETTINGS_PIN_CODE_USE_6_DIGITS_ONLY_ID_STR "9"
    #define SETTINGS_PIN_CODE_USE_6_DIGITS_ONLY_NAME "PIN code (use 6 digits only)"

    uint32_t pin_code_use_6_digits_only;

    void set_pin_code_use_6_digits_only (uint32_t value);
    uint32_t get_pin_code_use_6_digits_only();
    void display_pin_code_use_6_digits_only();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BEACON_MAC_ADDRESS_ID 10
    #define SETTINGS_BEACON_MAC_ADDRESS_ID_STR "10"
    #define SETTINGS_BEACON_MAC_ADDRESS_NAME "Beacon Mac Address"

    String beacon_mac_address;

    void set_beacon_mac_address (String value);
    String get_beacon_mac_address();
    void display_beacon_mac_address();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BEACON_RANGE_ID 11
    #define SETTINGS_BEACON_RANGE_ID_STR "11"
    #define SETTINGS_BEACON_RANGE_NAME "Beacon range"

    int8_t beacon_range;

    void set_beacon_range (int8_t value);
    int8_t get_beacon_range();
    void display_beacon_range();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID 12
    #define SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID_STR "12"
    #define SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_NAME "Original display speed adjustment"

    int8_t original_display_speed_adjustment;

    void set_original_display_speed_adjustment (int8_t value);
    int8_t get_original_display_speed_adjustment();
    void display_original_display_speed_adjustment();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ELECTRIC_BRAKE_PROGRESSIVE_MODE_ID 25
    #define SETTINGS_ELECTRIC_BRAKE_PROGRESSIVE_MODE_ID_STR "25"
    #define SETTINGS_ELECTRIC_BRAKE_PROGRESSIVE_MODE_NAME "Electric brake progressive mode"

    uint8_t electric_brake_progressive_mode;

    void set_electric_brake_progressive_mode (uint8_t value);
    uint8_t get_electric_brake_progressive_mode();
    void display_electric_brake_progressive_mode();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ELECTRIC_BRAKE_TYPE_ID 26
    #define SETTINGS_ELECTRIC_BRAKE_TYPE_ID_STR "26"
    #define SETTINGS_ELECTRIC_BRAKE_TYPE_NAME "Electric_brake_type"

    uint8_t electric_brake_type;
    typedef enum LIST_Electric_brake_type {
        LIST_Electric_brake_type_From_controller,
        LIST_Electric_brake_type_From_smart__digital_brake_lever,
        LIST_Electric_brake_type_From_smart__analog_brake_lever,
        LIST_Electric_brake_type_From_smart__digital_brake_lever__throttle_experimental, 
    } tElectric_brake_type;

    void set_electric_brake_type (uint8_t value);
    uint8_t get_electric_brake_type();
    void display_electric_brake_type();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ELECTRIC_BRAKE_MIN_VALUE_ID 27
    #define SETTINGS_ELECTRIC_BRAKE_MIN_VALUE_ID_STR "27"
    #define SETTINGS_ELECTRIC_BRAKE_MIN_VALUE_NAME "Electric brake min value"

    uint8_t electric_brake_min_value;

    void set_electric_brake_min_value (uint8_t value);
    uint8_t get_electric_brake_min_value();
    void display_electric_brake_min_value();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ELECTRIC_BRAKE_MAX_VALUE_ID 28
    #define SETTINGS_ELECTRIC_BRAKE_MAX_VALUE_ID_STR "28"
    #define SETTINGS_ELECTRIC_BRAKE_MAX_VALUE_NAME "Electric brake max value"

    uint8_t electric_brake_max_value;

    void set_electric_brake_max_value (uint8_t value);
    uint8_t get_electric_brake_max_value();
    void display_electric_brake_max_value();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ELECTRIC_BRAKE_TIME_BETWEEN_MODE_SHIFT_ID 29
    #define SETTINGS_ELECTRIC_BRAKE_TIME_BETWEEN_MODE_SHIFT_ID_STR "29"
    #define SETTINGS_ELECTRIC_BRAKE_TIME_BETWEEN_MODE_SHIFT_NAME "Electric brake time between mode shift"

    uint8_t electric_brake_time_between_mode_shift;

    void set_electric_brake_time_between_mode_shift (uint8_t value);
    uint8_t get_electric_brake_time_between_mode_shift();
    void display_electric_brake_time_between_mode_shift();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ELECTRIC_BRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID 30
    #define SETTINGS_ELECTRIC_BRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID_STR "30"
    #define SETTINGS_ELECTRIC_BRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_NAME "Electric brake disabled on high battery voltage"

    uint8_t electric_brake_disabled_on_high_battery_voltage;

    void set_electric_brake_disabled_on_high_battery_voltage (uint8_t value);
    uint8_t get_electric_brake_disabled_on_high_battery_voltage();
    void display_electric_brake_disabled_on_high_battery_voltage();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_ELECTRIC_BRAKE_DISABLED_PERCENT_LIMIT_ID 31
    #define SETTINGS_ELECTRIC_BRAKE_DISABLED_PERCENT_LIMIT_ID_STR "31"
    #define SETTINGS_ELECTRIC_BRAKE_DISABLED_PERCENT_LIMIT_NAME "Electric brake disabled percent limit"

    uint8_t electric_brake_disabled_percent_limit;

    void set_electric_brake_disabled_percent_limit (uint8_t value);
    uint8_t get_electric_brake_disabled_percent_limit();
    void display_electric_brake_disabled_percent_limit();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_THROTTLE_REGENERATION_ID 32
    #define SETTINGS_THROTTLE_REGENERATION_ID_STR "32"
    #define SETTINGS_THROTTLE_REGENERATION_NAME "Throttle regeneration"

    uint8_t throttle_regeneration;

    void set_throttle_regeneration (uint8_t value);
    uint8_t get_throttle_regeneration();
    void display_throttle_regeneration();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID 33
    #define SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR "33"
    #define SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_NAME "Throttle input min voltage (in millivolts)"

    uint32_t throttle_input_min_voltage_in_millivolts;

    void set_throttle_input_min_voltage_in_millivolts (uint32_t value);
    uint32_t get_throttle_input_min_voltage_in_millivolts();
    void display_throttle_input_min_voltage_in_millivolts();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID 34
    #define SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR "34"
    #define SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_NAME "Throttle input max voltage (in millivolts)"

    uint32_t throttle_input_max_voltage_in_millivolts;

    void set_throttle_input_max_voltage_in_millivolts (uint32_t value);
    uint32_t get_throttle_input_max_voltage_in_millivolts();
    void display_throttle_input_max_voltage_in_millivolts();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID 35
    #define SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR "35"
    #define SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_NAME "Throttle output min voltage (in millivolts)"

    uint32_t throttle_output_min_voltage_in_millivolts;

    void set_throttle_output_min_voltage_in_millivolts (uint32_t value);
    uint32_t get_throttle_output_min_voltage_in_millivolts();
    void display_throttle_output_min_voltage_in_millivolts();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID 36
    #define SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR "36"
    #define SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_NAME "Throttle output max voltage (in millivolts)"

    uint32_t throttle_output_max_voltage_in_millivolts;

    void set_throttle_output_max_voltage_in_millivolts (uint32_t value);
    uint32_t get_throttle_output_max_voltage_in_millivolts();
    void display_throttle_output_max_voltage_in_millivolts();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_THROTTLE_OUTPUT_CURVE_ID 37
    #define SETTINGS_THROTTLE_OUTPUT_CURVE_ID_STR "37"
    #define SETTINGS_THROTTLE_OUTPUT_CURVE_NAME "Throttle output curve"

    uint8_t throttle_output_curve;
    typedef enum LIST_Throttle_output_curve {
        LIST_Throttle_output_curve_Linear,
        LIST_Throttle_output_curve_Exponential_1,
        LIST_Throttle_output_curve_Exponential_2,
        LIST_Throttle_output_curve_Exponential_3,
        LIST_Throttle_output_curve_Exponential_4, 
    } tThrottle_output_curve;

    void set_throttle_output_curve (uint8_t value);
    uint8_t get_throttle_output_curve();
    void display_throttle_output_curve();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_ID 38
    #define SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_ID_STR "38"
    #define SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_NAME "Button 1 short press action"

    uint8_t button_1_short_press_action;
    typedef enum LIST_Button_1_short_press_action {
        LIST_Button_1_short_press_action_None,
        LIST_Button_1_short_press_action_Startup_speed_limitation_onoff,
        LIST_Button_1_short_press_action_Aux_onoff,
        LIST_Button_1_short_press_action_Mode_switch_123,
        LIST_Button_1_short_press_action_Mode_switch_23,
        LIST_Button_1_short_press_action_Eco_switch_nonemedmax,
        LIST_Button_1_short_press_action_Eco_switch_nonemed, 
    } tButton_1_short_press_action;

    void set_button_1_short_press_action (uint8_t value);
    uint8_t get_button_1_short_press_action();
    void display_button_1_short_press_action();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BUTTON_1_LONG_PRESS_ACTION_ID 39
    #define SETTINGS_BUTTON_1_LONG_PRESS_ACTION_ID_STR "39"
    #define SETTINGS_BUTTON_1_LONG_PRESS_ACTION_NAME "Button 1 long press action"

    uint8_t button_1_long_press_action;
    typedef enum LIST_Button_1_long_press_action {
        LIST_Button_1_long_press_action_None,
        LIST_Button_1_long_press_action_Startup_speed_limitation_onoff,
        LIST_Button_1_long_press_action_Aux_onoff,
        LIST_Button_1_long_press_action_Mode_switch_123,
        LIST_Button_1_long_press_action_Mode_switch_23,
        LIST_Button_1_long_press_action_Eco_switch_nonemedmax,
        LIST_Button_1_long_press_action_Eco_switch_nonemed,
        LIST_Button_1_long_press_action_Antitheft_manual_lock_on,
        LIST_Button_1_long_press_action_Nitro_boost_continuous, 
    } tButton_1_long_press_action;

    void set_button_1_long_press_action (uint8_t value);
    uint8_t get_button_1_long_press_action();
    void display_button_1_long_press_action();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_ID 40
    #define SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_ID_STR "40"
    #define SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_NAME "Button 2 short press action"

    uint8_t button_2_short_press_action;
    typedef enum LIST_Button_2_short_press_action {
        LIST_Button_2_short_press_action_None,
        LIST_Button_2_short_press_action_Startup_speed_limitation_onoff,
        LIST_Button_2_short_press_action_Aux_onoff,
        LIST_Button_2_short_press_action_Mode_switch_123,
        LIST_Button_2_short_press_action_Mode_switch_23,
        LIST_Button_2_short_press_action_Eco_switch_nonemedmax,
        LIST_Button_2_short_press_action_Eco_switch_nonemed, 
    } tButton_2_short_press_action;

    void set_button_2_short_press_action (uint8_t value);
    uint8_t get_button_2_short_press_action();
    void display_button_2_short_press_action();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_BUTTON_LONG_PRESS_DURATION_ID 41
    #define SETTINGS_BUTTON_LONG_PRESS_DURATION_ID_STR "41"
    #define SETTINGS_BUTTON_LONG_PRESS_DURATION_NAME "Button long press duration"

    uint8_t button_long_press_duration;

    void set_button_long_press_duration (uint8_t value);
    uint8_t get_button_long_press_duration();
    void display_button_long_press_duration();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_WIFI_NETWORK_NAME_SSID_ID 42
    #define SETTINGS_WIFI_NETWORK_NAME_SSID_ID_STR "42"
    #define SETTINGS_WIFI_NETWORK_NAME_SSID_NAME "Wifi network name (SSID)"

    String wifi_network_name_ssid;

    void set_wifi_network_name_ssid (String value);
    String get_wifi_network_name_ssid();
    void display_wifi_network_name_ssid();
            
    /*-------------------------------------------------------*/

    #define SETTINGS_WIFI_PASSWORD_ID 43
    #define SETTINGS_WIFI_PASSWORD_ID_STR "43"
    #define SETTINGS_WIFI_PASSWORD_NAME "Wifi password"

    String wifi_password;

    void set_wifi_password (String value);
    String get_wifi_password();
    void display_wifi_password();

};

#endif