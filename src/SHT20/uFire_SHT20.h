#ifndef UFIRE_SHT20
#define UFIRE_SHT20

#include <math.h>

#if defined(PARTICLE)
#include "application.h"
#else // if defined(PARTICLE)
#include <Arduino.h>
#include <Wire.h>
#endif // if defined(PARTICLE)

#define SHT20_I2C 0x40
#define SHT20_TEMP 0xF3
#define SHT20_HUMID 0xF5
#define SHT20_WRITE_USER_REG 0xE6
#define SHT20_READ_USER_REG 0xE7
#define SHT20_RESET 0xFE
#define _DISABLE_ONCHIP_HEATER 0b00000000
#define _ENABLE_OTP_RELOAD 0b00000000
#define _DISABLE_OTP_RELOAD 0b00000010
#define _RESERVED_BITMASK 0b00111000
#define SOFT_RESET_DELAY 20
#define TEMPERATURE_DELAY 100
#define HUMIDITY_DELAY 40
#define SHT20_RESOLUTION_12BITS 0b00000000
#define SHT20_RESOLUTION_11BITS 0b10000001
#define SHT20_RESOLUTION_10BITS 0b10000000
#define SHT20_RESOLUTION_8BITS 0b00000001

class uFire_SHT20
{
public:
  uint8_t RESOLUTION_12BITS = 0b00000000;
  uint8_t RESOLUTION_11BITS = 0b10000001;
  uint8_t RESOLUTION_10BITS = 0b10000000;
  uint8_t RESOLUTION_8BITS = 0b00000001;
  float tempC;
  float tempF;
  float vpd_kPa;
  float dew_pointC;
  float dew_pointF;
  float RH;

  bool begin(uint8_t resolution = SHT20_RESOLUTION_12BITS, uint8_t address = SHT20_I2C, TwoWire &wirePort = Wire);
  float temperature();
  float temperature_f();
  float humidity();
  float vpd();
  float dew_point();
  void measure_all();
  bool connected();
  void requestTemperature();
  float readTemperature();
  void requestHumidity();
  float readHumidity();

private:
  uint8_t _address;
  TwoWire *_i2cPort;
  void _reset();
  uint8_t _resolution;
  uint8_t _onchip_heater;
  uint8_t _otp_reload;
};

#endif // ifndef UFIRE_SHT20
