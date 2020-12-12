#pragma once

#include <uFire_SHT20.h>

class uFire_SHT20_MP
{
public:
  float value;
  uFire_SHT20_MP(){}
  void begin(uFire_SHT20 *sht20);
  String processMsgPack(String json);
private:
  uFire_SHT20 *sht20;
  String air_temp();
  String air_humidity();
  String air_connected();
};
