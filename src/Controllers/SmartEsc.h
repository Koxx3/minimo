#ifndef _SmartEsc_h
#define _SmartEsc_h

#include <Arduino.h>
#include "tools/buffer.h"
#include "tools/crc.h"
#include "SharedData.h"
#include "Settings.h"

#define BAUD_RATE_SMARTESC 115200

#define SERIAL_START_FRAME_ESC_TO_DISPLAY 0x5A // [-] Start frame definition for serial commands
#define SERIAL_START_FRAME_DISPLAY_TO_ESC 0xA5 // [-] Start frame definition for serial commands

typedef struct
{
	uint8_t Frame_start;
	uint8_t Type;
	uint8_t Destination;
	uint8_t Number_of_ESC;
	uint8_t BMS_protocol;
	uint8_t ESC_Jumps;
	uint8_t Display_Version_Maj;
	uint8_t Display_Version_Main;
	uint8_t Power_ON;
	uint8_t Throttle;
	uint8_t Brake;
	uint8_t Torque;
	uint8_t Brake_torque;
	uint8_t Lock;
	uint8_t Regulator;
	uint8_t Motor_direction;
	uint8_t Hall_sensors_direction;
	uint8_t Ligth_power;
	uint8_t Max_temperature_reduce;
	uint8_t Max_temperature_shutdown;
	uint8_t Speed_limit_;
	uint8_t Motor_start_speed;
	uint8_t CRC8;
} SerialCommand;

typedef struct
{

	uint8_t Frame_start;
	uint8_t Type;
	uint8_t ESC_Version_Maj;
	uint8_t ESC_Version_Min;
	uint8_t Throttle;
	uint8_t Brake;
	uint8_t Controller_Voltage_LSB;
	uint8_t Controller_Voltage_MSB;
	uint8_t Controller_Current_LSB;
	uint8_t Controller_Current_MSB;
	uint8_t MOSFET_temperature;
	uint8_t ERPM_LSB;
	uint8_t ERPM_MSB;
	uint8_t Lock_status;
	uint8_t Ligth_status;
	uint8_t Regulator_status;
	uint8_t Phase_1_current_max_LSB;
	uint8_t Phase_1_current_max_MSB;
	uint8_t Phase_1_voltage_max_LSB;
	uint8_t Phase_1_voltage_max_MSB;
	uint8_t BMS_Version_Maj;
	uint8_t BMS_Version_Min;
	uint8_t BMS_voltage_LSB;
	uint8_t BMS_voltage_MSB;
	uint8_t BMS_Current_LSB;
	uint8_t BMS_Current_MSB;
	uint8_t BMS_Cells_status_group_1;
	uint8_t BMS_Cells_status_group_2;
	uint8_t BMS_Cells_status_group_3;
	uint8_t BMS_Cells_status_group_4;
	uint8_t BMS_Cells_status_group_5;
	uint8_t BMS_Cells_status_group_6;
	uint8_t BMS_Cells_status_group_7;
	uint8_t BMS_Cells_status_group_8;
	uint8_t BMS_Cells_status_group_9;
	uint8_t BMS_Cells_status_group_10;
	uint8_t BMS_Cells_status_group_11;
	uint8_t BMS_Cells_status_group_12;
	uint8_t BMS_Cells_status_group_13;
	uint8_t BMS_Cells_status_group_14;
	uint8_t BMS_Cells_status_group_15;
	uint8_t BMS_Cells_status_group_16;
	uint8_t BMS_Cells_status_group_17;
	uint8_t BMS_Cells_status_group_18;
	uint8_t BMS_Cells_status_group_19;
	uint8_t BMS_Cells_status_group_20;
	uint8_t BMS_Cells_status_group_21;
	uint8_t BMS_Cells_status_group_22;
	uint8_t BMS_Cells_status_group_23;
	uint8_t BMS_Cells_status_group_24;
	uint8_t BMS_Battery_tempature_1;
	uint8_t BMS_Battery_tempature_2;
	uint8_t BMS_Charge_cycles_full_LSB;
	uint8_t BMS_Charge_cycles_full_MSB;
	uint8_t BMS_Charge_cycles_partial_LSB;
	uint8_t BMS_Charge_cycles_partial_MSB;
	uint8_t Errors_LSB;
	uint8_t Errors_MSB;
	uint8_t CRC8;
} SerialFeedback;

union SmartEscFeedbackBuffer
{
	SerialFeedback fields;
	unsigned char buffer[sizeof(SerialFeedback)];
};

union SmartEscCommandBuffer
{
	SerialCommand fields;
	unsigned char buffer[sizeof(SerialCommand)];
};

class SmartEsc
{
	/** Struct to store the telemetry data returned by the SmartEsc */
	struct dataPackage
	{
		uint8_t Type;
		uint8_t ESC_Version_Maj;
		uint8_t ESC_Version_Min;
		uint8_t Throttle;
		uint8_t Brake;
		uint16_t Controller_Voltage;
		uint16_t Controller_Current;
		uint8_t MOSFET_temperature;
		int16_t ERPM;
		uint8_t Lock_status;
		uint8_t Ligth_status;
		uint8_t Regulator_status;
		uint16_t Phase_1_current_max;
		uint16_t Phase_1_voltage_max;
		uint8_t BMS_Version_Maj;
		uint8_t BMS_Version_Min;
		uint16_t BMS_voltage;
		uint16_t BMS_Current;
		uint8_t BMS_Cells_status_group_1;
		uint8_t BMS_Cells_status_group_2;
		uint8_t BMS_Cells_status_group_3;
		uint8_t BMS_Cells_status_group_4;
		uint8_t BMS_Cells_status_group_5;
		uint8_t BMS_Cells_status_group_6;
		uint8_t BMS_Cells_status_group_7;
		uint8_t BMS_Cells_status_group_8;
		uint8_t BMS_Cells_status_group_9;
		uint8_t BMS_Cells_status_group_10;
		uint8_t BMS_Cells_status_group_11;
		uint8_t BMS_Cells_status_group_12;
		uint8_t BMS_Cells_status_group_13;
		uint8_t BMS_Cells_status_group_14;
		uint8_t BMS_Cells_status_group_15;
		uint8_t BMS_Cells_status_group_16;
		uint8_t BMS_Cells_status_group_17;
		uint8_t BMS_Cells_status_group_18;
		uint8_t BMS_Cells_status_group_19;
		uint8_t BMS_Cells_status_group_20;
		uint8_t BMS_Cells_status_group_21;
		uint8_t BMS_Cells_status_group_22;
		uint8_t BMS_Cells_status_group_23;
		uint8_t BMS_Cells_status_group_24;
		uint8_t BMS_Battery_tempature_1;
		uint8_t BMS_Battery_tempature_2;
		uint16_t BMS_Charge_cycles_full;
		uint16_t BMS_Charge_cycles_partial;
		uint16_t Error;
	};

	SmartEscCommandBuffer Tx_buff; //define send and receive data buffer field
	SmartEscFeedbackBuffer Rx_buff;

public:
	/**
		 * @brief      Class constructor
		 */
	SmartEsc(void);


    void setSettings(Settings *);
    void setSharedData(SharedData *);

	/** Variable to hold measurements returned from SmartEsc */
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
	void printSmartEscValues(void);

	/**
		 * @brief      Read returned values from SmartEsc
		 *
		 * @return     True if successfull otherwise false
		 */
	bool readSmartEscValues(void);

	
	int sendPayload();

private:
	/** Variable to hold the reference to the Serial object to use for UART */
	Stream *serialPort = NULL;

	/** Variable to hold the reference to the Serial object to use for debugging. 
		  * Uses the class Stream instead of HarwareSerial */
	Stream *debugPort = NULL;

    SharedData *shrd;
    Settings *settings;

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
