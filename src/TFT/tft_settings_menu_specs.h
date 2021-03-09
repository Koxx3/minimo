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

double dummy = 0.0;

MENU(SUBMENU_MANUAL_moreInfos,"  More informations",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, dummy, "  ODO","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Temperature","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Humidity","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Errors","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,EXIT("< Back")
);

MENU(SUBMENU_MANUAL_bluetooth,"  Bluetooth",doNothing,noEvent,noStyle
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Beacon RSSI","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,altFIELD(decPlaces<1>::menuField, dummy, "  Beacon MAC address","",0,10.0,0.5,0.01,doNothing,anyEvent,wrapStyle)
  ,OP("  Reset PIN code to 147258",resetBlePinCode,enterEvent)
  ,EXIT("< Back")
);

MENU(SUBMENU_MANUAL_firmware,"  Firmware",doNothing,noEvent,noStyle
  ,OP("  Type", doNothing, noEvent)
  ,OP("  Version", doNothing, noEvent)
  ,OP("  Configure wifi to prepare OTA", doNothing, noEvent)
  ,OP("  OTA update through PlatformIO", doNothing, noEvent)
  ,OP("  OTA update through Github server", doNothing, noEvent)
  ,EXIT("< Back")
);

#include "tft_settings_menu_specs_gen.h"


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