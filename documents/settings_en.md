
# Settings

## Escooter characteristics
### Wheel size
Default value : 10.0
Valid configuration : SmartController / SmartDisplay
Usage : Wheel size (in inches) is used to adjust the speed and distance when riding.
### Number of poles pairs
<em>
Default value : 15
Valid configuration : SmartController / SmartDisplay
</em>
Usage : Number of poles are used to compute speed/distance. Check you manual to set the correct value. If the manual specify '30 poles', set 15.
### Battery nominal voltage
<em>
Possible values :
- 36V (42V full / 10S) (value = 0)
- 46,8 (54,6V full / 13S) (value = 1)
- 50,4V (58,8V full / 14S) (value = 2)
- 57.6V (67,2V full / 16S) (value = 3)
- 72V (84V full / 20S) (value = 4)
- 86V (100,8V full / 24S) (value = 5)
Default value : 2
Valid configuration : SmartController / SmartDisplay
</em>
Usage : Battery voltage is used to compute the autonomy left and charge indicator. Check your manuel or charger (it indicate full charge) to set the correct setting.
### Battery maximum distance
<em>
Default value : 40
Valid configuration : SmartController / SmartDisplay
</em>
Usage : Use to compute autonomy. Set your regular max distance with one charge.

## General
### Speed limiter at startup
<em>
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : Enable speed limit after power on by setting a power limit (Dualtron/Speedway/Zeero �) or speed limit (VESC).
### Speed limiter max speed
<em>
Default value : 37
Valid configuration : SmartController / SmartDisplay
</em>
Usage : Power limit is expressed in percent (same power setting as in your manual). For VESC, the speed limit is set to 25 Km/h.
### Bluetooth lock mode
<em>
Possible values :
- None (value = 0)
- Smartphone connected (value = 1)
- Smartphone connected or beacon visible (value = 2)
- Beacon visible (value = 3)
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : Lock the scooter if bluetooth device check is not successful.<br>- None : no bluetooth peripheral check<br>Smartphone connected : your smartphone needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>- Smartphone connected or beacon visible : your smartphone OR a BLE beacon needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>Those modes are use as anti-theft technics. If an thief try to still you escooter, as soon as the escooter is out of range of your bluetooth device, it will cut power of the escooter.
Beacon visible
### PIN code
<em>
Default value : 147258
Valid configuration : SmartController / SmartDisplay
</em>
Usage : Bluetooth PIN code is used when you want to connect a smartphone to your SmartEelc device. Change it to protect your SmartElec device from any anybody connection.
### Beacon Mac Address
<em>
Default value : aa:bb:cc:dd:ee:ff
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Beacon range
<em>
Default value : -80
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Original display speed adjustment (in percent) / Speed adjustment
<em>
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Temperature warning
<em>
Default value : 70
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Humidity warning
<em>
Default value : 80
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Automatic power off / Auto power off
<em>
Possible values :
- None (value = 0)
- 2 min (value = 1)
- 3 min (value = 2)
- 4 min (value = 3)
- 5 min (value = 4)
- 10 min (value = 5)
- 15 min (value = 6)
- 20 min (value = 7)
- 30 min (value = 8)
- 60 min (value = 9)
Default value : 0
Valid configuration : SmartDisplay
</em>
Usage : 

## Smartphone display
### Speed adjustment (in percent) / Speed adjustment
<em>
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Aux relay name
<em>
Possible values :
- AUX (value = 0)
- DUAL (value = 1)
- LIGHT (value = 2)
- HORN (value = 3)
- RELAY (value = 4)
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Display GPS speed instead of escooter speed / Display GPS speed 
<em>
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 

## SmartDisplay
### Default mode at startup / Startup mode
<em>
Default value : 3
Valid configuration : SmartDisplay
</em>
Usage : 
### Default eco mode at startup / Startup eco mode
<em>
Possible values :
- Max (value = 0)
- Medium (value = 1)
- None (value = 2)
Default value : 2
Valid configuration : SmartDisplay
</em>
Usage : 
### Default acceleration / Startup acceleration
<em>
Possible values :
- Maximum (value = 0)
- Very fast (value = 1)
- Fast (value = 2)
- Medium (value = 3)
- Slow (value = 4)
- Very slow (value = 5)
Default value : 0
Valid configuration : SmartDisplay
</em>
Usage : 
### Default electric brake at startup / Startup  ebrake
<em>
Default value : 3
Valid configuration : SmartDisplay
</em>
Usage : 
### PAS enabled
<em>
Default value : 1
Valid configuration : SmartDisplay
</em>
Usage : 
### ABS enabled
<em>
Default value : 0
Valid configuration : SmartDisplay
</em>
Usage : 
### Display brightness
<em>
Default value : 100
Valid configuration : SmartDisplay
</em>
Usage : 
### Display splash screen
<em>
Default value : 1
Valid configuration : SmartDisplay
</em>
Usage : 
### Rotate screen
<em>
Possible values :
- 0 (value = 0)
- 1 (value = 1)
- 2 (value = 2)
- 3 (value = 3)
Default value : 1
Valid configuration : SmartDisplay
</em>
Usage : 

## Electric brake
### Progressive mode
<em>
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Smart brake type / Type
<em>
Possible values :
- Controller (value = 0)
- Smart - Digital brake lever (value = 1)
- Smart - Analog brake lever (value = 2)
- Smart - Digital brake + throttle (exp) (value = 3)
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Min power value / Min value
<em>
Default value : 1
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Max power value / Max value
<em>
Default value : 5
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Time between mode shift
<em>
Default value : 500
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Disabled on high battery voltage / Disabled with bat, HV
<em>
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Disabled percent limit / Disabled with bat. HV
<em>
Default value : 100
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 

## Throttle
### Throttle regeneration
<em>
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Input min voltage (in millivolts) / Input min voltage
<em>
Default value : 650
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Input max voltage (in millivolts) / Input max voltage
<em>
Default value : 3950
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Output min voltage (in millivolts) / Output min voltage
<em>
Default value : 800
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Output max voltage (in millivolts) / Output max voltage
<em>
Default value : 3350
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Output curve
<em>
Possible values :
- Linear (value = 0)
- Exponential 1 (value = 1)
- Exponential 2 (value = 2)
- Exponential 3 (value = 3)
- Exponential 4 (value = 4)
- Custom 6 points (value = 5)
Default value : 2
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Custom output curve points
<em>
Default value : 20,40,60,80
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 

## Escooter buttons
### Button 1 short press action / B1 short press 
<em>
Possible values :
- None (value = 0)
- Speed limit ON/OFF (value = 1)
- Aux ON/OFF (value = 2)
- Mode switch 1/2/3 (value = 3)
- Mode switch 2/3 (value = 4)
- Eco switch NONE/MED/MAX (value = 5)
- Eco switch NONE/MED (value = 6)
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Button 1 long press action / B1 long press 
<em>
Possible values :
- None (value = 0)
- Speed limitation ON/OFF (value = 1)
- Aux ON/OFF (value = 2)
- Mode switch 1/2/3 (value = 3)
- Mode switch 2/3 (value = 4)
- Eco switch NONE/MED/MAX (value = 5)
- Eco switch NONE/MED (value = 6)
- Anti-theft manual lock ON (value = 7)
- Nitro boost continuous (value = 8)
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Button 2 short press action / B2 short press 
<em>
Possible values :
- None (value = 0)
- Speed limitation ON/OFF (value = 1)
- Aux ON/OFF (value = 2)
- Mode switch 1/2/3 (value = 3)
- Mode switch 2/3 (value = 4)
- Eco switch NONE/MED/MAX (value = 5)
- Eco switch NONE/MED (value = 6)
Default value : 0
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Button long press duration
<em>
Default value : 3
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 

## OTA update
### Wifi network name (SSID) / Wifi network name
<em>
Default value : 
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 
### Wifi password
<em>
Default value : 
Valid configuration : SmartController / SmartDisplay
</em>
Usage : 


