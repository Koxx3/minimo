/*
 Battery.cpp - Battery library
 Copyright (c) 2014 Roberto Lo Giacco.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as 
 published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Battery.h"
#include <Arduino.h>

Battery::Battery(uint32_t minVoltage, uint32_t maxVoltage)
{
	this->minVoltage = minVoltage;
	this->maxVoltage = maxVoltage;
}

void Battery::begin(mapFn_t mapFunction)
{
	this->mapFunction = mapFunction ? mapFunction : &linear;
}

uint8_t Battery::level(uint32_t voltage)
{

	int nbcells = floor(maxVoltage / 4.2 / 1000);

	if (voltage < minVoltage)
	{
		return 0;
	}
	else if (voltage > maxVoltage)
	{
		return 100;
	}
	else
	{
		float voltagePerCell = (voltage / nbcells);
		voltagePerCell /= 1000;
		return (((-1.1688 * pow(voltagePerCell, 3)) + (12.699 * pow(voltagePerCell, 2)) + (-44.736 * voltagePerCell) + 51.488) * 100);
	}
	/*
	if (voltage <= minVoltage) {
		return 0;
	} else if (voltage >= maxVoltage) {
		return 100;
	} else {
		return (*mapFunction)(voltage, minVoltage, maxVoltage);
	}*/
}
