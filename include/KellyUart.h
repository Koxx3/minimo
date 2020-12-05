#ifndef _KellyUART_h
#define _KellyUART_h

#include <Arduino.h>
#include "datatypes.h"
#include "buffer.h"
#include "crc.h"

#define BAUD_RATE_Kelly 19200.

class KellyUart
{
	/** Struct to store the telemetry data returned by the Kelly */
	struct dataPackage
	{
		float avgMotorCurrent;
		float avgInputCurrent;
		float dutyCycleNow;
		float rpm;
		float inpVoltage;
		float ampHours;
		float ampHoursCharged;
		float wattHours;
		float wattHoursCharged;
		long tachometer;
		long tachometerAbs;
		float tempMosfet;
		float tempMotor;
		uint8_t error;
		float pidPos;
		uint8_t id;
	};


public:
	/**
		 * @brief      Class constructor
		 */
	KellyUart(void);

	/** Variable to hold measurements returned from Kelly */
	dataPackage data;

	/**
		 * @brief      Set the serial port for uart communication
		 * @param      port  - Reference to Serial port (pointer) 
		 */
	void setSerialPort(Stream *port);

	/**
		 * @brief      Set the serial port for debugging
		 * @param      port  - Reference to Serial port (pointer) 
		 */
	void setDebugPort(Stream *port);

	/**
		 * @brief      Help Function to print struct dataPackage over Serial for Debug
		 */
	void printKellyValues(void);

	/**
		 * @brief      Sends a command to Kelly 
		 *
		 * @return     True if successfull otherwise false
		 */
	bool requestKellyValues(void);

	/**
		 * @brief      Read returned values from Kelly
		 *
		 * @return     True if successfull otherwise false
		 */
	bool readKellyValues(void);


private:
	/** Variable to hold the reference to the Serial object to use for UART */
	Stream *serialPort = NULL;

	/** Variable to hold the reference to the Serial object to use for debugging. 
		  * Uses the class Stream instead of HarwareSerial */
	Stream *debugPort = NULL;

	/**
		 * @brief      Packs the payload and sends it over Serial
		 *
		 * @param      payload  - The payload as a unit8_t Array with length of int lenPayload
		 * @param      lenPay   - Length of payload
		 * @return     The number of bytes send
		 */
	int packSendPayload(uint8_t *payload, int lenPay);

	/**
		 * @brief      Receives the message over Serial
		 *
		 * @param      payloadReceived  - The received payload as a unit8_t Array
		 * @return     The number of bytes receeived within the payload
		 */
	int receiveUartMessage(uint8_t *payloadReceived);

	/**
		 * @brief      Verifies the message (CRC-16) and extracts the payload
		 *
		 * @param      message  - The received UART message
		 * @param      lenMes   - The lenght of the message
		 * @param      payload  - The final payload ready to extract data from
		 * @return     True if the process was a success
		 */
	bool unpackPayload(uint8_t *message, int lenMes, uint8_t *payload);

	/**
		 * @brief      Extracts the data from the received payload
		 *
		 * @param      message  - The payload to extract data from
		 * @return     True if the process was a success
		 */
	bool processReadPacket(uint8_t *message);

	/**
		 * @brief      Help Function to print uint8_t array over Serial for Debug
		 *
		 * @param      data  - Data array to print
		 * @param      len   - Lenght of the array to print
		 */
	void serialPrint(uint8_t *data, int len);
};

#endif
