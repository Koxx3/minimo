#include <stdint.h>
#include "SmartEsc.h"
#include "SharedData.h"

SmartEsc::SmartEsc(void)
{
}

void SmartEsc::setSerialPort(Stream *port)
{
	serialPort = port;
}

void SmartEsc::setDebugPort(Stream *port)
{
	debugPort = port;
}

void SmartEsc::setSettings(Settings *settings_p)
{

	// Init settings
	settings = settings_p;
}

void SmartEsc::setSharedData(SharedData *shrd_p)
{

	// Init shared data
	shrd = shrd_p;
}

int SmartEsc::receiveUartMessage(uint8_t *payloadReceived)
{

	bool messageRead = false;
	unsigned char len = 0; //Number of bytes

	uint32_t timeout = millis() + 10; // Defining the timestamp for timeout (10ms before timeout)

	while (millis() < timeout && messageRead == false)
	{

		while (serialPort->available())
		{
			Rx_buff.buffer[len] = serialPort->read();
			len++;

			if (len >= sizeof(SerialFeedback))
				break;
		}

		if (len >= sizeof(SerialFeedback))
		{

			//			debugPort->print(">> decode");
			//			debugPort->println();

			if (debugPort != NULL)
			{
				debugPort->print("receiveUartMessage : Received : ");
				serialPrint(Rx_buff.buffer, len);

				debugPort->print("len : ");
				debugPort->print(len);
				debugPort->println();
			}

			bool unpacked = false;
			messageRead = true;

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

bool SmartEsc::unpackPayload(uint8_t *message, int lenMes, uint8_t *payload)
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
	memcpy(payload, message, lenMes);

	/*
	if (debugPort != NULL)
	{
		debugPort->print("unpackPayload : Received : ");
		serialPrint(message, lenMes);
		debugPort->println();
	}
	*/

	return true;
}

int SmartEsc::sendPayload() //calculate checksum and transmitter data
{
	int lenPay = sizeof(Tx_buff);

	Tx_buff.fields.Frame_start = SERIAL_START_FRAME_DISPLAY_TO_ESC;
	Tx_buff.fields.Type = 0x01;

	Tx_buff.fields.Throttle = 250;

	// compute brake // TODO : move this elsewhere
	int16_t brakeRange = shrd->brakeMaxPressureRaw - shrd->brakeFilterInitMean;
	int16_t brakeAnalogValue = shrd->brakeAnalogValue;
	if (brakeAnalogValue > shrd->brakeMaxPressureRaw)
		brakeAnalogValue = shrd->brakeMaxPressureRaw;
	int32_t brakeValueWithCabib = (brakeAnalogValue - shrd->brakeFilterInitMean);
	if (brakeValueWithCabib < 0)
		brakeValueWithCabib = 0;
	Tx_buff.fields.Brake = brakeValueWithCabib * 255 / brakeRange;

	//Serial.printf("brakeAnalogValue = %04x / brakeAnalogValue = %d / shrd->brakeFilterInitMean = %d / Tx_buff.fields.Brake = %02x / Tx_buff.fields.Brake = %d\n", brakeAnalogValue, brakeAnalogValue, shrd->brakeFilterInitMean, Tx_buff.fields.Brake, Tx_buff.fields.Brake);

	Tx_buff.fields.CRC8 = (uint8_t)(
		Tx_buff.fields.Frame_start				  ///
		^ Tx_buff.fields.Type					  //
		^ Tx_buff.fields.Destination			  //
		^ Tx_buff.fields.Number_of_ESC			  //
		^ Tx_buff.fields.BMS_protocol			  //
		^ Tx_buff.fields.ESC_Jumps				  //
		^ Tx_buff.fields.Display_Version_Maj	  //
		^ Tx_buff.fields.Display_Version_Main	  //
		^ Tx_buff.fields.Power_ON				  //
		^ Tx_buff.fields.Throttle				  //
		^ Tx_buff.fields.Brake					  //
		^ Tx_buff.fields.Torque					  //
		^ Tx_buff.fields.Brake_torque			  //
		^ Tx_buff.fields.Lock					  //
		^ Tx_buff.fields.Regulator				  //
		^ Tx_buff.fields.Motor_direction		  //
		^ Tx_buff.fields.Hall_sensors_direction	  //
		^ Tx_buff.fields.Ligth_power			  //
		^ Tx_buff.fields.Max_temperature_reduce	  //
		^ Tx_buff.fields.Max_temperature_shutdown //
		^ Tx_buff.fields.Speed_limit_			  //
		^ Tx_buff.fields.Motor_start_speed		  //
	);

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

		debugPort->print("packSendPayload : Sending (" + (String) + ") : ");
		serialPrint(Tx_buff.buffer, lenPay);
		debugPort->println();
	}

	// Sending package
	if (serialPort != NULL)
		serialPort->write(Tx_buff.buffer, lenPay);

	// Returns number of send bytes
	return lenPay;
}

bool SmartEsc::processReadPacket(uint8_t *message)
{

	int32_t ind = 0;

	message++; // Removes the frame start

	data.Type = buffer_get_uint8(message, &ind);
	data.ESC_Version_Maj = buffer_get_uint8(message, &ind);
	data.ESC_Version_Min = buffer_get_uint8(message, &ind);
	data.Throttle = buffer_get_uint8(message, &ind);
	data.Brake = buffer_get_uint8(message, &ind);
	data.Controller_Voltage = buffer_get_uint16(message, &ind);
	data.Controller_Current = buffer_get_uint16(message, &ind);
	data.MOSFET_temperature = buffer_get_uint8(message, &ind);
	data.ERPM = buffer_get_uint16(message, &ind);
	data.Lock_status = buffer_get_uint8(message, &ind);
	data.Ligth_status = buffer_get_uint8(message, &ind);
	data.Regulator_status = buffer_get_uint8(message, &ind);
	data.Phase_1_current_max = buffer_get_uint16(message, &ind);
	data.Phase_1_voltage_max = buffer_get_uint16(message, &ind);
	data.BMS_Version_Maj = buffer_get_uint8(message, &ind);
	data.BMS_Version_Min = buffer_get_uint8(message, &ind);
	data.BMS_voltage = buffer_get_uint16(message, &ind);
	data.BMS_Current = buffer_get_uint16(message, &ind);
	data.BMS_Cells_status_group_1 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_2 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_3 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_4 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_5 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_6 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_7 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_8 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_9 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_10 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_11 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_12 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_13 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_14 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_15 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_16 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_17 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_18 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_19 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_20 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_21 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_22 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_23 = buffer_get_uint8(message, &ind);
	data.BMS_Cells_status_group_24 = buffer_get_uint8(message, &ind);
	data.BMS_Battery_tempature_1 = buffer_get_uint8(message, &ind);
	data.BMS_Battery_tempature_2 = buffer_get_uint8(message, &ind);
	data.BMS_Charge_cycles_full = buffer_get_uint16(message, &ind);
	data.BMS_Charge_cycles_partial = buffer_get_uint16(message, &ind);
	data.Error = buffer_get_uint16(message, &ind);

	return true;
}

bool SmartEsc::readSmartEscValues(void)
{
	uint8_t payload[256];
	receiveUartMessage(payload);

	bool read = processReadPacket(payload); //returns true if sucessful

	return read;
}

void SmartEsc::serialPrint(uint8_t *data, int len)
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

void SmartEsc::printSmartEscValues()
{
	if (debugPort != NULL)
	{
		/*
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
		*/
	}
}
