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

			if (debugPort != NULL)
			{
				debugPort->print("receiveUartMessage : Received : ");
				serialPrint(Rx_buff.buffer, len);
				debugPort->println();
			}
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

	return false; //don't receive data
}

bool KellyUart::unpackPayload(uint8_t *message, int lenMes, uint8_t *payload)
{

	// TODO : check CRC

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

	// Extract payload:
	memcpy(payload, &message[2], message[1]);

	if (debugPort != NULL)
	{
		debugPort->print("unpackPayload : Received : ");
		serialPrint(message, lenMes);
		debugPort->println();

		debugPort->print("unpackPayload : Payload :      ");
		serialPrint(payload, message[1] - 1);
		debugPort->println();
	}

	return true;
}

int KellyUart::packSendPayload(uint8_t *payload, int lenPay) //calculate checksum and transmitter data
{
	uint8_t i, check_sum;
	size_t size = 3;

	Tx_buff.fields.command = payload[0];
	Tx_buff.fields.no_bytes = lenPay;

	check_sum = 0;
	for (i = 0; i < Tx_buff.fields.no_bytes; i++)
		check_sum += Tx_buff.fields.data[i];
	check_sum += Tx_buff.fields.no_bytes;
	check_sum += Tx_buff.fields.command;
	Tx_buff.fields.data[Tx_buff.fields.no_bytes] = check_sum; //load checksum

	if (debugPort != NULL)
	{
		/*
		debugPort->print("packSendPayload : check_sum : ");
		char print_buffer[3];
		sprintf(print_buffer, "%02x ",
				check_sum);
		debugPort->print(print_buffer);
		debugPort->println("");
*/

		debugPort->print("packSendPayload : Sending : ");
		serialPrint(Tx_buff.buffer, size);
		debugPort->println();
	}

	// Sending package
	if (serialPort != NULL)
		serialPort->write(Tx_buff.buffer, size);

	// Returns number of send bytes
	return 1;
}

bool KellyUart::processReadPacket(uint8_t *message)
{

	KELLY_COMM_PACKET_ID packetId;
	int32_t ind = 0;

	packetId = (KELLY_COMM_PACKET_ID)message[0];
	message++; // Removes the packetId from the actual message (payload)

	switch (packetId)
	{
	case ETS_USER_MONITOR1:
	{
		data1.TPS_AD = buffer_get_int8(message, &ind);
		data1.Brake_AD = buffer_get_int8(message, &ind);
		data1.BRK_SW = buffer_get_int8(message, &ind);
		data1.FOOT_SW = buffer_get_int8(message, &ind);
		data1.FWD_SW = buffer_get_int8(message, &ind);
		data1.REV_SW = buffer_get_int8(message, &ind);
		data1.HALL_SA = buffer_get_int8(message, &ind);
		data1.HALL_SB = buffer_get_int8(message, &ind);
		data1.HALL_SC = buffer_get_int8(message, &ind);
		data1.B_Voltage = buffer_get_int8(message, &ind);
		data1.Motor_Temp = buffer_get_int8(message, &ind);
		data1.Controller_temperature = buffer_get_int8(message, &ind);
		data1.Setting_direction = buffer_get_int8(message, &ind);
		data1.Actual_direction = buffer_get_int8(message, &ind);
		data1.Break_SW2 = buffer_get_int8(message, &ind);

		return true;
	}
	case ETS_USER_MONITOR2:
	{
		data2.Controller_error_state = buffer_get_int16(message, &ind);
		data2.Mechanical_speed_in_RPM = buffer_get_int16(message, &ind);

		return true;
	}
	default:
		return false;
	}
}

bool KellyUart::getKellyValues1(void)
{

	uint8_t command[1] = {ETS_USER_MONITOR1};
	uint8_t payload[256];

	if (debugPort != NULL)
	{
		debugPort->println("Command: ETS_USER_MONITOR1");
	}

	packSendPayload(command, 0);

	receiveUartMessage(payload);

	delay(1); //needed, otherwise data is not read

	bool read = processReadPacket(payload); //returns true if sucessful

	return read;
}

bool KellyUart::getKellyValues2(void)
{

	uint8_t command[1] = {ETS_USER_MONITOR2};
	uint8_t payload[256];

	if (debugPort != NULL)
	{
		debugPort->println("Command: ETS_USER_MONITOR1");
	}

	packSendPayload(command, 0);

	receiveUartMessage(payload);

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
	receiveUartMessage(payload);

	bool read = processReadPacket(payload); //returns true if sucessful

	return read;
}

void KellyUart::serialPrint(uint8_t *data, int len)
{
	if (debugPort != NULL)
	{
		for (int i = 0; i < len; i++)
		{
			char print_buffer[3];
			sprintf(print_buffer, "%02x ",
					data[i]);
			debugPort->print(print_buffer);
		}

		debugPort->println("");
	}
}

void KellyUart::printKellyValues1()
{
	if (debugPort != NULL)
	{

		debugPort->print("TPS_AD: ");
		debugPort->println(data1.TPS_AD);
		debugPort->print("Brake_AD: ");
		debugPort->println(data1.Brake_AD);
		debugPort->print("BRK_SW: ");
		debugPort->println(data1.BRK_SW);
		debugPort->print("FOOT_SW: ");
		debugPort->println(data1.FOOT_SW);
		debugPort->print("FWD_SW: ");
		debugPort->println(data1.FWD_SW);
		debugPort->print("REV_SW: ");
		debugPort->println(data1.REV_SW);
		debugPort->print("HALL_SA: ");
		debugPort->println(data1.HALL_SA);
		debugPort->print("HALL_SB: ");
		debugPort->println(data1.HALL_SB);
		debugPort->print("HALL_SC: ");
		debugPort->println(data1.HALL_SC);
		debugPort->print("B_Voltage: ");
		debugPort->println(data1.B_Voltage);
		debugPort->print("Motor_Temp: ");
		debugPort->println(data1.Motor_Temp);
		debugPort->print("Controller_temperature: ");
		debugPort->println(data1.Controller_temperature);
		debugPort->print("Setting_direction: ");
		debugPort->println(data1.Setting_direction);
		debugPort->print("Actual_direction: ");
		debugPort->println(data1.Actual_direction);
		debugPort->print("Break_SW2: ");
		debugPort->println(data1.Break_SW2);
	}
}

void KellyUart::printKellyValues2()
{
	if (debugPort != NULL)
	{

		debugPort->print("Controller_error_state: ");
		debugPort->println(data2.Controller_error_state);
		debugPort->print("Mechanical_speed_in_RPM: ");
		debugPort->println(data2.Mechanical_speed_in_RPM);
	}
}