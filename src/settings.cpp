
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

////// Manage settings exchanged in BLE and stored in EEPOM

#include "Arduino.h"
#include "Settings.h"
#include "tools/buffer.h"

#define SETTINGS_STORAGE "SETTINGS"

Settings::Settings()
{
}

void Settings::restore() {

    Serial.println(" > restore settings");
    prefs.begin(SETTINGS_STORAGE, false);
    Wheel_size = prefs.getFloat(SETTINGS_WHEEL_SIZE_STORAGE_KEY, 10.0);
    Serial.println("  >> Wheel_size = " + (String)Wheel_size);
    Number_of_poles_pairs = prefs.getInt(SETTINGS_NUMBER_OF_POLES_PAIRS_STORAGE_KEY, 15);
    Serial.println("  >> Number_of_poles_pairs = " + (String)Number_of_poles_pairs);
    Battery_minimum_voltage = prefs.getFloat(SETTINGS_BATTERY_MINIMUM_VOLTAGE_STORAGE_KEY, 42.0);
    Serial.println("  >> Battery_minimum_voltage = " + (String)Battery_minimum_voltage);
    Battery_maximum_voltage = prefs.getFloat(SETTINGS_BATTERY_MAXIMUM_VOLTAGE_STORAGE_KEY, 58.8);
    Serial.println("  >> Battery_maximum_voltage = " + (String)Battery_maximum_voltage);
    Battery_maximum_distance = prefs.getInt(SETTINGS_BATTERY_MAXIMUM_DISTANCE_STORAGE_KEY, 40);
    Serial.println("  >> Battery_maximum_distance = " + (String)Battery_maximum_distance);
    Speed_limiter_at_startup = prefs.getInt(SETTINGS_SPEED_LIMITER_AT_STARTUP_STORAGE_KEY, 0);
    Serial.println("  >> Speed_limiter_at_startup = " + (String)Speed_limiter_at_startup);
    Speed_limiter_max_speed = prefs.getInt(SETTINGS_SPEED_LIMITER_MAX_SPEED_STORAGE_KEY, 37);
    Serial.println("  >> Speed_limiter_max_speed = " + (String)Speed_limiter_max_speed);
    Bluetooth_lock_mode = prefs.getInt(SETTINGS_BLUETOOTH_LOCK_MODE_STORAGE_KEY, 0);
    Serial.println("  >> Bluetooth_lock_mode = " + (String)Bluetooth_lock_mode);
    Ble_pin_code = prefs.getInt(SETTINGS_BLE_PIN_CODE_STORAGE_KEY, 147258);
    Serial.println("  >> Ble_pin_code = " + (String)Ble_pin_code);
    Ble_beacon_mac_address = prefs.getString(SETTINGS_BLE_BEACON_MAC_ADDRESS_STORAGE_KEY, "aa:bb:cc:dd:ee:ff");
    Serial.println("  >> Ble_beacon_mac_address = " + (String)Ble_beacon_mac_address);
    Ble_beacon_range = prefs.getInt(SETTINGS_BLE_BEACON_RANGE_STORAGE_KEY, -80);
    Serial.println("  >> Ble_beacon_range = " + (String)Ble_beacon_range);
    Original_display_speed_adjustment = prefs.getInt(SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_STORAGE_KEY, 0);
    Serial.println("  >> Original_display_speed_adjustment = " + (String)Original_display_speed_adjustment);
    Temperature_warning = prefs.getInt(SETTINGS_TEMPERATURE_WARNING_STORAGE_KEY, 70);
    Serial.println("  >> Temperature_warning = " + (String)Temperature_warning);
    Humidity_warning = prefs.getInt(SETTINGS_HUMIDITY_WARNING_STORAGE_KEY, 80);
    Serial.println("  >> Humidity_warning = " + (String)Humidity_warning);
    Smartdisplay_speed_adjustment = prefs.getInt(SETTINGS_SMARTDISPLAY_SPEED_ADJUSTMENT_STORAGE_KEY, 0);
    Serial.println("  >> Smartdisplay_speed_adjustment = " + (String)Smartdisplay_speed_adjustment);
    Aux_relay_name = prefs.getInt(SETTINGS_AUX_RELAY_NAME_STORAGE_KEY, 0);
    Serial.println("  >> Aux_relay_name = " + (String)Aux_relay_name);
    Display_gps_speed_instead_of_escooter_speed = prefs.getInt(SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_STORAGE_KEY, 0);
    Serial.println("  >> Display_gps_speed_instead_of_escooter_speed = " + (String)Display_gps_speed_instead_of_escooter_speed);
    Default_mode_at_startup = prefs.getInt(SETTINGS_DEFAULT_MODE_AT_STARTUP_STORAGE_KEY, 3);
    Serial.println("  >> Default_mode_at_startup = " + (String)Default_mode_at_startup);
    Default_eco_mode_at_startup = prefs.getInt(SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_STORAGE_KEY, 2);
    Serial.println("  >> Default_eco_mode_at_startup = " + (String)Default_eco_mode_at_startup);
    Default_acceleration = prefs.getInt(SETTINGS_DEFAULT_ACCELERATION_STORAGE_KEY, 0);
    Serial.println("  >> Default_acceleration = " + (String)Default_acceleration);
    Default_electric_brake_at_startup = prefs.getInt(SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_STORAGE_KEY, 3);
    Serial.println("  >> Default_electric_brake_at_startup = " + (String)Default_electric_brake_at_startup);
    Pas_enabled = prefs.getInt(SETTINGS_PAS_ENABLED_STORAGE_KEY, 1);
    Serial.println("  >> Pas_enabled = " + (String)Pas_enabled);
    Abs_enabled = prefs.getInt(SETTINGS_ABS_ENABLED_STORAGE_KEY, 0);
    Serial.println("  >> Abs_enabled = " + (String)Abs_enabled);
    Display_brightness = prefs.getInt(SETTINGS_DISPLAY_BRIGHTNESS_STORAGE_KEY, 100);
    Serial.println("  >> Display_brightness = " + (String)Display_brightness);
    Display_splash_screen = prefs.getInt(SETTINGS_DISPLAY_SPLASH_SCREEN_STORAGE_KEY, 1);
    Serial.println("  >> Display_splash_screen = " + (String)Display_splash_screen);
    Rotate_screen = prefs.getInt(SETTINGS_ROTATE_SCREEN_STORAGE_KEY, 1);
    Serial.println("  >> Rotate_screen = " + (String)Rotate_screen);
    Ebrake_progressive_mode = prefs.getInt(SETTINGS_EBRAKE_PROGRESSIVE_MODE_STORAGE_KEY, 0);
    Serial.println("  >> Ebrake_progressive_mode = " + (String)Ebrake_progressive_mode);
    Ebrake_smart_brake_type = prefs.getInt(SETTINGS_EBRAKE_SMART_BRAKE_TYPE_STORAGE_KEY, 0);
    Serial.println("  >> Ebrake_smart_brake_type = " + (String)Ebrake_smart_brake_type);
    Ebrake_min_power_value = prefs.getInt(SETTINGS_EBRAKE_MIN_POWER_VALUE_STORAGE_KEY, 1);
    Serial.println("  >> Ebrake_min_power_value = " + (String)Ebrake_min_power_value);
    Ebrake_max_power_value = prefs.getInt(SETTINGS_EBRAKE_MAX_POWER_VALUE_STORAGE_KEY, 5);
    Serial.println("  >> Ebrake_max_power_value = " + (String)Ebrake_max_power_value);
    Ebrake_time_between_mode_shift = prefs.getInt(SETTINGS_EBRAKE_TIME_BETWEEN_MODE_SHIFT_STORAGE_KEY, 500);
    Serial.println("  >> Ebrake_time_between_mode_shift = " + (String)Ebrake_time_between_mode_shift);
    Ebrake_disabled_on_high_battery_voltage = prefs.getInt(SETTINGS_EBRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_STORAGE_KEY, 0);
    Serial.println("  >> Ebrake_disabled_on_high_battery_voltage = " + (String)Ebrake_disabled_on_high_battery_voltage);
    Ebrake_disabled_percent_limit = prefs.getInt(SETTINGS_EBRAKE_DISABLED_PERCENT_LIMIT_STORAGE_KEY, 100);
    Serial.println("  >> Ebrake_disabled_percent_limit = " + (String)Ebrake_disabled_percent_limit);
    Throttle_regeneration = prefs.getInt(SETTINGS_THROTTLE_REGENERATION_STORAGE_KEY, 0);
    Serial.println("  >> Throttle_regeneration = " + (String)Throttle_regeneration);
    Throttle_input_min_voltage = prefs.getInt(SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_STORAGE_KEY, 650);
    Serial.println("  >> Throttle_input_min_voltage = " + (String)Throttle_input_min_voltage);
    Throttle_input_max_voltage = prefs.getInt(SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_STORAGE_KEY, 3950);
    Serial.println("  >> Throttle_input_max_voltage = " + (String)Throttle_input_max_voltage);
    Throttle_output_min_voltage = prefs.getInt(SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_STORAGE_KEY, 800);
    Serial.println("  >> Throttle_output_min_voltage = " + (String)Throttle_output_min_voltage);
    Throttle_output_max_voltage = prefs.getInt(SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_STORAGE_KEY, 3350);
    Serial.println("  >> Throttle_output_max_voltage = " + (String)Throttle_output_max_voltage);
    Throttle_output_curve = prefs.getInt(SETTINGS_THROTTLE_OUTPUT_CURVE_STORAGE_KEY, 2);
    Serial.println("  >> Throttle_output_curve = " + (String)Throttle_output_curve);
    Throttle_output_curve_custom = prefs.getString(SETTINGS_THROTTLE_OUTPUT_CURVE_CUSTOM_STORAGE_KEY, "20,40,60,80");
    Serial.println("  >> Throttle_output_curve_custom = " + (String)Throttle_output_curve_custom);
    Button_1_short_press_action = prefs.getInt(SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_STORAGE_KEY, 0);
    Serial.println("  >> Button_1_short_press_action = " + (String)Button_1_short_press_action);
    Button_1_long_press_action = prefs.getInt(SETTINGS_BUTTON_1_LONG_PRESS_ACTION_STORAGE_KEY, 0);
    Serial.println("  >> Button_1_long_press_action = " + (String)Button_1_long_press_action);
    Button_2_short_press_action = prefs.getInt(SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_STORAGE_KEY, 0);
    Serial.println("  >> Button_2_short_press_action = " + (String)Button_2_short_press_action);
    Button_long_press_duration = prefs.getInt(SETTINGS_BUTTON_LONG_PRESS_DURATION_STORAGE_KEY, 3);
    Serial.println("  >> Button_long_press_duration = " + (String)Button_long_press_duration);
    Wifi_ssid = prefs.getString(SETTINGS_WIFI_SSID_STORAGE_KEY, "");
    Serial.println("  >> Wifi_ssid = " + (String)Wifi_ssid);
    Wifi_password = prefs.getString(SETTINGS_WIFI_PASSWORD_STORAGE_KEY, "");
    Serial.println("  >> Wifi_password = " + (String)Wifi_password);
    prefs.end();
    Serial.println(" < restore settings");
}

void Settings::save() {

    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_WHEEL_SIZE_STORAGE_KEY, Wheel_size);
    prefs.putInt(SETTINGS_NUMBER_OF_POLES_PAIRS_STORAGE_KEY, Number_of_poles_pairs);
    prefs.putFloat(SETTINGS_BATTERY_MINIMUM_VOLTAGE_STORAGE_KEY, Battery_minimum_voltage);
    prefs.putFloat(SETTINGS_BATTERY_MAXIMUM_VOLTAGE_STORAGE_KEY, Battery_maximum_voltage);
    prefs.putInt(SETTINGS_BATTERY_MAXIMUM_DISTANCE_STORAGE_KEY, Battery_maximum_distance);
    prefs.putInt(SETTINGS_SPEED_LIMITER_AT_STARTUP_STORAGE_KEY, Speed_limiter_at_startup);
    prefs.putInt(SETTINGS_SPEED_LIMITER_MAX_SPEED_STORAGE_KEY, Speed_limiter_max_speed);
    prefs.putInt(SETTINGS_BLUETOOTH_LOCK_MODE_STORAGE_KEY, Bluetooth_lock_mode);
    prefs.putInt(SETTINGS_BLE_PIN_CODE_STORAGE_KEY, Ble_pin_code);
    prefs.putString(SETTINGS_BLE_BEACON_MAC_ADDRESS_STORAGE_KEY, Ble_beacon_mac_address);
    prefs.putInt(SETTINGS_BLE_BEACON_RANGE_STORAGE_KEY, Ble_beacon_range);
    prefs.putInt(SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_STORAGE_KEY, Original_display_speed_adjustment);
    prefs.putInt(SETTINGS_TEMPERATURE_WARNING_STORAGE_KEY, Temperature_warning);
    prefs.putInt(SETTINGS_HUMIDITY_WARNING_STORAGE_KEY, Humidity_warning);
    prefs.putInt(SETTINGS_SMARTDISPLAY_SPEED_ADJUSTMENT_STORAGE_KEY, Smartdisplay_speed_adjustment);
    prefs.putInt(SETTINGS_AUX_RELAY_NAME_STORAGE_KEY, Aux_relay_name);
    prefs.putInt(SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_STORAGE_KEY, Display_gps_speed_instead_of_escooter_speed);
    prefs.putInt(SETTINGS_DEFAULT_MODE_AT_STARTUP_STORAGE_KEY, Default_mode_at_startup);
    prefs.putInt(SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_STORAGE_KEY, Default_eco_mode_at_startup);
    prefs.putInt(SETTINGS_DEFAULT_ACCELERATION_STORAGE_KEY, Default_acceleration);
    prefs.putInt(SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_STORAGE_KEY, Default_electric_brake_at_startup);
    prefs.putInt(SETTINGS_PAS_ENABLED_STORAGE_KEY, Pas_enabled);
    prefs.putInt(SETTINGS_ABS_ENABLED_STORAGE_KEY, Abs_enabled);
    prefs.putInt(SETTINGS_DISPLAY_BRIGHTNESS_STORAGE_KEY, Display_brightness);
    prefs.putInt(SETTINGS_DISPLAY_SPLASH_SCREEN_STORAGE_KEY, Display_splash_screen);
    prefs.putInt(SETTINGS_ROTATE_SCREEN_STORAGE_KEY, Rotate_screen);
    prefs.putInt(SETTINGS_EBRAKE_PROGRESSIVE_MODE_STORAGE_KEY, Ebrake_progressive_mode);
    prefs.putInt(SETTINGS_EBRAKE_SMART_BRAKE_TYPE_STORAGE_KEY, Ebrake_smart_brake_type);
    prefs.putInt(SETTINGS_EBRAKE_MIN_POWER_VALUE_STORAGE_KEY, Ebrake_min_power_value);
    prefs.putInt(SETTINGS_EBRAKE_MAX_POWER_VALUE_STORAGE_KEY, Ebrake_max_power_value);
    prefs.putInt(SETTINGS_EBRAKE_TIME_BETWEEN_MODE_SHIFT_STORAGE_KEY, Ebrake_time_between_mode_shift);
    prefs.putInt(SETTINGS_EBRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_STORAGE_KEY, Ebrake_disabled_on_high_battery_voltage);
    prefs.putInt(SETTINGS_EBRAKE_DISABLED_PERCENT_LIMIT_STORAGE_KEY, Ebrake_disabled_percent_limit);
    prefs.putInt(SETTINGS_THROTTLE_REGENERATION_STORAGE_KEY, Throttle_regeneration);
    prefs.putInt(SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_STORAGE_KEY, Throttle_input_min_voltage);
    prefs.putInt(SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_STORAGE_KEY, Throttle_input_max_voltage);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_STORAGE_KEY, Throttle_output_min_voltage);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_STORAGE_KEY, Throttle_output_max_voltage);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_CURVE_STORAGE_KEY, Throttle_output_curve);
    prefs.putString(SETTINGS_THROTTLE_OUTPUT_CURVE_CUSTOM_STORAGE_KEY, Throttle_output_curve_custom);
    prefs.putInt(SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_STORAGE_KEY, Button_1_short_press_action);
    prefs.putInt(SETTINGS_BUTTON_1_LONG_PRESS_ACTION_STORAGE_KEY, Button_1_long_press_action);
    prefs.putInt(SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_STORAGE_KEY, Button_2_short_press_action);
    prefs.putInt(SETTINGS_BUTTON_LONG_PRESS_DURATION_STORAGE_KEY, Button_long_press_duration);
    prefs.putString(SETTINGS_WIFI_SSID_STORAGE_KEY, Wifi_ssid);
    prefs.putString(SETTINGS_WIFI_PASSWORD_STORAGE_KEY, Wifi_password);
    prefs.end();

}

void Settings::unpack_setting_packet(uint8_t* packet, uint8_t length) {
    
    int32_t ind = 0;

    uint16_t settingId = buffer_get_uint16(packet, &ind);
    uint16_t packetNumber = buffer_get_uint16(packet, &ind);

    //Serial.println("settingId = " + (String)settingId);
    //Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
    case SETTINGS_WHEEL_SIZE_BLE_ID :
        set_Wheel_size(buffer_get_float32_auto(packet, &ind));
        //Serial.print("unpack_setting_packet - Wheel_size : " + (String) Wheel_size + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_NUMBER_OF_POLES_PAIRS_BLE_ID :
        set_Number_of_poles_pairs(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Number_of_poles_pairs : " + (String) Number_of_poles_pairs + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BATTERY_MINIMUM_VOLTAGE_BLE_ID :
        set_Battery_minimum_voltage(buffer_get_float32_auto(packet, &ind));
        //Serial.print("unpack_setting_packet - Battery_minimum_voltage : " + (String) Battery_minimum_voltage + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BATTERY_MAXIMUM_VOLTAGE_BLE_ID :
        set_Battery_maximum_voltage(buffer_get_float32_auto(packet, &ind));
        //Serial.print("unpack_setting_packet - Battery_maximum_voltage : " + (String) Battery_maximum_voltage + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BATTERY_MAXIMUM_DISTANCE_BLE_ID :
        set_Battery_maximum_distance(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Battery_maximum_distance : " + (String) Battery_maximum_distance + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_SPEED_LIMITER_AT_STARTUP_BLE_ID :
        set_Speed_limiter_at_startup(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Speed_limiter_at_startup : " + (String) Speed_limiter_at_startup + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_SPEED_LIMITER_MAX_SPEED_BLE_ID :
        set_Speed_limiter_max_speed(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Speed_limiter_max_speed : " + (String) Speed_limiter_max_speed + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BLUETOOTH_LOCK_MODE_BLE_ID :
        set_Bluetooth_lock_mode(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Bluetooth_lock_mode : " + (String) Bluetooth_lock_mode + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BLE_PIN_CODE_BLE_ID :
        set_Ble_pin_code(buffer_get_uint32(packet, &ind));
        //Serial.print("unpack_setting_packet - Ble_pin_code : " + (String) Ble_pin_code + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BLE_BEACON_MAC_ADDRESS_BLE_ID :
        char Ble_beacon_mac_address_part[17];
        memset(Ble_beacon_mac_address_part, 0, 17 );
        if (packetNumber == 0) {
            Ble_beacon_mac_address = "";
        }
        memcpy(Ble_beacon_mac_address_part, &packet[ind], length  - 4);
        Ble_beacon_mac_address =  Ble_beacon_mac_address + Ble_beacon_mac_address_part;
        set_Ble_beacon_mac_address( Ble_beacon_mac_address);
        //Serial.print("unpack_setting_packet - Ble_beacon_mac_address : " + (String) Ble_beacon_mac_address + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BLE_BEACON_RANGE_BLE_ID :
        set_Ble_beacon_range(buffer_get_int8(packet, &ind));
        //Serial.print("unpack_setting_packet - Ble_beacon_range : " + (String) Ble_beacon_range + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_BLE_ID :
        set_Original_display_speed_adjustment(buffer_get_int8(packet, &ind));
        //Serial.print("unpack_setting_packet - Original_display_speed_adjustment : " + (String) Original_display_speed_adjustment + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_TEMPERATURE_WARNING_BLE_ID :
        set_Temperature_warning(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Temperature_warning : " + (String) Temperature_warning + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_HUMIDITY_WARNING_BLE_ID :
        set_Humidity_warning(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Humidity_warning : " + (String) Humidity_warning + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_SMARTDISPLAY_SPEED_ADJUSTMENT_BLE_ID :
        set_Smartdisplay_speed_adjustment(buffer_get_int8(packet, &ind));
        //Serial.print("unpack_setting_packet - Smartdisplay_speed_adjustment : " + (String) Smartdisplay_speed_adjustment + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_AUX_RELAY_NAME_BLE_ID :
        set_Aux_relay_name(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Aux_relay_name : " + (String) Aux_relay_name + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_BLE_ID :
        set_Display_gps_speed_instead_of_escooter_speed(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Display_gps_speed_instead_of_escooter_speed : " + (String) Display_gps_speed_instead_of_escooter_speed + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_DEFAULT_MODE_AT_STARTUP_BLE_ID :
        set_Default_mode_at_startup(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Default_mode_at_startup : " + (String) Default_mode_at_startup + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_BLE_ID :
        set_Default_eco_mode_at_startup(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Default_eco_mode_at_startup : " + (String) Default_eco_mode_at_startup + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_DEFAULT_ACCELERATION_BLE_ID :
        set_Default_acceleration(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Default_acceleration : " + (String) Default_acceleration + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_BLE_ID :
        set_Default_electric_brake_at_startup(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Default_electric_brake_at_startup : " + (String) Default_electric_brake_at_startup + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_PAS_ENABLED_BLE_ID :
        set_Pas_enabled(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Pas_enabled : " + (String) Pas_enabled + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_ABS_ENABLED_BLE_ID :
        set_Abs_enabled(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Abs_enabled : " + (String) Abs_enabled + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_DISPLAY_BRIGHTNESS_BLE_ID :
        set_Display_brightness(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Display_brightness : " + (String) Display_brightness + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_DISPLAY_SPLASH_SCREEN_BLE_ID :
        set_Display_splash_screen(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Display_splash_screen : " + (String) Display_splash_screen + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_ROTATE_SCREEN_BLE_ID :
        set_Rotate_screen(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Rotate_screen : " + (String) Rotate_screen + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_EBRAKE_PROGRESSIVE_MODE_BLE_ID :
        set_Ebrake_progressive_mode(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Ebrake_progressive_mode : " + (String) Ebrake_progressive_mode + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_EBRAKE_SMART_BRAKE_TYPE_BLE_ID :
        set_Ebrake_smart_brake_type(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Ebrake_smart_brake_type : " + (String) Ebrake_smart_brake_type + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_EBRAKE_MIN_POWER_VALUE_BLE_ID :
        set_Ebrake_min_power_value(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Ebrake_min_power_value : " + (String) Ebrake_min_power_value + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_EBRAKE_MAX_POWER_VALUE_BLE_ID :
        set_Ebrake_max_power_value(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Ebrake_max_power_value : " + (String) Ebrake_max_power_value + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_EBRAKE_TIME_BETWEEN_MODE_SHIFT_BLE_ID :
        set_Ebrake_time_between_mode_shift(buffer_get_uint32(packet, &ind));
        //Serial.print("unpack_setting_packet - Ebrake_time_between_mode_shift : " + (String) Ebrake_time_between_mode_shift + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_EBRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_BLE_ID :
        set_Ebrake_disabled_on_high_battery_voltage(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Ebrake_disabled_on_high_battery_voltage : " + (String) Ebrake_disabled_on_high_battery_voltage + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_EBRAKE_DISABLED_PERCENT_LIMIT_BLE_ID :
        set_Ebrake_disabled_percent_limit(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Ebrake_disabled_percent_limit : " + (String) Ebrake_disabled_percent_limit + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_THROTTLE_REGENERATION_BLE_ID :
        set_Throttle_regeneration(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Throttle_regeneration : " + (String) Throttle_regeneration + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_BLE_ID :
        set_Throttle_input_min_voltage(buffer_get_uint32(packet, &ind));
        //Serial.print("unpack_setting_packet - Throttle_input_min_voltage : " + (String) Throttle_input_min_voltage + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_BLE_ID :
        set_Throttle_input_max_voltage(buffer_get_uint32(packet, &ind));
        //Serial.print("unpack_setting_packet - Throttle_input_max_voltage : " + (String) Throttle_input_max_voltage + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_BLE_ID :
        set_Throttle_output_min_voltage(buffer_get_uint32(packet, &ind));
        //Serial.print("unpack_setting_packet - Throttle_output_min_voltage : " + (String) Throttle_output_min_voltage + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_BLE_ID :
        set_Throttle_output_max_voltage(buffer_get_uint32(packet, &ind));
        //Serial.print("unpack_setting_packet - Throttle_output_max_voltage : " + (String) Throttle_output_max_voltage + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_THROTTLE_OUTPUT_CURVE_BLE_ID :
        set_Throttle_output_curve(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Throttle_output_curve : " + (String) Throttle_output_curve + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_THROTTLE_OUTPUT_CURVE_CUSTOM_BLE_ID :
        char Throttle_output_curve_custom_part[17];
        memset(Throttle_output_curve_custom_part, 0, 17 );
        if (packetNumber == 0) {
            Throttle_output_curve_custom = "";
        }
        memcpy(Throttle_output_curve_custom_part, &packet[ind], length  - 4);
        Throttle_output_curve_custom =  Throttle_output_curve_custom + Throttle_output_curve_custom_part;
        set_Throttle_output_curve_custom( Throttle_output_curve_custom);
        //Serial.print("unpack_setting_packet - Throttle_output_curve_custom : " + (String) Throttle_output_curve_custom + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_BLE_ID :
        set_Button_1_short_press_action(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Button_1_short_press_action : " + (String) Button_1_short_press_action + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BUTTON_1_LONG_PRESS_ACTION_BLE_ID :
        set_Button_1_long_press_action(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Button_1_long_press_action : " + (String) Button_1_long_press_action + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_BLE_ID :
        set_Button_2_short_press_action(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Button_2_short_press_action : " + (String) Button_2_short_press_action + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_BUTTON_LONG_PRESS_DURATION_BLE_ID :
        set_Button_long_press_duration(buffer_get_uint8(packet, &ind));
        //Serial.print("unpack_setting_packet - Button_long_press_duration : " + (String) Button_long_press_duration + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_WIFI_SSID_BLE_ID :
        char Wifi_ssid_part[17];
        memset(Wifi_ssid_part, 0, 17 );
        if (packetNumber == 0) {
            Wifi_ssid = "";
        }
        memcpy(Wifi_ssid_part, &packet[ind], length  - 4);
        Wifi_ssid =  Wifi_ssid + Wifi_ssid_part;
        set_Wifi_ssid( Wifi_ssid);
        //Serial.print("unpack_setting_packet - Wifi_ssid : " + (String) Wifi_ssid + " / ");
        //buffer_display("", packet, length);
        break;
    case SETTINGS_WIFI_PASSWORD_BLE_ID :
        char Wifi_password_part[17];
        memset(Wifi_password_part, 0, 17 );
        if (packetNumber == 0) {
            Wifi_password = "";
        }
        memcpy(Wifi_password_part, &packet[ind], length  - 4);
        Wifi_password =  Wifi_password + Wifi_password_part;
        set_Wifi_password( Wifi_password);
        //Serial.print("unpack_setting_packet - Wifi_password : " + (String) Wifi_password + " / ");
        //buffer_display("", packet, length);
        break;
    default:
        Serial.println("unpack_setting_packet : ID error");
        break;
    }
}

bool Settings::pack_setting_packet(uint16_t settingId, uint16_t packetNumber, uint8_t* packet, int32_t* ind) {
    
    bool hasNextPacket = false;

    buffer_append_uint16(packet, settingId, ind);
    buffer_append_uint16(packet, packetNumber, ind);;

    //Serial.println("settingId = " + (String)settingId);
    //Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
    case SETTINGS_WHEEL_SIZE_BLE_ID :
        buffer_append_float32_auto(packet, Wheel_size, ind);
        //Serial.print("pack_setting_packet - Wheel_size : " + (String) Wheel_size + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_NUMBER_OF_POLES_PAIRS_BLE_ID :
        buffer_append_uint8(packet, Number_of_poles_pairs, ind);
        //Serial.print("pack_setting_packet - Number_of_poles_pairs : " + (String) Number_of_poles_pairs + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BATTERY_MINIMUM_VOLTAGE_BLE_ID :
        buffer_append_float32_auto(packet, Battery_minimum_voltage, ind);
        //Serial.print("pack_setting_packet - Battery_minimum_voltage : " + (String) Battery_minimum_voltage + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BATTERY_MAXIMUM_VOLTAGE_BLE_ID :
        buffer_append_float32_auto(packet, Battery_maximum_voltage, ind);
        //Serial.print("pack_setting_packet - Battery_maximum_voltage : " + (String) Battery_maximum_voltage + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BATTERY_MAXIMUM_DISTANCE_BLE_ID :
        buffer_append_uint8(packet, Battery_maximum_distance, ind);
        //Serial.print("pack_setting_packet - Battery_maximum_distance : " + (String) Battery_maximum_distance + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_SPEED_LIMITER_AT_STARTUP_BLE_ID :
        buffer_append_uint8(packet, Speed_limiter_at_startup, ind);
        //Serial.print("pack_setting_packet - Speed_limiter_at_startup : " + (String) Speed_limiter_at_startup + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_SPEED_LIMITER_MAX_SPEED_BLE_ID :
        buffer_append_uint8(packet, Speed_limiter_max_speed, ind);
        //Serial.print("pack_setting_packet - Speed_limiter_max_speed : " + (String) Speed_limiter_max_speed + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BLUETOOTH_LOCK_MODE_BLE_ID :
        buffer_append_uint8(packet, Bluetooth_lock_mode, ind);
        //Serial.print("pack_setting_packet - Bluetooth_lock_mode : " + (String) Bluetooth_lock_mode + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BLE_PIN_CODE_BLE_ID :
        buffer_append_uint32(packet, Ble_pin_code, ind);
        //Serial.print("pack_setting_packet - Ble_pin_code : " + (String) Ble_pin_code + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BLE_BEACON_MAC_ADDRESS_BLE_ID :
        if (packetNumber == 0) {
            if (Ble_beacon_mac_address.length() > 16) {
                hasNextPacket = true;
                memcpy(&packet[*ind], &Ble_beacon_mac_address[0], 16);
                *ind = *ind + 16;
            }
            else {
                memcpy(&packet[*ind], &Ble_beacon_mac_address[0], Ble_beacon_mac_address.length());
                *ind = *ind + Ble_beacon_mac_address.length();
            }
        }
        else if (packetNumber == 1) {
            memcpy(&packet[*ind], &Ble_beacon_mac_address[16], Ble_beacon_mac_address.length() - 16);
            *ind = *ind + Ble_beacon_mac_address.length() - 16;
        }
        //Serial.print("pack_setting_packet - Ble_beacon_mac_address : " + (String) Ble_beacon_mac_address + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BLE_BEACON_RANGE_BLE_ID :
        buffer_append_int8(packet, Ble_beacon_range, ind);
        //Serial.print("pack_setting_packet - Ble_beacon_range : " + (String) Ble_beacon_range + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_BLE_ID :
        buffer_append_int8(packet, Original_display_speed_adjustment, ind);
        //Serial.print("pack_setting_packet - Original_display_speed_adjustment : " + (String) Original_display_speed_adjustment + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_TEMPERATURE_WARNING_BLE_ID :
        buffer_append_uint8(packet, Temperature_warning, ind);
        //Serial.print("pack_setting_packet - Temperature_warning : " + (String) Temperature_warning + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_HUMIDITY_WARNING_BLE_ID :
        buffer_append_uint8(packet, Humidity_warning, ind);
        //Serial.print("pack_setting_packet - Humidity_warning : " + (String) Humidity_warning + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_SMARTDISPLAY_SPEED_ADJUSTMENT_BLE_ID :
        buffer_append_int8(packet, Smartdisplay_speed_adjustment, ind);
        //Serial.print("pack_setting_packet - Smartdisplay_speed_adjustment : " + (String) Smartdisplay_speed_adjustment + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_AUX_RELAY_NAME_BLE_ID :
        buffer_append_uint8(packet, Aux_relay_name, ind);
        //Serial.print("pack_setting_packet - Aux_relay_name : " + (String) Aux_relay_name + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_BLE_ID :
        buffer_append_uint8(packet, Display_gps_speed_instead_of_escooter_speed, ind);
        //Serial.print("pack_setting_packet - Display_gps_speed_instead_of_escooter_speed : " + (String) Display_gps_speed_instead_of_escooter_speed + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_DEFAULT_MODE_AT_STARTUP_BLE_ID :
        buffer_append_uint8(packet, Default_mode_at_startup, ind);
        //Serial.print("pack_setting_packet - Default_mode_at_startup : " + (String) Default_mode_at_startup + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_BLE_ID :
        buffer_append_uint8(packet, Default_eco_mode_at_startup, ind);
        //Serial.print("pack_setting_packet - Default_eco_mode_at_startup : " + (String) Default_eco_mode_at_startup + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_DEFAULT_ACCELERATION_BLE_ID :
        buffer_append_uint8(packet, Default_acceleration, ind);
        //Serial.print("pack_setting_packet - Default_acceleration : " + (String) Default_acceleration + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_BLE_ID :
        buffer_append_uint8(packet, Default_electric_brake_at_startup, ind);
        //Serial.print("pack_setting_packet - Default_electric_brake_at_startup : " + (String) Default_electric_brake_at_startup + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_PAS_ENABLED_BLE_ID :
        buffer_append_uint8(packet, Pas_enabled, ind);
        //Serial.print("pack_setting_packet - Pas_enabled : " + (String) Pas_enabled + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_ABS_ENABLED_BLE_ID :
        buffer_append_uint8(packet, Abs_enabled, ind);
        //Serial.print("pack_setting_packet - Abs_enabled : " + (String) Abs_enabled + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_DISPLAY_BRIGHTNESS_BLE_ID :
        buffer_append_uint8(packet, Display_brightness, ind);
        //Serial.print("pack_setting_packet - Display_brightness : " + (String) Display_brightness + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_DISPLAY_SPLASH_SCREEN_BLE_ID :
        buffer_append_uint8(packet, Display_splash_screen, ind);
        //Serial.print("pack_setting_packet - Display_splash_screen : " + (String) Display_splash_screen + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_ROTATE_SCREEN_BLE_ID :
        buffer_append_uint8(packet, Rotate_screen, ind);
        //Serial.print("pack_setting_packet - Rotate_screen : " + (String) Rotate_screen + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_EBRAKE_PROGRESSIVE_MODE_BLE_ID :
        buffer_append_uint8(packet, Ebrake_progressive_mode, ind);
        //Serial.print("pack_setting_packet - Ebrake_progressive_mode : " + (String) Ebrake_progressive_mode + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_EBRAKE_SMART_BRAKE_TYPE_BLE_ID :
        buffer_append_uint8(packet, Ebrake_smart_brake_type, ind);
        //Serial.print("pack_setting_packet - Ebrake_smart_brake_type : " + (String) Ebrake_smart_brake_type + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_EBRAKE_MIN_POWER_VALUE_BLE_ID :
        buffer_append_uint8(packet, Ebrake_min_power_value, ind);
        //Serial.print("pack_setting_packet - Ebrake_min_power_value : " + (String) Ebrake_min_power_value + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_EBRAKE_MAX_POWER_VALUE_BLE_ID :
        buffer_append_uint8(packet, Ebrake_max_power_value, ind);
        //Serial.print("pack_setting_packet - Ebrake_max_power_value : " + (String) Ebrake_max_power_value + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_EBRAKE_TIME_BETWEEN_MODE_SHIFT_BLE_ID :
        buffer_append_uint32(packet, Ebrake_time_between_mode_shift, ind);
        //Serial.print("pack_setting_packet - Ebrake_time_between_mode_shift : " + (String) Ebrake_time_between_mode_shift + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_EBRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_BLE_ID :
        buffer_append_uint8(packet, Ebrake_disabled_on_high_battery_voltage, ind);
        //Serial.print("pack_setting_packet - Ebrake_disabled_on_high_battery_voltage : " + (String) Ebrake_disabled_on_high_battery_voltage + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_EBRAKE_DISABLED_PERCENT_LIMIT_BLE_ID :
        buffer_append_uint8(packet, Ebrake_disabled_percent_limit, ind);
        //Serial.print("pack_setting_packet - Ebrake_disabled_percent_limit : " + (String) Ebrake_disabled_percent_limit + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_THROTTLE_REGENERATION_BLE_ID :
        buffer_append_uint8(packet, Throttle_regeneration, ind);
        //Serial.print("pack_setting_packet - Throttle_regeneration : " + (String) Throttle_regeneration + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_BLE_ID :
        buffer_append_uint32(packet, Throttle_input_min_voltage, ind);
        //Serial.print("pack_setting_packet - Throttle_input_min_voltage : " + (String) Throttle_input_min_voltage + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_BLE_ID :
        buffer_append_uint32(packet, Throttle_input_max_voltage, ind);
        //Serial.print("pack_setting_packet - Throttle_input_max_voltage : " + (String) Throttle_input_max_voltage + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_BLE_ID :
        buffer_append_uint32(packet, Throttle_output_min_voltage, ind);
        //Serial.print("pack_setting_packet - Throttle_output_min_voltage : " + (String) Throttle_output_min_voltage + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_BLE_ID :
        buffer_append_uint32(packet, Throttle_output_max_voltage, ind);
        //Serial.print("pack_setting_packet - Throttle_output_max_voltage : " + (String) Throttle_output_max_voltage + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_THROTTLE_OUTPUT_CURVE_BLE_ID :
        buffer_append_uint8(packet, Throttle_output_curve, ind);
        //Serial.print("pack_setting_packet - Throttle_output_curve : " + (String) Throttle_output_curve + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_THROTTLE_OUTPUT_CURVE_CUSTOM_BLE_ID :
        if (packetNumber == 0) {
            if (Throttle_output_curve_custom.length() > 16) {
                hasNextPacket = true;
                memcpy(&packet[*ind], &Throttle_output_curve_custom[0], 16);
                *ind = *ind + 16;
            }
            else {
                memcpy(&packet[*ind], &Throttle_output_curve_custom[0], Throttle_output_curve_custom.length());
                *ind = *ind + Throttle_output_curve_custom.length();
            }
        }
        else if (packetNumber == 1) {
            memcpy(&packet[*ind], &Throttle_output_curve_custom[16], Throttle_output_curve_custom.length() - 16);
            *ind = *ind + Throttle_output_curve_custom.length() - 16;
        }
        //Serial.print("pack_setting_packet - Throttle_output_curve_custom : " + (String) Throttle_output_curve_custom + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_BLE_ID :
        buffer_append_uint8(packet, Button_1_short_press_action, ind);
        //Serial.print("pack_setting_packet - Button_1_short_press_action : " + (String) Button_1_short_press_action + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BUTTON_1_LONG_PRESS_ACTION_BLE_ID :
        buffer_append_uint8(packet, Button_1_long_press_action, ind);
        //Serial.print("pack_setting_packet - Button_1_long_press_action : " + (String) Button_1_long_press_action + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_BLE_ID :
        buffer_append_uint8(packet, Button_2_short_press_action, ind);
        //Serial.print("pack_setting_packet - Button_2_short_press_action : " + (String) Button_2_short_press_action + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_BUTTON_LONG_PRESS_DURATION_BLE_ID :
        buffer_append_uint8(packet, Button_long_press_duration, ind);
        //Serial.print("pack_setting_packet - Button_long_press_duration : " + (String) Button_long_press_duration + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_WIFI_SSID_BLE_ID :
        if (packetNumber == 0) {
            if (Wifi_ssid.length() > 16) {
                hasNextPacket = true;
                memcpy(&packet[*ind], &Wifi_ssid[0], 16);
                *ind = *ind + 16;
            }
            else {
                memcpy(&packet[*ind], &Wifi_ssid[0], Wifi_ssid.length());
                *ind = *ind + Wifi_ssid.length();
            }
        }
        else if (packetNumber == 1) {
            memcpy(&packet[*ind], &Wifi_ssid[16], Wifi_ssid.length() - 16);
            *ind = *ind + Wifi_ssid.length() - 16;
        }
        //Serial.print("pack_setting_packet - Wifi_ssid : " + (String) Wifi_ssid + " / ");
        //buffer_display("", packet, *ind);
        break;
    case SETTINGS_WIFI_PASSWORD_BLE_ID :
        if (packetNumber == 0) {
            if (Wifi_password.length() > 16) {
                hasNextPacket = true;
                memcpy(&packet[*ind], &Wifi_password[0], 16);
                *ind = *ind + 16;
            }
            else {
                memcpy(&packet[*ind], &Wifi_password[0], Wifi_password.length());
                *ind = *ind + Wifi_password.length();
            }
        }
        else if (packetNumber == 1) {
            memcpy(&packet[*ind], &Wifi_password[16], Wifi_password.length() - 16);
            *ind = *ind + Wifi_password.length() - 16;
        }
        //Serial.print("pack_setting_packet - Wifi_password : " + (String) Wifi_password + " / ");
        //buffer_display("", packet, *ind);
        break;
    default:
        Serial.println("pack_setting_packet : ID error");
        break;
    }
    
    return hasNextPacket;
}
                

/*-------------------------------------------------------*/

void Settings::set_Wheel_size(float value) {
    Wheel_size = value;
}

float Settings::get_Wheel_size() {
    return Wheel_size ;
}

void Settings::display_Wheel_size() {
    Serial.println("  Wheel_size = " + (String) Wheel_size);
}

void Settings::save_Wheel_size(float value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_WHEEL_SIZE_STORAGE_KEY, Wheel_size);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Number_of_poles_pairs(uint8_t value) {
    Number_of_poles_pairs = value;
}

uint8_t Settings::get_Number_of_poles_pairs() {
    return Number_of_poles_pairs ;
}

void Settings::display_Number_of_poles_pairs() {
    Serial.println("  Number_of_poles_pairs = " + (String) Number_of_poles_pairs);
}

void Settings::save_Number_of_poles_pairs(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_NUMBER_OF_POLES_PAIRS_STORAGE_KEY, Number_of_poles_pairs);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Battery_minimum_voltage(float value) {
    Battery_minimum_voltage = value;
}

float Settings::get_Battery_minimum_voltage() {
    return Battery_minimum_voltage ;
}

void Settings::display_Battery_minimum_voltage() {
    Serial.println("  Battery_minimum_voltage = " + (String) Battery_minimum_voltage);
}

void Settings::save_Battery_minimum_voltage(float value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_BATTERY_MINIMUM_VOLTAGE_STORAGE_KEY, Battery_minimum_voltage);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Battery_maximum_voltage(float value) {
    Battery_maximum_voltage = value;
}

float Settings::get_Battery_maximum_voltage() {
    return Battery_maximum_voltage ;
}

void Settings::display_Battery_maximum_voltage() {
    Serial.println("  Battery_maximum_voltage = " + (String) Battery_maximum_voltage);
}

void Settings::save_Battery_maximum_voltage(float value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_BATTERY_MAXIMUM_VOLTAGE_STORAGE_KEY, Battery_maximum_voltage);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Battery_maximum_distance(uint8_t value) {
    Battery_maximum_distance = value;
}

uint8_t Settings::get_Battery_maximum_distance() {
    return Battery_maximum_distance ;
}

void Settings::display_Battery_maximum_distance() {
    Serial.println("  Battery_maximum_distance = " + (String) Battery_maximum_distance);
}

void Settings::save_Battery_maximum_distance(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BATTERY_MAXIMUM_DISTANCE_STORAGE_KEY, Battery_maximum_distance);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Speed_limiter_at_startup(uint8_t value) {
    Speed_limiter_at_startup = value;
}

uint8_t Settings::get_Speed_limiter_at_startup() {
    return Speed_limiter_at_startup ;
}

void Settings::display_Speed_limiter_at_startup() {
    Serial.println("  Speed_limiter_at_startup = " + (String) Speed_limiter_at_startup);
}

void Settings::save_Speed_limiter_at_startup(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SPEED_LIMITER_AT_STARTUP_STORAGE_KEY, Speed_limiter_at_startup);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Speed_limiter_max_speed(uint8_t value) {
    Speed_limiter_max_speed = value;
}

uint8_t Settings::get_Speed_limiter_max_speed() {
    return Speed_limiter_max_speed ;
}

void Settings::display_Speed_limiter_max_speed() {
    Serial.println("  Speed_limiter_max_speed = " + (String) Speed_limiter_max_speed);
}

void Settings::save_Speed_limiter_max_speed(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SPEED_LIMITER_MAX_SPEED_STORAGE_KEY, Speed_limiter_max_speed);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Bluetooth_lock_mode(uint8_t value) {
    Bluetooth_lock_mode = value;
}

uint8_t Settings::get_Bluetooth_lock_mode() {
    return Bluetooth_lock_mode ;
}

void Settings::display_Bluetooth_lock_mode() {
    Serial.println("  Bluetooth_lock_mode = " + (String) Bluetooth_lock_mode);
}

void Settings::save_Bluetooth_lock_mode(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BLUETOOTH_LOCK_MODE_STORAGE_KEY, Bluetooth_lock_mode);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ble_pin_code(uint32_t value) {
    Ble_pin_code = value;
}

uint32_t Settings::get_Ble_pin_code() {
    return Ble_pin_code ;
}

void Settings::display_Ble_pin_code() {
    Serial.println("  Ble_pin_code = " + (String) Ble_pin_code);
}

void Settings::save_Ble_pin_code(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BLE_PIN_CODE_STORAGE_KEY, Ble_pin_code);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ble_beacon_mac_address(String value) {
    Ble_beacon_mac_address = value;
}

String Settings::get_Ble_beacon_mac_address() {
    return Ble_beacon_mac_address ;
}

void Settings::display_Ble_beacon_mac_address() {
    Serial.println("  Ble_beacon_mac_address = " + (String) Ble_beacon_mac_address);
}

void Settings::save_Ble_beacon_mac_address(String value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_BLE_BEACON_MAC_ADDRESS_STORAGE_KEY, Ble_beacon_mac_address);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ble_beacon_range(int8_t value) {
    Ble_beacon_range = value;
}

int8_t Settings::get_Ble_beacon_range() {
    return Ble_beacon_range ;
}

void Settings::display_Ble_beacon_range() {
    Serial.println("  Ble_beacon_range = " + (String) Ble_beacon_range);
}

void Settings::save_Ble_beacon_range(int8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BLE_BEACON_RANGE_STORAGE_KEY, Ble_beacon_range);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Original_display_speed_adjustment(int8_t value) {
    Original_display_speed_adjustment = value;
}

int8_t Settings::get_Original_display_speed_adjustment() {
    return Original_display_speed_adjustment ;
}

void Settings::display_Original_display_speed_adjustment() {
    Serial.println("  Original_display_speed_adjustment = " + (String) Original_display_speed_adjustment);
}

void Settings::save_Original_display_speed_adjustment(int8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_STORAGE_KEY, Original_display_speed_adjustment);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Temperature_warning(uint8_t value) {
    Temperature_warning = value;
}

uint8_t Settings::get_Temperature_warning() {
    return Temperature_warning ;
}

void Settings::display_Temperature_warning() {
    Serial.println("  Temperature_warning = " + (String) Temperature_warning);
}

void Settings::save_Temperature_warning(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_TEMPERATURE_WARNING_STORAGE_KEY, Temperature_warning);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Humidity_warning(uint8_t value) {
    Humidity_warning = value;
}

uint8_t Settings::get_Humidity_warning() {
    return Humidity_warning ;
}

void Settings::display_Humidity_warning() {
    Serial.println("  Humidity_warning = " + (String) Humidity_warning);
}

void Settings::save_Humidity_warning(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_HUMIDITY_WARNING_STORAGE_KEY, Humidity_warning);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Smartdisplay_speed_adjustment(int8_t value) {
    Smartdisplay_speed_adjustment = value;
}

int8_t Settings::get_Smartdisplay_speed_adjustment() {
    return Smartdisplay_speed_adjustment ;
}

void Settings::display_Smartdisplay_speed_adjustment() {
    Serial.println("  Smartdisplay_speed_adjustment = " + (String) Smartdisplay_speed_adjustment);
}

void Settings::save_Smartdisplay_speed_adjustment(int8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SMARTDISPLAY_SPEED_ADJUSTMENT_STORAGE_KEY, Smartdisplay_speed_adjustment);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Aux_relay_name(uint8_t value) {
    Aux_relay_name = value;
}

uint8_t Settings::get_Aux_relay_name() {
    return Aux_relay_name ;
}

void Settings::display_Aux_relay_name() {
    Serial.println("  Aux_relay_name = " + (String) Aux_relay_name);
}

void Settings::save_Aux_relay_name(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_AUX_RELAY_NAME_STORAGE_KEY, Aux_relay_name);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Display_gps_speed_instead_of_escooter_speed(uint8_t value) {
    Display_gps_speed_instead_of_escooter_speed = value;
}

uint8_t Settings::get_Display_gps_speed_instead_of_escooter_speed() {
    return Display_gps_speed_instead_of_escooter_speed ;
}

void Settings::display_Display_gps_speed_instead_of_escooter_speed() {
    Serial.println("  Display_gps_speed_instead_of_escooter_speed = " + (String) Display_gps_speed_instead_of_escooter_speed);
}

void Settings::save_Display_gps_speed_instead_of_escooter_speed(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_STORAGE_KEY, Display_gps_speed_instead_of_escooter_speed);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Default_mode_at_startup(uint8_t value) {
    Default_mode_at_startup = value;
}

uint8_t Settings::get_Default_mode_at_startup() {
    return Default_mode_at_startup ;
}

void Settings::display_Default_mode_at_startup() {
    Serial.println("  Default_mode_at_startup = " + (String) Default_mode_at_startup);
}

void Settings::save_Default_mode_at_startup(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_MODE_AT_STARTUP_STORAGE_KEY, Default_mode_at_startup);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Default_eco_mode_at_startup(uint8_t value) {
    Default_eco_mode_at_startup = value;
}

uint8_t Settings::get_Default_eco_mode_at_startup() {
    return Default_eco_mode_at_startup ;
}

void Settings::display_Default_eco_mode_at_startup() {
    Serial.println("  Default_eco_mode_at_startup = " + (String) Default_eco_mode_at_startup);
}

void Settings::save_Default_eco_mode_at_startup(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_STORAGE_KEY, Default_eco_mode_at_startup);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Default_acceleration(uint8_t value) {
    Default_acceleration = value;
}

uint8_t Settings::get_Default_acceleration() {
    return Default_acceleration ;
}

void Settings::display_Default_acceleration() {
    Serial.println("  Default_acceleration = " + (String) Default_acceleration);
}

void Settings::save_Default_acceleration(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ACCELERATION_STORAGE_KEY, Default_acceleration);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Default_electric_brake_at_startup(uint8_t value) {
    Default_electric_brake_at_startup = value;
}

uint8_t Settings::get_Default_electric_brake_at_startup() {
    return Default_electric_brake_at_startup ;
}

void Settings::display_Default_electric_brake_at_startup() {
    Serial.println("  Default_electric_brake_at_startup = " + (String) Default_electric_brake_at_startup);
}

void Settings::save_Default_electric_brake_at_startup(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_STORAGE_KEY, Default_electric_brake_at_startup);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Pas_enabled(uint8_t value) {
    Pas_enabled = value;
}

uint8_t Settings::get_Pas_enabled() {
    return Pas_enabled ;
}

void Settings::display_Pas_enabled() {
    Serial.println("  Pas_enabled = " + (String) Pas_enabled);
}

void Settings::save_Pas_enabled(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_PAS_ENABLED_STORAGE_KEY, Pas_enabled);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Abs_enabled(uint8_t value) {
    Abs_enabled = value;
}

uint8_t Settings::get_Abs_enabled() {
    return Abs_enabled ;
}

void Settings::display_Abs_enabled() {
    Serial.println("  Abs_enabled = " + (String) Abs_enabled);
}

void Settings::save_Abs_enabled(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ABS_ENABLED_STORAGE_KEY, Abs_enabled);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Display_brightness(uint8_t value) {
    Display_brightness = value;
}

uint8_t Settings::get_Display_brightness() {
    return Display_brightness ;
}

void Settings::display_Display_brightness() {
    Serial.println("  Display_brightness = " + (String) Display_brightness);
}

void Settings::save_Display_brightness(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISPLAY_BRIGHTNESS_STORAGE_KEY, Display_brightness);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Display_splash_screen(uint8_t value) {
    Display_splash_screen = value;
}

uint8_t Settings::get_Display_splash_screen() {
    return Display_splash_screen ;
}

void Settings::display_Display_splash_screen() {
    Serial.println("  Display_splash_screen = " + (String) Display_splash_screen);
}

void Settings::save_Display_splash_screen(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISPLAY_SPLASH_SCREEN_STORAGE_KEY, Display_splash_screen);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Rotate_screen(uint8_t value) {
    Rotate_screen = value;
}

uint8_t Settings::get_Rotate_screen() {
    return Rotate_screen ;
}

void Settings::display_Rotate_screen() {
    Serial.println("  Rotate_screen = " + (String) Rotate_screen);
}

void Settings::save_Rotate_screen(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ROTATE_SCREEN_STORAGE_KEY, Rotate_screen);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ebrake_progressive_mode(uint8_t value) {
    Ebrake_progressive_mode = value;
}

uint8_t Settings::get_Ebrake_progressive_mode() {
    return Ebrake_progressive_mode ;
}

void Settings::display_Ebrake_progressive_mode() {
    Serial.println("  Ebrake_progressive_mode = " + (String) Ebrake_progressive_mode);
}

void Settings::save_Ebrake_progressive_mode(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_EBRAKE_PROGRESSIVE_MODE_STORAGE_KEY, Ebrake_progressive_mode);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ebrake_smart_brake_type(uint8_t value) {
    Ebrake_smart_brake_type = value;
}

uint8_t Settings::get_Ebrake_smart_brake_type() {
    return Ebrake_smart_brake_type ;
}

void Settings::display_Ebrake_smart_brake_type() {
    Serial.println("  Ebrake_smart_brake_type = " + (String) Ebrake_smart_brake_type);
}

void Settings::save_Ebrake_smart_brake_type(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_EBRAKE_SMART_BRAKE_TYPE_STORAGE_KEY, Ebrake_smart_brake_type);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ebrake_min_power_value(uint8_t value) {
    Ebrake_min_power_value = value;
}

uint8_t Settings::get_Ebrake_min_power_value() {
    return Ebrake_min_power_value ;
}

void Settings::display_Ebrake_min_power_value() {
    Serial.println("  Ebrake_min_power_value = " + (String) Ebrake_min_power_value);
}

void Settings::save_Ebrake_min_power_value(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_EBRAKE_MIN_POWER_VALUE_STORAGE_KEY, Ebrake_min_power_value);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ebrake_max_power_value(uint8_t value) {
    Ebrake_max_power_value = value;
}

uint8_t Settings::get_Ebrake_max_power_value() {
    return Ebrake_max_power_value ;
}

void Settings::display_Ebrake_max_power_value() {
    Serial.println("  Ebrake_max_power_value = " + (String) Ebrake_max_power_value);
}

void Settings::save_Ebrake_max_power_value(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_EBRAKE_MAX_POWER_VALUE_STORAGE_KEY, Ebrake_max_power_value);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ebrake_time_between_mode_shift(uint32_t value) {
    Ebrake_time_between_mode_shift = value;
}

uint32_t Settings::get_Ebrake_time_between_mode_shift() {
    return Ebrake_time_between_mode_shift ;
}

void Settings::display_Ebrake_time_between_mode_shift() {
    Serial.println("  Ebrake_time_between_mode_shift = " + (String) Ebrake_time_between_mode_shift);
}

void Settings::save_Ebrake_time_between_mode_shift(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_EBRAKE_TIME_BETWEEN_MODE_SHIFT_STORAGE_KEY, Ebrake_time_between_mode_shift);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ebrake_disabled_on_high_battery_voltage(uint8_t value) {
    Ebrake_disabled_on_high_battery_voltage = value;
}

uint8_t Settings::get_Ebrake_disabled_on_high_battery_voltage() {
    return Ebrake_disabled_on_high_battery_voltage ;
}

void Settings::display_Ebrake_disabled_on_high_battery_voltage() {
    Serial.println("  Ebrake_disabled_on_high_battery_voltage = " + (String) Ebrake_disabled_on_high_battery_voltage);
}

void Settings::save_Ebrake_disabled_on_high_battery_voltage(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_EBRAKE_DISABLED_ON_HIGH_BATTERY_VOLTAGE_STORAGE_KEY, Ebrake_disabled_on_high_battery_voltage);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Ebrake_disabled_percent_limit(uint8_t value) {
    Ebrake_disabled_percent_limit = value;
}

uint8_t Settings::get_Ebrake_disabled_percent_limit() {
    return Ebrake_disabled_percent_limit ;
}

void Settings::display_Ebrake_disabled_percent_limit() {
    Serial.println("  Ebrake_disabled_percent_limit = " + (String) Ebrake_disabled_percent_limit);
}

void Settings::save_Ebrake_disabled_percent_limit(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_EBRAKE_DISABLED_PERCENT_LIMIT_STORAGE_KEY, Ebrake_disabled_percent_limit);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Throttle_regeneration(uint8_t value) {
    Throttle_regeneration = value;
}

uint8_t Settings::get_Throttle_regeneration() {
    return Throttle_regeneration ;
}

void Settings::display_Throttle_regeneration() {
    Serial.println("  Throttle_regeneration = " + (String) Throttle_regeneration);
}

void Settings::save_Throttle_regeneration(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_REGENERATION_STORAGE_KEY, Throttle_regeneration);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Throttle_input_min_voltage(uint32_t value) {
    Throttle_input_min_voltage = value;
}

uint32_t Settings::get_Throttle_input_min_voltage() {
    return Throttle_input_min_voltage ;
}

void Settings::display_Throttle_input_min_voltage() {
    Serial.println("  Throttle_input_min_voltage = " + (String) Throttle_input_min_voltage);
}

void Settings::save_Throttle_input_min_voltage(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_INPUT_MIN_VOLTAGE_STORAGE_KEY, Throttle_input_min_voltage);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Throttle_input_max_voltage(uint32_t value) {
    Throttle_input_max_voltage = value;
}

uint32_t Settings::get_Throttle_input_max_voltage() {
    return Throttle_input_max_voltage ;
}

void Settings::display_Throttle_input_max_voltage() {
    Serial.println("  Throttle_input_max_voltage = " + (String) Throttle_input_max_voltage);
}

void Settings::save_Throttle_input_max_voltage(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_INPUT_MAX_VOLTAGE_STORAGE_KEY, Throttle_input_max_voltage);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Throttle_output_min_voltage(uint32_t value) {
    Throttle_output_min_voltage = value;
}

uint32_t Settings::get_Throttle_output_min_voltage() {
    return Throttle_output_min_voltage ;
}

void Settings::display_Throttle_output_min_voltage() {
    Serial.println("  Throttle_output_min_voltage = " + (String) Throttle_output_min_voltage);
}

void Settings::save_Throttle_output_min_voltage(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_MIN_VOLTAGE_STORAGE_KEY, Throttle_output_min_voltage);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Throttle_output_max_voltage(uint32_t value) {
    Throttle_output_max_voltage = value;
}

uint32_t Settings::get_Throttle_output_max_voltage() {
    return Throttle_output_max_voltage ;
}

void Settings::display_Throttle_output_max_voltage() {
    Serial.println("  Throttle_output_max_voltage = " + (String) Throttle_output_max_voltage);
}

void Settings::save_Throttle_output_max_voltage(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_MAX_VOLTAGE_STORAGE_KEY, Throttle_output_max_voltage);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Throttle_output_curve(uint8_t value) {
    Throttle_output_curve = value;
}

uint8_t Settings::get_Throttle_output_curve() {
    return Throttle_output_curve ;
}

void Settings::display_Throttle_output_curve() {
    Serial.println("  Throttle_output_curve = " + (String) Throttle_output_curve);
}

void Settings::save_Throttle_output_curve(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_OUTPUT_CURVE_STORAGE_KEY, Throttle_output_curve);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Throttle_output_curve_custom(String value) {
    Throttle_output_curve_custom = value;
}

String Settings::get_Throttle_output_curve_custom() {
    return Throttle_output_curve_custom ;
}

void Settings::display_Throttle_output_curve_custom() {
    Serial.println("  Throttle_output_curve_custom = " + (String) Throttle_output_curve_custom);
}

void Settings::save_Throttle_output_curve_custom(String value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_THROTTLE_OUTPUT_CURVE_CUSTOM_STORAGE_KEY, Throttle_output_curve_custom);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Button_1_short_press_action(uint8_t value) {
    Button_1_short_press_action = value;
}

uint8_t Settings::get_Button_1_short_press_action() {
    return Button_1_short_press_action ;
}

void Settings::display_Button_1_short_press_action() {
    Serial.println("  Button_1_short_press_action = " + (String) Button_1_short_press_action);
}

void Settings::save_Button_1_short_press_action(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_1_SHORT_PRESS_ACTION_STORAGE_KEY, Button_1_short_press_action);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Button_1_long_press_action(uint8_t value) {
    Button_1_long_press_action = value;
}

uint8_t Settings::get_Button_1_long_press_action() {
    return Button_1_long_press_action ;
}

void Settings::display_Button_1_long_press_action() {
    Serial.println("  Button_1_long_press_action = " + (String) Button_1_long_press_action);
}

void Settings::save_Button_1_long_press_action(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_1_LONG_PRESS_ACTION_STORAGE_KEY, Button_1_long_press_action);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Button_2_short_press_action(uint8_t value) {
    Button_2_short_press_action = value;
}

uint8_t Settings::get_Button_2_short_press_action() {
    return Button_2_short_press_action ;
}

void Settings::display_Button_2_short_press_action() {
    Serial.println("  Button_2_short_press_action = " + (String) Button_2_short_press_action);
}

void Settings::save_Button_2_short_press_action(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_2_SHORT_PRESS_ACTION_STORAGE_KEY, Button_2_short_press_action);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Button_long_press_duration(uint8_t value) {
    Button_long_press_duration = value;
}

uint8_t Settings::get_Button_long_press_duration() {
    return Button_long_press_duration ;
}

void Settings::display_Button_long_press_duration() {
    Serial.println("  Button_long_press_duration = " + (String) Button_long_press_duration);
}

void Settings::save_Button_long_press_duration(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_LONG_PRESS_DURATION_STORAGE_KEY, Button_long_press_duration);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Wifi_ssid(String value) {
    Wifi_ssid = value;
}

String Settings::get_Wifi_ssid() {
    return Wifi_ssid ;
}

void Settings::display_Wifi_ssid() {
    Serial.println("  Wifi_ssid = " + (String) Wifi_ssid);
}

void Settings::save_Wifi_ssid(String value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_WIFI_SSID_STORAGE_KEY, Wifi_ssid);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings::set_Wifi_password(String value) {
    Wifi_password = value;
}

String Settings::get_Wifi_password() {
    return Wifi_password ;
}

void Settings::display_Wifi_password() {
    Serial.println("  Wifi_password = " + (String) Wifi_password);
}

void Settings::save_Wifi_password(String value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_WIFI_PASSWORD_STORAGE_KEY, Wifi_password);
    prefs.end();
}

