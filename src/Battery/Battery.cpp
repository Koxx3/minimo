//////------------------------------------
////// Battery lib and battery level calculations

#include "Battery.h"
#include <Arduino.h>
#include "debug.h"

int OldMillis = 0;
uint8_t OldPercent = 0;

Battery::Battery(Settings *settings)
{

	if (settings != NULL)
	{
		if (settings->get_Battery_nominal_voltage() == settings->LIST_Battery_nominal_voltage_36v_42v_full___10s)
		{
			this->minVoltage = 30000;
			this->maxVoltage = 42000;
			this->nbcells = 10;
		}
		else if (settings->get_Battery_nominal_voltage() == settings->LIST_Battery_nominal_voltage_48v_546v_full___13s)
		{
			this->minVoltage = 39000;
			this->maxVoltage = 54600;
			this->nbcells = 13;
		}
		else if (settings->get_Battery_nominal_voltage() == settings->LIST_Battery_nominal_voltage_52v_588v_full___14s)
		{
			this->minVoltage = 42000;
			this->maxVoltage = 58800;
			this->nbcells = 14;
		}
		else if (settings->get_Battery_nominal_voltage() == settings->LIST_Battery_nominal_voltage_60v_672v_full___16s)
		{
			this->minVoltage = 48000;
			this->maxVoltage = 67200;
			this->nbcells = 16;
		}
		else if (settings->get_Battery_nominal_voltage() == settings->LIST_Battery_nominal_voltage_72v_84v_full___20s)
		{
			this->minVoltage = 60000;
			this->maxVoltage = 84000;
			this->nbcells = 20;
		}
		else if (settings->get_Battery_nominal_voltage() == settings->LIST_Battery_nominal_voltage_86v_1008v_full___24s)
		{
			this->minVoltage = 72000;
			this->maxVoltage = 108000;
			this->nbcells = 24;
		}
		else
		{
			this->minVoltage = 42000;
			this->maxVoltage = 58800;
			this->nbcells = 14;
		}
	}
}

uint8_t Battery::level(uint32_t voltage)
{

	if (voltage < minVoltage)
	{
		return 0;
#if DEBUG_DISPLAY_BATTERY
		Serial.printf("1- minVoltage = %d / maxVoltage = %d / nbcells = %d / voltage = %d / percent = %d\n", minVoltage, maxVoltage, nbcells, voltage, 0);
#endif
	}
	else if (voltage > maxVoltage)
	{
#if DEBUG_DISPLAY_BATTERY
		Serial.printf("2- minVoltage = %d / maxVoltage = %d / nbcells = %d / voltage = %d / percent = %d\n", minVoltage, maxVoltage, nbcells, voltage, 100);
#endif
		return 100;
	}
	//Jo's percentage V6.1
	else
	{
		float voltByCell = (voltage / nbcells);
		voltByCell /= 1000;
		int currentMillis = millis();
		uint8_t newPercent = max(0, (int)round(-1439.34794384241 * pow(voltByCell, 6) + 31480.0982535788 * pow(voltByCell, 5) - 286201.985664439 * pow(voltByCell, 4) + 1384323.16517329 * pow(voltByCell, 3) - 3756691.18815474 * pow(voltByCell, 2) + 5422659.60130914 * voltByCell - 3252516.81708377));

		uint8_t Intervalle = 60;
		if (newPercent < 30)
			Intervalle = 20;

		if (newPercent == OldPercent || currentMillis < 5000)
		{

#if DEBUG_DISPLAY_BATTERY
			Serial.printf("3- minVoltage = %d / maxVoltage = %d / nbcells = %d / voltage = %d / percent = %d\n", minVoltage, maxVoltage, nbcells, voltage, newPercent);
#endif

			return newPercent; // dans les 5 secondes après le startup, la tension se stabilise et on n'a pas tiré sur la batterie donc on prends la valeur "crue"
		}
		else if (OldPercent == 0)
		{
			OldPercent = newPercent; // Après les 5 secondes, old doit être initialisé

#if DEBUG_DISPLAY_BATTERY
			Serial.printf("4- minVoltage = %d / maxVoltage = %d / nbcells = %d / voltage = %d / percent = %d\n", minVoltage, maxVoltage, nbcells, voltage, OldPercent);
#endif
		}
		else if (currentMillis - OldMillis > Intervalle * 1000)
		{ // toutes les 60 sec, on abaisse ou remonte le % si jamais la tension à varié. Basé sur le fait qu'on ne peut pas cramer plus d'1% de batt en 60 sec dans une utilisation normale.
			OldMillis = currentMillis;
			if (newPercent > OldPercent)
				OldPercent++;
			else
				OldPercent--;
		}

		OldPercent = constrain(OldPercent, 0, 100);

#if DEBUG_DISPLAY_BATTERY
		Serial.printf("5- minVoltage = %d / maxVoltage = %d / nbcells = %d / voltage = %d / percent = %d\n", minVoltage, maxVoltage, nbcells, voltage, OldPercent);
#endif
		return OldPercent;
	}
	//Jo's percentage V6.1
}
