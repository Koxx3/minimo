#include "Arduino.h"
#include "ESP32httpUpdate.h"
#include "app_version.h"

// To inject firmware info into binary file, You have to use following macro according to let
// OTAdrive to detect binary info automatically
#define ProductKey "1046690b-a448-4df4-b959-516576470130"
#define MakeFirmwareInfo(k, v) "&_FirmwareInfo&k=" k "&v=" v "&FirmwareInfo_&"


void doUpdate();

void OTA_setup(char* wifi_ssid, char* wifi_pwd)
{
  // put your setup code here, to run once:
  WiFi.begin(wifi_ssid, wifi_pwd);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(150);
  }

  Serial.print("Wifi IP:");
  Serial.println(WiFi.localIP());
}

void OTA_loop()
{
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
