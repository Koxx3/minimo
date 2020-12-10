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


#include "dht_nonblocking.h"


#define DHT_IDLE                  0
#define DHT_BEGIN_MEASUREMENT     1
#define DHT_BEGIN_MEASUREMENT_2   2
#define DHT_DO_READING            3
#define DHT_COOLDOWN              4


/* Number of milliseconds before a new sensor read may be initiated. */
#define COOLDOWN_TIME  2000


/*
 * Constructor for the sensor.  It remembers the pin number and the
 * type of sensor, and initializes internal variables.
 */
DHT_nonblocking::DHT_nonblocking( uint8_t pin, uint8_t type )
	: _pin( pin ),
	  _type( type ),
	  _bit( digitalPinToBitMask( pin ) ),
	  _port( digitalPinToPort( pin ) ),
	  _maxcycles( microsecondsToClockCycles( 1000 ) )
{
  dht_state = DHT_IDLE;

  pinMode( _pin, INPUT );
  digitalWrite( _pin, HIGH );
}



/*
 * Instruct the DHT to begin sampling.  Keep polling until it returns true.
 * The tempearture is in degrees Celsius, and the humidity is in %.
 */
bool DHT_nonblocking::measure( float *temperature, float *humidity )
{
  if( read_nonblocking( ) == true )
  {
    *temperature = read_temperature( );
    *humidity    = read_humidity( );
    return( true );
  }
  else
  {
    return( false );
  }
}



float DHT_nonblocking::read_temperature( ) const
{
  int16_t value;
  float   to_return;

  switch( _type )
  {
  case DHT_TYPE_11:
    value = data[ 2 ];
    to_return = (float) value;
    break;

  case DHT_TYPE_21:
  case DHT_TYPE_22:
    value = ( data[ 2 ] & 0x7f ) << 8;
    value |= data[ 3 ];
    if( ( data[ 2 ] & 0x80 ) != 0 )
    {
      value = -value;
    }
    to_return = ( (float) value ) / 10.0;
    break;

  default:
    to_return = NAN;
    break;
  }

  return( to_return );
}



float DHT_nonblocking::read_humidity( ) const
{
  uint16_t value;
  float   to_return;

  switch( _type )
  {
  case DHT_TYPE_11:
    value =  data[ 0 ];
    to_return = (float) value;
    break;

  case DHT_TYPE_21:
  case DHT_TYPE_22:
    value =  data[ 0 ] << 8;
    value |= data[ 1 ];
    to_return = (float)value / 10.0;
    break;

  default:
    to_return = NAN;
    break;
  }

  return( to_return );
}



/*
 * Expect the input to be at the specified level and return the number
 * of loop cycles spent there.  This is identical to Adafruit's blocking
 * driver.
 */
uint32_t DHT_nonblocking::expect_pulse(bool level) const
{
  uint32_t count = 0;
  // On AVR platforms use direct GPIO port access as it's much faster and better
  // for catching pulses that are 10's of microseconds in length:
  #ifdef __AVR
    uint8_t portState = level ? _bit : 0;
    while ((*portInputRegister(_port) & _bit) == portState) {
      if (count++ >= _maxcycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  // Otherwise fall back to using digitalRead (this seems to be necessary on ESP8266
  // right now, perhaps bugs in direct port access functions?).
  #else
    while (digitalRead(_pin) == level) {
      if (count++ >= _maxcycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  #endif

  return count;
}



/*
 * State machine of the non-blocking read.
 */
bool DHT_nonblocking::read_nonblocking( )
{
  bool status = false;

  switch( dht_state )
  {
  /* We may begin measuring any time. */
  case DHT_IDLE:
    dht_state = DHT_BEGIN_MEASUREMENT;
    break;

  /* Initiate a sensor read.  The read begins by going to high impedance
     state for 250 ms. */
  case DHT_BEGIN_MEASUREMENT:
    digitalWrite( _pin, HIGH );
    /* Reset 40 bits of received data to zero. */
    data[ 0 ] = data[ 1 ] = data[ 2 ] = data[ 3 ] = data[ 4 ] = 0;
    dht_timestamp = millis( );
    dht_state = DHT_BEGIN_MEASUREMENT_2;
    break;

  /* After the high impedance state, pull the pin low for 20 ms. */
  case DHT_BEGIN_MEASUREMENT_2:
    /* Wait for 250 ms. */
    if( millis( ) - dht_timestamp > 250 )
    {
      pinMode( _pin, OUTPUT );
      digitalWrite( _pin, LOW );
      dht_timestamp = millis( );
      dht_state = DHT_DO_READING;
    }
    break;

  case DHT_DO_READING:
    /* Wait for 20 ms. */
    if( millis( ) - dht_timestamp > 20 )
    {
      dht_timestamp = millis( );
      dht_state = DHT_COOLDOWN;
      status = read_data( );
//      if( status != true )
//      {
//        Serial.println( "Reading failed" );
//      }
    }
    break;

  /* If it has been less than two seconds since the last time we read
     the sensor, then let the sensor cool down.. */
  case DHT_COOLDOWN:
    if( millis( ) - dht_timestamp > COOLDOWN_TIME )
    {
      dht_state = DHT_IDLE;
    }
    break;

  default:
    break;
  }

  return( status );
}



/* Read sensor data.  This is identical to Adafruit's blocking driver. */
bool DHT_nonblocking::read_data( )
{
  uint32_t cycles[ 80 ];

  /* Turn off interrupts temporarily because the next sections are timing critical
     and we don't want any interruptions. */
  {
    volatile DHT_interrupt interrupt;

    // End the start signal by setting data line high for 40 microseconds.
    digitalWrite( _pin, HIGH );
    delayMicroseconds( 40 );

    // Now start reading the data line to get the value from the DHT sensor.
    pinMode( _pin, INPUT );
    // Delay a bit to let sensor pull data line low.
    delayMicroseconds( 10 );

    // First expect a low signal for ~80 microseconds followed by a high signal
    // for ~80 microseconds again.
    if( expect_pulse( LOW ) == 0 )
    {
      return( false );
    }
    if( expect_pulse( HIGH ) == 0 )
    {
      return( false );
    }

    // Now read the 40 bits sent by the sensor.  Each bit is sent as a 50
    // microsecond low pulse followed by a variable length high pulse.  If the
    // high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds
    // then it's a 1.  We measure the cycle count of the initial 50us low pulse
    // and use that to compare to the cycle count of the high pulse to determine
    // if the bit is a 0 (high state cycle count < low state cycle count), or a
    // 1 (high state cycle count > low state cycle count). Note that for speed all
    // the pulses are read into a array and then examined in a later step.
    for( int i = 0; i < 80; i += 2 )
    {
      cycles[ i     ] = expect_pulse( LOW );
      cycles[ i + 1 ] = expect_pulse( HIGH );
    }

    /* Timing critical code is now complete. */
  }

  // Inspect pulses and determine which ones are 0 (high state cycle count < low
  // state cycle count), or 1 (high state cycle count > low state cycle count).
  for( int i = 0; i < 40; ++i )
  {
    uint32_t low_cycles  = cycles[ 2 * i     ];
    uint32_t high_cycles = cycles[ 2 * i + 1 ];
    if( ( low_cycles == 0 ) || ( high_cycles == 0 ) )
    {
      return( false );
    }
    data[ i / 8 ] <<= 1;
    // Now compare the low and high cycle times to see if the bit is a 0 or 1.
    if( high_cycles > low_cycles )
    {
      // High cycles are greater than 50us low cycle count, must be a 1.
      data[ i / 8 ] |= 1;
    }
    // Else high cycles are less than (or equal to, a weird case) the 50us low
    // cycle count so this must be a zero.  Nothing needs to be changed in the
    // stored data.
  }

  // Check we read 40 bits and that the checksum matches.
  if( data[ 4 ] == ( ( data[ 0 ] + data[ 1 ] + data[ 2 ] + data[ 3 ]) & 0xFF ) )
  {
    return( true );
  }
  else
  {
    return( false );
  }
}

