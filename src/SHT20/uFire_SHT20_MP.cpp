#if __has_include("ArduinoJson.h")
#include "uFire_SHT20_MP.h"
#include <ArduinoJson.h>

void uFire_SHT20_MP::begin(uFire_SHT20 *p_sht20)
{
  sht20 = p_sht20;
  sht20->begin();
}

String uFire_SHT20_MP::processMsgPack(String json)
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

String uFire_SHT20_MP::air_temp()
{
  String output;
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonDocument doc(bufferSize);
  doc["at"] = sht20->temperature();
  serializeMsgPack(doc, output);
  return output;
}

String uFire_SHT20_MP::air_humidity()
{
  String output;
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonDocument doc(bufferSize);
  doc["ah"] = sht20->humidity();
  serializeMsgPack(doc, output);
  return output;
}

String uFire_SHT20_MP::air_connected()
{
  String output;
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  DynamicJsonDocument doc(bufferSize);
  doc["ac"] = sht20->connected();
  serializeMsgPack(doc, output);
  return output;
}

#endif