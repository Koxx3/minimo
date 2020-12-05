#include <stdint.h>
#include "KellyUart.h"

KellyUart::KellyUart(void)
{
}

void KellyUart::setSerialPort(Stream *port)
{
	serialPort = port;
}

void KellyUart::setDebugPort(Stream *port)
{
	debugPort = port;
}

int KellyUart::receiveUartMessage(uint8_t *payloadReceived)
{

	bool messageRead = false;
	unsigned char len = 0; //Number of bytes

	uint32_t timeout = millis() + 100; // Defining the timestamp for timeout (100ms before timeout)

	while (millis() < timeout && messageRead == false)
	{

		while (serialPort->available())
		{
			Rx_buff.buffer[len] = serialPort->read();
			len++;
		}

		if (len > sizeof(struct T_Sync_Comm_Buff))
		{

			bool unpacked = false;

			if (messageRead)
			{
				unpacked = unpackPayload(Rx_buff.buffer, len, payloadReceived);
			}

			if (unpacked)
			{
				// Message was read
				return len;
			}
			else
			{
				// No Message Read
				return 0;
			}
		}
	}

	/*
	unsigned char check_sum, k = 0, i = 0, len = 0;
				for (i = 0; i < len; i++)
			{
				check_sum += Rx_buff.fields.data[i];
			}
			check_sum += Rx_buff.fields.no_bytes;
			check_sum += Rx_buff.fields.command;
			if (check_sum != Rx_buff.fields.data[Rx_buff.fields.no_bytes]) //checksum error
				return 0;
			else
				return 1; //OK
*/

	return false; //don't receive data
}

/*
int KellyUart::receiveUartMessage(uint8_t *payloadReceived)
{

	// Makes no sense to run this function if no serialPort is defined.
	if (serialPort == NULL)
		return -1;

	uint16_t counter = 0;
	uint16_t endMessage = 256;
	bool messageRead = false;
	uint8_t messageReceived[256];
	uint16_t lenPayload = 0;

	uint32_t timeout = millis() + 100; // Defining the timestamp for timeout (100ms before timeout)

	while (millis() < timeout && messageRead == false)
	{

		while (serialPort->available())
		{

			messageReceived[counter++] = serialPort->read();

			
			if (counter == 15)
			{

				switch (messageReceived[0])
				{
				case 2:
					endMessage = messageReceived[1] + 5; //Payload size + 2 for sice + 3 for SRC and End.
					lenPayload = messageReceived[1];
					break;

				case 3:
					// ToDo: Add Message Handling > 255 (starting with 3)
					if (debugPort != NULL)
					{
						debugPort->println("Message is larger than 256 bytes - not supported");
					}
					break;

				default:
					if (debugPort != NULL)
					{
						debugPort->println("Unvalid start bit");
					}
					break;
				}
			}

			if (counter >= sizeof(messageReceived))
			{
				break;
			}

			if (counter == endMessage && messageReceived[endMessage - 1] == 3)
			{
				messageReceived[endMessage] = 0;
				if (debugPort != NULL)
				{
					debugPort->println("End of message reached!");
				}
				messageRead = true;
				break; // Exit if end of message is reached, even if there is still more data in the buffer.
			}
		}
	}
	if (messageRead == false && debugPort != NULL)
	{
		debugPort->println("Timeout");
	}

	bool unpacked = false;

	if (messageRead)
	{
		unpacked = unpackPayload(messageReceived, endMessage, payloadReceived);
	}

	if (unpacked)
	{
		// Message was read
		return lenPayload;
	}
	else
	{
		// No Message Read
		return 0;
	}
}
*/

bool KellyUart::unpackPayload(uint8_t *message, int lenMes, uint8_t *payload)
{

	/*
	uint16_t crcMessage = 0;
	uint16_t crcPayload = 0;

	// Rebuild crc:
	crcMessage = message[lenMes - 3] << 8;
	crcMessage &= 0xFF00;
	crcMessage += message[lenMes - 2];

	if (debugPort != NULL)
	{
		debugPort->print("SRC received: ");
		debugPort->println(crcMessage);
	}
*/

	// Extract payload:
	memcpy(payload, &message[2], message[1]);
	/*
	crcPayload = crc16(payload, message[1]);

	if (debugPort != NULL)
	{
		debugPort->print("SRC calc: ");
		debugPort->println(crcPayload);
	}

	if (crcPayload == crcMessage)
	{
		if (debugPort != NULL)
		{
			debugPort->print("Received: ");
			serialPrint(message, lenMes);
			debugPort->println();

			debugPort->print("Payload :      ");
			serialPrint(payload, message[1] - 1);
			debugPort->println();
		}
*/
	return true;
	/*
	}
	else
	{
		return false;
	}
	*/
}

int KellyUart::packSendPayload(uint8_t *payload, int lenPay) //calculate checksum and transmitter data
{
	char i, check_sum;
	size_t size = 3;

	check_sum = 0;
	for (i = 0; i < Tx_buff.fields.no_bytes; i++)
		check_sum += Tx_buff.fields.data[i];
	check_sum += Tx_buff.fields.no_bytes;
	check_sum += Tx_buff.fields.command;
	Tx_buff.fields.data[Tx_buff.fields.no_bytes] = check_sum; //load checksum

	// Sending package
	if (serialPort != NULL)
		serialPort->write(Tx_buff.buffer, size);

	// Returns number of send bytes
	return 1;
}

bool KellyUart::processReadPacket(uint8_t *message)
{

	COMM_PACKET_ID packetId;
	int32_t ind = 0;

	packetId = (COMM_PACKET_ID)message[0];
	message++; // Removes the packetId from the actual message (payload)

	switch (packetId)
	{
	case ETS_USER_MONITOR1:
	{

		data.TPS_AD = buffer_get_int8(message, &ind);
		data.Brake_AD = buffer_get_int8(message, &ind);
		data.BRK_SW = buffer_get_int8(message, &ind);
		data.FOOT_SW = buffer_get_int8(message, &ind);
		data.FWD_SW = buffer_get_int8(message, &ind);
		data.REV_SW = buffer_get_int8(message, &ind);
		data.HALL_SA = buffer_get_int8(message, &ind);
		data.HALL_SB = buffer_get_int8(message, &ind);
		data.HALL_SC = buffer_get_int8(message, &ind);
		data.B_Voltage = buffer_get_int8(message, &ind);
		data.Motor_Temp = buffer_get_int8(message, &ind);
		data.Controller_temperature = buffer_get_int8(message, &ind);
		data.Setting_direction = buffer_get_int8(message, &ind);
		data.Actual_direction = buffer_get_int8(message, &ind);
		data.Break_SW2 = buffer_get_int8(message, &ind);

		return true;
	}
	case ETS_USER_MONITOR2:
	{
		// TODO
	}
	default:
		return false;
	}
}

bool KellyUart::getKellyValues(void)
{

	uint8_t command[1] = {ETS_USER_MONITOR1};
	uint8_t payload[256];

	if (debugPort != NULL)
	{
		debugPort->println("Command: ETS_USER_MONITOR1");
	}

	packSendPayload(command, 1);

	int lenPayload = receiveUartMessage(payload);

	delay(1); //needed, otherwise data is not read

	bool read = processReadPacket(payload); //returns true if sucessful

	return read;
}

bool KellyUart::requestKellyValues(void)
{

	uint8_t command[1] = {ETS_USER_MONITOR1};

	if (debugPort != NULL)
	{
		debugPort->println("Command: ETS_USER_MONITOR1");
	}

	packSendPayload(command, 1);

	return true;
}

bool KellyUart::readKellyValues(void)
{
	uint8_t payload[256];
	int lenPayload = receiveUartMessage(payload);

	bool read = processReadPacket(payload); //returns true if sucessful

	return read;
}

void KellyUart::serialPrint(uint8_t *data, int len)
{
	if (debugPort != NULL)
	{
		for (int i = 0; i <= len; i++)
		{
			debugPort->print(data[i]);
			debugPort->print(" ");
		}

		debugPort->println("");
	}
}

void KellyUart::printKellyValues()
{
	if (debugPort != NULL)
	{

		debugPort->print("TPS_AD: ");
		debugPort->println(data.TPS_AD);
		debugPort->print("Brake_AD: ");
		debugPort->println(data.Brake_AD);
		debugPort->print("BRK_SW: ");
		debugPort->println(data.BRK_SW);
		debugPort->print("FOOT_SW: ");
		debugPort->println(data.FOOT_SW);
		debugPort->print("FWD_SW: ");
		debugPort->println(data.FWD_SW);
		debugPort->print("REV_SW: ");
		debugPort->println(data.REV_SW);
		debugPort->print("HALL_SA: ");
		debugPort->println(data.HALL_SA);
		debugPort->print("HALL_SB: ");
		debugPort->println(data.HALL_SB);
		debugPort->print("HALL_SC: ");
		debugPort->println(data.HALL_SC);
		debugPort->print("B_Voltage: ");
		debugPort->println(data.B_Voltage);
		debugPort->print("Motor_Temp: ");
		debugPort->println(data.Motor_Temp);
		debugPort->print("Controller_temperature: ");
		debugPort->println(data.Controller_temperature);
		debugPort->print("Setting_direction: ");
		debugPort->println(data.Setting_direction);
		debugPort->print("Actual_direction: ");
		debugPort->println(data.Actual_direction);
		debugPort->print("Break_SW2: ");
		debugPort->println(data.Break_SW2);
	}
}