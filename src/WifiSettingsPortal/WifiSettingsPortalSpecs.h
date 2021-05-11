
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
ACText(ACS1, "<h2>Escooter characteristics</h2>", "");
ACInput(ACS1_1, "10.0", "Wheel size", "^[0-9]+[.]?[0-9]*([eE][-+]?[0-9]+)?$", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS1_2, "15", "Number of poles pairs", "^[0-9]+$", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACS1_3, {"36V (42V full / 10S)","48V (54,6V full / 13S)","52V (58,8V full / 14S)","60V (67,2V full / 16S)","72V (84V full / 20S)","86V (100,8V full / 24S)"}, "Battery nominal voltage", 2);
ACInput(ACS1_4, "40", "Battery maximum distance", "^[0-9]+$", "", AC_Tag_BR, AC_Input_Text);
ACText(ACS2, "<h2>General</h2>", "");
ACCheckbox(ACS2_1, "ACS2_1", "Speed limiter at startup", false, AC_Infront);
ACInput(ACS2_2, "37", "Speed limiter max speed", "", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACS2_3, {"None","Smartphone connected","Smartphone connected or beacon visible","Beacon visible"}, "Bluetooth lock mode", 0);
ACInput(ACS2_4, "147258", "PIN code", "^[0-9]{6,6}$", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS2_5, "aa:bb:cc:dd:ee:ff", "Beacon Mac Address 1", "^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS2_6, "aa:bb:cc:dd:ee:ff", "Beacon Mac Address 2", "^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS2_7, "-80", "Beacon range", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS2_8, "0", "Speed adjustment", "^[-]?[0-9]+$", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS2_9, "70", "Temperature warning", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS2_10, "80", "Humidity warning", "", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACS2_11, {"None","2 min","3 min","4 min","5 min","10 min","15 min","20 min","30 min","60 min"}, "Auto power off", 0);
ACText(ACS3, "<h2>Smartphone display</h2>", "");
ACInput(ACS3_1, "0", "Speed adjustment", "^[-]?[0-9]+$", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACS3_2, {"AUX","DUAL","LIGHT","HORN","RELAY"}, "Aux relay name", 0);
ACCheckbox(ACS3_3, "ACS3_3", "Display GPS speed ", false, AC_Infront);
ACText(ACS4, "<h2>SmartDisplay</h2>", "");
ACInput(ACS4_1, "3", "Startup mode", "", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACS4_2, {"Max","Medium","None"}, "Startup eco mode", 2);
ACSelect(ACS4_3, {"Maximum","Very fast","Fast","Medium","Slow","Very slow"}, "Startup acceleration", 0);
ACInput(ACS4_4, "3", "Startup  ebrake", "", "", AC_Tag_BR, AC_Input_Text);
ACCheckbox(ACS4_5, "ACS4_5", "PAS enabled", false, AC_Infront);
ACCheckbox(ACS4_6, "ACS4_6", "ABS enabled", false, AC_Infront);
ACInput(ACS4_7, "100", "Display brightness", "", "", AC_Tag_BR, AC_Input_Text);
ACCheckbox(ACS4_8, "ACS4_8", "Display splash screen", false, AC_Infront);
ACSelect(ACS4_9, {"0","1","2","3"}, "Rotate screen", 1);
ACText(ACS5, "<h2>Electric brake</h2>", "");
ACCheckbox(ACS5_1, "ACS5_1", "Progressive mode", false, AC_Infront);
ACSelect(ACS5_2, {"Controller","Smart - Digital brake lever","Smart - Analog brake lever","Smart - Digital brake + throttle (exp)"}, "Type", 0);
ACInput(ACS5_3, "1", "Min value", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS5_4, "5", "Max value", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS5_5, "500", "Time between mode shift", "", "", AC_Tag_BR, AC_Input_Text);
ACCheckbox(ACS5_6, "ACS5_6", "Disabled with bat, HV", false, AC_Infront);
ACInput(ACS5_7, "100", "Disabled with bat. HV", "", "", AC_Tag_BR, AC_Input_Text);
ACText(ACS6, "<h2>Throttle</h2>", "");
ACCheckbox(ACS6_1, "ACS6_1", "Throttle regeneration", false, AC_Infront);
ACInput(ACS6_2, "650", "Input min voltage", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS6_3, "3950", "Input max voltage", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS6_4, "800", "Output min voltage", "", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS6_5, "3350", "Output max voltage", "", "", AC_Tag_BR, AC_Input_Text);
ACSelect(ACS6_6, {"Linear","Exponential 1","Exponential 2","Exponential 3","Exponential 4","Custom 6 points"}, "Output curve", 2);
ACInput(ACS6_7, "20,40,60,80", "Custom output curve points", "^[0-9]+(,[0-9]+){3}$", "", AC_Tag_BR, AC_Input_Text);
ACText(ACS7, "<h2>Escooter buttons</h2>", "");
ACSelect(ACS7_1, {"None","Speed limit ON/OFF","Aux ON/OFF","Mode switch 1/2/3","Mode switch 2/3","Eco switch NONE/MED/MAX","Eco switch NONE/MED"}, "B1 short press ", 0);
ACSelect(ACS7_2, {"None","Speed limitation ON/OFF","Aux ON/OFF","Mode switch 1/2/3","Mode switch 2/3","Eco switch NONE/MED/MAX","Eco switch NONE/MED","Anti-theft manual lock ON","Nitro boost continuous"}, "B1 long press ", 0);
ACSelect(ACS7_3, {"None","Speed limitation ON/OFF","Aux ON/OFF","Mode switch 1/2/3","Mode switch 2/3","Eco switch NONE/MED/MAX","Eco switch NONE/MED"}, "B2 short press ", 0);
ACInput(ACS7_4, "3", "Button long press duration", "", "", AC_Tag_BR, AC_Input_Text);
ACText(ACS8, "<h2>OTA update</h2>", "");
ACInput(ACS8_1, "", "Wifi network name", "^(.{0,32})$", "", AC_Tag_BR, AC_Input_Text);
ACInput(ACS8_2, "", "Wifi password", "^(.{0,32})$", "", AC_Tag_BR, AC_Input_Password);


/* menus */
AutoConnectAux settingsPageAux("/settingspage", "SmartElec settings", true, {
    ACE_Style1,
    ACE_Style2,
    ACE_Style3,
    ACE_Style4,
        
    ACS1,
    ACS1_1,
    ACS1_2,
    ACS1_3,
    ACS1_4,
        
    ACS2,
    ACS2_1,
    ACS2_2,
    ACS2_3,
    ACS2_4,
    ACS2_5,
    ACS2_6,
    ACS2_7,
    ACS2_8,
    ACS2_9,
    ACS2_10,
    ACS2_11,
        
    ACS3,
    ACS3_1,
    ACS3_2,
    ACS3_3,
        
    ACS4,
    ACS4_1,
    ACS4_2,
    ACS4_3,
    ACS4_4,
    ACS4_5,
    ACS4_6,
    ACS4_7,
    ACS4_8,
    ACS4_9,
        
    ACS5,
    ACS5_1,
    ACS5_2,
    ACS5_3,
    ACS5_4,
    ACS5_5,
    ACS5_6,
    ACS5_7,
        
    ACS6,
    ACS6_1,
    ACS6_2,
    ACS6_3,
    ACS6_4,
    ACS6_5,
    ACS6_6,
    ACS6_7,
        
    ACS7,
    ACS7_1,
    ACS7_2,
    ACS7_3,
    ACS7_4,
        
    ACS8,
    ACS8_1,
    ACS8_2,
    ACE_SETTINGS_Discard, 
    ACE_SETTINGS_Save,
    ACE_SETTINGS_Js1 });        


/* save */
void saveConfig(AutoConnectAux &aux)
{
    WifiSettingsPortal_settings->set_Wheel_size((settingsPageAux["ACS1_1"].as<AutoConnectInput>()).value.toFloat());
    WifiSettingsPortal_settings->set_Number_of_poles_pairs((settingsPageAux["ACS1_2"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Battery_nominal_voltage((settingsPageAux["ACS1_3"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Battery_maximum_distance((settingsPageAux["ACS1_4"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Speed_limiter_at_startup((settingsPageAux["ACS2_1"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    WifiSettingsPortal_settings->set_Speed_limiter_max_speed((settingsPageAux["ACS2_2"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Bluetooth_lock_mode((settingsPageAux["ACS2_3"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Ble_pin_code((settingsPageAux["ACS2_4"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Ble_beacon_mac_address1((settingsPageAux["ACS2_5"].as<AutoConnectInput>()).value);
    WifiSettingsPortal_settings->set_Ble_beacon_mac_address2((settingsPageAux["ACS2_6"].as<AutoConnectInput>()).value);
    WifiSettingsPortal_settings->set_Ble_beacon_range((settingsPageAux["ACS2_7"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Original_display_speed_adjustment((settingsPageAux["ACS2_8"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Temperature_warning((settingsPageAux["ACS2_9"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Humidity_warning((settingsPageAux["ACS2_10"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Auto_power_off((settingsPageAux["ACS2_11"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Smartdisplay_speed_adjustment((settingsPageAux["ACS3_1"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Aux_relay_name((settingsPageAux["ACS3_2"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Display_gps_speed_instead_of_escooter_speed((settingsPageAux["ACS3_3"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    WifiSettingsPortal_settings->set_Default_mode_at_startup((settingsPageAux["ACS4_1"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Default_eco_mode_at_startup((settingsPageAux["ACS4_2"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Default_acceleration((settingsPageAux["ACS4_3"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Default_electric_brake_at_startup((settingsPageAux["ACS4_4"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Pas_enabled((settingsPageAux["ACS4_5"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    WifiSettingsPortal_settings->set_Abs_enabled((settingsPageAux["ACS4_6"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    WifiSettingsPortal_settings->set_Display_brightness((settingsPageAux["ACS4_7"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Display_splash_screen((settingsPageAux["ACS4_8"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    WifiSettingsPortal_settings->set_Rotate_screen((settingsPageAux["ACS4_9"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Ebrake_progressive_mode((settingsPageAux["ACS5_1"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    WifiSettingsPortal_settings->set_Ebrake_smart_brake_type((settingsPageAux["ACS5_2"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Ebrake_min_power_value((settingsPageAux["ACS5_3"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Ebrake_max_power_value((settingsPageAux["ACS5_4"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Ebrake_time_between_mode_shift((settingsPageAux["ACS5_5"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Ebrake_disabled_on_high_battery_voltage((settingsPageAux["ACS5_6"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    WifiSettingsPortal_settings->set_Ebrake_disabled_percent_limit((settingsPageAux["ACS5_7"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_regeneration((settingsPageAux["ACS6_1"].as<AutoConnectCheckbox>()).checked ? 1 : 0);
    WifiSettingsPortal_settings->set_Throttle_input_min_voltage((settingsPageAux["ACS6_2"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_input_max_voltage((settingsPageAux["ACS6_3"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_output_min_voltage((settingsPageAux["ACS6_4"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_output_max_voltage((settingsPageAux["ACS6_5"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Throttle_output_curve((settingsPageAux["ACS6_6"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Throttle_output_curve_custom4((settingsPageAux["ACS6_7"].as<AutoConnectInput>()).value);
    WifiSettingsPortal_settings->set_Button_1_short_press_action((settingsPageAux["ACS7_1"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Button_1_long_press_action((settingsPageAux["ACS7_2"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Button_2_short_press_action((settingsPageAux["ACS7_3"].as<AutoConnectSelect>()).selected - 1);
    WifiSettingsPortal_settings->set_Button_long_press_duration((settingsPageAux["ACS7_4"].as<AutoConnectInput>()).value.toInt());
    WifiSettingsPortal_settings->set_Wifi_ssid((settingsPageAux["ACS8_1"].as<AutoConnectInput>()).value);
    WifiSettingsPortal_settings->set_Wifi_password((settingsPageAux["ACS8_2"].as<AutoConnectInput>()).value);
    WifiSettingsPortal_settings->save();
    
}

/* load */
void loadConfig(AutoConnectAux &aux)
{
    aux.setElementValue("ACS1_1", (String)WifiSettingsPortal_settings->get_Wheel_size());
    aux.setElementValue("ACS1_2", (String)WifiSettingsPortal_settings->get_Number_of_poles_pairs());
    uint8_t val_Battery_nominal_voltage = WifiSettingsPortal_settings->get_Battery_nominal_voltage();
    if (val_Battery_nominal_voltage == 0)
        aux.setElementValue("ACS1_3", "36V (42V full / 10S)");
    if (val_Battery_nominal_voltage == 1)
        aux.setElementValue("ACS1_3", "48V (54,6V full / 13S)");
    if (val_Battery_nominal_voltage == 2)
        aux.setElementValue("ACS1_3", "52V (58,8V full / 14S)");
    if (val_Battery_nominal_voltage == 3)
        aux.setElementValue("ACS1_3", "60V (67,2V full / 16S)");
    if (val_Battery_nominal_voltage == 4)
        aux.setElementValue("ACS1_3", "72V (84V full / 20S)");
    if (val_Battery_nominal_voltage == 5)
        aux.setElementValue("ACS1_3", "86V (100,8V full / 24S)");
    aux.setElementValue("ACS1_4", (String)WifiSettingsPortal_settings->get_Battery_maximum_distance());
    aux.setElementValue("ACS2_1", WifiSettingsPortal_settings->get_Speed_limiter_at_startup() ? "checked" : "");
    aux.setElementValue("ACS2_2", (String)WifiSettingsPortal_settings->get_Speed_limiter_max_speed());
    uint8_t val_Bluetooth_lock_mode = WifiSettingsPortal_settings->get_Bluetooth_lock_mode();
    if (val_Bluetooth_lock_mode == 0)
        aux.setElementValue("ACS2_3", "None");
    if (val_Bluetooth_lock_mode == 1)
        aux.setElementValue("ACS2_3", "Smartphone connected");
    if (val_Bluetooth_lock_mode == 2)
        aux.setElementValue("ACS2_3", "Smartphone connected or beacon visible");
    if (val_Bluetooth_lock_mode == 3)
        aux.setElementValue("ACS2_3", "Beacon visible");
    aux.setElementValue("ACS2_4", (String)WifiSettingsPortal_settings->get_Ble_pin_code());
    aux.setElementValue("ACS2_5", (String)WifiSettingsPortal_settings->get_Ble_beacon_mac_address1());
    aux.setElementValue("ACS2_6", (String)WifiSettingsPortal_settings->get_Ble_beacon_mac_address2());
    aux.setElementValue("ACS2_7", (String)WifiSettingsPortal_settings->get_Ble_beacon_range());
    aux.setElementValue("ACS2_8", (String)WifiSettingsPortal_settings->get_Original_display_speed_adjustment());
    aux.setElementValue("ACS2_9", (String)WifiSettingsPortal_settings->get_Temperature_warning());
    aux.setElementValue("ACS2_10", (String)WifiSettingsPortal_settings->get_Humidity_warning());
    uint8_t val_Auto_power_off = WifiSettingsPortal_settings->get_Auto_power_off();
    if (val_Auto_power_off == 0)
        aux.setElementValue("ACS2_11", "None");
    if (val_Auto_power_off == 1)
        aux.setElementValue("ACS2_11", "2 min");
    if (val_Auto_power_off == 2)
        aux.setElementValue("ACS2_11", "3 min");
    if (val_Auto_power_off == 3)
        aux.setElementValue("ACS2_11", "4 min");
    if (val_Auto_power_off == 4)
        aux.setElementValue("ACS2_11", "5 min");
    if (val_Auto_power_off == 5)
        aux.setElementValue("ACS2_11", "10 min");
    if (val_Auto_power_off == 6)
        aux.setElementValue("ACS2_11", "15 min");
    if (val_Auto_power_off == 7)
        aux.setElementValue("ACS2_11", "20 min");
    if (val_Auto_power_off == 8)
        aux.setElementValue("ACS2_11", "30 min");
    if (val_Auto_power_off == 9)
        aux.setElementValue("ACS2_11", "60 min");
    aux.setElementValue("ACS3_1", (String)WifiSettingsPortal_settings->get_Smartdisplay_speed_adjustment());
    uint8_t val_Aux_relay_name = WifiSettingsPortal_settings->get_Aux_relay_name();
    if (val_Aux_relay_name == 0)
        aux.setElementValue("ACS3_2", "AUX");
    if (val_Aux_relay_name == 1)
        aux.setElementValue("ACS3_2", "DUAL");
    if (val_Aux_relay_name == 2)
        aux.setElementValue("ACS3_2", "LIGHT");
    if (val_Aux_relay_name == 3)
        aux.setElementValue("ACS3_2", "HORN");
    if (val_Aux_relay_name == 4)
        aux.setElementValue("ACS3_2", "RELAY");
    aux.setElementValue("ACS3_3", WifiSettingsPortal_settings->get_Display_gps_speed_instead_of_escooter_speed() ? "checked" : "");
    aux.setElementValue("ACS4_1", (String)WifiSettingsPortal_settings->get_Default_mode_at_startup());
    uint8_t val_Default_eco_mode_at_startup = WifiSettingsPortal_settings->get_Default_eco_mode_at_startup();
    if (val_Default_eco_mode_at_startup == 0)
        aux.setElementValue("ACS4_2", "Max");
    if (val_Default_eco_mode_at_startup == 1)
        aux.setElementValue("ACS4_2", "Medium");
    if (val_Default_eco_mode_at_startup == 2)
        aux.setElementValue("ACS4_2", "None");
    uint8_t val_Default_acceleration = WifiSettingsPortal_settings->get_Default_acceleration();
    if (val_Default_acceleration == 0)
        aux.setElementValue("ACS4_3", "Maximum");
    if (val_Default_acceleration == 1)
        aux.setElementValue("ACS4_3", "Very fast");
    if (val_Default_acceleration == 2)
        aux.setElementValue("ACS4_3", "Fast");
    if (val_Default_acceleration == 3)
        aux.setElementValue("ACS4_3", "Medium");
    if (val_Default_acceleration == 4)
        aux.setElementValue("ACS4_3", "Slow");
    if (val_Default_acceleration == 5)
        aux.setElementValue("ACS4_3", "Very slow");
    aux.setElementValue("ACS4_4", (String)WifiSettingsPortal_settings->get_Default_electric_brake_at_startup());
    aux.setElementValue("ACS4_5", WifiSettingsPortal_settings->get_Pas_enabled() ? "checked" : "");
    aux.setElementValue("ACS4_6", WifiSettingsPortal_settings->get_Abs_enabled() ? "checked" : "");
    aux.setElementValue("ACS4_7", (String)WifiSettingsPortal_settings->get_Display_brightness());
    aux.setElementValue("ACS4_8", WifiSettingsPortal_settings->get_Display_splash_screen() ? "checked" : "");
    uint8_t val_Rotate_screen = WifiSettingsPortal_settings->get_Rotate_screen();
    if (val_Rotate_screen == 0)
        aux.setElementValue("ACS4_9", "0");
    if (val_Rotate_screen == 1)
        aux.setElementValue("ACS4_9", "1");
    if (val_Rotate_screen == 2)
        aux.setElementValue("ACS4_9", "2");
    if (val_Rotate_screen == 3)
        aux.setElementValue("ACS4_9", "3");
    aux.setElementValue("ACS5_1", WifiSettingsPortal_settings->get_Ebrake_progressive_mode() ? "checked" : "");
    uint8_t val_Ebrake_smart_brake_type = WifiSettingsPortal_settings->get_Ebrake_smart_brake_type();
    if (val_Ebrake_smart_brake_type == 0)
        aux.setElementValue("ACS5_2", "Controller");
    if (val_Ebrake_smart_brake_type == 1)
        aux.setElementValue("ACS5_2", "Smart - Digital brake lever");
    if (val_Ebrake_smart_brake_type == 2)
        aux.setElementValue("ACS5_2", "Smart - Analog brake lever");
    if (val_Ebrake_smart_brake_type == 3)
        aux.setElementValue("ACS5_2", "Smart - Digital brake + throttle (exp)");
    aux.setElementValue("ACS5_3", (String)WifiSettingsPortal_settings->get_Ebrake_min_power_value());
    aux.setElementValue("ACS5_4", (String)WifiSettingsPortal_settings->get_Ebrake_max_power_value());
    aux.setElementValue("ACS5_5", (String)WifiSettingsPortal_settings->get_Ebrake_time_between_mode_shift());
    aux.setElementValue("ACS5_6", WifiSettingsPortal_settings->get_Ebrake_disabled_on_high_battery_voltage() ? "checked" : "");
    aux.setElementValue("ACS5_7", (String)WifiSettingsPortal_settings->get_Ebrake_disabled_percent_limit());
    aux.setElementValue("ACS6_1", WifiSettingsPortal_settings->get_Throttle_regeneration() ? "checked" : "");
    aux.setElementValue("ACS6_2", (String)WifiSettingsPortal_settings->get_Throttle_input_min_voltage());
    aux.setElementValue("ACS6_3", (String)WifiSettingsPortal_settings->get_Throttle_input_max_voltage());
    aux.setElementValue("ACS6_4", (String)WifiSettingsPortal_settings->get_Throttle_output_min_voltage());
    aux.setElementValue("ACS6_5", (String)WifiSettingsPortal_settings->get_Throttle_output_max_voltage());
    uint8_t val_Throttle_output_curve = WifiSettingsPortal_settings->get_Throttle_output_curve();
    if (val_Throttle_output_curve == 0)
        aux.setElementValue("ACS6_6", "Linear");
    if (val_Throttle_output_curve == 1)
        aux.setElementValue("ACS6_6", "Exponential 1");
    if (val_Throttle_output_curve == 2)
        aux.setElementValue("ACS6_6", "Exponential 2");
    if (val_Throttle_output_curve == 3)
        aux.setElementValue("ACS6_6", "Exponential 3");
    if (val_Throttle_output_curve == 4)
        aux.setElementValue("ACS6_6", "Exponential 4");
    if (val_Throttle_output_curve == 5)
        aux.setElementValue("ACS6_6", "Custom 6 points");
    aux.setElementValue("ACS6_7", (String)WifiSettingsPortal_settings->get_Throttle_output_curve_custom4());
    uint8_t val_Button_1_short_press_action = WifiSettingsPortal_settings->get_Button_1_short_press_action();
    if (val_Button_1_short_press_action == 0)
        aux.setElementValue("ACS7_1", "None");
    if (val_Button_1_short_press_action == 1)
        aux.setElementValue("ACS7_1", "Speed limit ON/OFF");
    if (val_Button_1_short_press_action == 2)
        aux.setElementValue("ACS7_1", "Aux ON/OFF");
    if (val_Button_1_short_press_action == 3)
        aux.setElementValue("ACS7_1", "Mode switch 1/2/3");
    if (val_Button_1_short_press_action == 4)
        aux.setElementValue("ACS7_1", "Mode switch 2/3");
    if (val_Button_1_short_press_action == 5)
        aux.setElementValue("ACS7_1", "Eco switch NONE/MED/MAX");
    if (val_Button_1_short_press_action == 6)
        aux.setElementValue("ACS7_1", "Eco switch NONE/MED");
    uint8_t val_Button_1_long_press_action = WifiSettingsPortal_settings->get_Button_1_long_press_action();
    if (val_Button_1_long_press_action == 0)
        aux.setElementValue("ACS7_2", "None");
    if (val_Button_1_long_press_action == 1)
        aux.setElementValue("ACS7_2", "Speed limitation ON/OFF");
    if (val_Button_1_long_press_action == 2)
        aux.setElementValue("ACS7_2", "Aux ON/OFF");
    if (val_Button_1_long_press_action == 3)
        aux.setElementValue("ACS7_2", "Mode switch 1/2/3");
    if (val_Button_1_long_press_action == 4)
        aux.setElementValue("ACS7_2", "Mode switch 2/3");
    if (val_Button_1_long_press_action == 5)
        aux.setElementValue("ACS7_2", "Eco switch NONE/MED/MAX");
    if (val_Button_1_long_press_action == 6)
        aux.setElementValue("ACS7_2", "Eco switch NONE/MED");
    if (val_Button_1_long_press_action == 7)
        aux.setElementValue("ACS7_2", "Anti-theft manual lock ON");
    if (val_Button_1_long_press_action == 8)
        aux.setElementValue("ACS7_2", "Nitro boost continuous");
    uint8_t val_Button_2_short_press_action = WifiSettingsPortal_settings->get_Button_2_short_press_action();
    if (val_Button_2_short_press_action == 0)
        aux.setElementValue("ACS7_3", "None");
    if (val_Button_2_short_press_action == 1)
        aux.setElementValue("ACS7_3", "Speed limitation ON/OFF");
    if (val_Button_2_short_press_action == 2)
        aux.setElementValue("ACS7_3", "Aux ON/OFF");
    if (val_Button_2_short_press_action == 3)
        aux.setElementValue("ACS7_3", "Mode switch 1/2/3");
    if (val_Button_2_short_press_action == 4)
        aux.setElementValue("ACS7_3", "Mode switch 2/3");
    if (val_Button_2_short_press_action == 5)
        aux.setElementValue("ACS7_3", "Eco switch NONE/MED/MAX");
    if (val_Button_2_short_press_action == 6)
        aux.setElementValue("ACS7_3", "Eco switch NONE/MED");
    aux.setElementValue("ACS7_4", (String)WifiSettingsPortal_settings->get_Button_long_press_duration());
    aux.setElementValue("ACS8_1", (String)WifiSettingsPortal_settings->get_Wifi_ssid());
    aux.setElementValue("ACS8_2", (String)WifiSettingsPortal_settings->get_Wifi_password());

}
