#include "uFire_SHT20.h"

bool uFire_SHT20::begin(uint8_t resolution, uint8_t address, TwoWire &wirePort)
{
  _address = address;
  _resolution = resolution;
  _i2cPort = &wirePort;
  _reset();

  return connected();
}
void uFire_SHT20::_reset()
{
  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_RESET);
  _i2cPort->endTransmission();
  delay(SOFT_RESET_DELAY);
  _onchip_heater = _DISABLE_ONCHIP_HEATER;
  _otp_reload = _DISABLE_OTP_RELOAD;

  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_READ_USER_REG);
  _i2cPort->endTransmission();
  _i2cPort->requestFrom(SHT20_I2C, 1);
  uint8_t config = _i2cPort->read();
  config = ((config & _RESERVED_BITMASK) | _resolution | _onchip_heater | _otp_reload);
  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_WRITE_USER_REG);
  _i2cPort->write(config);
  _i2cPort->endTransmission();
}

void uFire_SHT20::measure_all()
{
  // also measures temp/humidity
  vpd();
  dew_point();
}

float uFire_SHT20::temperature()
{
  _reset();
  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_TEMP);
  _i2cPort->endTransmission();
  delay(TEMPERATURE_DELAY);
  _i2cPort->requestFrom(SHT20_I2C, 2);
  uint8_t msb = _i2cPort->read();
  uint8_t lsb = _i2cPort->read();
  uint16_t value = msb << 8 | lsb;
  tempC = value * (175.72 / 65536.0) - 46.85;
  tempF = ((value * (175.72 / 65536.0) - 46.85) * 1.8) + 32;
  return tempC;
}

void uFire_SHT20::requestTemperature()
{
  //_reset();
  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_TEMP);
  _i2cPort->endTransmission();
}

float uFire_SHT20::readTemperature()
{
  _i2cPort->requestFrom(SHT20_I2C, 2);
  uint8_t msb = _i2cPort->read();
  uint8_t lsb = _i2cPort->read();
  uint16_t value = msb << 8 | lsb;
  tempC = value * (175.72 / 65536.0) - 46.85;
  return tempC;
}

float uFire_SHT20::temperature_f()
{
  _reset();
  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_TEMP);
  _i2cPort->endTransmission();
  delay(TEMPERATURE_DELAY);
  _i2cPort->requestFrom(SHT20_I2C, 2);
  uint8_t msb = _i2cPort->read();
  uint8_t lsb = _i2cPort->read();
  uint16_t value = msb << 8 | lsb;
  tempC = value * (175.72 / 65536.0) - 46.85;
  tempF = ((value * (175.72 / 65536.0) - 46.85) * 1.8) + 32;
  return tempF;
}

float uFire_SHT20::humidity()
{
  _reset();
  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_HUMID);
  _i2cPort->endTransmission();
  delay(HUMIDITY_DELAY);
  _i2cPort->requestFrom(SHT20_I2C, 2);
  uint8_t msb = _i2cPort->read();
  uint8_t lsb = _i2cPort->read();
  uint16_t value = msb << 8 | lsb;
  RH = value * (125.0 / 65536.0) - 6.0;
  return RH;
}


void uFire_SHT20::requestHumidity()
{
  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_HUMID);
  _i2cPort->endTransmission();
}


float uFire_SHT20::readHumidity()
{
  _i2cPort->requestFrom(SHT20_I2C, 2);
  uint8_t msb = _i2cPort->read();
  uint8_t lsb = _i2cPort->read();
  uint16_t value = msb << 8 | lsb;
  RH = value * (125.0 / 65536.0) - 6.0;
  return RH;
}


float uFire_SHT20::vpd()
{
  temperature();
  humidity();

  float es = 0.6108 * exp(17.27 * tempC / (tempC + 237.3));
  float ae = RH / 100 * es;
  vpd_kPa = es - ae;

  return vpd_kPa;
}

float uFire_SHT20::dew_point()
{
  temperature();
  humidity();

  float tem = -1.0 * tempC;
  float esdp = 6.112 * exp(-1.0 * 17.67 * tem / (243.5 - tem));
  float ed = RH / 100.0 * esdp;
  float eln = log(ed / 6.112);
  dew_pointC = -243.5 * eln / (eln - 17.67);

  dew_pointF = (dew_pointC * 1.8) + 32;
  return dew_pointC;
}

bool uFire_SHT20::connected()
{
  _i2cPort->beginTransmission(SHT20_I2C);
  _i2cPort->write(SHT20_READ_USER_REG);
  _i2cPort->endTransmission();
  _i2cPort->requestFrom(SHT20_I2C, 1);
  uint8_t config = _i2cPort->read();

  if (config != 0xFF)
  {
    return true;
  }
  else
  {
    return false;
  }
}