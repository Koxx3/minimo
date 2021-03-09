
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

MENU(mainMenu,"  Main menu",doNothing,noEvent,wrapStyle //
  ,SUBMENU(moreInfos) //
  ,SUBMENU(escooterCharac) //
  ,SUBMENU(escooterSettings) //
  ,SUBMENU(escooterAccessories) //
  ,SUBMENU(electricBrake) //
  ,SUBMENU(electricThrottle) //
  ,SUBMENU(displaySettings) //
  ,SUBMENU(bluetooth) //
  ,SUBMENU(firmware) //
  ,OP("< Discard & exit", discard_exit,enterEvent) //
  ,OP("< Save & exit", save_exit,enterEvent) //
);
