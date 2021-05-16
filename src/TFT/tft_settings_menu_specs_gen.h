
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

//-----------------------
// variables and lists
//-----------------------
float tft_Wheel_size = 10.0;
uint8_t tft_Number_of_poles_pairs = 15;
uint8_t tft_Battery_nominal_voltage = 2;
TOGGLE(tft_Battery_nominal_voltage, tft_Battery_nominal_voltage_LIST,"  Battery nominal voltage ",doNothing,noEvent,noStyle //
    ,VALUE("36V (42V full / 10S)", 0 ,doNothing,noEvent) //
    ,VALUE("48V (54,6V full / 13S)", 1 ,doNothing,noEvent) //
    ,VALUE("52V (58,8V full / 14S)", 2 ,doNothing,noEvent) //
    ,VALUE("60V (67,2V full / 16S)", 3 ,doNothing,noEvent) //
    ,VALUE("72V (84V full / 20S)", 4 ,doNothing,noEvent) //
    ,VALUE("86V (100,8V full / 24S)", 5 ,doNothing,noEvent) //
);
uint8_t tft_Battery_maximum_distance = 40;
uint8_t tft_Speed_limiter_at_startup = 0;
TOGGLE(tft_Speed_limiter_at_startup, tft_Speed_limiter_at_startup_LIST,"  Speed limiter at startup ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_Speed_limiter_max_speed = 37;
int8_t tft_Original_display_speed_adjustment = 0;
uint8_t tft_Temperature_warning = 70;
uint8_t tft_Humidity_warning = 80;
uint8_t tft_Auto_power_off = 0;
TOGGLE(tft_Auto_power_off, tft_Auto_power_off_LIST,"  Auto power off ",doNothing,noEvent,noStyle //
    ,VALUE("None", 0 ,doNothing,noEvent) //
    ,VALUE("2 min", 1 ,doNothing,noEvent) //
    ,VALUE("3 min", 2 ,doNothing,noEvent) //
    ,VALUE("4 min", 3 ,doNothing,noEvent) //
    ,VALUE("5 min", 4 ,doNothing,noEvent) //
    ,VALUE("10 min", 5 ,doNothing,noEvent) //
    ,VALUE("15 min", 6 ,doNothing,noEvent) //
    ,VALUE("20 min", 7 ,doNothing,noEvent) //
    ,VALUE("30 min", 8 ,doNothing,noEvent) //
    ,VALUE("60 min", 9 ,doNothing,noEvent) //
);
uint8_t tft_Default_mode_at_startup = 3;
uint8_t tft_Default_eco_mode_at_startup = 2;
TOGGLE(tft_Default_eco_mode_at_startup, tft_Default_eco_mode_at_startup_LIST,"  Startup eco mode ",doNothing,noEvent,noStyle //
    ,VALUE("Max", 0 ,doNothing,noEvent) //
    ,VALUE("Medium", 1 ,doNothing,noEvent) //
    ,VALUE("None", 2 ,doNothing,noEvent) //
);
uint8_t tft_Default_acceleration = 0;
TOGGLE(tft_Default_acceleration, tft_Default_acceleration_LIST,"  Startup acceleration ",doNothing,noEvent,noStyle //
    ,VALUE("Maximum", 0 ,doNothing,noEvent) //
    ,VALUE("Very fast", 1 ,doNothing,noEvent) //
    ,VALUE("Fast", 2 ,doNothing,noEvent) //
    ,VALUE("Medium", 3 ,doNothing,noEvent) //
    ,VALUE("Slow", 4 ,doNothing,noEvent) //
    ,VALUE("Very slow", 5 ,doNothing,noEvent) //
);
uint8_t tft_Default_electric_brake_at_startup = 3;
uint8_t tft_Pas_enabled = 0;
TOGGLE(tft_Pas_enabled, tft_Pas_enabled_LIST,"  PAS enabled ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_Abs_enabled = 0;
TOGGLE(tft_Abs_enabled, tft_Abs_enabled_LIST,"  ABS enabled ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_Display_brightness = 100;
uint8_t tft_Display_splash_screen = 1;
TOGGLE(tft_Display_splash_screen, tft_Display_splash_screen_LIST,"  Display splash screen ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_Rotate_screen = 1;
TOGGLE(tft_Rotate_screen, tft_Rotate_screen_LIST,"  Rotate screen ",doNothing,noEvent,noStyle //
    ,VALUE("0", 0 ,doNothing,noEvent) //
    ,VALUE("1", 1 ,doNothing,noEvent) //
    ,VALUE("2", 2 ,doNothing,noEvent) //
    ,VALUE("3", 3 ,doNothing,noEvent) //
);
uint8_t tft_Ebrake_progressive_mode = 0;
TOGGLE(tft_Ebrake_progressive_mode, tft_Ebrake_progressive_mode_LIST,"  Progressive mode ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_Ebrake_smart_brake_type = 0;
TOGGLE(tft_Ebrake_smart_brake_type, tft_Ebrake_smart_brake_type_LIST,"  Type ",doNothing,noEvent,noStyle //
    ,VALUE("Controller", 0 ,doNothing,noEvent) //
    ,VALUE("Smart - Digital brake lever", 1 ,doNothing,noEvent) //
    ,VALUE("Smart - Analog brake lever", 2 ,doNothing,noEvent) //
    ,VALUE("Smart - Digital brake + throttle (exp)", 3 ,doNothing,noEvent) //
);
uint8_t tft_Ebrake_min_power_value = 1;
uint8_t tft_Ebrake_max_power_value = 5;
uint32_t tft_Ebrake_time_between_mode_shift = 500;
uint8_t tft_Ebrake_disabled_on_high_battery_voltage = 0;
TOGGLE(tft_Ebrake_disabled_on_high_battery_voltage, tft_Ebrake_disabled_on_high_battery_voltage_LIST,"  Disabled with bat, HV ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_Ebrake_disabled_percent_limit = 100;
uint8_t tft_Throttle_regeneration = 0;
TOGGLE(tft_Throttle_regeneration, tft_Throttle_regeneration_LIST,"  Throttle regeneration ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint32_t tft_Throttle_input_min_voltage = 650;
uint32_t tft_Throttle_input_max_voltage = 3950;
uint32_t tft_Throttle_output_min_voltage = 800;
uint32_t tft_Throttle_output_max_voltage = 3350;
uint8_t tft_Throttle_output_curve = 2;
TOGGLE(tft_Throttle_output_curve, tft_Throttle_output_curve_LIST,"  Output curve ",doNothing,noEvent,noStyle //
    ,VALUE("Linear", 0 ,doNothing,noEvent) //
    ,VALUE("Exponential 1", 1 ,doNothing,noEvent) //
    ,VALUE("Exponential 2", 2 ,doNothing,noEvent) //
    ,VALUE("Exponential 3", 3 ,doNothing,noEvent) //
    ,VALUE("Exponential 4", 4 ,doNothing,noEvent) //
    ,VALUE("Custom 6 points", 5 ,doNothing,noEvent) //
);
uint8_t tft_Button_1_short_press_action = 0;
TOGGLE(tft_Button_1_short_press_action, tft_Button_1_short_press_action_LIST,"  B1 short press  ",doNothing,noEvent,noStyle //
    ,VALUE("None", 0 ,doNothing,noEvent) //
    ,VALUE("Speed limit ON/OFF", 1 ,doNothing,noEvent) //
    ,VALUE("Aux ON/OFF", 2 ,doNothing,noEvent) //
    ,VALUE("Mode switch 1/2/3", 3 ,doNothing,noEvent) //
    ,VALUE("Mode switch 2/3", 4 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED/MAX", 5 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED", 6 ,doNothing,noEvent) //
);
uint8_t tft_Button_1_long_press_action = 0;
TOGGLE(tft_Button_1_long_press_action, tft_Button_1_long_press_action_LIST,"  B1 long press  ",doNothing,noEvent,noStyle //
    ,VALUE("None", 0 ,doNothing,noEvent) //
    ,VALUE("Speed limitation ON/OFF", 1 ,doNothing,noEvent) //
    ,VALUE("Aux ON/OFF", 2 ,doNothing,noEvent) //
    ,VALUE("Mode switch 1/2/3", 3 ,doNothing,noEvent) //
    ,VALUE("Mode switch 2/3", 4 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED/MAX", 5 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED", 6 ,doNothing,noEvent) //
    ,VALUE("Anti-theft manual lock ON", 7 ,doNothing,noEvent) //
    ,VALUE("Nitro boost continuous", 8 ,doNothing,noEvent) //
);
uint8_t tft_Button_2_short_press_action = 0;
TOGGLE(tft_Button_2_short_press_action, tft_Button_2_short_press_action_LIST,"  B2 short press  ",doNothing,noEvent,noStyle //
    ,VALUE("None", 0 ,doNothing,noEvent) //
    ,VALUE("Speed limitation ON/OFF", 1 ,doNothing,noEvent) //
    ,VALUE("Aux ON/OFF", 2 ,doNothing,noEvent) //
    ,VALUE("Mode switch 1/2/3", 3 ,doNothing,noEvent) //
    ,VALUE("Mode switch 2/3", 4 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED/MAX", 5 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED", 6 ,doNothing,noEvent) //
);
uint8_t tft_Button_long_press_duration = 3;


/* submenus */
MENU(SUBMENU_Escooter_characteristics,"  Escooter characteristics",doNothing,noEvent,noStyle //
    ,altFIELD(decPlaces<1>::menuField, tft_Wheel_size, "  Wheel size " ," inch", 8.0, 20.0, 0.5, 0.1, doNothing,anyEvent,noStyle) //
    ,FIELD(tft_Number_of_poles_pairs,"  Number of poles pairs "," ", 10, 30, 1, 1, doNothing,noEvent,noStyle) //
    ,SUBMENU(tft_Battery_nominal_voltage_LIST) //
    ,FIELD(tft_Battery_maximum_distance,"  Battery maximum distance "," Km", 10, 100, 5, 1, doNothing,noEvent,noStyle) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_General,"  General",doNothing,noEvent,noStyle //
    ,SUBMENU(tft_Speed_limiter_at_startup_LIST) //
    ,FIELD(tft_Speed_limiter_max_speed,"  Speed limiter max speed "," %", 5, 100, 5, 1, doNothing,noEvent,noStyle) //
    ,FIELD(tft_Original_display_speed_adjustment,"  Speed adjustment "," %", -100, 100, 5, 1, doNothing,noEvent,noStyle) //
    ,FIELD(tft_Temperature_warning,"  Temperature warning "," °C", 50, 100, 10, 5, doNothing,noEvent,noStyle) //
    ,FIELD(tft_Humidity_warning,"  Humidity warning "," RH", 20, 100, 10, 5, doNothing,noEvent,noStyle) //
    ,SUBMENU(tft_Auto_power_off_LIST) //
    ,EXIT("< Back")
);
    
    
MENU(SUBMENU_SmartDisplay,"  SmartDisplay",doNothing,noEvent,noStyle //
    ,FIELD(tft_Default_mode_at_startup,"  Startup mode "," ", 1, 3, 1, 1, doNothing,noEvent,noStyle) //
    ,SUBMENU(tft_Default_eco_mode_at_startup_LIST) //
    ,SUBMENU(tft_Default_acceleration_LIST) //
    ,FIELD(tft_Default_electric_brake_at_startup,"  Startup  ebrake "," ", 0, 5, 1, 1, doNothing,noEvent,noStyle) //
    ,SUBMENU(tft_Pas_enabled_LIST) //
    ,SUBMENU(tft_Abs_enabled_LIST) //
    ,FIELD(tft_Display_brightness,"  Display brightness "," ", 50, 100, 10, 5, doNothing,noEvent,noStyle) //
    ,SUBMENU(tft_Display_splash_screen_LIST) //
    ,SUBMENU(tft_Rotate_screen_LIST) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Electric_brake,"  Electric brake",doNothing,noEvent,noStyle //
    ,SUBMENU(tft_Ebrake_progressive_mode_LIST) //
    ,SUBMENU(tft_Ebrake_smart_brake_type_LIST) //
    ,FIELD(tft_Ebrake_min_power_value,"  Min value "," ", 0, 5, 1, 1, doNothing,noEvent,noStyle) //
    ,FIELD(tft_Ebrake_max_power_value,"  Max value "," ", 0, 5, 1, 1, doNothing,noEvent,noStyle) //
    ,FIELD(tft_Ebrake_time_between_mode_shift,"  Time between mode shift "," ms", 100, 2000, 100, 50, doNothing,noEvent,noStyle) //
    ,SUBMENU(tft_Ebrake_disabled_on_high_battery_voltage_LIST) //
    ,FIELD(tft_Ebrake_disabled_percent_limit,"  Disabled with bat. HV "," %", 80, 100, 5, 1, doNothing,noEvent,noStyle) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Throttle,"  Throttle",doNothing,noEvent,noStyle //
    ,SUBMENU(tft_Throttle_regeneration_LIST) //
    ,FIELD(tft_Throttle_input_min_voltage,"  Input min voltage "," mV", 500, 1500, 100, 50, doNothing,noEvent,noStyle) //
    ,FIELD(tft_Throttle_input_max_voltage,"  Input max voltage "," mV", 2500, 5000, 100, 50, doNothing,noEvent,noStyle) //
    ,FIELD(tft_Throttle_output_min_voltage,"  Output min voltage "," mV", 500, 2000, 100, 50, doNothing,noEvent,noStyle) //
    ,FIELD(tft_Throttle_output_max_voltage,"  Output max voltage "," mV", 2500, 5000, 100, 50, doNothing,noEvent,noStyle) //
    ,SUBMENU(tft_Throttle_output_curve_LIST) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Escooter_buttons,"  Escooter buttons",doNothing,noEvent,noStyle //
    ,SUBMENU(tft_Button_1_short_press_action_LIST) //
    ,SUBMENU(tft_Button_1_long_press_action_LIST) //
    ,SUBMENU(tft_Button_2_short_press_action_LIST) //
    ,FIELD(tft_Button_long_press_duration,"  Button long press duration "," s", 1, 10, 1, 1, doNothing,noEvent,noStyle) //
    ,EXIT("< Back")
);
    
    

//-----------------------
// menu
//-----------------------
MENU(mainMenu,"  Main menu",doNothing,noEvent,noStyle //
    ,SUBMENU(SUBMENU_MANUAL_status) //
    ,SUBMENU(SUBMENU_Escooter_characteristics) //
    ,SUBMENU(SUBMENU_General) //
    ,SUBMENU(SUBMENU_SmartDisplay) //
    ,SUBMENU(SUBMENU_Electric_brake) //
    ,SUBMENU(SUBMENU_Throttle) //
    ,SUBMENU(SUBMENU_Escooter_buttons) //
    ,SUBMENU(SUBMENU_MANUAL_calibrations) //
    ,SUBMENU(SUBMENU_MANUAL_more) //
    ,OP("< Discard & exit", discard_exit,enterEvent) //
    ,OP("< Save & exit", save_exit,enterEvent) //
);

//-----------------------
// functions
//-----------------------
void settings_menu_init_from_settings() {
    tft_Wheel_size = TFT_menu_settings->get_Wheel_size();
    tft_Number_of_poles_pairs = TFT_menu_settings->get_Number_of_poles_pairs();
    tft_Battery_nominal_voltage = TFT_menu_settings->get_Battery_nominal_voltage();
    tft_Battery_maximum_distance = TFT_menu_settings->get_Battery_maximum_distance();
    tft_Speed_limiter_at_startup = TFT_menu_settings->get_Speed_limiter_at_startup();
    tft_Speed_limiter_max_speed = TFT_menu_settings->get_Speed_limiter_max_speed();
    tft_Original_display_speed_adjustment = TFT_menu_settings->get_Original_display_speed_adjustment();
    tft_Temperature_warning = TFT_menu_settings->get_Temperature_warning();
    tft_Humidity_warning = TFT_menu_settings->get_Humidity_warning();
    tft_Auto_power_off = TFT_menu_settings->get_Auto_power_off();
    tft_Default_mode_at_startup = TFT_menu_settings->get_Default_mode_at_startup();
    tft_Default_eco_mode_at_startup = TFT_menu_settings->get_Default_eco_mode_at_startup();
    tft_Default_acceleration = TFT_menu_settings->get_Default_acceleration();
    tft_Default_electric_brake_at_startup = TFT_menu_settings->get_Default_electric_brake_at_startup();
    tft_Pas_enabled = TFT_menu_settings->get_Pas_enabled();
    tft_Abs_enabled = TFT_menu_settings->get_Abs_enabled();
    tft_Display_brightness = TFT_menu_settings->get_Display_brightness();
    tft_Display_splash_screen = TFT_menu_settings->get_Display_splash_screen();
    tft_Rotate_screen = TFT_menu_settings->get_Rotate_screen();
    tft_Ebrake_progressive_mode = TFT_menu_settings->get_Ebrake_progressive_mode();
    tft_Ebrake_smart_brake_type = TFT_menu_settings->get_Ebrake_smart_brake_type();
    tft_Ebrake_min_power_value = TFT_menu_settings->get_Ebrake_min_power_value();
    tft_Ebrake_max_power_value = TFT_menu_settings->get_Ebrake_max_power_value();
    tft_Ebrake_time_between_mode_shift = TFT_menu_settings->get_Ebrake_time_between_mode_shift();
    tft_Ebrake_disabled_on_high_battery_voltage = TFT_menu_settings->get_Ebrake_disabled_on_high_battery_voltage();
    tft_Ebrake_disabled_percent_limit = TFT_menu_settings->get_Ebrake_disabled_percent_limit();
    tft_Throttle_regeneration = TFT_menu_settings->get_Throttle_regeneration();
    tft_Throttle_input_min_voltage = TFT_menu_settings->get_Throttle_input_min_voltage();
    tft_Throttle_input_max_voltage = TFT_menu_settings->get_Throttle_input_max_voltage();
    tft_Throttle_output_min_voltage = TFT_menu_settings->get_Throttle_output_min_voltage();
    tft_Throttle_output_max_voltage = TFT_menu_settings->get_Throttle_output_max_voltage();
    tft_Throttle_output_curve = TFT_menu_settings->get_Throttle_output_curve();
    tft_Button_1_short_press_action = TFT_menu_settings->get_Button_1_short_press_action();
    tft_Button_1_long_press_action = TFT_menu_settings->get_Button_1_long_press_action();
    tft_Button_2_short_press_action = TFT_menu_settings->get_Button_2_short_press_action();
    tft_Button_long_press_duration = TFT_menu_settings->get_Button_long_press_duration();

}

void settings_menu_save_to_settings() {
    TFT_menu_settings->set_Wheel_size(tft_Wheel_size);
    TFT_menu_settings->set_Number_of_poles_pairs(tft_Number_of_poles_pairs);
    TFT_menu_settings->set_Battery_nominal_voltage(tft_Battery_nominal_voltage);
    TFT_menu_settings->set_Battery_maximum_distance(tft_Battery_maximum_distance);
    TFT_menu_settings->set_Speed_limiter_at_startup(tft_Speed_limiter_at_startup);
    TFT_menu_settings->set_Speed_limiter_max_speed(tft_Speed_limiter_max_speed);
    TFT_menu_settings->set_Original_display_speed_adjustment(tft_Original_display_speed_adjustment);
    TFT_menu_settings->set_Temperature_warning(tft_Temperature_warning);
    TFT_menu_settings->set_Humidity_warning(tft_Humidity_warning);
    TFT_menu_settings->set_Auto_power_off(tft_Auto_power_off);
    TFT_menu_settings->set_Default_mode_at_startup(tft_Default_mode_at_startup);
    TFT_menu_settings->set_Default_eco_mode_at_startup(tft_Default_eco_mode_at_startup);
    TFT_menu_settings->set_Default_acceleration(tft_Default_acceleration);
    TFT_menu_settings->set_Default_electric_brake_at_startup(tft_Default_electric_brake_at_startup);
    TFT_menu_settings->set_Pas_enabled(tft_Pas_enabled);
    TFT_menu_settings->set_Abs_enabled(tft_Abs_enabled);
    TFT_menu_settings->set_Display_brightness(tft_Display_brightness);
    TFT_menu_settings->set_Display_splash_screen(tft_Display_splash_screen);
    TFT_menu_settings->set_Rotate_screen(tft_Rotate_screen);
    TFT_menu_settings->set_Ebrake_progressive_mode(tft_Ebrake_progressive_mode);
    TFT_menu_settings->set_Ebrake_smart_brake_type(tft_Ebrake_smart_brake_type);
    TFT_menu_settings->set_Ebrake_min_power_value(tft_Ebrake_min_power_value);
    TFT_menu_settings->set_Ebrake_max_power_value(tft_Ebrake_max_power_value);
    TFT_menu_settings->set_Ebrake_time_between_mode_shift(tft_Ebrake_time_between_mode_shift);
    TFT_menu_settings->set_Ebrake_disabled_on_high_battery_voltage(tft_Ebrake_disabled_on_high_battery_voltage);
    TFT_menu_settings->set_Ebrake_disabled_percent_limit(tft_Ebrake_disabled_percent_limit);
    TFT_menu_settings->set_Throttle_regeneration(tft_Throttle_regeneration);
    TFT_menu_settings->set_Throttle_input_min_voltage(tft_Throttle_input_min_voltage);
    TFT_menu_settings->set_Throttle_input_max_voltage(tft_Throttle_input_max_voltage);
    TFT_menu_settings->set_Throttle_output_min_voltage(tft_Throttle_output_min_voltage);
    TFT_menu_settings->set_Throttle_output_max_voltage(tft_Throttle_output_max_voltage);
    TFT_menu_settings->set_Throttle_output_curve(tft_Throttle_output_curve);
    TFT_menu_settings->set_Button_1_short_press_action(tft_Button_1_short_press_action);
    TFT_menu_settings->set_Button_1_long_press_action(tft_Button_1_long_press_action);
    TFT_menu_settings->set_Button_2_short_press_action(tft_Button_2_short_press_action);
    TFT_menu_settings->set_Button_long_press_duration(tft_Button_long_press_duration);

    TFT_menu_settings->save();
}
