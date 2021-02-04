#ifndef BATTERY_H_
#define BATTERY_H_

#include <Arduino.h>

class Battery {
	public:
		/**
		 * Creates an instance to monitor battery voltage and level.
		 * Initialization parameters depend on battery type and configuration.
		 *
		 * @param minVoltage is the voltage, expressed in millivolts, corresponding to an empty battery
		 * @param maxVoltage is the voltage, expressed in millivolts, corresponding to a full battery
		 */
		Battery(uint32_t minVoltage, uint32_t maxVoltage);

		/**
		 * Returns the current battery level as a number between 0% and 100%
		 */
		uint8_t level(uint32_t voltage);

	private:
		uint32_t minVoltage;
		uint32_t maxVoltage;
};
#endif // BATTERY_H_
