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

int OldMillis = 0;
uint8_t OldPercent = 0;

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

	int nbcells = ceil(maxVoltage / 4.2 / 1000);

	if (voltage < minVoltage)
	{
		return 0;
	}
	else if (voltage > maxVoltage)
	{
		return 100;
	}
	//Jo's percentage V6
	else
	{
		float voltByCell = (voltage / nbcells);
		voltByCell /= 1000;
		int currentMillis = millis();
		uint8_t newPercent = max(0,(int)round(-1439.34794384241 * pow(voltByCell, 6)+ 31480.0982535788 * pow(voltByCell, 5) - 286201.985664439 * pow(voltByCell, 4) + 1384323.16517329 * pow(voltByCell, 3) - 3756691.18815474 * pow(voltByCell, 2) + 5422659.60130914 * voltByCell - 3252516.81708377));
	
		if (newPercent == OldPercent || currentMillis < 5000)
		{
			return newPercent; // dans les 5 secondes après le startup, la tension se stabilise et on n'a pas baisé la batterie donc on prends la valeur "crue"
		}
		if (OldPercent == 0)
		{
			OldPercent = newPercent; // Après les 5 secondes, old est pas initialisé (si pas cette ligne on retombe à 0 %)
		}
		else if (currentMillis - OldMillis > 20000)
		{ // toutes les 20 sec, on abaisse ou remonte le % si jamais la tension à varié. Basé sur le fait qu'on ne peut pas cramer plus d'1% de batt en 20 sec.
			OldMillis = currentMillis;
			if (newPercent > OldPercent)
				OldPercent++;
			else
				OldPercent--;
		}

		return OldPercent;
	}
	//Jo's percentage V6
}
