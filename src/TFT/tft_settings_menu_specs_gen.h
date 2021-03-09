
//////------------------------------------
//////------------------------------------
//////------------------------------------
////// GENERATED FILE - DO NOT EDIT MANUALLY
//////------------------------------------
//////------------------------------------
//////------------------------------------

////// Manage settings exchanged in BLE and stored in EEPOM

/*

float Wheel_size = 8.5;
int Motor_number_of_magents_pairs=15;
float Battery_minimum_voltage =42.0;
float Battery_maximum_voltage=59.0;
int Battery_distance=40;
int thottleRegen=0;

TOGGLE(thottleRegen,setThottleRegen,"  Throttle signal regeneration: ",doNothing,noEvent,noStyle //
  ,VALUE("On",1,doNothing,noEvent) //
  ,VALUE("Off",0,doNothing,noEvent) //
);

MENU(escooterCharac,"  E-scooter characteristics",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, Wheel_size, "  Wheel size (inches)","",8.5,13.0,0.5,0.1,doNothing,anyEvent,wrapStyle) //
  ,FIELD(Motor_number_of_magents_pairs,"  Motor number of magents pairs","",5,20,1,0,doNothing,noEvent,wrapStyle) //
  ,altFIELD(decPlaces<1>::menuField, Battery_minimum_voltage,"  Battery minimum voltage (volts)","",30, 100,1,0.1,doNothing,noEvent,wrapStyle) //
  ,altFIELD(decPlaces<1>::menuField, Battery_maximum_voltage,"  Battery maximum voltage (volts)","",30, 100,1,0.1,doNothing,noEvent,wrapStyle) //
  ,FIELD(Battery_distance,"  Battery distance (km)","",30, 100,5,1,doNothing,noEvent,wrapStyle) //
  ,SUBMENU(setThottleRegen) //
  ,EXIT("< Back")
);

MENU(escooterSettings,"  E-scooter settings",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);

MENU(escooterAccessories,"  E-scooter accessories",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);

MENU(electricBrake,"  Electric brake",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);

MENU(electricThrottle,"  Electric throttle",doNothing,noEvent,noStyle // 
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);

MENU(displaySettings,"  Display",doNothing,noEvent,noStyle //
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
  ,EXIT("< Back") //
);


*/

int test = 0;
TOGGLE(test,setTest,"  Test: ",doNothing,noEvent,noStyle //
  ,VALUE("On",1,doNothing,noEvent) //
  ,VALUE("Off",0,doNothing,noEvent) //
);
    float wheel_size;
    uint8_t number_of_poles_pairs;
    float battery_minimum_voltage;
    float battery_maximum_voltage;
    uint8_t battery_maximum_distance;
    uint8_t default_mode_at_startup;
    uint8_t default_eco_mode_at_startup;
    TOGGLE(default_eco_mode_at_startup,default_eco_mode_at_startup_LIST,"  Default eco mode at startup : ",doNothing,noEvent,noStyle //
        ,VALUE("Max", 0 ,doNothing,noEvent) //
        ,VALUE("Medium", 1 ,doNothing,noEvent) //
        ,VALUE("None", 2 ,doNothing,noEvent) //
    );
    uint8_t default_acceleration;
    TOGGLE(default_acceleration,default_acceleration_LIST,"  Default acceleration : ",doNothing,noEvent,noStyle //
        ,VALUE("Very slow", 0 ,doNothing,noEvent) //
        ,VALUE("Slow", 1 ,doNothing,noEvent) //
        ,VALUE("Medium", 2 ,doNothing,noEvent) //
        ,VALUE("Fast", 3 ,doNothing,noEvent) //
        ,VALUE("Very fast", 4 ,doNothing,noEvent) //
        ,VALUE("Maximum", 5 ,doNothing,noEvent) //
    );
    uint8_t default_electric_brake_at_startup;
    uint8_t pas_enabled;
    TOGGLE(pas_enabled,pas_enabled_LIST,"  PAS enabled : ",doNothing,noEvent,noStyle //
        ,VALUE("Off",0,doNothing,noEvent) //
        ,VALUE("On",1,doNothing,noEvent) //
    );
    uint8_t abs_enabled;
    TOGGLE(abs_enabled,abs_enabled_LIST,"  Abs enabled : ",doNothing,noEvent,noStyle //
        ,VALUE("Off",0,doNothing,noEvent) //
        ,VALUE("On",1,doNothing,noEvent) //
    );
    uint8_t display_brightness;
    uint8_t temperature_warning;
    uint8_t humidity_warning;
    uint8_t speed_limiter_at_startup;
    TOGGLE(speed_limiter_at_startup,speed_limiter_at_startup_LIST,"  Speed limiter at startup : ",doNothing,noEvent,noStyle //
        ,VALUE("Off",0,doNothing,noEvent) //
        ,VALUE("On",1,doNothing,noEvent) //
    );
    uint8_t speed_limiter_max_speed;
    uint8_t progressive_mode;
    TOGGLE(progressive_mode,progressive_mode_LIST,"  Progressive mode : ",doNothing,noEvent,noStyle //
        ,VALUE("Off",0,doNothing,noEvent) //
        ,VALUE("On",1,doNothing,noEvent) //
    );
    uint8_t type;
    TOGGLE(type,type_LIST,"  Type : ",doNothing,noEvent,noStyle //
        ,VALUE("Controller", 0 ,doNothing,noEvent) //
        ,VALUE("Smart - Digital brake lever", 1 ,doNothing,noEvent) //
        ,VALUE("Smart - Analog brake lever", 2 ,doNothing,noEvent) //
        ,VALUE("Smart - Digital brake + throttle (exp)", 3 ,doNothing,noEvent) //
    );
    uint8_t min_value;
    uint8_t max_value;
    uint8_t time_between_mode_shift;
    uint8_t disabled_on_high_battery_voltage;
    TOGGLE(disabled_on_high_battery_voltage,disabled_on_high_battery_voltage_LIST,"  Disabled on high battery voltage : ",doNothing,noEvent,noStyle //
        ,VALUE("Off",0,doNothing,noEvent) //
        ,VALUE("On",1,doNothing,noEvent) //
    );
    uint8_t disabled_percent_limit;
    uint8_t throttle_regeneration;
    TOGGLE(throttle_regeneration,throttle_regeneration_LIST,"  Throttle regeneration : ",doNothing,noEvent,noStyle //
        ,VALUE("Off",0,doNothing,noEvent) //
        ,VALUE("On",1,doNothing,noEvent) //
    );
    uint32_t input_min_voltage_in_millivolts;
    uint32_t input_max_voltage_in_millivolts;
    uint32_t output_min_voltage_in_millivolts;
    uint32_t output_max_voltage_in_millivolts;
    uint8_t output_curve;
    TOGGLE(output_curve,output_curve_LIST,"  Output curve : ",doNothing,noEvent,noStyle //
        ,VALUE("Linear", 0 ,doNothing,noEvent) //
        ,VALUE("Exponential 1", 1 ,doNothing,noEvent) //
        ,VALUE("Exponential 2", 2 ,doNothing,noEvent) //
        ,VALUE("Exponential 3", 3 ,doNothing,noEvent) //
        ,VALUE("Exponential 4", 4 ,doNothing,noEvent) //
    );
    uint8_t b1_short_press_action;
    TOGGLE(b1_short_press_action,b1_short_press_action_LIST,"  B1 short press action : ",doNothing,noEvent,noStyle //
        ,VALUE("None", 0 ,doNothing,noEvent) //
        ,VALUE("Startup speed limi", 1 ,doNothing,noEvent) //
        ,VALUE("Aux ON/OFF", 2 ,doNothing,noEvent) //
        ,VALUE("Mode switch 1/2/3", 3 ,doNothing,noEvent) //
        ,VALUE("Mode switch 2/3", 4 ,doNothing,noEvent) //
        ,VALUE("Eco switch NONE/MED/MAX", 5 ,doNothing,noEvent) //
        ,VALUE("Eco switch NONE/MED", 6 ,doNothing,noEvent) //
    );
    uint8_t b1_long_press_action;
    TOGGLE(b1_long_press_action,b1_long_press_action_LIST,"  B1 long press action : ",doNothing,noEvent,noStyle //
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
    uint8_t b2_short_press_action;
    TOGGLE(b2_short_press_action,b2_short_press_action_LIST,"  B2 short press action : ",doNothing,noEvent,noStyle //
        ,VALUE("None", 0 ,doNothing,noEvent) //
        ,VALUE("Startup speed limitation ON/OFF", 1 ,doNothing,noEvent) //
        ,VALUE("Aux ON/OFF", 2 ,doNothing,noEvent) //
        ,VALUE("Mode switch 1/2/3", 3 ,doNothing,noEvent) //
        ,VALUE("Mode switch 2/3", 4 ,doNothing,noEvent) //
        ,VALUE("Eco switch NONE/MED/MAX", 5 ,doNothing,noEvent) //
        ,VALUE("Eco switch NONE/MED", 6 ,doNothing,noEvent) //
    );
    uint8_t button_long_press_duration;

MENU(SUBMENU_Escooter_characteristics,"  Escooter characteristics",doNothing,noEvent,noStyle //
    ,altFIELD(decPlaces<1>::menuField, wheel_size, "  Wheel size","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
    ,FIELD(number_of_poles_pairs,"  Number of poles pairs","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,altFIELD(decPlaces<1>::menuField, battery_minimum_voltage, "  Battery minimum voltage","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
    ,altFIELD(decPlaces<1>::menuField, battery_maximum_voltage, "  Battery maximum voltage","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle) //
    ,FIELD(battery_maximum_distance,"  Battery maximum distance","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
    
MENU(SUBMENU_SmartDisplay,"  SmartDisplay",doNothing,noEvent,noStyle //
    ,FIELD(default_mode_at_startup,"  Default mode at startup","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,SUBMENU(default_eco_mode_at_startup_LIST) //
    ,SUBMENU(default_acceleration_LIST) //
    ,FIELD(default_electric_brake_at_startup,"  Default electric brake at startup","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,SUBMENU(pas_enabled_LIST) //
    ,SUBMENU(abs_enabled_LIST) //
    ,FIELD(display_brightness,"  Display brightness","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,FIELD(temperature_warning,"  Temperature warning","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,FIELD(humidity_warning,"  Humidity warning","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_General,"  General",doNothing,noEvent,noStyle //
    ,SUBMENU(speed_limiter_at_startup_LIST) //
    ,FIELD(speed_limiter_max_speed,"  Speed limiter max speed","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Electric_brake,"  Electric brake",doNothing,noEvent,noStyle //
    ,SUBMENU(progressive_mode_LIST) //
    ,SUBMENU(type_LIST) //
    ,FIELD(min_value,"  Min value","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,FIELD(max_value,"  Max value","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,FIELD(time_between_mode_shift,"  Time between mode shift","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,SUBMENU(disabled_on_high_battery_voltage_LIST) //
    ,FIELD(disabled_percent_limit,"  Disabled percent limit","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Throttle,"  Throttle",doNothing,noEvent,noStyle //
    ,SUBMENU(throttle_regeneration_LIST) //
    ,FIELD(input_min_voltage_in_millivolts,"  Input min voltage (in millivolts)","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,FIELD(input_max_voltage_in_millivolts,"  Input max voltage (in millivolts)","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,FIELD(output_min_voltage_in_millivolts,"  Output min voltage (in millivolts)","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,FIELD(output_max_voltage_in_millivolts,"  Output max voltage (in millivolts)","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,SUBMENU(output_curve_LIST) //
    ,EXIT("< Back")
);
    
MENU(SUBMENU_Escooter_buttons,"  Escooter buttons",doNothing,noEvent,noStyle //
    ,SUBMENU(b1_short_press_action_LIST) //
    ,SUBMENU(b1_long_press_action_LIST) //
    ,SUBMENU(b2_short_press_action_LIST) //
    ,FIELD(button_long_press_duration,"  Button long press duration","",5,20,1,0,doNothing,noEvent,wrapStyle) //
    ,EXIT("< Back")
);
    
    

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
