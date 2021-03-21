
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

#include <AutoConnect.h>


//-----------------------
// variables and lists
//-----------------------

/* elements */
ACText(ACE_SETTINGS_Escooter_characteristics, "<h2>Escooter characteristics</h2>", "color:#303F9F;padding:10px;");
ACInput(ACE_SETTINGS_Wheel_size, "10.0", "Wheel size", "^[0-9]+[.]?[0-9]*([eE][-+]?[0-9]+)?$", "edit_text_number_float", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Number_of_poles_pairs, "15", "Number of poles pairs", "^[0-9]+$", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Battery_minimum_voltage, "42.0", "Battery minimum voltage", "^[0-9]+[.]?[0-9]*([eE][-+]?[0-9]+)?$", "edit_text_number_float", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Battery_maximum_voltage, "58.8", "Battery maximum voltage", "^[0-9]+[.]?[0-9]*([eE][-+]?[0-9]+)?$", "edit_text_number_float", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Battery_maximum_distance, "40", "Battery maximum distance", "^[0-9]+$", "edit_text_number_integer", AC_Tag_BR, AC_Input_Text);
ACText(ACE_SETTINGS_General, "<h2>General</h2>", "color:#303F9F;padding:10px;");
ACCheckbox(ACE_SETTINGS_Speed_limiter_at_startup, "ACE_SETTINGS_Speed_limiter_at_startup", "Speed limiter at startup", false, AC_Infront);
ACInput(ACE_SETTINGS_Speed_limiter_max_speed, "37", "Speed limiter max speed", "", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACE_SETTINGS_Bluetooth_lock_mode, {"None","Smartphone connected","Smartphone connected or beacon visible","Beacon visible"}, "Bluetooth lock mode", 0);
ACInput(ACE_SETTINGS_Ble_pin_code, "147258", "PIN code", "^[0-9]{6,6}$", "edit_text_number_integer", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Ble_beacon_mac_address, "aa:bb:cc:dd:ee:ff", "Beacon Mac Address", "^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$", "edit_text_string", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Ble_beacon_range, "-80", "Beacon range", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Original_display_speed_adjustment, "0", "Speed adjustment", "^[-]?[0-9]+$", "edit_text_number_integer_signed", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Temperature_warning, "70", "Temperature warning", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Humidity_warning, "80", "Humidity warning", "", "", AC_Tag_BR, AC_Input_Text);
ACText(ACE_SETTINGS_Smartphone_display, "<h2>Smartphone display</h2>", "color:#303F9F;padding:10px;");
ACInput(ACE_SETTINGS_Smartdisplay_speed_adjustment, "0", "Speed adjustment", "^[-]?[0-9]+$", "edit_text_number_integer_signed", AC_Tag_BR, AC_Input_Text);
ACSelect(ACE_SETTINGS_Aux_relay_name, {"AUX","DUAL","LIGHT","HORN","RELAY"}, "Aux relay name", 0);
ACCheckbox(ACE_SETTINGS_Display_gps_speed_instead_of_escooter_speed, "ACE_SETTINGS_Display_gps_speed_instead_of_escooter_speed", "Display GPS speed ", false, AC_Infront);
ACText(ACE_SETTINGS_Smartdisplay, "<h2>SmartDisplay</h2>", "color:#303F9F;padding:10px;");
ACInput(ACE_SETTINGS_Default_mode_at_startup, "3", "Startup mode", "", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACE_SETTINGS_Default_eco_mode_at_startup, {"Max","Medium","None"}, "Startup eco mode", 2);
ACSelect(ACE_SETTINGS_Default_acceleration, {"Maximum","Very fast","Fast","Medium","Slow","Very slow"}, "Startup acceleration", 0);
ACInput(ACE_SETTINGS_Default_electric_brake_at_startup, "3", "Startup  ebrake", "", "", AC_Tag_BR, AC_Input_Text);
ACCheckbox(ACE_SETTINGS_Pas_enabled, "ACE_SETTINGS_Pas_enabled", "PAS enabled", false, AC_Infront);
ACCheckbox(ACE_SETTINGS_Abs_enabled, "ACE_SETTINGS_Abs_enabled", "ABS enabled", false, AC_Infront);
ACInput(ACE_SETTINGS_Display_brightness, "100", "Display brightness", "", "", AC_Tag_BR, AC_Input_Text);
ACCheckbox(ACE_SETTINGS_Display_splash_screen, "ACE_SETTINGS_Display_splash_screen", "Display splash screen", false, AC_Infront);
ACText(ACE_SETTINGS_Electric_brake, "<h2>Electric brake</h2>", "color:#303F9F;padding:10px;");
ACCheckbox(ACE_SETTINGS_Ebrake_progressive_mode, "ACE_SETTINGS_Ebrake_progressive_mode", "Progressive mode", false, AC_Infront);
ACSelect(ACE_SETTINGS_Ebrake_smart_brake_type, {"Controller","Smart - Digital brake lever","Smart - Analog brake lever","Smart - Digital brake + throttle (exp)"}, "Type", 0);
ACInput(ACE_SETTINGS_Ebrake_min_power_value, "1", "Min value", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Ebrake_max_power_value, "5", "Max value", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Ebrake_time_between_mode_shift, "500", "Time between mode shift", "", "", AC_Tag_BR, AC_Input_Text);
ACCheckbox(ACE_SETTINGS_Ebrake_disabled_on_high_battery_voltage, "ACE_SETTINGS_Ebrake_disabled_on_high_battery_voltage", "Disabled with bat, HV", false, AC_Infront);
ACInput(ACE_SETTINGS_Ebrake_disabled_percent_limit, "100", "Disabled with bat. HV", "", "", AC_Tag_BR, AC_Input_Text);
ACText(ACE_SETTINGS_Throttle, "<h2>Throttle</h2>", "color:#303F9F;padding:10px;");
ACCheckbox(ACE_SETTINGS_Throttle_regeneration, "ACE_SETTINGS_Throttle_regeneration", "Throttle regeneration", false, AC_Infront);
ACInput(ACE_SETTINGS_Throttle_input_min_voltage, "650", "Input min voltage", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Throttle_input_max_voltage, "3950", "Input max voltage", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Throttle_output_min_voltage, "800", "Output min voltage", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Throttle_output_max_voltage, "3350", "Output max voltage", "", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACE_SETTINGS_Throttle_output_curve, {"Linear","Exponential 1","Exponential 2","Exponential 3","Exponential 4"}, "Output curve", 2);
ACText(ACE_SETTINGS_Escooter_buttons, "<h2>Escooter buttons</h2>", "color:#303F9F;padding:10px;");
ACSelect(ACE_SETTINGS_Button_1_short_press_action, {"None","Speed limit ON/OFF","Aux ON/OFF","Mode switch 1/2/3","Mode switch 2/3","Eco switch NONE/MED/MAX","Eco switch NONE/MED"}, "B1 short press ", 0);
ACSelect(ACE_SETTINGS_Button_1_long_press_action, {"None","Speed limitation ON/OFF","Aux ON/OFF","Mode switch 1/2/3","Mode switch 2/3","Eco switch NONE/MED/MAX","Eco switch NONE/MED","Anti-theft manual lock ON","Nitro boost continuous"}, "B1 long press ", 0);
ACSelect(ACE_SETTINGS_Button_2_short_press_action, {"None","Speed limitation ON/OFF","Aux ON/OFF","Mode switch 1/2/3","Mode switch 2/3","Eco switch NONE/MED/MAX","Eco switch NONE/MED"}, "B2 short press ", 0);
ACInput(ACE_SETTINGS_Button_long_press_duration, "3", "Button long press duration", "", "", AC_Tag_BR, AC_Input_Text);
ACText(ACE_SETTINGS_Ota_update, "<h2>OTA update</h2>", "color:#303F9F;padding:10px;");
ACInput(ACE_SETTINGS_Wifi_ssid, "", "Wifi network name", "^(.{0,32})$", "edit_text_string", AC_Tag_BR, AC_Input_Text);
ACInput(ACE_SETTINGS_Wifi_password, "", "Wifi password", "^(.{0,32})$", "edit_text_password", AC_Tag_BR, AC_Input_Password);


/* menus */
AutoConnectAux settingsPageAux("/settingspage", "SmartElec settings", true, {
    ACE_Style1,
    ACE_Style2,
    ACE_Style3,
    ACE_Style4,
    ACE_SETTINGS_Escooter_characteristics,
    ACE_SETTINGS_Wheel_size,
    ACE_SETTINGS_Number_of_poles_pairs,
    ACE_SETTINGS_Battery_minimum_voltage,
    ACE_SETTINGS_Battery_maximum_voltage,
    ACE_SETTINGS_Battery_maximum_distance,
    ACE_SETTINGS_General,
    ACE_SETTINGS_Speed_limiter_at_startup,
    ACE_SETTINGS_Speed_limiter_max_speed,
    ACE_SETTINGS_Bluetooth_lock_mode,
    ACE_SETTINGS_Ble_pin_code,
    ACE_SETTINGS_Ble_beacon_mac_address,
    ACE_SETTINGS_Ble_beacon_range,
    ACE_SETTINGS_Original_display_speed_adjustment,
    ACE_SETTINGS_Temperature_warning,
    ACE_SETTINGS_Humidity_warning,
    ACE_SETTINGS_Smartphone_display,
    ACE_SETTINGS_Smartdisplay_speed_adjustment,
    ACE_SETTINGS_Aux_relay_name,
    ACE_SETTINGS_Display_gps_speed_instead_of_escooter_speed,
    ACE_SETTINGS_Smartdisplay,
    ACE_SETTINGS_Default_mode_at_startup,
    ACE_SETTINGS_Default_eco_mode_at_startup,
    ACE_SETTINGS_Default_acceleration,
    ACE_SETTINGS_Default_electric_brake_at_startup,
    ACE_SETTINGS_Pas_enabled,
    ACE_SETTINGS_Abs_enabled,
    ACE_SETTINGS_Display_brightness,
    ACE_SETTINGS_Display_splash_screen,
    ACE_SETTINGS_Electric_brake,
    ACE_SETTINGS_Ebrake_progressive_mode,
    ACE_SETTINGS_Ebrake_smart_brake_type,
    ACE_SETTINGS_Ebrake_min_power_value,
    ACE_SETTINGS_Ebrake_max_power_value,
    ACE_SETTINGS_Ebrake_time_between_mode_shift,
    ACE_SETTINGS_Ebrake_disabled_on_high_battery_voltage,
    ACE_SETTINGS_Ebrake_disabled_percent_limit,
    ACE_SETTINGS_Throttle,
    ACE_SETTINGS_Throttle_regeneration,
    ACE_SETTINGS_Throttle_input_min_voltage,
    ACE_SETTINGS_Throttle_input_max_voltage,
    ACE_SETTINGS_Throttle_output_min_voltage,
    ACE_SETTINGS_Throttle_output_max_voltage,
    ACE_SETTINGS_Throttle_output_curve,
    ACE_SETTINGS_Escooter_buttons,
    ACE_SETTINGS_Button_1_short_press_action,
    ACE_SETTINGS_Button_1_long_press_action,
    ACE_SETTINGS_Button_2_short_press_action,
    ACE_SETTINGS_Button_long_press_duration,
    ACE_SETTINGS_Ota_update,
    ACE_SETTINGS_Wifi_ssid,
    ACE_SETTINGS_Wifi_password,
    ACE_SETTINGS_Discard, 
    ACE_SETTINGS_Save,
    ACE_SETTINGS_Js1 });        


/* save */
void saveConfig(AutoConnectAux &aux)
{
    WifiSettingsPortal_settings->set_Wheel_size((settingsPageAux["ACE_SETTINGS_Wheel_size"].as<AutoConnectInput>()).value.toFloat());
    WifiSettingsPortal_settings->set_Number_of_poles_pairs((settingsPageAux["ACE_SETTINGS_Number_of_poles_pairs"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Battery_minimum_voltage((settingsPageAux["ACE_SETTINGS_Battery_minimum_voltage"].as<AutoConnectInput>()).value.toFloat());
    WifiSettingsPortal_settings->set_Battery_maximum_voltage((settingsPageAux["ACE_SETTINGS_Battery_maximum_voltage"].as<AutoConnectInput>()).value.toFloat());
    WifiSettingsPortal_settings->set_Battery_maximum_distance((settingsPageAux["ACE_SETTINGS_Battery_maximum_distance"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Speed_limiter_at_startup((settingsPageAux["ACE_SETTINGS_Speed_limiter_at_startup"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("Speed_limiter_at_startup = " + (String) (settingsPageAux["ACE_SETTINGS_Speed_limiter_at_startup"].as<AutoConnectCheckbox>()).checked);
    WifiSettingsPortal_settings->set_Speed_limiter_max_speed((settingsPageAux["ACE_SETTINGS_Speed_limiter_max_speed"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Bluetooth_lock_mode((settingsPageAux["ACE_SETTINGS_Bluetooth_lock_mode"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Ble_pin_code((settingsPageAux["ACE_SETTINGS_Ble_pin_code"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Ble_beacon_mac_address((settingsPageAux["ACE_SETTINGS_Ble_beacon_mac_address"].as<AutoConnectInput>()).value);
    WifiSettingsPortal_settings->set_Ble_beacon_range((settingsPageAux["ACE_SETTINGS_Ble_beacon_range"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Original_display_speed_adjustment((settingsPageAux["ACE_SETTINGS_Original_display_speed_adjustment"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Temperature_warning((settingsPageAux["ACE_SETTINGS_Temperature_warning"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Humidity_warning((settingsPageAux["ACE_SETTINGS_Humidity_warning"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Smartdisplay_speed_adjustment((settingsPageAux["ACE_SETTINGS_Smartdisplay_speed_adjustment"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Aux_relay_name((settingsPageAux["ACE_SETTINGS_Aux_relay_name"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Display_gps_speed_instead_of_escooter_speed((settingsPageAux["ACE_SETTINGS_Display_gps_speed_instead_of_escooter_speed"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("Display_gps_speed_instead_of_escooter_speed = " + (String) (settingsPageAux["ACE_SETTINGS_Display_gps_speed_instead_of_escooter_speed"].as<AutoConnectCheckbox>()).checked);
    WifiSettingsPortal_settings->set_Default_mode_at_startup((settingsPageAux["ACE_SETTINGS_Default_mode_at_startup"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Default_eco_mode_at_startup((settingsPageAux["ACE_SETTINGS_Default_eco_mode_at_startup"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Default_acceleration((settingsPageAux["ACE_SETTINGS_Default_acceleration"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Default_electric_brake_at_startup((settingsPageAux["ACE_SETTINGS_Default_electric_brake_at_startup"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Pas_enabled((settingsPageAux["ACE_SETTINGS_Pas_enabled"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("Pas_enabled = " + (String) (settingsPageAux["ACE_SETTINGS_Pas_enabled"].as<AutoConnectCheckbox>()).checked);
    WifiSettingsPortal_settings->set_Abs_enabled((settingsPageAux["ACE_SETTINGS_Abs_enabled"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("Abs_enabled = " + (String) (settingsPageAux["ACE_SETTINGS_Abs_enabled"].as<AutoConnectCheckbox>()).checked);
    WifiSettingsPortal_settings->set_Display_brightness((settingsPageAux["ACE_SETTINGS_Display_brightness"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Display_splash_screen((settingsPageAux["ACE_SETTINGS_Display_splash_screen"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("Display_splash_screen = " + (String) (settingsPageAux["ACE_SETTINGS_Display_splash_screen"].as<AutoConnectCheckbox>()).checked);
    WifiSettingsPortal_settings->set_Ebrake_progressive_mode((settingsPageAux["ACE_SETTINGS_Ebrake_progressive_mode"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("Ebrake_progressive_mode = " + (String) (settingsPageAux["ACE_SETTINGS_Ebrake_progressive_mode"].as<AutoConnectCheckbox>()).checked);
    WifiSettingsPortal_settings->set_Ebrake_smart_brake_type((settingsPageAux["ACE_SETTINGS_Ebrake_smart_brake_type"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Ebrake_min_power_value((settingsPageAux["ACE_SETTINGS_Ebrake_min_power_value"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Ebrake_max_power_value((settingsPageAux["ACE_SETTINGS_Ebrake_max_power_value"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Ebrake_time_between_mode_shift((settingsPageAux["ACE_SETTINGS_Ebrake_time_between_mode_shift"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Ebrake_disabled_on_high_battery_voltage((settingsPageAux["ACE_SETTINGS_Ebrake_disabled_on_high_battery_voltage"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("Ebrake_disabled_on_high_battery_voltage = " + (String) (settingsPageAux["ACE_SETTINGS_Ebrake_disabled_on_high_battery_voltage"].as<AutoConnectCheckbox>()).checked);
    WifiSettingsPortal_settings->set_Ebrake_disabled_percent_limit((settingsPageAux["ACE_SETTINGS_Ebrake_disabled_percent_limit"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_regeneration((settingsPageAux["ACE_SETTINGS_Throttle_regeneration"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    Serial.println("Throttle_regeneration = " + (String) (settingsPageAux["ACE_SETTINGS_Throttle_regeneration"].as<AutoConnectCheckbox>()).checked);
    WifiSettingsPortal_settings->set_Throttle_input_min_voltage((settingsPageAux["ACE_SETTINGS_Throttle_input_min_voltage"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_input_max_voltage((settingsPageAux["ACE_SETTINGS_Throttle_input_max_voltage"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_output_min_voltage((settingsPageAux["ACE_SETTINGS_Throttle_output_min_voltage"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_output_max_voltage((settingsPageAux["ACE_SETTINGS_Throttle_output_max_voltage"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_output_curve((settingsPageAux["ACE_SETTINGS_Throttle_output_curve"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Button_1_short_press_action((settingsPageAux["ACE_SETTINGS_Button_1_short_press_action"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Button_1_long_press_action((settingsPageAux["ACE_SETTINGS_Button_1_long_press_action"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Button_2_short_press_action((settingsPageAux["ACE_SETTINGS_Button_2_short_press_action"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Button_long_press_duration((settingsPageAux["ACE_SETTINGS_Button_long_press_duration"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Wifi_ssid((settingsPageAux["ACE_SETTINGS_Wifi_ssid"].as<AutoConnectInput>()).value);
    WifiSettingsPortal_settings->set_Wifi_password((settingsPageAux["ACE_SETTINGS_Wifi_password"].as<AutoConnectInput>()).value);
    WifiSettingsPortal_settings->save();
    
}

/* load */
void loadConfig(AutoConnectAux &aux)
{
    aux.setElementValue("ACE_SETTINGS_Wheel_size", (String)WifiSettingsPortal_settings->get_Wheel_size());
    Serial.println("get_Wheel_size = " + (String)WifiSettingsPortal_settings->get_Wheel_size());
    aux.setElementValue("ACE_SETTINGS_Number_of_poles_pairs", (String)WifiSettingsPortal_settings->get_Number_of_poles_pairs());
    aux.setElementValue("ACE_SETTINGS_Battery_minimum_voltage", (String)WifiSettingsPortal_settings->get_Battery_minimum_voltage());
    aux.setElementValue("ACE_SETTINGS_Battery_maximum_voltage", (String)WifiSettingsPortal_settings->get_Battery_maximum_voltage());
    aux.setElementValue("ACE_SETTINGS_Battery_maximum_distance", (String)WifiSettingsPortal_settings->get_Battery_maximum_distance());
    aux.setElementValue("ACE_SETTINGS_Speed_limiter_at_startup", WifiSettingsPortal_settings->get_Speed_limiter_at_startup() ? "checked" : "");
    aux.setElementValue("ACE_SETTINGS_Speed_limiter_max_speed", (String)WifiSettingsPortal_settings->get_Speed_limiter_max_speed());
    uint8_t val_Bluetooth_lock_mode = WifiSettingsPortal_settings->get_Bluetooth_lock_mode();
    if (val_Bluetooth_lock_mode == 0)
        aux.setElementValue("ACE_SETTINGS_Bluetooth_lock_mode", "None");
    if (val_Bluetooth_lock_mode == 1)
        aux.setElementValue("ACE_SETTINGS_Bluetooth_lock_mode", "Smartphone connected");
    if (val_Bluetooth_lock_mode == 2)
        aux.setElementValue("ACE_SETTINGS_Bluetooth_lock_mode", "Smartphone connected or beacon visible");
    if (val_Bluetooth_lock_mode == 3)
        aux.setElementValue("ACE_SETTINGS_Bluetooth_lock_mode", "Beacon visible");
    aux.setElementValue("ACE_SETTINGS_Ble_pin_code", (String)WifiSettingsPortal_settings->get_Ble_pin_code());
    aux.setElementValue("ACE_SETTINGS_Ble_beacon_mac_address", (String)WifiSettingsPortal_settings->get_Ble_beacon_mac_address());
    aux.setElementValue("ACE_SETTINGS_Ble_beacon_range", (String)WifiSettingsPortal_settings->get_Ble_beacon_range());
    aux.setElementValue("ACE_SETTINGS_Original_display_speed_adjustment", (String)WifiSettingsPortal_settings->get_Original_display_speed_adjustment());
    aux.setElementValue("ACE_SETTINGS_Temperature_warning", (String)WifiSettingsPortal_settings->get_Temperature_warning());
    aux.setElementValue("ACE_SETTINGS_Humidity_warning", (String)WifiSettingsPortal_settings->get_Humidity_warning());
    aux.setElementValue("ACE_SETTINGS_Smartdisplay_speed_adjustment", (String)WifiSettingsPortal_settings->get_Smartdisplay_speed_adjustment());
    uint8_t val_Aux_relay_name = WifiSettingsPortal_settings->get_Aux_relay_name();
    if (val_Aux_relay_name == 0)
        aux.setElementValue("ACE_SETTINGS_Aux_relay_name", "AUX");
    if (val_Aux_relay_name == 1)
        aux.setElementValue("ACE_SETTINGS_Aux_relay_name", "DUAL");
    if (val_Aux_relay_name == 2)
        aux.setElementValue("ACE_SETTINGS_Aux_relay_name", "LIGHT");
    if (val_Aux_relay_name == 3)
        aux.setElementValue("ACE_SETTINGS_Aux_relay_name", "HORN");
    if (val_Aux_relay_name == 4)
        aux.setElementValue("ACE_SETTINGS_Aux_relay_name", "RELAY");
    aux.setElementValue("ACE_SETTINGS_Display_gps_speed_instead_of_escooter_speed", WifiSettingsPortal_settings->get_Display_gps_speed_instead_of_escooter_speed() ? "checked" : "");
    aux.setElementValue("ACE_SETTINGS_Default_mode_at_startup", (String)WifiSettingsPortal_settings->get_Default_mode_at_startup());
    uint8_t val_Default_eco_mode_at_startup = WifiSettingsPortal_settings->get_Default_eco_mode_at_startup();
    if (val_Default_eco_mode_at_startup == 0)
        aux.setElementValue("ACE_SETTINGS_Default_eco_mode_at_startup", "Max");
    if (val_Default_eco_mode_at_startup == 1)
        aux.setElementValue("ACE_SETTINGS_Default_eco_mode_at_startup", "Medium");
    if (val_Default_eco_mode_at_startup == 2)
        aux.setElementValue("ACE_SETTINGS_Default_eco_mode_at_startup", "None");
    uint8_t val_Default_acceleration = WifiSettingsPortal_settings->get_Default_acceleration();
    if (val_Default_acceleration == 0)
        aux.setElementValue("ACE_SETTINGS_Default_acceleration", "Maximum");
    if (val_Default_acceleration == 1)
        aux.setElementValue("ACE_SETTINGS_Default_acceleration", "Very fast");
    if (val_Default_acceleration == 2)
        aux.setElementValue("ACE_SETTINGS_Default_acceleration", "Fast");
    if (val_Default_acceleration == 3)
        aux.setElementValue("ACE_SETTINGS_Default_acceleration", "Medium");
    if (val_Default_acceleration == 4)
        aux.setElementValue("ACE_SETTINGS_Default_acceleration", "Slow");
    if (val_Default_acceleration == 5)
        aux.setElementValue("ACE_SETTINGS_Default_acceleration", "Very slow");
    aux.setElementValue("ACE_SETTINGS_Default_electric_brake_at_startup", (String)WifiSettingsPortal_settings->get_Default_electric_brake_at_startup());
    aux.setElementValue("ACE_SETTINGS_Pas_enabled", WifiSettingsPortal_settings->get_Pas_enabled() ? "checked" : "");
    aux.setElementValue("ACE_SETTINGS_Abs_enabled", WifiSettingsPortal_settings->get_Abs_enabled() ? "checked" : "");
    aux.setElementValue("ACE_SETTINGS_Display_brightness", (String)WifiSettingsPortal_settings->get_Display_brightness());
    aux.setElementValue("ACE_SETTINGS_Display_splash_screen", WifiSettingsPortal_settings->get_Display_splash_screen() ? "checked" : "");
    aux.setElementValue("ACE_SETTINGS_Ebrake_progressive_mode", WifiSettingsPortal_settings->get_Ebrake_progressive_mode() ? "checked" : "");
    uint8_t val_Ebrake_smart_brake_type = WifiSettingsPortal_settings->get_Ebrake_smart_brake_type();
    if (val_Ebrake_smart_brake_type == 0)
        aux.setElementValue("ACE_SETTINGS_Ebrake_smart_brake_type", "Controller");
    if (val_Ebrake_smart_brake_type == 1)
        aux.setElementValue("ACE_SETTINGS_Ebrake_smart_brake_type", "Smart - Digital brake lever");
    if (val_Ebrake_smart_brake_type == 2)
        aux.setElementValue("ACE_SETTINGS_Ebrake_smart_brake_type", "Smart - Analog brake lever");
    if (val_Ebrake_smart_brake_type == 3)
        aux.setElementValue("ACE_SETTINGS_Ebrake_smart_brake_type", "Smart - Digital brake + throttle (exp)");
    aux.setElementValue("ACE_SETTINGS_Ebrake_min_power_value", (String)WifiSettingsPortal_settings->get_Ebrake_min_power_value());
    aux.setElementValue("ACE_SETTINGS_Ebrake_max_power_value", (String)WifiSettingsPortal_settings->get_Ebrake_max_power_value());
    aux.setElementValue("ACE_SETTINGS_Ebrake_time_between_mode_shift", (String)WifiSettingsPortal_settings->get_Ebrake_time_between_mode_shift());
    aux.setElementValue("ACE_SETTINGS_Ebrake_disabled_on_high_battery_voltage", WifiSettingsPortal_settings->get_Ebrake_disabled_on_high_battery_voltage() ? "checked" : "");
    aux.setElementValue("ACE_SETTINGS_Ebrake_disabled_percent_limit", (String)WifiSettingsPortal_settings->get_Ebrake_disabled_percent_limit());
    aux.setElementValue("ACE_SETTINGS_Throttle_regeneration", WifiSettingsPortal_settings->get_Throttle_regeneration() ? "checked" : "");
    aux.setElementValue("ACE_SETTINGS_Throttle_input_min_voltage", (String)WifiSettingsPortal_settings->get_Throttle_input_min_voltage());
    aux.setElementValue("ACE_SETTINGS_Throttle_input_max_voltage", (String)WifiSettingsPortal_settings->get_Throttle_input_max_voltage());
    aux.setElementValue("ACE_SETTINGS_Throttle_output_min_voltage", (String)WifiSettingsPortal_settings->get_Throttle_output_min_voltage());
    aux.setElementValue("ACE_SETTINGS_Throttle_output_max_voltage", (String)WifiSettingsPortal_settings->get_Throttle_output_max_voltage());
    uint8_t val_Throttle_output_curve = WifiSettingsPortal_settings->get_Throttle_output_curve();
    if (val_Throttle_output_curve == 0)
        aux.setElementValue("ACE_SETTINGS_Throttle_output_curve", "Linear");
    if (val_Throttle_output_curve == 1)
        aux.setElementValue("ACE_SETTINGS_Throttle_output_curve", "Exponential 1");
    if (val_Throttle_output_curve == 2)
        aux.setElementValue("ACE_SETTINGS_Throttle_output_curve", "Exponential 2");
    if (val_Throttle_output_curve == 3)
        aux.setElementValue("ACE_SETTINGS_Throttle_output_curve", "Exponential 3");
    if (val_Throttle_output_curve == 4)
        aux.setElementValue("ACE_SETTINGS_Throttle_output_curve", "Exponential 4");
    uint8_t val_Button_1_short_press_action = WifiSettingsPortal_settings->get_Button_1_short_press_action();
    if (val_Button_1_short_press_action == 0)
        aux.setElementValue("ACE_SETTINGS_Button_1_short_press_action", "None");
    if (val_Button_1_short_press_action == 1)
        aux.setElementValue("ACE_SETTINGS_Button_1_short_press_action", "Speed limit ON/OFF");
    if (val_Button_1_short_press_action == 2)
        aux.setElementValue("ACE_SETTINGS_Button_1_short_press_action", "Aux ON/OFF");
    if (val_Button_1_short_press_action == 3)
        aux.setElementValue("ACE_SETTINGS_Button_1_short_press_action", "Mode switch 1/2/3");
    if (val_Button_1_short_press_action == 4)
        aux.setElementValue("ACE_SETTINGS_Button_1_short_press_action", "Mode switch 2/3");
    if (val_Button_1_short_press_action == 5)
        aux.setElementValue("ACE_SETTINGS_Button_1_short_press_action", "Eco switch NONE/MED/MAX");
    if (val_Button_1_short_press_action == 6)
        aux.setElementValue("ACE_SETTINGS_Button_1_short_press_action", "Eco switch NONE/MED");
    uint8_t val_Button_1_long_press_action = WifiSettingsPortal_settings->get_Button_1_long_press_action();
    if (val_Button_1_long_press_action == 0)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "None");
    if (val_Button_1_long_press_action == 1)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "Speed limitation ON/OFF");
    if (val_Button_1_long_press_action == 2)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "Aux ON/OFF");
    if (val_Button_1_long_press_action == 3)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "Mode switch 1/2/3");
    if (val_Button_1_long_press_action == 4)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "Mode switch 2/3");
    if (val_Button_1_long_press_action == 5)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "Eco switch NONE/MED/MAX");
    if (val_Button_1_long_press_action == 6)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "Eco switch NONE/MED");
    if (val_Button_1_long_press_action == 7)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "Anti-theft manual lock ON");
    if (val_Button_1_long_press_action == 8)
        aux.setElementValue("ACE_SETTINGS_Button_1_long_press_action", "Nitro boost continuous");
    uint8_t val_Button_2_short_press_action = WifiSettingsPortal_settings->get_Button_2_short_press_action();
    if (val_Button_2_short_press_action == 0)
        aux.setElementValue("ACE_SETTINGS_Button_2_short_press_action", "None");
    if (val_Button_2_short_press_action == 1)
        aux.setElementValue("ACE_SETTINGS_Button_2_short_press_action", "Speed limitation ON/OFF");
    if (val_Button_2_short_press_action == 2)
        aux.setElementValue("ACE_SETTINGS_Button_2_short_press_action", "Aux ON/OFF");
    if (val_Button_2_short_press_action == 3)
        aux.setElementValue("ACE_SETTINGS_Button_2_short_press_action", "Mode switch 1/2/3");
    if (val_Button_2_short_press_action == 4)
        aux.setElementValue("ACE_SETTINGS_Button_2_short_press_action", "Mode switch 2/3");
    if (val_Button_2_short_press_action == 5)
        aux.setElementValue("ACE_SETTINGS_Button_2_short_press_action", "Eco switch NONE/MED/MAX");
    if (val_Button_2_short_press_action == 6)
        aux.setElementValue("ACE_SETTINGS_Button_2_short_press_action", "Eco switch NONE/MED");
    aux.setElementValue("ACE_SETTINGS_Button_long_press_duration", (String)WifiSettingsPortal_settings->get_Button_long_press_duration());
    aux.setElementValue("ACE_SETTINGS_Wifi_ssid", (String)WifiSettingsPortal_settings->get_Wifi_ssid());
    aux.setElementValue("ACE_SETTINGS_Wifi_password", (String)WifiSettingsPortal_settings->get_Wifi_password());

}
