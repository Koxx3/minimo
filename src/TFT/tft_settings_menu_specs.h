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
#include <menuIO/esp8266Out.h> //must include this even if not doing web output...
#include "SharedData.h"



////////////////////////////////////////////////
// Simple processing functions
result discard_exit()
{
  Serial.print("\n\nEXIT - discard_exit!!!!\n\n");

  TFT_menu_shrd->inSettingsMenu = SETTINGS_MENU_STATE_OUT;
  Serial.println("discard_exit_item ===> SETTINGS_MENU_STATE_OUT");

  return proceed;
}

result save_exit()
{
  Serial.print("\n\nEXIT - save_exit!!!!\n\n");

  TFT_menu_shrd->inSettingsMenu = SETTINGS_MENU_STATE_OUT;
  Serial.println("discard_exit_item ===> SETTINGS_MENU_STATE_OUT");

  settings_menu_save_to_settings();
  notifySettingsChangedWithBle();

  return proceed;
}

////////////////////////////////////////////////
// Menu processing functions
class currrent_temperature_item : public prompt
{
public:
  currrent_temperature_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Current temperature : " + (String)(TFT_menu_shrd->currentTemperature) + " °C";
    return out.printRaw(F(val.c_str()), len);
  }
};

class max_temperature_item : public prompt
{
public:
  max_temperature_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Max temperature : " + (String)(TFT_menu_shrd->maxTemperature) + " °C";
    return out.printRaw(F(val.c_str()), len);
  }
};

class humidity_item : public prompt
{
public:
  humidity_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Humidity : " + (String)((int)TFT_menu_shrd->currentHumidity) + " RH";
    return out.printRaw(F(val.c_str()), len);
  }
};

class errors_item : public prompt
{
public:
  errors_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Errors : ";
    int count = 0;
    if (TFT_menu_shrd->errorBrake)
    {
      val += "1 ";
      count++;
    }
    if (TFT_menu_shrd->errorThrottle)
    {
      val += "2 ";
      count++;
    }
    if (TFT_menu_shrd->errorContrl)
    {
      val += "3 ";
      count++;
    }
    if (TFT_menu_shrd->errorSerialFromContrl)
    {
      val += "4 ";
      count++;
    }
    if (TFT_menu_shrd->errorSerialFromDisplay)
    {
      val += "5 ";
      count++;
    }
    if (count == 0)
    {
      val += "none";
    }
    return out.printRaw(F(val.c_str()), len);
  }
};

class firmware_type_item : public prompt
{
public:
  firmware_type_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String firmType = (String)FIRMWARE_TYPE;
    firmType.replace("smartdisplay", "sd");
    firmType.replace("smartcontroller", "sc");
    String val = "  Firm. Type : " + firmType;
    return out.printRaw(F(val.c_str()), len);
  }
};

class firmware_version_item : public prompt
{
public:
  firmware_version_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Firm. version : " + (String)FIRMWARE_VERSION;
    return out.printRaw(F(val.c_str()), len);
  }
};

class beacon_rssi_item : public prompt
{
public:
  beacon_rssi_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Beacon RSSI : " + (String)TFT_menu_shrd->beaconRSSI;
    return out.printRaw(F(val.c_str()), len);
  }
};

class beacon_mac_item : public prompt
{
public:
  beacon_mac_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Beacon MAC : " + (String)TFT_menu_settings->Ble_beacon_mac_address;
    return out.printRaw(F(val.c_str()), len);
  }
};

class ble_pin_code_item : public prompt
{
public:
  ble_pin_code_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Bluetooth PIN code : " + (String)TFT_menu_settings->Ble_pin_code;
    return out.printRaw(F(val.c_str()), len);
  }
};

class wifi_conf_ap_ssid_item : public prompt
{
public:
  wifi_conf_ap_ssid_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Wifi AP SSID : ";
    if (TFT_menu_settings->Wifi_ssid.length() == 0)
      val = val + "<empty>";
    else
      val = val + (String)TFT_menu_settings->Wifi_ssid;
    return out.printRaw(F(val.c_str()), len);
  }
};

class wifi_conf_ap_pwd_item : public prompt
{
public:
  wifi_conf_ap_pwd_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String pwd_hidden_str = "";
    Serial.printf("wifi pwd length  = %d\n", TFT_menu_settings->Wifi_password.length());
    if (TFT_menu_settings->Wifi_password.length() == 0)
    {
      pwd_hidden_str = "<empty>";
    }
    else
    {
      for (int i = 0; i < TFT_menu_settings->Wifi_password.length(); i++)
        pwd_hidden_str = pwd_hidden_str + "*";
    }
    String val = "  Wifi AP pwd : " + (String)pwd_hidden_str;
    return out.printRaw(F(val.c_str()), len);
  }
};

class wifi_connect_item : public prompt
{
public:
  wifi_connect_item(constMEM promptShadow &p) : prompt(p) {}
  Used printTo(navRoot &root, bool sel, menuOut &out, idx_t idx, idx_t len, idx_t) override
  {
    String val = "  Wifi connect password : " + (String)TFT_menu_settings->Ble_pin_code + (String)TFT_menu_settings->Ble_pin_code;
    return out.printRaw(F(val.c_str()), len);
  }
};

result reset_ble_pin_code_item()
{
  TFT_menu_settings->Ble_pin_code = 147258;
  TFT_menu_settings->save();

  return proceed;
}

result calibration_brake_min_item()
{

  TFT_menu_shrd->brakeMinPressureRaw = TFT_menu_shrd->brakeAnalogValue;
  saveBrakeMinPressure();

  return proceed;
}

result calibration_brake_max_item()
{

  TFT_menu_shrd->brakeMaxPressureRaw = TFT_menu_shrd->brakeAnalogValue;
  saveBrakeMaxPressure();
  return proceed;
}

result ota_update_pio()
{
  TFT_menu_shrd->inOtaMode = OTA_IDE;
  save_exit();

  return proceed;
}


MENU(SUBMENU_MANUAL_status, "  Status", doNothing, noEvent, noStyle,
     altOP(currrent_temperature_item, "", doNothing, updateEvent), // updateEvent
     altOP(max_temperature_item, "", doNothing, updateEvent),      // updateEvent
     altOP(humidity_item, "", doNothing, updateEvent),             // updateEvent
     altOP(errors_item, "", doNothing, updateEvent),               // updateEvent
     EXIT("< Back"));

MENU(SUBMENU_MANUAL_calibrations, "  Calibrations", doNothing, noEvent, noStyle,
     OP("  Calibrate ebrake min position", calibration_brake_min_item, enterEvent),
     OP("  Calibrate ebrake max position", calibration_brake_max_item, enterEvent),
     EXIT("< Back"));

MENU(SUBMENU_MANUAL_more, "  More", doNothing, noEvent, noStyle,
     altOP(firmware_type_item, "", doNothing, updateEvent),    // updateEvent
     altOP(firmware_version_item, "", doNothing, updateEvent), // updateEvent
     altOP(beacon_rssi_item, "", doNothing, updateEvent),      // updateEvent
     altOP(beacon_mac_item, "", doNothing, updateEvent),       // updateEvent
     altOP(ble_pin_code_item, "", doNothing, updateEvent),     // updateEvent
     OP("  Bluetooth reset PIN code to 147258", reset_ble_pin_code_item, enterEvent),
     altOP(wifi_conf_ap_ssid_item, "", doNothing, updateEvent), // updateEvent
     altOP(wifi_conf_ap_pwd_item, "", doNothing, updateEvent),  // updateEvent
     altOP(wifi_connect_item, "", doNothing, updateEvent),      // updateEvent
     OP("  OTA update through PlatformIO", ota_update_pio, enterEvent),
     EXIT("< Back"));

#include "tft_settings_menu_specs_gen.h"

// define menu colors --------------------------------------------------------
//monochromatic color table
#define Black RGB565(0, 0, 0)
#define Red RGB565(255, 0, 0)
#define Green RGB565(0, 255, 0)
#define Blue RGB565(0, 0, 255)
#define Gray RGB565(128, 128, 128)
#define LighterSelected RGB565(230, 230, 255)
#define LighterRed RGB565(255, 150, 150)
#define LighterBlue RGB565(200, 200, 255)
#define DarkerRed RGB565(150, 0, 0)
#define DarkerGreen RGB565(0, 150, 0)
#define DarkerBlue RGB565(0, 0, 150)
#define Cyan RGB565(0, 255, 255)
#define Magenta RGB565(255, 0, 255)
#define Yellow RGB565(255, 255, 0)
#define White RGB565(255, 255, 255)
#define GreenAndroid RGB565(0x66, 0x99, 0x00)
#define TitleDarkAndroid RGB565(0x66, 0x3F, 0x9F)
#define LineSelectAndroid RGB565(0x9e, 0x5f, 0xff)

//  {{disabled normal,disabled selected},{enabled normal,enabled selected, enabled editing}}
const colorDef<uint16_t> colors[6] MEMMODE = {
    {{(uint16_t)Black,
      (uint16_t)Black},
     {(uint16_t)Black,
      (uint16_t)LineSelectAndroid,
      (uint16_t)LineSelectAndroid}}, //bgColor
    {
        {(uint16_t)Gray,
         (uint16_t)Gray},
        {(uint16_t)White,
         (uint16_t)White,
         (uint16_t)White}}, //fgColor
    {
        {(uint16_t)White,
         (uint16_t)Black},
        {(uint16_t)LighterBlue,
         (uint16_t)LighterSelected,
         (uint16_t)Red}}, //valColor
    {
        {(uint16_t)White,
         (uint16_t)Black},
        {(uint16_t)LighterBlue,
         (uint16_t)LighterSelected,
         (uint16_t)Red}}, //unitColor
    {
        {(uint16_t)White,
         (uint16_t)Gray},
        {(uint16_t)Black,
         (uint16_t)Gray,
         (uint16_t)White}}, //cursorColor
    {
        {(uint16_t)White,
         (uint16_t)LighterBlue},
        {(uint16_t)TitleDarkAndroid,
         (uint16_t)White,
         (uint16_t)White}}, //titleColor
};

#endif