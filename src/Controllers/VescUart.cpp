#include "VescUart.h"
#include "VescConfgenerator.h"
#include "tools/utils.h"
#include <stdint.h>

mc_configuration mcconf;

Settings *VescUart::settings;
SharedData *VescUart::shrd;
BluetoothHandler *VescUart::blh;

uint8_t old_modeOrder = -1;

VescUart::VescUart(void)
{
	nunchuck.valueX = 127;
	nunchuck.valueY = 127;
	nunchuck.lowerButton = false;
	nunchuck.upperButton = false;
}

void VescUart::setSerialPort(Stream *port)
{
	serialPort = port;
}

void VescUart::setDebugPort(Stream *port)
{
	debugPort = port;
}

void VescUart::setup(SharedData *shrd_p, BluetoothHandler *blh_p, Settings *settings_p)
{

	shrd = shrd_p;
	blh = blh_p;
	settings = settings_p;
}

int VescUart::receiveUartMessage(uint8_t *payloadReceived)
{

	// Messages <= 255 starts with "2", 2nd byte is length
	// Messages > 255 starts with "3" 2nd and 3rd byte is length combined with 1st >>8 and then &0xFF

	// Makes no sense to run this function if no serialPort is defined.
	if (serialPort == NULL)
		return -1;

	uint16_t counter = 0;
	uint16_t endMessage = 256;
	bool messageRead = false;
	uint8_t messageReceived[256];
	uint16_t lenPayload = 0;

	uint32_t timeout = millis() + 20; // Defining the timestamp for timeout (100ms before timeout)

	while (millis() < timeout && messageRead == false)
	{
		while (serialPort->available())
		{
			uint8_t byte = serialPort->read();

			if (counter < sizeof(messageReceived))
			{
				messageReceived[counter++] = byte;
				//Serial.print(".");
			}
			else
			{
				//Serial.printf("received more than buffer size : %02x\n", byte);
			}

			if (counter == 2)
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

		//Serial.println();
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

bool VescUart::unpackPayload(uint8_t *message, int lenMes, uint8_t *payload)
{

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

	// Extract payload:
	memcpy(payload, &message[2], message[1]);

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

		return true;
	}
	else
	{
		return false;
	}
}

int VescUart::packSendPayload(uint8_t *payload, int lenPay)
{

	uint16_t crcPayload = crc16(payload, lenPay);
	int count = 0;
	uint8_t messageSend[256];

	if (lenPay <= 256)
	{
		messageSend[count++] = 2;
		messageSend[count++] = lenPay;
	}
	else
	{
		messageSend[count++] = 3;
		messageSend[count++] = (uint8_t)(lenPay >> 8);
		messageSend[count++] = (uint8_t)(lenPay & 0xFF);
	}

	memcpy(&messageSend[count], payload, lenPay);

	count += lenPay;
	messageSend[count++] = (uint8_t)(crcPayload >> 8);
	messageSend[count++] = (uint8_t)(crcPayload & 0xFF);
	messageSend[count++] = 3;
	messageSend[count] = '\0';

	if (debugPort != NULL)
	{
		debugPort->print("Package to send: ");
		serialPrint(messageSend, count);
	}

	// Sending package
	if (serialPort != NULL)
		serialPort->write(messageSend, count);

	// Returns number of send bytes
	return count;
}

bool VescUart::processReadPacket(uint8_t *message)
{

	COMM_PACKET_ID packetId;
	int32_t ind = 0;

	packetId = (COMM_PACKET_ID)message[0];
	message++; // Removes the packetId from the actual message (payload)

	switch (packetId)
	{
	case COMM_GET_VALUES: // Structure defined here: https://github.com/vedderb/bldc/blob/43c3bbaf91f5052a35b75c2ff17b5fe99fad94d1/commands.c#L164
	{
		data.tempMosfet = buffer_get_float16(message, 10.0, &ind);			// 2 bytes - mc_interface_temp_fet_filtered()
		data.tempMotor = buffer_get_float16(message, 10.0, &ind);			// 2 bytes - mc_interface_temp_motor_filtered()
		data.avgMotorCurrent = buffer_get_float32(message, 100.0, &ind);	// 4 bytes - mc_interface_read_reset_avg_motor_current()
		data.avgInputCurrent = buffer_get_float32(message, 100.0, &ind);	// 4 bytes - mc_interface_read_reset_avg_input_current()
		ind += 4;															// Skip 4 bytes - mc_interface_read_reset_avg_id()
		ind += 4;															// Skip 4 bytes - mc_interface_read_reset_avg_iq()
		data.dutyCycleNow = buffer_get_float16(message, 1000.0, &ind);		// 2 bytes - mc_interface_get_duty_cycle_now()
		data.rpm = buffer_get_float32(message, 1.0, &ind);					// 4 bytes - mc_interface_get_rpm()
		data.inpVoltage = buffer_get_float16(message, 10.0, &ind);			// 2 bytes - GET_INPUT_VOLTAGE()
		data.ampHours = buffer_get_float32(message, 10000.0, &ind);			// 4 bytes - mc_interface_get_amp_hours(false)
		data.ampHoursCharged = buffer_get_float32(message, 10000.0, &ind);	// 4 bytes - mc_interface_get_amp_hours_charged(false)
		data.wattHours = buffer_get_float32(message, 10000.0, &ind);		// 4 bytes - mc_interface_get_watt_hours(false)
		data.wattHoursCharged = buffer_get_float32(message, 10000.0, &ind); // 4 bytes - mc_interface_get_watt_hours_charged(false)
		data.tachometer = buffer_get_int32(message, &ind);					// 4 bytes - mc_interface_get_tachometer_value(false)
		data.tachometerAbs = buffer_get_int32(message, &ind);				// 4 bytes - mc_interface_get_tachometer_abs_value(false)
		data.error = message[ind++];										// 1 byte  - mc_interface_get_fault()
		data.pidPos = buffer_get_float32(message, 1000000.0, &ind);			// 4 bytes - mc_interface_get_pid_pos_now()
		data.id = message[ind++];											// 1 byte  - app_get_configuration()->controller_id

		return true;
	}
	case COMM_GET_VALUES_SELECTIVE:
	{
		uint32_t mask = 0xFFFFFFFF;
		return true;
	}

	case COMM_GET_MCCONF_TEMP:
	{
		Serial.println("COMM_GET_MCCONF_TEMP -- received");

		//send_buffer[ind++] = packet_id;
		mcconf.l_current_min_scale = buffer_get_float32_auto(message, &ind);
		Serial.println("l_current_min_scale = " + (String)mcconf.l_current_min_scale);
		mcconf.l_current_max_scale = buffer_get_float32_auto(message, &ind);
		Serial.println("l_current_max_scale = " + (String)mcconf.l_current_max_scale);
		mcconf.l_min_erpm = buffer_get_float32_auto(message, &ind);
		Serial.println("l_min_erpm = " + (String)mcconf.l_min_erpm);
		mcconf.l_max_erpm = buffer_get_float32_auto(message, &ind);
		Serial.println("l_max_erpm = " + (String)mcconf.l_max_erpm);
		mcconf.l_min_duty = buffer_get_float32_auto(message, &ind);
		Serial.println("l_maxl_min_duty_erpm = " + (String)mcconf.l_min_duty);
		mcconf.l_max_duty = buffer_get_float32_auto(message, &ind);
		Serial.println("l_max_duty = " + (String)mcconf.l_max_duty);
		mcconf.l_watt_min = buffer_get_float32_auto(message, &ind);
		Serial.println("l_watt_min = " + (String)mcconf.l_watt_min);
		mcconf.l_watt_max = buffer_get_float32_auto(message, &ind);
		Serial.println("l_watt_max = " + (String)mcconf.l_watt_max);
		mcconf.l_in_current_min = buffer_get_float32_auto(message, &ind);
		Serial.println("l_in_current_min = " + (String)mcconf.l_in_current_min);
		mcconf.l_in_current_max = buffer_get_float32_auto(message, &ind);
		Serial.println("l_in_current_max = " + (String)mcconf.l_in_current_max);
		mcconf.si_motor_poles = buffer_get_uint8(message, &ind);
		Serial.println("si_motor_poles = " + (String)mcconf.si_motor_poles);
		mcconf.si_gear_ratio = buffer_get_float32_auto(message, &ind);
		Serial.println("si_gear_ratio = " + (String)mcconf.si_gear_ratio);
		mcconf.si_wheel_diameter = buffer_get_float32_auto(message, &ind);
		Serial.println("si_wheel_diameter = " + (String)mcconf.si_wheel_diameter);

		return true;
	}

	case COMM_GET_MCCONF:
	{

		Serial.println("COMM_GET_MCCONF -- received");

		//confgenerator_deserialize_mcconf(message, &mcconf);
		Serial.println("l_max_erpm = " + (String)mcconf.l_max_erpm);
		Serial.println("l_temp_fet_end = " + (String)mcconf.l_temp_fet_end);
		Serial.println("l_in_current_max = " + (String)mcconf.l_in_current_max);
		Serial.println("l_current_max = " + (String)mcconf.l_current_max);

		return true;
	}

	default:
	{
		Serial.println("UNKNONW -- received");
		return false;
	}
	}
}

bool VescUart::getVescValues(void)
{

	uint8_t command[1] = {COMM_GET_VALUES};
	uint8_t payload[256];

	if (debugPort != NULL)
	{
		debugPort->println("Command: COMM_GET_VALUES");
	}

	packSendPayload(command, 1);

	int lenPayload = receiveUartMessage(payload);

	if (lenPayload > 55)
	{
		bool read = processReadPacket(payload); //returns true if sucessful

		return read;
	}
	else
	{
		return false;
	}
}

bool VescUart::requestVescValues(void)
{

	uint8_t command[1] = {COMM_GET_VALUES};

	if (debugPort != NULL)
	{
		debugPort->println("Command: COMM_GET_VALUES");
	}

	packSendPayload(command, 1);
	// delay(1); //needed, otherwise data is not read

	return true;
}

bool VescUart::readVescSerialFeedback(void)
{
	uint8_t payload[256];
	int lenPayload = receiveUartMessage(payload);

	if (lenPayload > 55)
	{
		bool read = processReadPacket(payload); //returns true if sucessful

		return read;
	}
	else
	{
		return false;
	}
}

bool VescUart::requestMotorConfigTemp(void)
{

	uint8_t command[1] = {COMM_GET_MCCONF_TEMP};
	uint8_t payload[256];

	if (debugPort != NULL)
	{
		debugPort->println("Command: COMM_GET_MCCONF_TEMP");
	}

	packSendPayload(command, 1);

	int lenPayload = receiveUartMessage(payload);

	if (lenPayload > 2)
	{
		bool read = processReadPacket(payload); //returns true if sucessful

		return read;
	}
	else
	{
		return false;
	}
}

bool VescUart::requestMotorConfig(void)
{

	uint8_t command[1] = {COMM_GET_MCCONF};
	uint8_t payload[256];

	if (debugPort != NULL)
	{
		debugPort->println("Command: COMM_GET_MCCONF");
	}

	packSendPayload(command, 1);

	int lenPayload = receiveUartMessage(payload);

	if (lenPayload > 2)
	{
		bool read = processReadPacket(payload); //returns true if sucessful

		return read;
	}
	else
	{
		return false;
	}
}

void VescUart::setNunchuckValues()
{
	int32_t ind = 0;
	uint8_t payload[11];

	if (debugPort != NULL)
	{
		debugPort->println("Command: COMM_SET_CHUCK_DATA");
	}

	payload[ind++] = COMM_SET_CHUCK_DATA;
	payload[ind++] = nunchuck.valueX;
	payload[ind++] = nunchuck.valueY;
	buffer_append_bool(payload, nunchuck.lowerButton, &ind);
	buffer_append_bool(payload, nunchuck.upperButton, &ind);

	// Acceleration Data. Not used, Int16 (2 byte)
	payload[ind++] = 0;
	payload[ind++] = 0;
	payload[ind++] = 0;
	payload[ind++] = 0;
	payload[ind++] = 0;
	payload[ind++] = 0;

	if (debugPort != NULL)
	{
		debugPort->println("Nunchuck Values:");
		debugPort->print("x=");
		debugPort->print(nunchuck.valueX);
		debugPort->print(" y=");
		debugPort->print(nunchuck.valueY);
		debugPort->print(" LBTN=");
		debugPort->print(nunchuck.lowerButton);
		debugPort->print(" UBTN=");
		debugPort->println(nunchuck.upperButton);
	}

	packSendPayload(payload, 11);
}

void VescUart::setMaxSpeed(uint8_t modeOrder)
{

	int32_t index = 0;
	uint8_t payload[55];

	if ((old_modeOrder != modeOrder) || (shrd->speedLimiter != shrd->speedLimiterOld))
	{
		if (modeOrder == 1)
		{
			mcconf.l_max_erpm = KmhToErpm2(settings, 25 + 0.5);
		}
		if (modeOrder == 2)
		{
			mcconf.l_max_erpm = KmhToErpm2(settings, 33 + 0.5);
		}
		if (modeOrder == 3)
		{
			mcconf.l_max_erpm = 1000000;
		}
		//		Serial.println("mode = " + (String)modeOrder + " / KmhToErpm2(1) = " + (String)KmhToErpm2(settings, 1) + " / l_max_erpm = " + (String)mcconf.l_max_erpm);

		// override speed mode if speed limit is enabled
		if ((shrd->speedLimiter) && (mcconf.l_max_erpm > KmhToErpm2(settings, settings->getS1F().Speed_limiter_max_speed + 0.5)))
		{
			mcconf.l_max_erpm = KmhToErpm2(settings, settings->getS1F().Speed_limiter_max_speed + 0.5);
		}
		shrd->speedLimiterOld = shrd->speedLimiter;

		old_modeOrder = modeOrder;

		payload[index++] = COMM_SET_MCCONF_TEMP;

		payload[index++] = 0; // store
		payload[index++] = 0; // forward_can
		payload[index++] = 0; // ack
		payload[index++] = 0; // divide_by_controllers

		buffer_append_float32_auto(payload, mcconf.l_current_min_scale, &index);
		buffer_append_float32_auto(payload, mcconf.l_current_max_scale, &index);
		buffer_append_float32_auto(payload, mcconf.l_min_erpm, &index);
		buffer_append_float32_auto(payload, mcconf.l_max_erpm, &index);
		buffer_append_float32_auto(payload, mcconf.l_min_duty, &index);
		buffer_append_float32_auto(payload, mcconf.l_max_duty, &index);
		buffer_append_float32_auto(payload, mcconf.l_watt_min, &index);
		buffer_append_float32_auto(payload, mcconf.l_watt_max, &index);
		buffer_append_float32_auto(payload, mcconf.l_in_current_min, &index);
		buffer_append_float32_auto(payload, mcconf.l_in_current_max, &index);
		// Setup config needed for speed calculation
		payload[index++] = (uint8_t)mcconf.si_motor_poles;
		buffer_append_float32_auto(payload, mcconf.si_gear_ratio, &index);
		buffer_append_float32_auto(payload, mcconf.si_wheel_diameter, &index);

		packSendPayload(payload, sizeof(payload));
	}
	
}

void VescUart::setCurrent(float current)
{
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_CURRENT;
	buffer_append_int32(payload, (int32_t)(current * 1000), &index);

	packSendPayload(payload, 5);
}

void VescUart::setBrakeCurrent(float brakeCurrent)
{
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_CURRENT_BRAKE;
	buffer_append_int32(payload, (int32_t)(brakeCurrent * 1000), &index);

	packSendPayload(payload, 5);
}

void VescUart::setRPM(float rpm)
{
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_RPM;
	buffer_append_int32(payload, (int32_t)(rpm), &index);

	packSendPayload(payload, 5);
}

void VescUart::setDuty(float duty)
{
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_DUTY;
	buffer_append_int32(payload, (int32_t)(duty * 100000), &index);

	packSendPayload(payload, 5);
}

void VescUart::setHandBrakeCurrent(float brakeCurrent)
{
	int32_t index = 0;
	uint8_t payload[5];

	payload[index++] = COMM_SET_HANDBRAKE;
	buffer_append_int32(payload, (int32_t)(brakeCurrent * 1000), &index);

	packSendPayload(payload, 5);
}

void VescUart::serialPrint(uint8_t *data, int len)
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

void VescUart::printVescValues()
{
	if (debugPort != NULL)
	{
		debugPort->print("avgMotorCurrent: ");
		debugPort->println(data.avgMotorCurrent);
		debugPort->print("avgInputCurrent: ");
		debugPort->println(data.avgInputCurrent);
		debugPort->print("dutyCycleNow: ");
		debugPort->println(data.dutyCycleNow);
		debugPort->print("rpm: ");
		debugPort->println(data.rpm);
		debugPort->print("inputVoltage: ");
		debugPort->println(data.inpVoltage);
		debugPort->print("ampHours: ");
		debugPort->println(data.ampHours);
		debugPort->print("ampHoursCharged: ");
		debugPort->println(data.ampHoursCharged);
		debugPort->print("wattHours: ");
		debugPort->println(data.wattHours);
		debugPort->print("wattHoursCharged: ");
		debugPort->println(data.wattHoursCharged);
		debugPort->print("tachometer: ");
		debugPort->println(data.tachometer);
		debugPort->print("tachometerAbs: ");
		debugPort->println(data.tachometerAbs);
		debugPort->print("tempMosfet: ");
		debugPort->println(data.tempMosfet);
		debugPort->print("tempMotor: ");
		debugPort->println(data.tempMotor);
		debugPort->print("error: ");
		debugPort->println(data.error);
	}
}