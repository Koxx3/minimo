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

using WebServerClass = WebServer;

Settings *WifiSettingsPortal_settings;
SharedData *WifiSettingsPortal_shrd;
BluetoothHandler *WifiSettingsPortal_blh;

WebServerClass server;
WebSocketsServer webSocket = WebSocketsServer(81);
AutoConnect portal(server);
AutoConnectConfig config;

// General style elements
ACStyle(ACE_Style1, "label{display:inline-block;padding-right: 10px !important;padding-left: 0px !important;}");
ACStyle(ACE_Style2, "input[type='button']{background-color:#303F9F; border-color:#303F9F}");
ACStyle(ACE_Style3, "select{width:44%} h2{ color:#303F9F;padding:10px; }");
ACStyle(ACE_Style4, ".noorder{width:100%}.noorder label{display:inline-block;width:40%;cursor:pointer;padding:5px}.noorder .noorder input[type='text']{width:40%} .noorder input[type='password']{width:40%} .noorder input[type='text']{width:40%}");

// Settings page
ACSubmit(ACE_SETTINGS_Save, "Save", "/settingssave");
ACSubmit(ACE_SETTINGS_Discard, "Discard", "/settingspage");
ACElement(ACE_SETTINGS_Js1, "");
#include "WifiSettingsPortalSpecs.h"

// Settings sage
ACText(ACE_SETTINGS_SAVE_header, "<h4>Settings has been saved.</h4>", "text-align:center");
ACSubmit(ACE_SETTINGS_SAVE_confirm, "Ok", "/settingspage");
AutoConnectAux settingsSaveAux("/settingssave", "SmartElec settings", false, {ACE_Style2, ACE_Style3, ACE_Style4, ACE_SETTINGS_SAVE_header, ACE_SETTINGS_SAVE_confirm});

//format bytes
String formatBytes(size_t bytes)
{
  if (bytes < 1024)
  {
    return String(bytes) + "B";
  }
  else if (bytes < (1024 * 1024))
  {
    return String(bytes / 1024.0) + "KB";
  }
  else if (bytes < (1024 * 1024 * 1024))
  {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }
  else
  {
    return String(bytes / 1024.0 / 1024.0 / 1024.0) + "GB";
  }
}

String getContentType(String filename)
{
  if (server.hasArg("download"))
  {
    return "application/octet-stream";
  }
  else if (filename.endsWith(".htm"))
  {
    return "text/html";
  }
  else if (filename.endsWith(".html"))
  {
    return "text/html";
  }
  else if (filename.endsWith(".css"))
  {
    return "text/css";
  }
  else if (filename.endsWith(".js"))
  {
    return "application/javascript";
  }
  else if (filename.endsWith(".png"))
  {
    return "image/png";
  }
  else if (filename.endsWith(".gif"))
  {
    return "image/gif";
  }
  else if (filename.endsWith(".jpg"))
  {
    return "image/jpeg";
  }
  else if (filename.endsWith(".ico"))
  {
    return "image/x-icon";
  }
  else if (filename.endsWith(".xml"))
  {
    return "text/xml";
  }
  else if (filename.endsWith(".pdf"))
  {
    return "application/x-pdf";
  }
  else if (filename.endsWith(".zip"))
  {
    return "application/x-zip";
  }
  else if (filename.endsWith(".js.gz"))
  {
    return "application/javascript";
  }
  else if (filename.endsWith(".gz"))
  {
    return "application/x-gzip";
  }
  return "text/plain";
}

bool exists(String path)
{
  bool yes = false;
  File file = SPIFFS.open(path, "r");
  if (!file.isDirectory())
  {
    yes = true;
  }
  file.close();
  return yes;
}

bool handleFileRead(String path)
{
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/"))
  {
    path += "index.htm";
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (exists(pathWithGz) || exists(path))
  {
    if (exists(pathWithGz))
    {
      path += ".gz";
    }
    File file = SPIFFS.open(path, "r");
    server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

void WifiSettingsPortal_setup()
{

  Serial.setDebugOutput(true);
  SPIFFS.begin();
  {
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
      String fileName = file.name();
      size_t fileSize = file.size();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
      file = root.openNextFile();
    }
    Serial.printf("\n");
  }

  // Responder to pages
  server.on("/_ac", []() {
    server.sendHeader("Location", "/settingspage", true);
    server.send(302, "text/plain", "");
  });
  server.on("/dashboardpage", HTTP_GET, []() {
    if (!handleFileRead("/dash.html"))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  server.on("/calibpage", HTTP_GET, []() {
    if (!handleFileRead("/calib.html"))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  server.on("/otapage", HTTP_GET, []() {
    if (!handleFileRead("/ota.html"))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });
  server.on("/otaflash", HTTP_GET, []() {
    if (!handleFileRead("/otainprogress.html"))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  // launch OTA flash
  server.on("/otaflash", HTTP_POST, []() {
    bool success = false;

    Serial.println("flash process");
    if (server.arg("version_selected") != "")
    {
      //Serial.println("flash process : version_selected = " + (String)server.arg("version_selected"));

      WifiSettingsPortal_shrd->inOtaModeVersion = server.arg("version_selected").toInt();
      WifiSettingsPortal_shrd->inOtaMode = OTA_SERVER;

      success = true;
    }
    else if (server.arg("ACE_OTA_version_manual") != "")
    {
      //Serial.println("flash process ACE_OTA_version_manual : arg = " + (String)server.arg("ACE_OTA_version_manual"));

      WifiSettingsPortal_shrd->inOtaModeVersion = server.arg("ACE_OTA_version_manual").toInt();
      WifiSettingsPortal_shrd->inOtaMode = OTA_SERVER;

      success = true;
    }

    if (success)
    {
      server.sendHeader("Location", "/otainprogress.html", true);
      server.send(302, "text/plain", "");
    }
    else
    {
      return String("error");
    }

    return String();
  });

  // get firmware version
  server.on("/getversion", HTTP_GET, []() {
    Serial.println("getversion");
    DynamicJsonDocument doc(50);
    String str;
    doc["version"] = FIRMWARE_VERSION;
    serializeJson(doc, str);
    server.send(200, "application/json", str);
    return String();
  });

  // calibration
  server.on("/calibpage", HTTP_POST, []() {
    Serial.println("calib page");

    if (server.arg("max") != "")
    {
      WifiSettingsPortal_shrd->brakeMaxPressureRaw = WifiSettingsPortal_shrd->brakeAnalogValue;
      saveBrakeMaxPressure();
      Serial.println("save max pressure");

      server.sendHeader("Location", "/calibpage?calibmax=ok", true);
      server.send(302, "text/plain", "");

    }
    else if (server.arg("min") != "")
    {
      WifiSettingsPortal_shrd->brakeMinPressureRaw = WifiSettingsPortal_shrd->brakeAnalogValue;
      saveBrakeMinPressure();
      Serial.println("save min pressure");
      
      server.sendHeader("Location", "/calibpage?calibmin=ok", true);
      server.send(302, "text/plain", "");
    }
    else
    {
      Serial.println("calib : no arg match");
    }
    return String();
  });

  // Load a custom web page described in JSON as PAGE_ELEMENT and
  // register a handler. This handler will be invoked from
  // AutoConnectSubmit named the Load defined on the same page.
  settingsPageAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("load begin");
    loadConfig(aux);
    Serial.println("load end");
    return String();
  });

  settingsSaveAux.on([](AutoConnectAux &aux, PageArgument &arg) {
    Serial.println("save begin");
    saveConfig(aux);
    WifiSettingsPortal_blh->startBleScan();
    return String();
  });

  // Default handler for all URIs not defined above
  // Use it to read files from SPIFFS
  // To make AutoConnect recognize the 404 handler, replace it with:
  //called when the url is not defined here
  //use it to load content from SPIFFS
  // server.onNotFound([]() {
  portal.onNotFound([]() {
    if (!handleFileRead(server.uri()))
    {
      server.send(404, "text/plain", "FileNotFound");
    }
  });

  // In the setup(),
  // Join the custom Web pages and performs begin
  portal.append("/dashboardpage", "SmartElec dashboard");
  portal.append("/calibpage", "SmartElec calibration");
  portal.append("/otapage", "SmartElec firmware update");

  portal.join({settingsPageAux, settingsSaveAux /*, calibPageAux, otaPageAux, otaFlashAux*/});

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
      case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);
        // send some response to the client

        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);

        // Test if parsing succeeds.
        if (error)
        {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
          return;
        }

        // Fetch values.
        if (doc.containsKey("modeOrder"))
        {
          WifiSettingsPortal_shrd->modeOrder = doc["modeOrder"];
          Serial.println("WifiSettingsPortal_shrd modeOrder = " + (String)WifiSettingsPortal_shrd->modeOrder);
        }
        else if (doc.containsKey("distanceTrip"))
        {
          WifiSettingsPortal_shrd->distanceTrip = doc["distanceTrip"];
          Serial.println("WifiSettingsPortal_shrd distanceTrip = " + (String)WifiSettingsPortal_shrd->distanceTrip);
        }
        else if (doc.containsKey("bleLockForced"))
        {
          WifiSettingsPortal_shrd->bleLockForced = doc["bleLockForced"];
          Serial.println("WifiSettingsPortal_shrd bleLockForced = " + (String)WifiSettingsPortal_shrd->bleLockForced);
        }
        else if (doc.containsKey("speedLimiter"))
        {
          WifiSettingsPortal_shrd->speedLimiter = doc["speedLimiter"];
          Serial.println("WifiSettingsPortal_shrd speedLimiter = " + (String)WifiSettingsPortal_shrd->speedLimiter);
        }
        else if (doc.containsKey("ecoOrder"))
        {
          WifiSettingsPortal_shrd->ecoOrder = doc["ecoOrder"];
          Serial.println("WifiSettingsPortal_shrd ecoOrder = " + (String)WifiSettingsPortal_shrd->ecoOrder);
        }
        else if (doc.containsKey("accelOrder"))
        {
          WifiSettingsPortal_shrd->accelOrder = doc["accelOrder"];
          Serial.println("WifiSettingsPortal_shrd accelOrder = " + (String)WifiSettingsPortal_shrd->accelOrder);
        }
        else if (doc.containsKey("auxOrder"))
        {
          WifiSettingsPortal_shrd->auxOrder = doc["auxOrder"];
          Serial.println("WifiSettingsPortal_shrd auxOrder = " + (String)WifiSettingsPortal_shrd->auxOrder);
        }
        else if (doc.containsKey("brakeSentOrderFromBLE"))
        {
          WifiSettingsPortal_shrd->brakeSentOrderFromBLE = doc["brakeSentOrderFromBLE"];
          Serial.println("WifiSettingsPortal_shrd brakeSentOrderFromBLE = " + (String)WifiSettingsPortal_shrd->brakeSentOrderFromBLE);
        }
        else if (doc.containsKey("speedMax"))
        {
          WifiSettingsPortal_shrd->speedMax = doc["speedMax"];
          Serial.println("WifiSettingsPortal_shrd speedMax = " + (String)WifiSettingsPortal_shrd->speedMax);
        }

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

    doc["speedCurrent"] = (int)WifiSettingsPortal_shrd->speedCurrent;
    doc["speedMax"] = (int)WifiSettingsPortal_shrd->speedMax;
    doc["voltageFilterMean"] = ceil(WifiSettingsPortal_shrd->voltageFilterMean);
    doc["currentActual"] = WifiSettingsPortal_shrd->currentActual;
    doc["currentTemperature"] = WifiSettingsPortal_shrd->currentTemperature;
    doc["currentHumidity"] = WifiSettingsPortal_shrd->currentHumidity;
    doc["distanceTrip"] = WifiSettingsPortal_shrd->distanceTrip / 100;
    doc["distanceOdo"] = WifiSettingsPortal_shrd->distanceOdo;
    doc["batteryLevel"] = WifiSettingsPortal_shrd->batteryLevel;
    doc["autonomyLeft"] = WifiSettingsPortal_shrd->autonomyLeft;
    doc["isLocked"] = WifiSettingsPortal_shrd->isLocked;
    doc["bleBeaconVisible1"] = WifiSettingsPortal_shrd->bleBeaconVisible1;
    doc["beaconRSSI1"] = WifiSettingsPortal_shrd->beaconRSSI1;
    doc["bleBeaconVisible2"] = WifiSettingsPortal_shrd->bleBeaconVisible2;
    doc["beaconRSSI2"] = WifiSettingsPortal_shrd->beaconRSSI2;
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
    doc["Ebrake_progressive_mode"] = WifiSettingsPortal_settings->Ebrake_progressive_mode;

    doc["time"] = millis();

    serializeJson(doc, str);
    
    //Serial.println("send WifiSettingsPortal_shrd->auxOrder : " + (String)WifiSettingsPortal_shrd->auxOrder);

    webSocket.sendTXT(0, str);
  }
}
