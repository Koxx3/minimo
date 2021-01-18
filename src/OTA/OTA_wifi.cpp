#include "Arduino.h"
#include "ESP32httpUpdate.h"
#include "app_version.h"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


// To inject firmware info into binary file, You have to use following macro according to let
// OTAdrive to detect binary info automatically
#define ProductKey "1046690b-a448-4df4-b959-516576470130"
#define MakeFirmwareInfo(k, v) "&_FirmwareInfo&k=" k "&v=" v "&FirmwareInfo_&"


// void doUpdate(); //WHAT IS THIS ?
bool initOTA = false;
// OTA
void OTA_setup(char* wifi_ssid, char* wifi_pwd)
{
  initOTA = true;
  // put your setup code here, to run once:
    WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_pwd);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.print("Wifi IP:");
  Serial.println(WiFi.localIP());
}

void OTA_loop(char* wifi_ssid, char* wifi_pwd)
{
  if (!initOTA) OTA_setup(wifi_ssid,wifi_pwd);
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8)
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  String url = "http://otadrive.com/DeviceApi/GetEsp8266Update?k=";
  url += ProductKey;
  url += "&s=" + String(chipId);
  url += "&v=" + String(Version);
  
  // mark binary with version
  String markVersion = MakeFirmwareInfo(ProductKey, Version);

  Serial.print("Update url :");
  Serial.println(url);

  t_httpUpdate_return ret = ESPhttpUpdate.update(url, String(Version));
  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    Serial.println("Update faild!");
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("No new update available");
    break;
  // We can't see this, because of reset chip after update OK
  case HTTP_UPDATE_OK:
    Serial.println("Update OK");
    break;

  default:
    break;
  }
  delay(2000);
}

// STD OTA
void std_OTA_setup(char* wifi_ssid, char* wifi_pwd){
  initOTA = true;
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_pwd);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.setHostname("SmartContro_OTA");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
    ESP.restart();
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void std_OTA_loop(char* wifi_ssid, char* wifi_pwd){
  if (!initOTA) std_OTA_setup(wifi_ssid,wifi_pwd);
  ArduinoOTA.handle();
}
