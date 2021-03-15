#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <AutoConnect.h>
#include "Settings.h"
#include <time.h>
#include "SharedData.h"

using WebServerClass = WebServer;

Settings *WifiSettingsPortal_settings;
SharedData *WifiSettingsPortal_shrd;

WebServerClass server;
AutoConnect portal(server);
AutoConnectConfig config;

#define USERNAME "admin" // For HTTP authentication
#define PASSWORD "admin" // For HTTP authentication

static const char JSPAGE[] PROGMEM = R"=====(
  
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<script type='text/javascript'>

$.getJSON( "https://raw.githubusercontent.com/Koxx3/SmartController_SmartDisplay_ESP32/master/ota_updates/smartcontroller_minimo/firmware.json", function( data ) {
  var items = [];

$.each(data.versions, function(id, item) {
  $("#versionslist").append('<input type="radio" name="version_selected" id="' + item.version + '" value="' + item.version + '"><label>' + item.version + " - " + EpochToDate(item.date) + '</albel></input><br>')
});

//Epoch To Date
function EpochToDate(epoch) {
    if (epoch < 10000000000)
        epoch *= 1000; // convert to milliseconds (Epoch is usually expressed in seconds, but Javascript uses Milliseconds)
    var epoch = epoch + (new Date().getTimezoneOffset() * -1); //for timeZone        
    var dat = new Date(epoch);
    //return dat.toLocaleDateString(undefined, options);
    return dat.toLocaleString();
}
});

</script>

)=====";

// General style elements
ACStyle(ACE_Style1, "");
ACStyle(ACE_Style2, "input[type='button']{background-color:#303F9F; border-color:#303F9F}");
ACStyle(ACE_Style3, "select{width:44%}");
ACStyle(ACE_Style4, ".noorder{width:100%}.noorder label{display:inline-block;width:40%;cursor:pointer;padding:5px}.noorder .noorder input[type='text']{width:40%} .noorder input[type='password']{width:40%} .noorder input[type='text']{width:40%}");

// Settings page
ACSubmit(ACE_SETTINGS_Save, "Save", "/settingssave");
ACSubmit(ACE_SETTINGS_Discard, "Discard", "/settingspage");
//ACElement(ACE_SETTINGS_Js1, "<script type=\"text/javascript\">window.onload=function(){var t=document.querySelectorAll(\"input[type='text']\");for(i=0;i<t.length;i++){var e=t[i].getAttribute(\"placeholder\");e&&t[i].setAttribute(\"class\",e)}};</script>");
ACElement(ACE_SETTINGS_Js1, "");
#include "WifiSettingsPortalSpecs.h"

// Settings sage
ACText(ACE_SETTINGS_SAVE_header, "<h4>Settings has been saved.</h4>", "text-align:center;padding:10px;");
ACSubmit(ACE_SETTINGS_SAVE_confirm, "Ok", "/settingspage");
AutoConnectAux settingsSaveAux("/settingssave", "SmartElec Setting", false, {ACE_Style1, ACE_Style2, ACE_Style3, ACE_Style4, ACE_SETTINGS_SAVE_header, ACE_SETTINGS_SAVE_confirm});

// OTA flash pages
ACText(ACE_OTA_body, "<h4>Available versions</h4><label>If you don't see versions, this is because you need to be connected to an accecc point before loading this page.</label><br><div class='versionslist' id='versionslist'></div>", "padding:10px;");
ACInput(ACE_OTA_version_manual, "0", "Version to flash", "^[0-9]+$", "version_selected_manual", AC_Tag_BR, AC_Input_Text);
ACSubmit(ACE_OTA_submit, "Flash this version", "/otaflash");
ACElement(ACE_OTA_js, JSPAGE);
AutoConnectAux otaPageAux("/otapage", "Firmware update", true, {ACE_Style2, ACE_OTA_body, ACE_OTA_version_manual, ACE_OTA_submit, ACE_OTA_js});

// OTA flash in progress
ACText(ACE_OTA_FLASH_in_progress, "<h4>Flash in progress</h4>The SmartElec device will reboot after flash", "");
AutoConnectAux otaFlashAux("/otaflash", "Flash in progress", true, {ACE_Style2, ACE_OTA_FLASH_in_progress});

void WifiSettingsPortal_setup()
{

  // Responder of root page handled directly from WebServer class.
  server.on("/_ac", []() {
    server.sendHeader("Location", "/settingspage", true);
    server.send(302, "text/plain", "");
    //server.client().stop();
  });

  // Load a custom web page described in JSON as PAGE_ELEMENT and
  // register a handler. This handler will be invoked from
  // AutoConnectSubmit named the Load defined on the same page.
  settingsPageAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("portal where = " + (String)portal.where());
    if ((portal.where() == "/settingspage") || (portal.where() == ""))
    {
      Serial.println("load begin");

      loadConfig(aux);
    }
    Serial.println("load end");
    return String();
  });

  settingsSaveAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("save begin");

    saveConfig(aux);

    return String();
  });

  otaPageAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("flash page");

    return String();
  });

  otaFlashAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("flash process");
    if (arg.hasArg("version_selected") && (arg.arg(0) != "") && (arg.argName(0) == "version_selected"))
    {
      Serial.println("flash process : version_selected = " + (String)arg.arg(0));

      WifiSettingsPortal_shrd->inOtaModeVersion = arg.arg(0).toInt();
      WifiSettingsPortal_shrd->inOtaMode = OTA_SERVER;
    }
    else
    {
      if (arg.hasArg("ACE_OTA_version_manual") && (arg.arg(0) != "") && (arg.argName(0) == "ACE_OTA_version_manual"))
      {
        Serial.println("flash process ACE_OTA_version_manual 0  : arg = " + (String)arg.arg(0));

        WifiSettingsPortal_shrd->inOtaModeVersion = arg.arg(0).toInt();
        WifiSettingsPortal_shrd->inOtaMode = OTA_SERVER;
      }
      else if (arg.hasArg("ACE_OTA_version_manual") && (arg.arg(1) != "") && (arg.argName(1) == "ACE_OTA_version_manual"))
      {
        Serial.println("flash process : ACE_OTA_version_manual 1 = " + (String)arg.arg(1));

        WifiSettingsPortal_shrd->inOtaModeVersion = arg.arg(1).toInt();
        WifiSettingsPortal_shrd->inOtaMode = OTA_SERVER;
      } else
      {
        Serial.println("flash process : arg 0 = " + (String)arg.arg(0));
        Serial.println("flash process : argName 0 = " + (String)arg.argName(0));
        Serial.println("flash process : arg 1 = " + (String)arg.arg(1));
        Serial.println("flash process : argName 1 = " + (String)arg.argName(1));
      }
    }
    return String();
  });

  // In the setup(),
  // Join the custom Web pages and performs begin
  portal.join({settingsPageAux, settingsSaveAux, otaPageAux, otaFlashAux});

  config.auth = AC_AUTH_NONE;
  config.authScope = AC_AUTHSCOPE_AUX;
  config.psk = "12345678";

  //config.immediateStart = true;
  //config.autoReconnect = true;
  config.retainPortal = true;
  config.preserveAPMode = true;
  config.homeUri = "/settingspage";
  //config.menuItems = AC_MENUITEM_RESET | AC_MENUITEM_UPDATE;

  //config.autoRise;           /**< Automatic starting the captive portal */
  //config.autoReset;          /**< Reset ESP8266 module automatically when WLAN disconnected. */
  //config.autoReconnect;      /**< Automatic reconnect with past SSID */
  //config.immediateStart;     /**< Skips WiFi.begin(), start portal immediately */
  //config.retainPortal;       /**< Even if the captive portal times out, it maintains the portal state. */
  //config.preserveAPMode;     /**< Keep existing AP WiFi mode if captive portal won't be started. */
  //config.beginTimeout;   /**< Timeout value for WiFi.begin */
  //config.portalTimeout;  /**< Timeout value for stay in the captive portal */
  //config.reconnectInterval;  /**< Auto-reconnect attempt interval uint */

  config.autoRise = true;

  // fix wifi name ... same as BLE
  uint8_t base_mac_addr[6] = {0};
  char bleName[20];
  esp_efuse_mac_get_default(base_mac_addr);
  sprintf(bleName, "Smart-%x",
          base_mac_addr[5]);
  config.apid = bleName;

  //config.ota;
  config.title = "SmartElec";

  portal.config(config);
  portal.begin();

  Serial.println("wifi portal open !");
}

void WifiSettingsPortal_loop()
{
  portal.handleClient();
}

void WifiSettingsPortal_close()
{
  portal.end();
  server.close();
  Serial.println("Wifi portal closed");
}

void WifiSettingsPortal_setSettings(Settings *set)
{
  WifiSettingsPortal_settings = set;
}

void WifiSettingsPortal_setSharedData(SharedData *set)
{
  WifiSettingsPortal_shrd = set;
}