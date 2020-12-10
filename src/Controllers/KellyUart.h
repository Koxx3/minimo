#ifndef _KellyUART_h
#define _KellyUART_h

#include <Arduino.h>
#include "tools/buffer.h"
#include "tools/crc.h"

#define BAUD_RATE_KELLY 19200

// Communication commands
typedef enum
{
	ETS_FLASH_OPEN = 0xF1,
	ETS_FLASH_READ = 0xF2,
	ETS_A2D_BATCH_READ = 0x1b,
	ETS_USER_MONITOR1 = 0x3A,
	ETS_USER_MONITOR2 = 0x3B
} KELLY_COMM_PACKET_ID;

#define ETS_MAX_DATA_LEN 16 //max length,can not be changed

#pragma pack(push, 1)
struct T_Sync_Comm_Buff
{
	unsigned char command;
	unsigned char no_bytes;
	unsigned char data[ETS_MAX_DATA_LEN + 1];
} __attribute__((packed));
#pragma pack(pop)

union KellyBuffer
{
	struct T_Sync_Comm_Buff fields;
	unsigned char buffer[sizeof(struct T_Sync_Comm_Buff)];
};

class KellyUart
{
	/** Struct to store the telemetry data returned by the Kelly */
	struct dataPackage1
	{
		uint8_t TPS_AD;
		uint8_t Brake_AD;
		uint8_t BRK_SW;
		uint8_t FOOT_SW;
		uint8_t FWD_SW;
		uint8_t REV_SW;
		uint8_t HALL_SA;
		uint8_t HALL_SB;
		uint8_t HALL_SC;
		uint8_t B_Voltage;
		uint8_t Motor_Temp;
		uint8_t Controller_temperature;
		uint8_t Setting_direction;
		uint8_t Actual_direction;
		uint8_t Break_SW2;
	};

	/** Struct to store the telemetry data returned by the Kelly */
	struct dataPackage2
	{
		uint16_t Controller_error_state;
		uint16_t Mechanical_speed_in_RPM;
	};


	KellyBuffer Rx_buff, Tx_buff; //define send and receive data buffer field

public:
	/**
		 * @brief      Class constructor
		 */
	KellyUart(void);

	/** Variable to hold measurements returned from Kelly */
	dataPackage1 data1;
	dataPackage2 data2;

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
		 * @brief      Sends a command to KELLY and stores the returned data
		 *
		 * @return     True if successfull otherwise false
		 */
	bool getKellyValues1(void);
	bool getKellyValues2(void);

	/**
		 * @brief      Help Function to print struct dataPackage over Serial for Debug
		 */
	void printKellyValues1(void);
	void printKellyValues2(void);

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
