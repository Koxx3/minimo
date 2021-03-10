
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
    speed_limiter_max_speed = prefs.getInt(SETTINGS_SPEED_LIMITER_MAX_SPEED_ID_STR, 37);
    Serial.println("  >> speed_limiter_max_speed = " + (String)speed_limiter_max_speed);
    bluetooth_lock_mode = prefs.getInt(SETTINGS_BLUETOOTH_LOCK_MODE_ID_STR, 0);
    Serial.println("  >> bluetooth_lock_mode = " + (String)bluetooth_lock_mode);
    pin_code = prefs.getInt(SETTINGS_PIN_CODE_ID_STR, 147258);
    Serial.println("  >> pin_code = " + (String)pin_code);
    beacon_mac_address = prefs.getString(SETTINGS_BEACON_MAC_ADDRESS_ID_STR, "aa:bb:cc:dd:ee:ff");
    Serial.println("  >> beacon_mac_address = " + (String)beacon_mac_address);
    beacon_range = prefs.getInt(SETTINGS_BEACON_RANGE_ID_STR, -80);
    Serial.println("  >> beacon_range = " + (String)beacon_range);
    original_display_speed_adjustment = prefs.getInt(SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID_STR, 0);
    Serial.println("  >> original_display_speed_adjustment = " + (String)original_display_speed_adjustment);
    progressive_mode = prefs.getInt(SETTINGS_PROGRESSIVE_MODE_ID_STR, 0);
    Serial.println("  >> progressive_mode = " + (String)progressive_mode);
    type = prefs.getInt(SETTINGS_TYPE_ID_STR, 0);
    Serial.println("  >> type = " + (String)type);
    min_value = prefs.getInt(SETTINGS_MIN_VALUE_ID_STR, 1);
    Serial.println("  >> min_value = " + (String)min_value);
    max_value = prefs.getInt(SETTINGS_MAX_VALUE_ID_STR, 5);
    Serial.println("  >> max_value = " + (String)max_value);
    time_between_mode_shift = prefs.getInt(SETTINGS_TIME_BETWEEN_MODE_SHIFT_ID_STR, 500);
    Serial.println("  >> time_between_mode_shift = " + (String)time_between_mode_shift);
    disabled_on_high_battery_voltage = prefs.getInt(SETTINGS_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID_STR, 0);
    Serial.println("  >> disabled_on_high_battery_voltage = " + (String)disabled_on_high_battery_voltage);
    disabled_percent_limit = prefs.getInt(SETTINGS_DISABLED_PERCENT_LIMIT_ID_STR, 100);
    Serial.println("  >> disabled_percent_limit = " + (String)disabled_percent_limit);
    throttle_regeneration = prefs.getInt(SETTINGS_THROTTLE_REGENERATION_ID_STR, 0);
    Serial.println("  >> throttle_regeneration = " + (String)throttle_regeneration);
    input_min_voltage_in_millivolts = prefs.getInt(SETTINGS_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, 650);
    Serial.println("  >> input_min_voltage_in_millivolts = " + (String)input_min_voltage_in_millivolts);
    input_max_voltage_in_millivolts = prefs.getInt(SETTINGS_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, 3950);
    Serial.println("  >> input_max_voltage_in_millivolts = " + (String)input_max_voltage_in_millivolts);
    output_min_voltage_in_millivolts = prefs.getInt(SETTINGS_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, 800);
    Serial.println("  >> output_min_voltage_in_millivolts = " + (String)output_min_voltage_in_millivolts);
    output_max_voltage_in_millivolts = prefs.getInt(SETTINGS_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, 3350);
    Serial.println("  >> output_max_voltage_in_millivolts = " + (String)output_max_voltage_in_millivolts);
    output_curve = prefs.getInt(SETTINGS_OUTPUT_CURVE_ID_STR, 2);
    Serial.println("  >> output_curve = " + (String)output_curve);
    b1_short_press_action = prefs.getInt(SETTINGS_B1_SHORT_PRESS_ACTION_ID_STR, 0);
    Serial.println("  >> b1_short_press_action = " + (String)b1_short_press_action);
    b1_long_press_action = prefs.getInt(SETTINGS_B1_LONG_PRESS_ACTION_ID_STR, 0);
    Serial.println("  >> b1_long_press_action = " + (String)b1_long_press_action);
    b2_short_press_action = prefs.getInt(SETTINGS_B2_SHORT_PRESS_ACTION_ID_STR, 0);
    Serial.println("  >> b2_short_press_action = " + (String)b2_short_press_action);
    button_long_press_duration = prefs.getInt(SETTINGS_BUTTON_LONG_PRESS_DURATION_ID_STR, 5);
    Serial.println("  >> button_long_press_duration = " + (String)button_long_press_duration);
    wifi_network_name_ssid = prefs.getString(SETTINGS_WIFI_NETWORK_NAME_SSID_ID_STR, "");
    Serial.println("  >> wifi_network_name_ssid = " + (String)wifi_network_name_ssid);
    wifi_password = prefs.getString(SETTINGS_WIFI_PASSWORD_ID_STR, "");
    Serial.println("  >> wifi_password = " + (String)wifi_password);
    prefs.end();
    Serial.println(" < restore settings");
}

void Settings2::save() {

    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_WHEEL_SIZE_ID_STR, wheel_size);
    prefs.putInt(SETTINGS_NUMBER_OF_POLES_PAIRS_ID_STR, number_of_poles_pairs);
    prefs.putFloat(SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID_STR, battery_minimum_voltage);
    prefs.putFloat(SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID_STR, battery_maximum_voltage);
    prefs.putInt(SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID_STR, battery_maximum_distance);
    prefs.putInt(SETTINGS_SPEED_ADJUSTMENT_ID_STR, speed_adjustment);
    prefs.putInt(SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID_STR, display_gps_speed_instead_of_escooter_speed);
    prefs.putInt(SETTINGS_AUX_RELAY_NAME_ID_STR, aux_relay_name);
    prefs.putInt(SETTINGS_DEFAULT_MODE_AT_STARTUP_ID_STR, default_mode_at_startup);
    prefs.putInt(SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID_STR, default_eco_mode_at_startup);
    prefs.putInt(SETTINGS_DEFAULT_ACCELERATION_ID_STR, default_acceleration);
    prefs.putInt(SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID_STR, default_electric_brake_at_startup);
    prefs.putInt(SETTINGS_PAS_ENABLED_ID_STR, pas_enabled);
    prefs.putInt(SETTINGS_ABS_ENABLED_ID_STR, abs_enabled);
    prefs.putInt(SETTINGS_DISPLAY_BRIGHTNESS_ID_STR, display_brightness);
    prefs.putInt(SETTINGS_TEMPERATURE_WARNING_ID_STR, temperature_warning);
    prefs.putInt(SETTINGS_HUMIDITY_WARNING_ID_STR, humidity_warning);
    prefs.putInt(SETTINGS_SPEED_LIMITER_AT_STARTUP_ID_STR, speed_limiter_at_startup);
    prefs.putInt(SETTINGS_SPEED_LIMITER_MAX_SPEED_ID_STR, speed_limiter_max_speed);
    prefs.putInt(SETTINGS_BLUETOOTH_LOCK_MODE_ID_STR, bluetooth_lock_mode);
    prefs.putInt(SETTINGS_PIN_CODE_ID_STR, pin_code);
    prefs.putString(SETTINGS_BEACON_MAC_ADDRESS_ID_STR, beacon_mac_address);
    prefs.putInt(SETTINGS_BEACON_RANGE_ID_STR, beacon_range);
    prefs.putInt(SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID_STR, original_display_speed_adjustment);
    prefs.putInt(SETTINGS_PROGRESSIVE_MODE_ID_STR, progressive_mode);
    prefs.putInt(SETTINGS_TYPE_ID_STR, type);
    prefs.putInt(SETTINGS_MIN_VALUE_ID_STR, min_value);
    prefs.putInt(SETTINGS_MAX_VALUE_ID_STR, max_value);
    prefs.putInt(SETTINGS_TIME_BETWEEN_MODE_SHIFT_ID_STR, time_between_mode_shift);
    prefs.putInt(SETTINGS_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID_STR, disabled_on_high_battery_voltage);
    prefs.putInt(SETTINGS_DISABLED_PERCENT_LIMIT_ID_STR, disabled_percent_limit);
    prefs.putInt(SETTINGS_THROTTLE_REGENERATION_ID_STR, throttle_regeneration);
    prefs.putInt(SETTINGS_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, input_min_voltage_in_millivolts);
    prefs.putInt(SETTINGS_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, input_max_voltage_in_millivolts);
    prefs.putInt(SETTINGS_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, output_min_voltage_in_millivolts);
    prefs.putInt(SETTINGS_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, output_max_voltage_in_millivolts);
    prefs.putInt(SETTINGS_OUTPUT_CURVE_ID_STR, output_curve);
    prefs.putInt(SETTINGS_B1_SHORT_PRESS_ACTION_ID_STR, b1_short_press_action);
    prefs.putInt(SETTINGS_B1_LONG_PRESS_ACTION_ID_STR, b1_long_press_action);
    prefs.putInt(SETTINGS_B2_SHORT_PRESS_ACTION_ID_STR, b2_short_press_action);
    prefs.putInt(SETTINGS_BUTTON_LONG_PRESS_DURATION_ID_STR, button_long_press_duration);
    prefs.putString(SETTINGS_WIFI_NETWORK_NAME_SSID_ID_STR, wifi_network_name_ssid);
    prefs.putString(SETTINGS_WIFI_PASSWORD_ID_STR, wifi_password);

    prefs.end();

}

void Settings2::unpack_setting_packet(uint8_t* packet, uint8_t length) {
    
    int32_t ind = 0;

    uint16_t settingId = buffer_get_uint16(packet, &ind);
    uint16_t packetNumber = buffer_get_uint16(packet, &ind);

    //Serial.println("settingId = " + (String)settingId);
    //Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
    case SETTINGS_WHEEL_SIZE_ID :
        set_wheel_size(buffer_get_float32_auto(packet, &ind));
        Serial.print("unpack_setting_packet - wheel_size : " + (String) wheel_size + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_NUMBER_OF_POLES_PAIRS_ID :
        set_number_of_poles_pairs(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - number_of_poles_pairs : " + (String) number_of_poles_pairs + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID :
        set_battery_minimum_voltage(buffer_get_float32_auto(packet, &ind));
        Serial.print("unpack_setting_packet - battery_minimum_voltage : " + (String) battery_minimum_voltage + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID :
        set_battery_maximum_voltage(buffer_get_float32_auto(packet, &ind));
        Serial.print("unpack_setting_packet - battery_maximum_voltage : " + (String) battery_maximum_voltage + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID :
        set_battery_maximum_distance(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - battery_maximum_distance : " + (String) battery_maximum_distance + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_SPEED_ADJUSTMENT_ID :
        set_speed_adjustment(buffer_get_int8(packet, &ind));
        Serial.print("unpack_setting_packet - speed_adjustment : " + (String) speed_adjustment + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID :
        set_display_gps_speed_instead_of_escooter_speed(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - display_gps_speed_instead_of_escooter_speed : " + (String) display_gps_speed_instead_of_escooter_speed + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_AUX_RELAY_NAME_ID :
        set_aux_relay_name(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - aux_relay_name : " + (String) aux_relay_name + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_DEFAULT_MODE_AT_STARTUP_ID :
        set_default_mode_at_startup(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - default_mode_at_startup : " + (String) default_mode_at_startup + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID :
        set_default_eco_mode_at_startup(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - default_eco_mode_at_startup : " + (String) default_eco_mode_at_startup + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_DEFAULT_ACCELERATION_ID :
        set_default_acceleration(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - default_acceleration : " + (String) default_acceleration + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID :
        set_default_electric_brake_at_startup(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - default_electric_brake_at_startup : " + (String) default_electric_brake_at_startup + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_PAS_ENABLED_ID :
        set_pas_enabled(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - pas_enabled : " + (String) pas_enabled + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_ABS_ENABLED_ID :
        set_abs_enabled(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - abs_enabled : " + (String) abs_enabled + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_DISPLAY_BRIGHTNESS_ID :
        set_display_brightness(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - display_brightness : " + (String) display_brightness + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_TEMPERATURE_WARNING_ID :
        set_temperature_warning(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - temperature_warning : " + (String) temperature_warning + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_HUMIDITY_WARNING_ID :
        set_humidity_warning(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - humidity_warning : " + (String) humidity_warning + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_SPEED_LIMITER_AT_STARTUP_ID :
        set_speed_limiter_at_startup(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - speed_limiter_at_startup : " + (String) speed_limiter_at_startup + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_SPEED_LIMITER_MAX_SPEED_ID :
        set_speed_limiter_max_speed(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - speed_limiter_max_speed : " + (String) speed_limiter_max_speed + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_BLUETOOTH_LOCK_MODE_ID :
        set_bluetooth_lock_mode(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - bluetooth_lock_mode : " + (String) bluetooth_lock_mode + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_PIN_CODE_ID :
        set_pin_code(buffer_get_uint32(packet, &ind));
        Serial.print("unpack_setting_packet - pin_code : " + (String) pin_code + " / ");
        buffer_display("", packet, length);
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
        Serial.print("unpack_setting_packet - beacon_mac_address : " + (String) beacon_mac_address + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_BEACON_RANGE_ID :
        set_beacon_range(buffer_get_int8(packet, &ind));
        Serial.print("unpack_setting_packet - beacon_range : " + (String) beacon_range + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID :
        set_original_display_speed_adjustment(buffer_get_int8(packet, &ind));
        Serial.print("unpack_setting_packet - original_display_speed_adjustment : " + (String) original_display_speed_adjustment + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_PROGRESSIVE_MODE_ID :
        set_progressive_mode(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - progressive_mode : " + (String) progressive_mode + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_TYPE_ID :
        set_type(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - type : " + (String) type + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_MIN_VALUE_ID :
        set_min_value(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - min_value : " + (String) min_value + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_MAX_VALUE_ID :
        set_max_value(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - max_value : " + (String) max_value + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_TIME_BETWEEN_MODE_SHIFT_ID :
        set_time_between_mode_shift(buffer_get_uint32(packet, &ind));
        Serial.print("unpack_setting_packet - time_between_mode_shift : " + (String) time_between_mode_shift + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID :
        set_disabled_on_high_battery_voltage(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - disabled_on_high_battery_voltage : " + (String) disabled_on_high_battery_voltage + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_DISABLED_PERCENT_LIMIT_ID :
        set_disabled_percent_limit(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - disabled_percent_limit : " + (String) disabled_percent_limit + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_THROTTLE_REGENERATION_ID :
        set_throttle_regeneration(buffer_get_uint32(packet, &ind));
        Serial.print("unpack_setting_packet - throttle_regeneration : " + (String) throttle_regeneration + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID :
        set_input_min_voltage_in_millivolts(buffer_get_uint32(packet, &ind));
        Serial.print("unpack_setting_packet - input_min_voltage_in_millivolts : " + (String) input_min_voltage_in_millivolts + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID :
        set_input_max_voltage_in_millivolts(buffer_get_uint32(packet, &ind));
        Serial.print("unpack_setting_packet - input_max_voltage_in_millivolts : " + (String) input_max_voltage_in_millivolts + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID :
        set_output_min_voltage_in_millivolts(buffer_get_uint32(packet, &ind));
        Serial.print("unpack_setting_packet - output_min_voltage_in_millivolts : " + (String) output_min_voltage_in_millivolts + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID :
        set_output_max_voltage_in_millivolts(buffer_get_uint32(packet, &ind));
        Serial.print("unpack_setting_packet - output_max_voltage_in_millivolts : " + (String) output_max_voltage_in_millivolts + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_OUTPUT_CURVE_ID :
        set_output_curve(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - output_curve : " + (String) output_curve + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_B1_SHORT_PRESS_ACTION_ID :
        set_b1_short_press_action(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - b1_short_press_action : " + (String) b1_short_press_action + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_B1_LONG_PRESS_ACTION_ID :
        set_b1_long_press_action(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - b1_long_press_action : " + (String) b1_long_press_action + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_B2_SHORT_PRESS_ACTION_ID :
        set_b2_short_press_action(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - b2_short_press_action : " + (String) b2_short_press_action + " / ");
        buffer_display("", packet, length);
        break;
    case SETTINGS_BUTTON_LONG_PRESS_DURATION_ID :
        set_button_long_press_duration(buffer_get_uint8(packet, &ind));
        Serial.print("unpack_setting_packet - button_long_press_duration : " + (String) button_long_press_duration + " / ");
        buffer_display("", packet, length);
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
        Serial.print("unpack_setting_packet - wifi_network_name_ssid : " + (String) wifi_network_name_ssid + " / ");
        buffer_display("", packet, length);
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
        Serial.print("unpack_setting_packet - wifi_password : " + (String) wifi_password + " / ");
        buffer_display("", packet, length);
        break;
    default:
        Serial.println("unpack_setting_packet : ID error");
        break;
    }
}

void Settings2::pack_setting_packet(uint16_t settingId, uint16_t packetNumber, uint8_t* packet, int32_t* ind) {
    
    buffer_append_uint16(packet, settingId, ind);
    buffer_append_uint16(packet, packetNumber, ind);;

    //Serial.println("settingId = " + (String)settingId);
    //Serial.println("packetNumber = " + (String)packetNumber);

    switch(settingId) {
    case SETTINGS_WHEEL_SIZE_ID :
        buffer_append_float32_auto(packet, wheel_size, ind);
        Serial.print("pack_setting_packet - wheel_size : " + (String) wheel_size + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_NUMBER_OF_POLES_PAIRS_ID :
        buffer_append_uint8(packet, number_of_poles_pairs, ind);
        Serial.print("pack_setting_packet - number_of_poles_pairs : " + (String) number_of_poles_pairs + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID :
        buffer_append_float32_auto(packet, battery_minimum_voltage, ind);
        Serial.print("pack_setting_packet - battery_minimum_voltage : " + (String) battery_minimum_voltage + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID :
        buffer_append_float32_auto(packet, battery_maximum_voltage, ind);
        Serial.print("pack_setting_packet - battery_maximum_voltage : " + (String) battery_maximum_voltage + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID :
        buffer_append_uint8(packet, battery_maximum_distance, ind);
        Serial.print("pack_setting_packet - battery_maximum_distance : " + (String) battery_maximum_distance + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_SPEED_ADJUSTMENT_ID :
        buffer_append_int8(packet, speed_adjustment, ind);
        Serial.print("pack_setting_packet - speed_adjustment : " + (String) speed_adjustment + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID :
        buffer_append_uint8(packet, display_gps_speed_instead_of_escooter_speed, ind);
        Serial.print("pack_setting_packet - display_gps_speed_instead_of_escooter_speed : " + (String) display_gps_speed_instead_of_escooter_speed + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_AUX_RELAY_NAME_ID :
        buffer_append_uint8(packet, aux_relay_name, ind);
        Serial.print("pack_setting_packet - aux_relay_name : " + (String) aux_relay_name + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_DEFAULT_MODE_AT_STARTUP_ID :
        buffer_append_uint8(packet, default_mode_at_startup, ind);
        Serial.print("pack_setting_packet - default_mode_at_startup : " + (String) default_mode_at_startup + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID :
        buffer_append_uint8(packet, default_eco_mode_at_startup, ind);
        Serial.print("pack_setting_packet - default_eco_mode_at_startup : " + (String) default_eco_mode_at_startup + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_DEFAULT_ACCELERATION_ID :
        buffer_append_uint8(packet, default_acceleration, ind);
        Serial.print("pack_setting_packet - default_acceleration : " + (String) default_acceleration + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID :
        buffer_append_uint8(packet, default_electric_brake_at_startup, ind);
        Serial.print("pack_setting_packet - default_electric_brake_at_startup : " + (String) default_electric_brake_at_startup + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_PAS_ENABLED_ID :
        buffer_append_uint8(packet, pas_enabled, ind);
        Serial.print("pack_setting_packet - pas_enabled : " + (String) pas_enabled + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_ABS_ENABLED_ID :
        buffer_append_uint8(packet, abs_enabled, ind);
        Serial.print("pack_setting_packet - abs_enabled : " + (String) abs_enabled + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_DISPLAY_BRIGHTNESS_ID :
        buffer_append_uint8(packet, display_brightness, ind);
        Serial.print("pack_setting_packet - display_brightness : " + (String) display_brightness + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_TEMPERATURE_WARNING_ID :
        buffer_append_uint8(packet, temperature_warning, ind);
        Serial.print("pack_setting_packet - temperature_warning : " + (String) temperature_warning + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_HUMIDITY_WARNING_ID :
        buffer_append_uint8(packet, humidity_warning, ind);
        Serial.print("pack_setting_packet - humidity_warning : " + (String) humidity_warning + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_SPEED_LIMITER_AT_STARTUP_ID :
        buffer_append_uint8(packet, speed_limiter_at_startup, ind);
        Serial.print("pack_setting_packet - speed_limiter_at_startup : " + (String) speed_limiter_at_startup + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_SPEED_LIMITER_MAX_SPEED_ID :
        buffer_append_uint8(packet, speed_limiter_max_speed, ind);
        Serial.print("pack_setting_packet - speed_limiter_max_speed : " + (String) speed_limiter_max_speed + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_BLUETOOTH_LOCK_MODE_ID :
        buffer_append_uint8(packet, bluetooth_lock_mode, ind);
        Serial.print("pack_setting_packet - bluetooth_lock_mode : " + (String) bluetooth_lock_mode + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_PIN_CODE_ID :
        buffer_append_uint32(packet, pin_code, ind);
        Serial.print("pack_setting_packet - pin_code : " + (String) pin_code + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_BEACON_MAC_ADDRESS_ID :
/*
        char beacon_mac_address_part[17];
        memset(beacon_mac_address_part, 0, 17 );
        if (packetNumber == 0) {
            beacon_mac_address = "";
        }
        memcpy(beacon_mac_address_part, &packet[ind], length  - 4);
        beacon_mac_address =  beacon_mac_address + beacon_mac_address_part;
        set_beacon_mac_address( beacon_mac_address);
*/
        Serial.print("pack_setting_packet - beacon_mac_address : " + (String) beacon_mac_address + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_BEACON_RANGE_ID :
        buffer_append_int8(packet, beacon_range, ind);
        Serial.print("pack_setting_packet - beacon_range : " + (String) beacon_range + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID :
        buffer_append_int8(packet, original_display_speed_adjustment, ind);
        Serial.print("pack_setting_packet - original_display_speed_adjustment : " + (String) original_display_speed_adjustment + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_PROGRESSIVE_MODE_ID :
        buffer_append_uint8(packet, progressive_mode, ind);
        Serial.print("pack_setting_packet - progressive_mode : " + (String) progressive_mode + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_TYPE_ID :
        buffer_append_uint8(packet, type, ind);
        Serial.print("pack_setting_packet - type : " + (String) type + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_MIN_VALUE_ID :
        buffer_append_uint8(packet, min_value, ind);
        Serial.print("pack_setting_packet - min_value : " + (String) min_value + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_MAX_VALUE_ID :
        buffer_append_uint8(packet, max_value, ind);
        Serial.print("pack_setting_packet - max_value : " + (String) max_value + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_TIME_BETWEEN_MODE_SHIFT_ID :
        buffer_append_uint32(packet, time_between_mode_shift, ind);
        Serial.print("pack_setting_packet - time_between_mode_shift : " + (String) time_between_mode_shift + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID :
        buffer_append_uint8(packet, disabled_on_high_battery_voltage, ind);
        Serial.print("pack_setting_packet - disabled_on_high_battery_voltage : " + (String) disabled_on_high_battery_voltage + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_DISABLED_PERCENT_LIMIT_ID :
        buffer_append_uint8(packet, disabled_percent_limit, ind);
        Serial.print("pack_setting_packet - disabled_percent_limit : " + (String) disabled_percent_limit + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_THROTTLE_REGENERATION_ID :
        buffer_append_uint32(packet, throttle_regeneration, ind);
        Serial.print("pack_setting_packet - throttle_regeneration : " + (String) throttle_regeneration + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID :
        buffer_append_uint32(packet, input_min_voltage_in_millivolts, ind);
        Serial.print("pack_setting_packet - input_min_voltage_in_millivolts : " + (String) input_min_voltage_in_millivolts + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID :
        buffer_append_uint32(packet, input_max_voltage_in_millivolts, ind);
        Serial.print("pack_setting_packet - input_max_voltage_in_millivolts : " + (String) input_max_voltage_in_millivolts + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID :
        buffer_append_uint32(packet, output_min_voltage_in_millivolts, ind);
        Serial.print("pack_setting_packet - output_min_voltage_in_millivolts : " + (String) output_min_voltage_in_millivolts + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID :
        buffer_append_uint32(packet, output_max_voltage_in_millivolts, ind);
        Serial.print("pack_setting_packet - output_max_voltage_in_millivolts : " + (String) output_max_voltage_in_millivolts + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_OUTPUT_CURVE_ID :
        buffer_append_uint8(packet, output_curve, ind);
        Serial.print("pack_setting_packet - output_curve : " + (String) output_curve + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_B1_SHORT_PRESS_ACTION_ID :
        buffer_append_uint8(packet, b1_short_press_action, ind);
        Serial.print("pack_setting_packet - b1_short_press_action : " + (String) b1_short_press_action + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_B1_LONG_PRESS_ACTION_ID :
        buffer_append_uint8(packet, b1_long_press_action, ind);
        Serial.print("pack_setting_packet - b1_long_press_action : " + (String) b1_long_press_action + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_B2_SHORT_PRESS_ACTION_ID :
        buffer_append_uint8(packet, b2_short_press_action, ind);
        Serial.print("pack_setting_packet - b2_short_press_action : " + (String) b2_short_press_action + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_BUTTON_LONG_PRESS_DURATION_ID :
        buffer_append_uint8(packet, button_long_press_duration, ind);
        Serial.print("pack_setting_packet - button_long_press_duration : " + (String) button_long_press_duration + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_WIFI_NETWORK_NAME_SSID_ID :
/*
        char wifi_network_name_ssid_part[17];
        memset(wifi_network_name_ssid_part, 0, 17 );
        if (packetNumber == 0) {
            wifi_network_name_ssid = "";
        }
        memcpy(wifi_network_name_ssid_part, &packet[ind], length  - 4);
        wifi_network_name_ssid =  wifi_network_name_ssid + wifi_network_name_ssid_part;
        set_wifi_network_name_ssid( wifi_network_name_ssid);
*/
        Serial.print("pack_setting_packet - wifi_network_name_ssid : " + (String) wifi_network_name_ssid + " / ");
        buffer_display("", packet, *ind);
        break;
    case SETTINGS_WIFI_PASSWORD_ID :
/*
        char wifi_password_part[17];
        memset(wifi_password_part, 0, 17 );
        if (packetNumber == 0) {
            wifi_password = "";
        }
        memcpy(wifi_password_part, &packet[ind], length  - 4);
        wifi_password =  wifi_password + wifi_password_part;
        set_wifi_password( wifi_password);
*/
        Serial.print("pack_setting_packet - wifi_password : " + (String) wifi_password + " / ");
        buffer_display("", packet, *ind);
        break;
    default:
        Serial.println("pack_setting_packet : ID error");
        break;
    }
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

void Settings2::save_wheel_size(float value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_WHEEL_SIZE_ID_STR, wheel_size);
    prefs.end();
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

void Settings2::save_number_of_poles_pairs(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_NUMBER_OF_POLES_PAIRS_ID_STR, number_of_poles_pairs);
    prefs.end();
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

void Settings2::save_battery_minimum_voltage(float value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_BATTERY_MINIMUM_VOLTAGE_ID_STR, battery_minimum_voltage);
    prefs.end();
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

void Settings2::save_battery_maximum_voltage(float value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putFloat(SETTINGS_BATTERY_MAXIMUM_VOLTAGE_ID_STR, battery_maximum_voltage);
    prefs.end();
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

void Settings2::save_battery_maximum_distance(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BATTERY_MAXIMUM_DISTANCE_ID_STR, battery_maximum_distance);
    prefs.end();
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

void Settings2::save_speed_adjustment(int8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SPEED_ADJUSTMENT_ID_STR, speed_adjustment);
    prefs.end();
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

void Settings2::save_display_gps_speed_instead_of_escooter_speed(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISPLAY_GPS_SPEED_INSTEAD_OF_ESCOOTER_SPEED_ID_STR, display_gps_speed_instead_of_escooter_speed);
    prefs.end();
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

void Settings2::save_aux_relay_name(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_AUX_RELAY_NAME_ID_STR, aux_relay_name);
    prefs.end();
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

void Settings2::save_default_mode_at_startup(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_MODE_AT_STARTUP_ID_STR, default_mode_at_startup);
    prefs.end();
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

void Settings2::save_default_eco_mode_at_startup(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ECO_MODE_AT_STARTUP_ID_STR, default_eco_mode_at_startup);
    prefs.end();
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

void Settings2::save_default_acceleration(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ACCELERATION_ID_STR, default_acceleration);
    prefs.end();
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

void Settings2::save_default_electric_brake_at_startup(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DEFAULT_ELECTRIC_BRAKE_AT_STARTUP_ID_STR, default_electric_brake_at_startup);
    prefs.end();
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

void Settings2::save_pas_enabled(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_PAS_ENABLED_ID_STR, pas_enabled);
    prefs.end();
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

void Settings2::save_abs_enabled(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ABS_ENABLED_ID_STR, abs_enabled);
    prefs.end();
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

void Settings2::save_display_brightness(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISPLAY_BRIGHTNESS_ID_STR, display_brightness);
    prefs.end();
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

void Settings2::save_temperature_warning(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_TEMPERATURE_WARNING_ID_STR, temperature_warning);
    prefs.end();
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

void Settings2::save_humidity_warning(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_HUMIDITY_WARNING_ID_STR, humidity_warning);
    prefs.end();
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

void Settings2::save_speed_limiter_at_startup(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SPEED_LIMITER_AT_STARTUP_ID_STR, speed_limiter_at_startup);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_speed_limiter_max_speed(uint8_t value) {
    speed_limiter_max_speed = value;
}

uint8_t Settings2::get_speed_limiter_max_speed() {
    return speed_limiter_max_speed ;
}

void Settings2::display_speed_limiter_max_speed() {
    Serial.println("  speed_limiter_max_speed = " + (String) speed_limiter_max_speed);
}

void Settings2::save_speed_limiter_max_speed(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_SPEED_LIMITER_MAX_SPEED_ID_STR, speed_limiter_max_speed);
    prefs.end();
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

void Settings2::save_bluetooth_lock_mode(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BLUETOOTH_LOCK_MODE_ID_STR, bluetooth_lock_mode);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_pin_code(uint32_t value) {
    pin_code = value;
}

uint32_t Settings2::get_pin_code() {
    return pin_code ;
}

void Settings2::display_pin_code() {
    Serial.println("  pin_code = " + (String) pin_code);
}

void Settings2::save_pin_code(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_PIN_CODE_ID_STR, pin_code);
    prefs.end();
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

void Settings2::save_beacon_mac_address(String value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_BEACON_MAC_ADDRESS_ID_STR, beacon_mac_address);
    prefs.end();
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

void Settings2::save_beacon_range(int8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BEACON_RANGE_ID_STR, beacon_range);
    prefs.end();
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

void Settings2::save_original_display_speed_adjustment(int8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_ORIGINAL_DISPLAY_SPEED_ADJUSTMENT_ID_STR, original_display_speed_adjustment);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_progressive_mode(uint8_t value) {
    progressive_mode = value;
}

uint8_t Settings2::get_progressive_mode() {
    return progressive_mode ;
}

void Settings2::display_progressive_mode() {
    Serial.println("  progressive_mode = " + (String) progressive_mode);
}

void Settings2::save_progressive_mode(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_PROGRESSIVE_MODE_ID_STR, progressive_mode);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_type(uint8_t value) {
    type = value;
}

uint8_t Settings2::get_type() {
    return type ;
}

void Settings2::display_type() {
    Serial.println("  type = " + (String) type);
}

void Settings2::save_type(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_TYPE_ID_STR, type);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_min_value(uint8_t value) {
    min_value = value;
}

uint8_t Settings2::get_min_value() {
    return min_value ;
}

void Settings2::display_min_value() {
    Serial.println("  min_value = " + (String) min_value);
}

void Settings2::save_min_value(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_MIN_VALUE_ID_STR, min_value);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_max_value(uint8_t value) {
    max_value = value;
}

uint8_t Settings2::get_max_value() {
    return max_value ;
}

void Settings2::display_max_value() {
    Serial.println("  max_value = " + (String) max_value);
}

void Settings2::save_max_value(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_MAX_VALUE_ID_STR, max_value);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_time_between_mode_shift(uint32_t value) {
    time_between_mode_shift = value;
}

uint32_t Settings2::get_time_between_mode_shift() {
    return time_between_mode_shift ;
}

void Settings2::display_time_between_mode_shift() {
    Serial.println("  time_between_mode_shift = " + (String) time_between_mode_shift);
}

void Settings2::save_time_between_mode_shift(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_TIME_BETWEEN_MODE_SHIFT_ID_STR, time_between_mode_shift);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_disabled_on_high_battery_voltage(uint8_t value) {
    disabled_on_high_battery_voltage = value;
}

uint8_t Settings2::get_disabled_on_high_battery_voltage() {
    return disabled_on_high_battery_voltage ;
}

void Settings2::display_disabled_on_high_battery_voltage() {
    Serial.println("  disabled_on_high_battery_voltage = " + (String) disabled_on_high_battery_voltage);
}

void Settings2::save_disabled_on_high_battery_voltage(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISABLED_ON_HIGH_BATTERY_VOLTAGE_ID_STR, disabled_on_high_battery_voltage);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_disabled_percent_limit(uint8_t value) {
    disabled_percent_limit = value;
}

uint8_t Settings2::get_disabled_percent_limit() {
    return disabled_percent_limit ;
}

void Settings2::display_disabled_percent_limit() {
    Serial.println("  disabled_percent_limit = " + (String) disabled_percent_limit);
}

void Settings2::save_disabled_percent_limit(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_DISABLED_PERCENT_LIMIT_ID_STR, disabled_percent_limit);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_throttle_regeneration(uint32_t value) {
    throttle_regeneration = value;
}

uint32_t Settings2::get_throttle_regeneration() {
    return throttle_regeneration ;
}

void Settings2::display_throttle_regeneration() {
    Serial.println("  throttle_regeneration = " + (String) throttle_regeneration);
}

void Settings2::save_throttle_regeneration(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_THROTTLE_REGENERATION_ID_STR, throttle_regeneration);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_input_min_voltage_in_millivolts(uint32_t value) {
    input_min_voltage_in_millivolts = value;
}

uint32_t Settings2::get_input_min_voltage_in_millivolts() {
    return input_min_voltage_in_millivolts ;
}

void Settings2::display_input_min_voltage_in_millivolts() {
    Serial.println("  input_min_voltage_in_millivolts = " + (String) input_min_voltage_in_millivolts);
}

void Settings2::save_input_min_voltage_in_millivolts(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_INPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, input_min_voltage_in_millivolts);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_input_max_voltage_in_millivolts(uint32_t value) {
    input_max_voltage_in_millivolts = value;
}

uint32_t Settings2::get_input_max_voltage_in_millivolts() {
    return input_max_voltage_in_millivolts ;
}

void Settings2::display_input_max_voltage_in_millivolts() {
    Serial.println("  input_max_voltage_in_millivolts = " + (String) input_max_voltage_in_millivolts);
}

void Settings2::save_input_max_voltage_in_millivolts(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_INPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, input_max_voltage_in_millivolts);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_output_min_voltage_in_millivolts(uint32_t value) {
    output_min_voltage_in_millivolts = value;
}

uint32_t Settings2::get_output_min_voltage_in_millivolts() {
    return output_min_voltage_in_millivolts ;
}

void Settings2::display_output_min_voltage_in_millivolts() {
    Serial.println("  output_min_voltage_in_millivolts = " + (String) output_min_voltage_in_millivolts);
}

void Settings2::save_output_min_voltage_in_millivolts(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_OUTPUT_MIN_VOLTAGE_IN_MILLIVOLTS_ID_STR, output_min_voltage_in_millivolts);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_output_max_voltage_in_millivolts(uint32_t value) {
    output_max_voltage_in_millivolts = value;
}

uint32_t Settings2::get_output_max_voltage_in_millivolts() {
    return output_max_voltage_in_millivolts ;
}

void Settings2::display_output_max_voltage_in_millivolts() {
    Serial.println("  output_max_voltage_in_millivolts = " + (String) output_max_voltage_in_millivolts);
}

void Settings2::save_output_max_voltage_in_millivolts(uint32_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_OUTPUT_MAX_VOLTAGE_IN_MILLIVOLTS_ID_STR, output_max_voltage_in_millivolts);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_output_curve(uint8_t value) {
    output_curve = value;
}

uint8_t Settings2::get_output_curve() {
    return output_curve ;
}

void Settings2::display_output_curve() {
    Serial.println("  output_curve = " + (String) output_curve);
}

void Settings2::save_output_curve(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_OUTPUT_CURVE_ID_STR, output_curve);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_b1_short_press_action(uint8_t value) {
    b1_short_press_action = value;
}

uint8_t Settings2::get_b1_short_press_action() {
    return b1_short_press_action ;
}

void Settings2::display_b1_short_press_action() {
    Serial.println("  b1_short_press_action = " + (String) b1_short_press_action);
}

void Settings2::save_b1_short_press_action(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_B1_SHORT_PRESS_ACTION_ID_STR, b1_short_press_action);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_b1_long_press_action(uint8_t value) {
    b1_long_press_action = value;
}

uint8_t Settings2::get_b1_long_press_action() {
    return b1_long_press_action ;
}

void Settings2::display_b1_long_press_action() {
    Serial.println("  b1_long_press_action = " + (String) b1_long_press_action);
}

void Settings2::save_b1_long_press_action(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_B1_LONG_PRESS_ACTION_ID_STR, b1_long_press_action);
    prefs.end();
}
                

/*-------------------------------------------------------*/

void Settings2::set_b2_short_press_action(uint8_t value) {
    b2_short_press_action = value;
}

uint8_t Settings2::get_b2_short_press_action() {
    return b2_short_press_action ;
}

void Settings2::display_b2_short_press_action() {
    Serial.println("  b2_short_press_action = " + (String) b2_short_press_action);
}

void Settings2::save_b2_short_press_action(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_B2_SHORT_PRESS_ACTION_ID_STR, b2_short_press_action);
    prefs.end();
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

void Settings2::save_button_long_press_duration(uint8_t value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putInt(SETTINGS_BUTTON_LONG_PRESS_DURATION_ID_STR, button_long_press_duration);
    prefs.end();
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

void Settings2::save_wifi_network_name_ssid(String value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_WIFI_NETWORK_NAME_SSID_ID_STR, wifi_network_name_ssid);
    prefs.end();
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

void Settings2::save_wifi_password(String value) {
    prefs.begin(SETTINGS_STORAGE, false);
    prefs.putString(SETTINGS_WIFI_PASSWORD_ID_STR, wifi_password);
    prefs.end();
}


