//////------------------------------------
////// Battery lib and battery level calculations

#include "Battery.h"
#include <Arduino.h>

int OldMillis = 0;
uint8_t OldPercent = 0;

Battery::Battery(uint32_t minVoltage, uint32_t maxVoltage)
{
	this->minVoltage = minVoltage;
	this->maxVoltage = maxVoltage;
}

uint8_t Battery::level(uint32_t voltage)
{

	int nbcells = max((int)ceil(maxVoltage / 4.2 / 1000),1);

	if (voltage < minVoltage)
	{
		return 0;
	}
	else if (voltage > maxVoltage)
	{
		return 100;
	}
	//Jo's percentage V6.1
	else
	{
		float voltByCell = (voltage / nbcells);
		voltByCell /= 1000;
		int currentMillis = millis();
		uint8_t newPercent = max(0,(int)round(-1439.34794384241 * pow(voltByCell, 6)+ 31480.0982535788 * pow(voltByCell, 5) - 286201.985664439 * pow(voltByCell, 4) + 1384323.16517329 * pow(voltByCell, 3) - 3756691.18815474 * pow(voltByCell, 2) + 5422659.60130914 * voltByCell - 3252516.81708377));
	
		if (newPercent == OldPercent || currentMillis < 5000)
		{
			return newPercent; // dans les 5 secondes après le startup, la tension se stabilise et on n'a pas tiré sur la batterie donc on prends la valeur "crue"
		}
		else if (OldPercent == 0)
		{
			OldPercent = newPercent; // Après les 5 secondes, old doit être initialisé 
		}
		else if (currentMillis - OldMillis > 60000)
		{ // toutes les 60 sec, on abaisse ou remonte le % si jamais la tension à varié. Basé sur le fait qu'on ne peut pas cramer plus d'1% de batt en 60 sec dans une utilisation normale.
			OldMillis = currentMillis;
			if (newPercent > OldPercent)
				OldPercent++;
			else
				OldPercent--;
		}

		return OldPercent;
	}
	//Jo's percentage V6.1
}
