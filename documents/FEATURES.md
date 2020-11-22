# Features
The SmartController add many features to your escooter.
This list is not complete and many features can be added very kickly by software ... if you have ideas ;)

Just a reminder, it doens't replace the e-scooter controller or display. It's an interface between both.

## Smartphone dashboard
Requirements :
- an Android smartphone (not compatible with Apple phones at the moment, contact me if you have skill please).
- to display current / power : the additional current sensor
- to display temperature / humidity : the additionnal temperature / humidity

I can display on your smartphone most of the e-scooter informations.
You can even remove the EYE display and replace it with a simple on/off switch and a trigger.

Displayed informations :
- Current/max speed
- Current mode (with a customized Z mode)
- Current electric brake level
- Eco mode
- Accelaration mode
- Anti-theft lock status
- Speed limiter status
- Auxiliary relay status
- Voltage and battery percentage
- Actual/max current flowing from the battery
- Computer power in watts
- Current and maximum temperature inside the deck
- Current and maximum humidity inside the deck
- Trip / driving time 
- Distance
- Graphics with speed/current
- Logs from the microcontroller

## Progressive electric brake
Requirements :
- a Xiaomi brake lever or any other lever with linear hall sensor

Warning : 
- not compatible with hydrolic brake lever (please contact me if you know any hydrolic brake lever with linear hall sensor)

With the smartphone, you can se the minimal / maximal brake level you want to achieve with the lever.
The brake pressure will determine the electric brake level applied between min/max.

## Progressive acceleration (NOT READY YET - will require v1.3.1 hardware at minimum)
You can change the acceleration curve to have a more controllable e-scooter at low speed and avoid jerky accelerations.
The acceleration curve (normal or different expentionnals) can be set in the smartphone settings.

## Custom mode
This mode is called 'Z' mode.
You can configure a set of spefic datas for this mode :
- Eco mode
- Accelaration mode
- Max electric brake level

This mode can be enabled with the smartphone or a specific hardware button.

## Controller safety
This option allow you to disable the electric brake if the battery level is too high.
I avoid overcharging the battery if you use the electric brake whereas the battery is already at full voltage.

## Automatic battery saving (EXPERIMENTAL)
This option allow you to reduce performance of the e-scooter whereas the battery is depleting.
It should avoid the LCD flasing when you reach 50/40 percent.

## Anti-theft
You can lock the e-scooter from :
- manually the smartphone dashboard
- automatically when the scooter if too far from the smartphone or not connected to the smartphone
- automatically when the scooter if too far any Bluetooth Low energy device (device configurable)

## Speed limiter
You e-scooter will start with a configurable limited speed.
You can disable this limit from the smartphone dashboard or with a physical button.
If you power down you e-scooter, it can restart with speed limiter or without (configurable in the settings)

## External control with buttons
Many functions can be controlled with physical momentary buttons that you can add to your e-scooter.
The SmartController can handle 1 or 2 buttons.
You can configure the buttons actions in the smartphone settings.

## Auxiliary relay
You can control any external electrical object (lights, horn ...) with this relay (Solid State Relay)
It can be installed with those relays :
- G3VM-31AR/DR (30V/2A max)
- G3VM-61AR1/DR1 (60V/2A max)
- G3VM-101AR1/DR1 (100V/2A max)
This relay can be controlled by the smartphone or a physical button.

## Sensors
There is 2 external possible sensors you can add :
- temperature / humidity sensors. They will help you to understand if you controller is too hot or if you have water in your deck
- current sensor. It will allow you to see the drawned current. It's for geeks ;)

## Performance graphics
It can display performance graphics on your smartphone.

## Error logs
It can display all error logs from the internal microcontroller.
I can help you to understand issues you have with your e-scooter (overheating, damaged cables, humidity traces ...)
