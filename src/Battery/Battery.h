#ifndef BATTERY_H_
#define BATTERY_H_

#include <Arduino.h>
#include "settings.h"

class Battery
{
public:
	/**
		 * Creates an instance to monitor battery voltage and level.
		 * Initialization parameters depend on battery type and configuration.
		 */
	Battery(Settings *settings);

	/**
		 * Returns the current battery level as a number between 0% and 100%
		 */
	uint8_t level(uint32_t voltage);

private:
	uint32_t minVoltage;
	uint32_t maxVoltage;
	uint8_t nbcells = 1;
};
#endif // BATTERY_H_
