#if __has_include("ArduinoJson.h")
#include "uFire_SHT20_JSON.h"
#include <ArduinoJson.h>

void uFire_SHT20_JSON::begin(uFire_SHT20 *p_sht20)
{
  sht20 = p_sht20;
  sht20->begin();
}

String uFire_SHT20_JSON::processJSON(String json)
{
  String cmd = json.substring(0, json.indexOf(" ", 0));
  cmd.trim();
  json.remove(0, json.indexOf(" ", 0));
  json.trim();
  String parameter = json.substring(0, json.indexOf(" ", 0));
  parameter.trim();

  String value = "";
  if (cmd == "at")           value = air_temp();
  if (cmd == "ah")           value = air_humidity();
  if (cmd == "ac")           value = air_connected();

  if (value != "")
  {
    this->value = value.toFloat();
    return value;
  } else
  {
    this->value = -1;
    return "";
  }
}

String uFire_SHT20_JSON::air_temp()
{
  String output;
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonDocument doc(bufferSize);
  doc["at"] = sht20->temperature();
  serializeJson(doc, output);
  return output;
}

String uFire_SHT20_JSON::air_humidity()
{
  String output;
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonDocument doc(bufferSize);
  doc["ah"] = sht20->humidity();
  serializeJson(doc, output);
  return output;
}

String uFire_SHT20_JSON::air_connected()
{
  String output;
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonDocument doc(bufferSize);
  doc["ac"] = sht20->connected();
  serializeJson(doc, output);
  return output;
}

#endif