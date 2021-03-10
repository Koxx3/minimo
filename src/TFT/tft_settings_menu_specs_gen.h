
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
float tft_wheel_size = 10.0;
uint8_t tft_number_of_poles_pairs = 15;
float tft_battery_minimum_voltage = 42.0;
float tft_battery_maximum_voltage = 58.8;
uint8_t tft_battery_maximum_distance = 40;
uint8_t tft_default_mode_at_startup = 3;
uint8_t tft_default_eco_mode_at_startup = 2;
TOGGLE(tft_default_eco_mode_at_startup, tft_default_eco_mode_at_startup_LIST,"  Default eco mode at startup ",doNothing,noEvent,noStyle //
    ,VALUE("Max", 0 ,doNothing,noEvent) //
    ,VALUE("Medium", 1 ,doNothing,noEvent) //
    ,VALUE("None", 2 ,doNothing,noEvent) //
);
uint8_t tft_default_acceleration = 5;
TOGGLE(tft_default_acceleration, tft_default_acceleration_LIST,"  Default acceleration ",doNothing,noEvent,noStyle //
    ,VALUE("Very slow", 0 ,doNothing,noEvent) //
    ,VALUE("Slow", 1 ,doNothing,noEvent) //
    ,VALUE("Medium", 2 ,doNothing,noEvent) //
    ,VALUE("Fast", 3 ,doNothing,noEvent) //
    ,VALUE("Very fast", 4 ,doNothing,noEvent) //
    ,VALUE("Maximum", 5 ,doNothing,noEvent) //
);
uint8_t tft_default_electric_brake_at_startup = 3;
uint8_t tft_pas_enabled = 0;
TOGGLE(tft_pas_enabled, tft_pas_enabled_LIST,"  PAS enabled ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_abs_enabled = 0;
TOGGLE(tft_abs_enabled, tft_abs_enabled_LIST,"  Abs enabled ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_display_brightness = 100;
uint8_t tft_temperature_warning = 70;
uint8_t tft_humidity_warning = 80;
uint8_t tft_speed_limiter_at_startup = 0;
TOGGLE(tft_speed_limiter_at_startup, tft_speed_limiter_at_startup_LIST,"  Speed limiter at startup ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_speed_limiter_max_speed = 37;
uint8_t tft_progressive_mode = 0;
TOGGLE(tft_progressive_mode, tft_progressive_mode_LIST,"  Progressive mode ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_smart_brake_type = 0;
TOGGLE(tft_smart_brake_type, tft_smart_brake_type_LIST,"  Type ",doNothing,noEvent,noStyle //
    ,VALUE("Controller", 0 ,doNothing,noEvent) //
    ,VALUE("Smart - Digital brake lever", 1 ,doNothing,noEvent) //
    ,VALUE("Smart - Analog brake lever", 2 ,doNothing,noEvent) //
    ,VALUE("Smart - Digital brake + throttle (exp)", 3 ,doNothing,noEvent) //
);
uint8_t tft_min_power_value = 1;
uint8_t tft_max_power_value = 5;
uint32_t tft_time_between_mode_shift = 500;
uint8_t tft_disabled_on_high_battery_voltage = 0;
TOGGLE(tft_disabled_on_high_battery_voltage, tft_disabled_on_high_battery_voltage_LIST,"  Disabled on high battery voltage ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint8_t tft_disabled_percent_limit = 100;
uint32_t tft_throttle_regeneration = 0;
TOGGLE(tft_throttle_regeneration, tft_throttle_regeneration_LIST,"  Throttle regeneration ",doNothing,noEvent,noStyle //
    ,VALUE("Off",0,doNothing,noEvent) //
    ,VALUE("On",1,doNothing,noEvent) //
);
uint32_t tft_input_min_voltage_in_millivolts = 650;
uint32_t tft_input_max_voltage_in_millivolts = 3950;
uint32_t tft_output_min_voltage_in_millivolts = 800;
uint32_t tft_output_max_voltage_in_millivolts = 3350;
uint8_t tft_output_curve = 2;
TOGGLE(tft_output_curve, tft_output_curve_LIST,"  Output curve ",doNothing,noEvent,noStyle //
    ,VALUE("Linear", 0 ,doNothing,noEvent) //
    ,VALUE("Exponential 1", 1 ,doNothing,noEvent) //
    ,VALUE("Exponential 2", 2 ,doNothing,noEvent) //
    ,VALUE("Exponential 3", 3 ,doNothing,noEvent) //
    ,VALUE("Exponential 4", 4 ,doNothing,noEvent) //
);
uint8_t tft_button_1_short_press_action = 0;
TOGGLE(tft_button_1_short_press_action, tft_button_1_short_press_action_LIST,"  B1 short press action ",doNothing,noEvent,noStyle //
    ,VALUE("None", 0 ,doNothing,noEvent) //
    ,VALUE("Startup speed limi", 1 ,doNothing,noEvent) //
    ,VALUE("Aux ON/OFF", 2 ,doNothing,noEvent) //
    ,VALUE("Mode switch 1/2/3", 3 ,doNothing,noEvent) //
    ,VALUE("Mode switch 2/3", 4 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED/MAX", 5 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED", 6 ,doNothing,noEvent) //
);
uint8_t tft_button_1_long_press_action = 0;
TOGGLE(tft_button_1_long_press_action, tft_button_1_long_press_action_LIST,"  B1 long press action ",doNothing,noEvent,noStyle //
    ,VALUE("None", 0 ,doNothing,noEvent) //
    ,VALUE("Startup speed limitation", 1 ,doNothing,noEvent) //
    ,VALUE("Aux ON/OFF", 2 ,doNothing,noEvent) //
    ,VALUE("Mode switch 1/2/3", 3 ,doNothing,noEvent) //
    ,VALUE("Mode switch 2/3", 4 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED/MAX", 5 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED", 6 ,doNothing,noEvent) //
    ,VALUE("Anti-theft manual lock ON", 7 ,doNothing,noEvent) //
    ,VALUE("Nitro boost continuous", 8 ,doNothing,noEvent) //
);
uint8_t tft_button_2_short_press_action = 0;
TOGGLE(tft_button_2_short_press_action, tft_button_2_short_press_action_LIST,"  B2 short press action ",doNothing,noEvent,noStyle //
    ,VALUE("None", 0 ,doNothing,noEvent) //
    ,VALUE("Startup speed limitation ON/OFF", 1 ,doNothing,noEvent) //
    ,VALUE("Aux ON/OFF", 2 ,doNothing,noEvent) //
    ,VALUE("Mode switch 1/2/3", 3 ,doNothing,noEvent) //
    ,VALUE("Mode switch 2/3", 4 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED/MAX", 5 ,doNothing,noEvent) //
    ,VALUE("Eco switch NONE/MED", 6 ,doNothing,noEvent) //
);
uint8_t tft_button_long_press_duration = 5;


/* submenus */
MENU(SUBMENU_Escooter_characteristics,"  Escooter characteristics",doNothing,noEvent,noStyle //
    ,altFIELD(decPlaces<1>::menuField, tft_wheel_size, "  Wheel size " ,"", 8.0, 20.0, 1.0, 0.1, doNothing,anyEvent,wrapStyle) //
    ,FIELD(tft_number_of_poles_pairs,"  Number of poles pairs ","", 10, 30, 1, 1, doNothing,noEvent,wrapStyle) //
    ,altFIELD(decPlaces<1>::menuField, tft_battery_minimum_voltage, "  Battery minimum voltage " ,"", 30.0, 100.0, 1.0, 0.1, doNothing,anyEvent,wrapStyle) //
    ,altFIELD(decPlaces<1>::menuField, tft_battery_maximum_voltage, "  Battery maximum voltage " ,"", 30.0, 100.0, 1.0, 0.1, doNothing,anyEvent,wrapStyle) //
    ,FIELD(tft_battery_maximum_distance,"  Battery maximum distance ","", 10, 100, 5, 1, doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
    
MENU(SUBMENU_SmartDisplay,"  SmartDisplay",doNothing,noEvent,noStyle //
    ,FIELD(tft_default_mode_at_startup,"  Default mode at startup ","", 1, 3, 1, 1, doNothing,noEvent,wrapStyle) //
    ,SUBMENU(tft_default_eco_mode_at_startup_LIST) //
    ,SUBMENU(tft_default_acceleration_LIST) //
    ,FIELD(tft_default_electric_brake_at_startup,"  Default electric brake at startup ","", 1, 5, 1, 1, doNothing,noEvent,wrapStyle) //
    ,SUBMENU(tft_pas_enabled_LIST) //
    ,SUBMENU(tft_abs_enabled_LIST) //
    ,FIELD(tft_display_brightness,"  Display brightness ","", 50, 100, 10, 5, doNothing,noEvent,wrapStyle) //
    ,FIELD(tft_temperature_warning,"  Temperature warning ","", 50, 100, 10, 5, doNothing,noEvent,wrapStyle) //
    ,FIELD(tft_humidity_warning,"  Humidity warning ","", 20, 100, 10, 5, doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_General,"  General",doNothing,noEvent,noStyle //
    ,SUBMENU(tft_speed_limiter_at_startup_LIST) //
    ,FIELD(tft_speed_limiter_max_speed,"  Speed limiter max speed ","", 5, 100, 1, 1, doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Electric_brake,"  Electric brake",doNothing,noEvent,noStyle //
    ,SUBMENU(tft_progressive_mode_LIST) //
    ,SUBMENU(tft_smart_brake_type_LIST) //
    ,FIELD(tft_min_power_value,"  Min value ","", 0, 5, 1, 1, doNothing,noEvent,wrapStyle) //
    ,FIELD(tft_max_power_value,"  Max value ","", 0, 5, 1, 1, doNothing,noEvent,wrapStyle) //
    ,FIELD(tft_time_between_mode_shift,"  Time between mode shift ","", 100, 2000, 50, 50, doNothing,noEvent,wrapStyle) //
    ,SUBMENU(tft_disabled_on_high_battery_voltage_LIST) //
    ,FIELD(tft_disabled_percent_limit,"  Disabled percent limit ","", 80, 100, 1, 1, doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Throttle,"  Throttle",doNothing,noEvent,noStyle //
    ,SUBMENU(tft_throttle_regeneration_LIST) //
    ,FIELD(tft_input_min_voltage_in_millivolts,"  Input min voltage (in millivolts) ","", 500, 1500, 50, 50, doNothing,noEvent,wrapStyle) //
    ,FIELD(tft_input_max_voltage_in_millivolts,"  Input max voltage (in millivolts) ","", 2500, 5000, 50, 50, doNothing,noEvent,wrapStyle) //
    ,FIELD(tft_output_min_voltage_in_millivolts,"  Output min voltage (in millivolts) ","", 500, 2000, 50, 50, doNothing,noEvent,wrapStyle) //
    ,FIELD(tft_output_max_voltage_in_millivolts,"  Output max voltage (in millivolts) ","", 2500, 5000, 50, 50, doNothing,noEvent,wrapStyle) //
    ,SUBMENU(tft_output_curve_LIST) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Escooter_buttons,"  Escooter buttons",doNothing,noEvent,noStyle //
    ,SUBMENU(tft_button_1_short_press_action_LIST) //
    ,SUBMENU(tft_button_1_long_press_action_LIST) //
    ,SUBMENU(tft_button_2_short_press_action_LIST) //
    ,FIELD(tft_button_long_press_duration,"  Button long press duration ","", 2, 30, 1, 1, doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
    

//-----------------------
// menu
//-----------------------
MENU(mainMenu,"  Main menu",doNothing,noEvent,wrapStyle //
    ,SUBMENU(SUBMENU_MANUAL_status) //
    ,SUBMENU(SUBMENU_Escooter_characteristics) //
    ,SUBMENU(SUBMENU_SmartDisplay) //
    ,SUBMENU(SUBMENU_General) //
    ,SUBMENU(SUBMENU_Electric_brake) //
    ,SUBMENU(SUBMENU_Throttle) //
    ,SUBMENU(SUBMENU_Escooter_buttons) //
    ,SUBMENU(SUBMENU_MANUAL_more) //
    ,OP("< Discard & exit", discard_exit,enterEvent) //
    ,OP("< Save & exit", save_exit,enterEvent) //
);

//-----------------------
// functions
//-----------------------
void settings_menu_init_from_settings() {
    tft_wheel_size = app_settings->get_wheel_size();
    tft_number_of_poles_pairs = app_settings->get_number_of_poles_pairs();
    tft_battery_minimum_voltage = app_settings->get_battery_minimum_voltage();
    tft_battery_maximum_voltage = app_settings->get_battery_maximum_voltage();
    tft_battery_maximum_distance = app_settings->get_battery_maximum_distance();
    tft_default_mode_at_startup = app_settings->get_default_mode_at_startup();
    tft_default_eco_mode_at_startup = app_settings->get_default_eco_mode_at_startup();
    tft_default_acceleration = app_settings->get_default_acceleration();
    tft_default_electric_brake_at_startup = app_settings->get_default_electric_brake_at_startup();
    tft_pas_enabled = app_settings->get_pas_enabled();
    tft_abs_enabled = app_settings->get_abs_enabled();
    tft_display_brightness = app_settings->get_display_brightness();
    tft_temperature_warning = app_settings->get_temperature_warning();
    tft_humidity_warning = app_settings->get_humidity_warning();
    tft_speed_limiter_at_startup = app_settings->get_speed_limiter_at_startup();
    tft_speed_limiter_max_speed = app_settings->get_speed_limiter_max_speed();
    tft_progressive_mode = app_settings->get_progressive_mode();
    tft_smart_brake_type = app_settings->get_smart_brake_type();
    tft_min_power_value = app_settings->get_min_power_value();
    tft_max_power_value = app_settings->get_max_power_value();
    tft_time_between_mode_shift = app_settings->get_time_between_mode_shift();
    tft_disabled_on_high_battery_voltage = app_settings->get_disabled_on_high_battery_voltage();
    tft_disabled_percent_limit = app_settings->get_disabled_percent_limit();
    tft_throttle_regeneration = app_settings->get_throttle_regeneration();
    tft_input_min_voltage_in_millivolts = app_settings->get_input_min_voltage_in_millivolts();
    tft_input_max_voltage_in_millivolts = app_settings->get_input_max_voltage_in_millivolts();
    tft_output_min_voltage_in_millivolts = app_settings->get_output_min_voltage_in_millivolts();
    tft_output_max_voltage_in_millivolts = app_settings->get_output_max_voltage_in_millivolts();
    tft_output_curve = app_settings->get_output_curve();
    tft_button_1_short_press_action = app_settings->get_button_1_short_press_action();
    tft_button_1_long_press_action = app_settings->get_button_1_long_press_action();
    tft_button_2_short_press_action = app_settings->get_button_2_short_press_action();
    tft_button_long_press_duration = app_settings->get_button_long_press_duration();

}

void settings_menu_save_to_settings() {
    app_settings->set_wheel_size(tft_wheel_size);
    app_settings->set_number_of_poles_pairs(tft_number_of_poles_pairs);
    app_settings->set_battery_minimum_voltage(tft_battery_minimum_voltage);
    app_settings->set_battery_maximum_voltage(tft_battery_maximum_voltage);
    app_settings->set_battery_maximum_distance(tft_battery_maximum_distance);
    app_settings->set_default_mode_at_startup(tft_default_mode_at_startup);
    app_settings->set_default_eco_mode_at_startup(tft_default_eco_mode_at_startup);
    app_settings->set_default_acceleration(tft_default_acceleration);
    app_settings->set_default_electric_brake_at_startup(tft_default_electric_brake_at_startup);
    app_settings->set_pas_enabled(tft_pas_enabled);
    app_settings->set_abs_enabled(tft_abs_enabled);
    app_settings->set_display_brightness(tft_display_brightness);
    app_settings->set_temperature_warning(tft_temperature_warning);
    app_settings->set_humidity_warning(tft_humidity_warning);
    app_settings->set_speed_limiter_at_startup(tft_speed_limiter_at_startup);
    app_settings->set_speed_limiter_max_speed(tft_speed_limiter_max_speed);
    app_settings->set_progressive_mode(tft_progressive_mode);
    app_settings->set_smart_brake_type(tft_smart_brake_type);
    app_settings->set_min_power_value(tft_min_power_value);
    app_settings->set_max_power_value(tft_max_power_value);
    app_settings->set_time_between_mode_shift(tft_time_between_mode_shift);
    app_settings->set_disabled_on_high_battery_voltage(tft_disabled_on_high_battery_voltage);
    app_settings->set_disabled_percent_limit(tft_disabled_percent_limit);
    app_settings->set_throttle_regeneration(tft_throttle_regeneration);
    app_settings->set_input_min_voltage_in_millivolts(tft_input_min_voltage_in_millivolts);
    app_settings->set_input_max_voltage_in_millivolts(tft_input_max_voltage_in_millivolts);
    app_settings->set_output_min_voltage_in_millivolts(tft_output_min_voltage_in_millivolts);
    app_settings->set_output_max_voltage_in_millivolts(tft_output_max_voltage_in_millivolts);
    app_settings->set_output_curve(tft_output_curve);
    app_settings->set_button_1_short_press_action(tft_button_1_short_press_action);
    app_settings->set_button_1_long_press_action(tft_button_1_long_press_action);
    app_settings->set_button_2_short_press_action(tft_button_2_short_press_action);
    app_settings->set_button_long_press_duration(tft_button_long_press_duration);

    app_settings->save();
}
