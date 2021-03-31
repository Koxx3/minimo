#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <AutoConnect.h>
#include "Settings.h"
#include <time.h>
#include "SharedData.h"
#include "BLE/BluetoothHandler.h"
#include "main.h"
#include <WebSocketsServer.h>
#include "ArduinoJson.h"

#include "WifiSettingsPortalDashboard.h"

using WebServerClass = WebServer;

Settings *WifiSettingsPortal_settings;
SharedData *WifiSettingsPortal_shrd;
BluetoothHandler *WifiSettingsPortal_blh;

WebServerClass server;
WebSocketsServer webSocket = WebSocketsServer(81);
AutoConnect portal(server);
AutoConnectConfig config;

static const char JSPAGE[] PROGMEM = R"=====(
  
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<script type='text/javascript'>

$.getJSON( "https://raw.githubusercontent.com/Koxx3/SmartController_SmartDisplay_ESP32/master/ota_updates/smartcontroller_minimo/firmware.json", function( data ) {
  var items = [];

$.each(data.versions, function(id, item) {
  $("#versionslist").append('<input type="radio" name="version_selected" id="' + item.version + '" value="' + item.version + '"><label>' + item.version + " - " + EpochToDate(item.date) + '</albel></input><br>')
});

$("#versions_not_available").hide();
$('label[for="ACE_OTA_version_manual"]').hide();
$("#ACE_OTA_version_manual").hide();

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
ACStyle(ACE_Style1, "label{display:inline-block;padding-right: 10px !important;padding-left: 0px !important;}");
ACStyle(ACE_Style2, "input[type='button']{background-color:#303F9F; border-color:#303F9F}");
ACStyle(ACE_Style3, "select{width:44%} h2{ color:#303F9F;padding:10px; }");
ACStyle(ACE_Style4, ".noorder{width:100%}.noorder label{display:inline-block;width:40%;cursor:pointer;padding:5px}.noorder .noorder input[type='text']{width:40%} .noorder input[type='password']{width:40%} .noorder input[type='text']{width:40%}");
ACStyle(ACE_Style5, "input[type='text']{paddingLeft:10px}");
ACStyle(ACE_Style6, CSS_DASHBOARD_PAGE);

// Settings page
ACSubmit(ACE_SETTINGS_Save, "Save", "/settingssave");
ACSubmit(ACE_SETTINGS_Discard, "Discard", "/settingspage");
//ACElement(ACE_SETTINGS_Js1, "<script type=\"text/javascript\">window.onload=function(){var t=document.querySelectorAll(\"input[type='text']\");for(i=0;i<t.length;i++){var e=t[i].getAttribute(\"placeholder\");e&&t[i].setAttribute(\"class\",e)}};</script>");
ACElement(ACE_SETTINGS_Js1, "");
#include "WifiSettingsPortalSpecs.h"

// Settings sage
ACText(ACE_SETTINGS_SAVE_header, "<h4>Settings has been saved.</h4>", "text-align:center");
ACSubmit(ACE_SETTINGS_SAVE_confirm, "Ok", "/settingspage");
AutoConnectAux settingsSaveAux("/settingssave", "SmartElec settings", false, {ACE_Style2, ACE_Style3, ACE_Style4, ACE_SETTINGS_SAVE_header, ACE_SETTINGS_SAVE_confirm});

// Calibration page
ACText(ACE_CALIB_title, "<h2>Brake calibrations</h2>", "", "", AC_Tag_BR);
ACText(ACE_CALIB_text1, "Use only if you have an analog brake lever like Xiaomi.", "", "", AC_Tag_BR);
ACText(ACE_CALIB_text2, "Press and maintain brake lever a fix position and press the page button. It will determine the minimal pressure to start electric brake and maximal pressure for maximum brake.", "", "", AC_Tag_BR);
ACSubmit(ACE_CALIB_submit_min, "Min position", "/calibpage?min=1");
ACSubmit(ACE_CALIB_submit_max, "Max position", "/calibpage?max=1");
AutoConnectAux calibPageAux("/calibpage", "SmartElec calibrations", true, {ACE_Style1, ACE_Style2, ACE_Style4, ACE_Style5, ACE_CALIB_title, ACE_CALIB_text1, ACE_CALIB_text2, ACE_CALIB_submit_min, ACE_CALIB_submit_max});

// Dashboard page
ACElement(ACE_DASHBOARD_html_body, HTML_DASHBOARD_PAGE);
ACElement(ACE_DASHBOARD_js, JS_DASHBOARD_PAGE);
AutoConnectAux dashboardPageAux("/dashboardpage", "SmartElec dashboard", true, {ACE_Style6, ACE_DASHBOARD_html_body, ACE_DASHBOARD_js});

// OTA flash pages
ACText(ACE_OTA_title, "Available versions : ", "", "", AC_Tag_BR);
ACText(ACE_OTA_current_version, "Current version : ", "", "", AC_Tag_BR);
ACText(ACE_OTA_versions_list, "<div class='versionslist' id='versionslist'><div id='versions_not_available' style='padding-left:20px'>Versions list not available when connected directly with the phone. Connect the SmartElec to a wifi access point to list versions or manually enter a verion number.</div>", "", "", AC_Tag_BR);
ACInput(ACE_OTA_version_manual, "0", "Version to flash", "^[0-9]+$", "version_selected_manual", AC_Tag_BR, AC_Input_Text);
ACSubmit(ACE_OTA_submit, "Flash this version", "/otaflash");
ACElement(ACE_OTA_js, JSPAGE);
AutoConnectAux otaPageAux("/otapage", "SmartElec firmware update", true, {ACE_Style1, ACE_Style2, ACE_Style5, ACE_OTA_current_version, ACE_OTA_title, ACE_OTA_versions_list, ACE_OTA_version_manual, ACE_OTA_submit, ACE_OTA_js});

// OTA flash in progress
ACText(ACE_OTA_FLASH_in_progress, "<h4>Flash in progress</h4>The SmartElec device will reboot after flash", "");
AutoConnectAux otaFlashAux("/otaflash", "SmartElec flash in progress", false, {ACE_Style2, ACE_OTA_FLASH_in_progress});

void WifiSettingsPortal_setup()
{

  // Responder of root page handled directly from WebServer class.
  server.on("/_ac", []() {
    server.sendHeader("Location", "/settingspage", true);
    server.send(302, "text/plain", "");
  });

  // Load a custom web page described in JSON as PAGE_ELEMENT and
  // register a handler. This handler will be invoked from
  // AutoConnectSubmit named the Load defined on the same page.
  settingsPageAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("portal where = " + (String)portal.where());
    //    if ((portal.where() == "/settingspage") || (portal.where() == "/settingssave") || (portal.where() == ""))
    //    {
    Serial.println("load begin");

    loadConfig(aux);
    //    }
    //    else
    //    {
    //      Serial.println("where reject : " + (String)portal.where());
    //    }
    Serial.println("load end");
    return String();
  });

  settingsSaveAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("save begin");

    saveConfig(aux);

    WifiSettingsPortal_blh->startBleScan();

    return String();
  });

  calibPageAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("calib page");

    if ((arg.size() >= 2) && (arg.argName(0) == "max"))
    {
      WifiSettingsPortal_shrd->brakeMaxPressureRaw = WifiSettingsPortal_shrd->brakeAnalogValue;
      saveBrakeMaxPressure();
      Serial.println("save max pressure");
    }
    else if ((arg.size() >= 2) && (arg.argName(0) == "min"))
    {
      WifiSettingsPortal_shrd->brakeMinPressureRaw = WifiSettingsPortal_shrd->brakeAnalogValue;
      saveBrakeMinPressure();
      Serial.println("save min pressure");
    }
    else
    {
      Serial.println("calib : no arg match");
    }

    return String();
  });

  otaPageAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("flash page");
    String versionStr = "Current version : " + (String)FIRMWARE_VERSION;
    aux.setElementValue("ACE_OTA_current_version", versionStr);
    //otaPageAux["ACE_OTA_current_version"].value = versionStr;

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
      }
      else
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
  portal.join({dashboardPageAux, settingsPageAux, settingsSaveAux, calibPageAux, otaPageAux, otaFlashAux});

  // fix wifi name ... same as BLE
  uint8_t base_mac_addr[6] = {0};
  char bleName[20];
  esp_efuse_mac_get_default(base_mac_addr);
  sprintf(bleName, "Smart-%x",
          base_mac_addr[5]);

  config.apid = bleName;
  config.auth = AC_AUTH_NONE;
  config.authScope = AC_AUTHSCOPE_AUX;
  config.psk = (String)WifiSettingsPortal_settings->get_Ble_pin_code() + (String)WifiSettingsPortal_settings->get_Ble_pin_code();
  config.retainPortal = true;   /**< Even if the captive portal times out, it maintains the portal state. */
  config.preserveAPMode = true; /**< Keep existing AP WiFi mode if captive portal won't be started. */
  config.homeUri = "/settingspage";
  config.autoReset = false; /**< Reset ESP8266 module automatically when WLAN disconnected. */
  config.menuItems = AC_MENUITEM_CONFIGNEW | AC_MENUITEM_OPENSSIDS | AC_MENUITEM_DISCONNECT | AC_MENUITEM_RESET;
  config.autoRise = true; /**< Automatic starting the captive portal */
  config.title = "SmartElec";

  //config.autoReconnect;      /**< Automatic reconnect with past SSID */
  //config.immediateStart;     /**< Skips WiFi.begin(), start portal immediately */
  config.beginTimeout = 5000;  /**< Timeout value for WiFi.begin */
  config.portalTimeout = 5000; /**< Timeout value for stay in the captive portal */
  //config.reconnectInterval;  /**< Auto-reconnect attempt interval uint */
}

void WifiSettingsPortal_begin()
{

  portal.config(config);
  if (portal.begin())
  {

    webSocket.begin(); // <--- After AutoConnect::begin
    webSocket.onEvent([](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
      switch (type)
      {
      case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
      case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %s\n", num, ip.toString().c_str());
        DynamicJsonDocument doc(1024);
        String str;
        doc["id"] = 1;
        doc["value"] = "hello";
        serializeJson(doc, str);
        webSocket.sendTXT(num, str);
      }
      break;
      case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);
        // send some response to the client

        DynamicJsonDocument doc(1024);
        String str;
        doc["id"] = 2;
        doc["value"] = "reply : " + (String)(millis());
        serializeJson(doc, str);
        Serial.println("str : " + (String)str);
        webSocket.sendTXT(num, str);
        break;
      }
    });
  }
  Serial.println("wifi portal open !");
}

void WifiSettingsPortal_loop()
{
  //Serial.println("wifi portal loop - begin ----------------");
  webSocket.loop();
  portal.handleClient();
  //Serial.print(".");
}

void WifiSettingsPortal_close()
{
  portal.end();
  //server.close();
  Serial.println("Wifi portal closed");
  //WiFi.disconnect(true, false);
  WiFi.mode(WIFI_MODE_NULL);
}

void WifiSettingsPortal_setSettings(Settings *set)
{
  WifiSettingsPortal_settings = set;
}

void WifiSettingsPortal_setSharedData(SharedData *set)
{
  WifiSettingsPortal_shrd = set;
}

void WifiSettingsPortal_setBluetoothHandler(BluetoothHandler *set)
{
  WifiSettingsPortal_blh = set;
}

void WifiSettingsPortal_sendValues()
{

  if (webSocket.connectedClients() > 0)
  {
    DynamicJsonDocument doc(2048);
    String str;

    int32_t power = ((WifiSettingsPortal_shrd->currentActual / 1000.0) * (WifiSettingsPortal_shrd->voltageActual / 1000.0)); // / 1000000;
    power = constrain(power, 0, 65535);

    doc["speedCurrent"] = WifiSettingsPortal_shrd->speedCurrent;
    doc["voltageFilterMean"] = ceil(WifiSettingsPortal_shrd->voltageFilterMean / 100.0);
    doc["currentActual"] = WifiSettingsPortal_shrd->currentActual / 100;
    doc["currentTemperature"] = WifiSettingsPortal_shrd->currentTemperature;
    doc["currentHumidity"] = WifiSettingsPortal_shrd->currentHumidity;
    doc["distanceTrip"] = WifiSettingsPortal_shrd->distanceTrip / 100;
    doc["distanceOdo"] = WifiSettingsPortal_shrd->distanceOdo;
    doc["batteryLevel"] = WifiSettingsPortal_shrd->batteryLevel;
    doc["isLocked"] = WifiSettingsPortal_shrd->isLocked;
    doc["bleBeaconVisible"] = WifiSettingsPortal_shrd->bleBeaconVisible;
    doc["beaconRSSI"] = WifiSettingsPortal_shrd->beaconRSSI;
    doc["bleLockForced"] = WifiSettingsPortal_shrd->bleLockForced;
    doc["modeOrder"] = WifiSettingsPortal_shrd->modeOrder;
    doc["speedLimiter"] = WifiSettingsPortal_shrd->speedLimiter;
    doc["ecoOrder"] = WifiSettingsPortal_shrd->ecoOrder;
    doc["accelOrder"] = WifiSettingsPortal_shrd->accelOrder;
    doc["auxOrder"] = WifiSettingsPortal_shrd->auxOrder;
    doc["brakeSentOrder"] = WifiSettingsPortal_shrd->brakeSentOrder;
    doc["brakeSentOrderFromBLE"] = WifiSettingsPortal_shrd->brakeSentOrderFromBLE;
    doc["brakePressedStatus"] = WifiSettingsPortal_shrd->brakePressedStatus;
    doc["brakeFordidenHighVoltage"] = WifiSettingsPortal_shrd->brakeFordidenHighVoltage;

    serializeJson(doc, str);
    //Serial.println("str : " + (String)str);
    webSocket.sendTXT(0, str);
  }
}