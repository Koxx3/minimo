////////// NEVER REFORMAT //////////
////////// NEVER REFORMAT //////////
////////// NEVER REFORMAT //////////
////////// NEVER REFORMAT //////////
////////// NEVER REFORMAT //////////
////////// NEVER REFORMAT //////////
////////// NEVER REFORMAT //////////

#ifndef _Settings_menu_specs_h

#define _Settings_menu_specs_h

#include <menu.h>
#include <menuIO/serialIO.h>
#include <menuIO/TFT_eSPIOut.h>
#include <menuIO/esp8266Out.h>//must include this even if not doing web output...

extern bool isInMenu;

int thottleRegen=0;
TOGGLE(thottleRegen,setThottleRegen,"Throttle signal regeneration: ",doNothing,noEvent,noStyle//,doExit,enterEvent,noStyle
  ,VALUE("On",1,doNothing,noEvent)
  ,VALUE("Off",0,doNothing,noEvent)
);

class discard_exit_item:public prompt {
public:
  discard_exit_item(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t) override {
    return out.printRaw(F("< Discard & exit"),len);;
  }
};

class save_exit_item:public prompt {
public:
  save_exit_item(constMEM promptShadow& p):prompt(p) {}
  Used printTo(navRoot &root,bool sel,menuOut& out, idx_t idx,idx_t len,idx_t) override {
    return out.printRaw(F("< Save & exit"),len);;
  }
};


////////////////////////////////////////////////
// Simple processing functions
result discard_exit() {
  isInMenu = false;
  Serial.print("\n\nEXIT - discard_exit!!!!\n\n");
  return proceed;
}

result save_exit() {
  isInMenu = false;
  Serial.print("\n\nEXIT - save_exit!!!!\n\n");
  return proceed;
}

result resetBlePinCode() {
  return proceed;
}

const char* constMEM hexDigit MEMMODE="0123456789ABCDEF";
const char* constMEM hexNr[] MEMMODE={"0","x",hexDigit,hexDigit};
char buf1[]="Test";

double dummy = 0.0;

MENU(moreInfos,"More informations",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, dummy, "Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,EXIT("< Back")
);

double Wheel_size = 8.5;
int Motor_number_of_magents_pairs=15;
double Battery_minimum_voltage =42.0;
double Battery_maximum_voltage=59.0;
int Battery_distance=40;
MENU(escooterCharac,"E-scooter characteristics",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, Wheel_size, "Wheel size (inches)","",8.5,13.0,0.5,0.1,doNothing,anyEvent,wrapStyle)
  ,FIELD(Motor_number_of_magents_pairs,"Motor number of magents pairs","",5,20,1,0,doNothing,noEvent,wrapStyle)
  ,altFIELD(decPlaces<1>::menuField, Battery_minimum_voltage,"Battery minimum voltage (volts)","",30, 100,1,0.1,doNothing,noEvent,wrapStyle)
  ,altFIELD(decPlaces<1>::menuField, Battery_maximum_voltage,"Battery maximum voltage (volts)","",30, 100,1,0.1,doNothing,noEvent,wrapStyle)
  ,FIELD(Battery_distance,"Battery distance (km)","",30, 100,5,1,doNothing,noEvent,wrapStyle)
  ,SUBMENU(setThottleRegen)
  ,EXIT("< Back")
);

MENU(escooterSettings,"E-scooter settings",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, dummy, "Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,EXIT("< Back")
);

MENU(escooterAccessories,"E-scooter accessories",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, dummy, "Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,EXIT("< Back")
);

MENU(electricBrake,"Electric brake",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, dummy, "Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,EXIT("< Back")
);

MENU(electricThrottle,"Electric throttle",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, dummy, "Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,EXIT("< Back")
);

MENU(displaySettings,"Display",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, dummy, "Dummy","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,EXIT("< Back")
);

MENU(bluetooth,"Bluetooth",doNothing,noEvent,noStyle
  ,OP("Reset PIN code to 147258",resetBlePinCode,enterEvent)
  ,EXIT("< Back")
);

MENU(softwareUpdates,"Sotfware upddates (Wifi)",doNothing,noEvent,noStyle
  //,EDIT("Wifi SSID",buf1,hexNr,doNothing,noEvent,noStyle)
  //,EDIT("Wifi password",buf1,hexNr,doNothing,noEvent,noStyle)
  ,OP("Wifi settings needs to be set with bluetooth, sorry !", doNothing, noEvent)
  ,OP("OTA update through PlatformIO", resetBlePinCode, enterEvent)
  ,OP("OTA update through Github server", resetBlePinCode, enterEvent)
  ,EXIT("< Back")
);

MENU(mainMenu,"Main menu",doNothing,noEvent,wrapStyle
  ,SUBMENU(moreInfos)
  ,SUBMENU(escooterCharac)
  ,SUBMENU(escooterSettings)
  ,SUBMENU(escooterAccessories)
  ,SUBMENU(electricBrake)
  ,SUBMENU(electricThrottle)
  ,SUBMENU(displaySettings)
  ,SUBMENU(bluetooth)
  ,SUBMENU(softwareUpdates)
  ,OP("< Discard & exit", discard_exit,enterEvent)
  ,OP("< Save & exit", save_exit,enterEvent)
);


// define menu colors --------------------------------------------------------
//monochromatic color table
#define Black RGB565(0,0,0)
#define Red	RGB565(255,0,0)
#define Green RGB565(0,255,0)
#define Blue RGB565(0,0,255)
#define Gray RGB565(128,128,128)
#define LighterRed RGB565(255,150,150)
#define LighterGreen RGB565(150,255,150)
#define LighterBlue RGB565(150,150,255)
#define DarkerRed RGB565(150,0,0)
#define DarkerGreen RGB565(0,150,0)
#define DarkerBlue RGB565(0,0,150)
#define Cyan RGB565(0,255,255)
#define Magenta RGB565(255,0,255)
#define Yellow RGB565(255,255,0)
#define White RGB565(255,255,255)
#define GreenAndroid RGB565(0x66, 0x99, 0x00)

//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
const colorDef<uint16_t> colors[6] MEMMODE={
  {
    {
      (uint16_t)Black,
      (uint16_t)Black
    },
    {
      (uint16_t)Black,
      (uint16_t)GreenAndroid,
      (uint16_t)GreenAndroid
    }
  },//bgColor
  {
    {
      (uint16_t)Gray,
      (uint16_t)Gray
    },
    {
      (uint16_t)White,
      (uint16_t)White,
      (uint16_t)White
    }
  },//fgColor
  {
    {
      (uint16_t)White,
      (uint16_t)Black
    },
    {
      (uint16_t)LighterBlue,
      (uint16_t)White,
      (uint16_t)Red
    }
  },//valColor
  {
    {
      (uint16_t)White,
      (uint16_t)Black
    },
    {
      (uint16_t)White,
      (uint16_t)White,
      (uint16_t)LighterBlue
    }
  },//unitColor
  {
    {
      (uint16_t)White,
      (uint16_t)Gray
    },
    {
      (uint16_t)Black,
      (uint16_t)Gray,
      (uint16_t)White
    }
  },//cursorColor
  {
    {
      (uint16_t)White,
      (uint16_t)LighterBlue
    },
    {
      (uint16_t)Red,
      (uint16_t)White,
      (uint16_t)White
    }
  },//titleColor
};

#define MAX_DEPTH 5

#endif