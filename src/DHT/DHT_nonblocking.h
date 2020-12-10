/*
 * DHT11, DHT21, and DHT22 non-blocking library.
 * Based on Adafruit Industries' DHT driver library.
 *
 * (C) 2015 Ole Wolf <wolf@blazingangles.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef _DHT_NONBLOCKING_H
#define _DHT_NONBLOCKING_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#define DHT_TYPE_11  0
#define DHT_TYPE_21  1
#define DHT_TYPE_22  2


class DHT_nonblocking
{
  public:
    DHT_nonblocking( uint8_t pin, uint8_t type );
    bool measure( float *temperature, float *humidity );

  private:
    bool read_data( );
    bool read_nonblocking( );
    float read_temperature( ) const;
    float read_humidity( ) const;

    uint8_t dht_state;
    unsigned long dht_timestamp;
    uint8_t data[ 6 ];
    const uint8_t _pin, _type, _bit, _port;
    const uint32_t _maxcycles;

    uint32_t expect_pulse( bool level ) const;
};


class DHT_interrupt
{
  public:
  DHT_interrupt( )
  {
    noInterrupts( );
  }
  ~DHT_interrupt( )
  {
    interrupts( );
  }
};


#endif /* _DHT_NONBLOCKING_H */

