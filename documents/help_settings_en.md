
# Manual

## Escooter characteristics
### Wheel size
Default value : 10.0 (inches)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Wheel size (in inches) is used to adjust the speed and distance when riding.<br></b>
                
### Number of poles pairs
Default value : 15<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Number of poles are used to compute speed/distance. Check you manual to set the correct value. If the manual specify '30 poles', set 15.<br></b>
                
### Battery nominal voltage
Possible values : <br>
    - 36V (42V full / 10S) (value = 0)<br>
    - 46,8 (54,6V full / 13S) (value = 1)<br>
    - 50,4V (58,8V full / 14S) (value = 2)<br>
    - 57.6V (67,2V full / 16S) (value = 3)<br>
    - 72V (84V full / 20S) (value = 4)<br>
    - 86V (100,8V full / 24S) (value = 5)<br>
Default value : 2<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Battery voltage is used to compute the autonomy left and charge indicator. Check your manuel or charger (it indicate full charge voltage) to set the correct setting.<br></b>
                
### Battery maximum distance
Default value : 40 (kilometers)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Use to compute autonomy. Set your regular max distance with one charge.<br></b>
                

## General
### Speed limiter at startup
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Enable speed limit after power on by setting a power limit (Dualtron/Speedway/Zeero …) or speed limit (VESC).<br></b>
                
### Speed limiter max speed
Default value : 37<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Power limit is expressed in percent (same power setting as in your manual). For VESC, the speed limit is set to 25 Km/h.<br></b>
                
### Bluetooth lock mode
Possible values : <br>
    - None (value = 0)<br>
    - Smartphone connected (value = 1)<br>
    - Smartphone connected or beacon visible (value = 2)<br>
    - Beacon visible (value = 3)<br>
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Lock the scooter if bluetooth device check is not successful.<br>- None : no bluetooth peripheral check<br>- Smartphone connected : your smartphone needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>- Smartphone connected or beacon visible : your smartphone OR a BLE beacon needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>- Beacon visible : BLE beacon needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>Those modes are use as anti-theft technics. If an thief try to still you escooter, as soon as the escooter is out of range of your bluetooth device, it will cut power of the escooter.
Beacon visible<br></b>
                
### PIN code
Default value : 147258<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Bluetooth PIN code is used when you want to connect a smartphone to your SmartEelc device. Change it to protect your SmartElec device from any anybody connection.<br></b>
                
### Beacon Mac Address
Default value : aa:bb:cc:dd:ee:ff<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : To search a beacon you must specify the beacon mac address. Search on your smartphone store for a 'BLE Scanner', it will scan all Bluetooth devices and the MAC address (an unique identifier) for the beacon, then enter this MAC address in this setting.<br>
You can use any Bluetooth Low Energy device as beacon (any BLE band for example) or use a BLE beacon (10$ on aliexpress).<br></b>
                
### Beacon range
Default value : -80 (dB)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Minimal reception power of the beacon to unlock the escooter. This is in dB, it means a -100 value will be a longer distance than -50.<br></b>
                
### Original display speed adjustment (in percent) / Speed adjustment
Default value : 0 (percent)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Use this value to correct the speed on the escooter display.<br></b>
                
### Temperature warning
Default value : 70 (celcuis)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Trigger a error indicator on the display or the smartphone application if this temperature threshold is reached. If you place the temperature sensor inside the deck, it will warn you if the controller or the battery are too hot.<br></b>
                
### Humidity warning
Default value : 80 (RH)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Trigger a error indicator on the display or the smartphone application if this humidy threshold is reached. If you place the humidity sensor inside the deck, it will warn you if the water is entered inside the deck.<br></b>
                
### Automatic power off / Auto power off
Possible values : <br>
    - None (value = 0)<br>
    - 2 min (value = 1)<br>
    - 3 min (value = 2)<br>
    - 4 min (value = 3)<br>
    - 5 min (value = 4)<br>
    - 10 min (value = 5)<br>
    - 15 min (value = 6)<br>
    - 20 min (value = 7)<br>
    - 30 min (value = 8)<br>
    - 60 min (value = 9)<br>
Default value : 0<br>
Valid configuration : SmartDisplay<br>
<b>Usage : Automatically shutdown the escooter at a fixed time at 0 speed.<br></b>
                

## Smartphone display
### Speed adjustment (in percent) / Speed adjustment
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Use this value to correct the speed on the smartphone display.<br></b>
                
### Aux relay name
Possible values : <br>
    - AUX (value = 0)<br>
    - DUAL (value = 1)<br>
    - LIGHT (value = 2)<br>
    - HORN (value = 3)<br>
    - RELAY (value = 4)<br>
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Change the name on the display and smartphone for the integrated relay action. Exemple : if you connect a light, you can display "light" on the dashboard instead of "aux".<br></b>
                
### Display GPS speed instead of escooter speed / Display GPS speed 
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Display GPS speed instead of escooter speed on the smartphone display.<br></b>
                

## SmartDisplay
### Default mode at startup / Startup mode
Default value : 3<br>
Valid configuration : SmartDisplay<br>
<b>Usage : Default speed mode after escooter power up.<br></b>
                
### Default eco mode at startup / Startup eco mode
Possible values : <br>
    - Max (value = 0)<br>
    - Medium (value = 1)<br>
    - None (value = 2)<br>
Default value : 2<br>
Valid configuration : SmartDisplay<br>
<b>Usage : Default eco mode after escooter power up.<br></b>
                
### Default acceleration / Startup acceleration
Possible values : <br>
    - Maximum (value = 0)<br>
    - Very fast (value = 1)<br>
    - Fast (value = 2)<br>
    - Medium (value = 3)<br>
    - Slow (value = 4)<br>
    - Very slow (value = 5)<br>
Default value : 0<br>
Valid configuration : SmartDisplay<br>
<b>Usage : Default acceleration mode after escooter power up.<br></b>
                
### Default electric brake at startup / Startup  ebrake
Default value : 3<br>
Valid configuration : SmartDisplay<br>
<b>Usage : Default electric brake power mode after escooter power up.<br></b>
                
### PAS enabled
Default value : 1<br>
Valid configuration : SmartDisplay<br>
<b>Usage : The 'pedal assitance switch' will for you to push the escooter to start the engine. If you don't push the scooter, the thottle is not effective.<br></b>
                
### ABS enabled
Default value : 0<br>
Valid configuration : SmartDisplay<br>
<b>Usage : Enable the Anti-lock braking system (not advised)<br></b>
                
### Display brightness
Default value : 100<br>
Valid configuration : SmartDisplay<br>
<b>Usage : Set the escooter display brightness<br></b>
                
### Display splash screen
Default value : 1<br>
Valid configuration : SmartDisplay<br>
<b>Usage : <br></b>
                
### Rotate screen
Possible values : <br>
    - 0 (value = 0)<br>
    - 1 (value = 1)<br>
    - 2 (value = 2)<br>
    - 3 (value = 3)<br>
Default value : 1<br>
Valid configuration : SmartDisplay<br>
<b>Usage : <br></b>
                

## Electric brake
### Progressive mode
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : Set electric brake progressive mode.<br></b>
                
### Smart brake type / Type
Possible values : <br>
    - Controller (value = 0)<br>
    - Smart - Digital brake lever (value = 1)<br>
    - Smart - Analog brake lever (value = 2)<br>
    - Smart - Digital brake + throttle (exp) (value = 3)<br>
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Min power value / Min value
Default value : 1<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Max power value / Max value
Default value : 5<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Time between mode shift
Default value : 500 (milliseconds)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Disabled on high battery voltage / Disabled with bat, HV
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Disabled percent limit / Disabled with bat. HV
Default value : 100 (percent)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                

## Throttle
### Throttle regeneration
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Input min voltage (in millivolts) / Input min voltage
Default value : 650 (millivolts)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Input max voltage (in millivolts) / Input max voltage
Default value : 3950 (millivolts)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Output min voltage (in millivolts) / Output min voltage
Default value : 800 (millivolts)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Output max voltage (in millivolts) / Output max voltage
Default value : 3350 (millivolts)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Output curve
Possible values : <br>
    - Linear (value = 0)<br>
    - Exponential 1 (value = 1)<br>
    - Exponential 2 (value = 2)<br>
    - Exponential 3 (value = 3)<br>
    - Exponential 4 (value = 4)<br>
    - Custom 6 points (value = 5)<br>
Default value : 2<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Custom output curve points
Default value : 20,40,60,80<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                

## Escooter buttons
### Button 1 short press action / B1 short press 
Possible values : <br>
    - None (value = 0)<br>
    - Speed limit ON/OFF (value = 1)<br>
    - Aux ON/OFF (value = 2)<br>
    - Mode switch 1/2/3 (value = 3)<br>
    - Mode switch 2/3 (value = 4)<br>
    - Eco switch NONE/MED/MAX (value = 5)<br>
    - Eco switch NONE/MED (value = 6)<br>
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Button 1 long press action / B1 long press 
Possible values : <br>
    - None (value = 0)<br>
    - Speed limitation ON/OFF (value = 1)<br>
    - Aux ON/OFF (value = 2)<br>
    - Mode switch 1/2/3 (value = 3)<br>
    - Mode switch 2/3 (value = 4)<br>
    - Eco switch NONE/MED/MAX (value = 5)<br>
    - Eco switch NONE/MED (value = 6)<br>
    - Anti-theft manual lock ON (value = 7)<br>
    - Nitro boost continuous (value = 8)<br>
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Button 2 short press action / B2 short press 
Possible values : <br>
    - None (value = 0)<br>
    - Speed limitation ON/OFF (value = 1)<br>
    - Aux ON/OFF (value = 2)<br>
    - Mode switch 1/2/3 (value = 3)<br>
    - Mode switch 2/3 (value = 4)<br>
    - Eco switch NONE/MED/MAX (value = 5)<br>
    - Eco switch NONE/MED (value = 6)<br>
Default value : 0<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Button long press duration
Default value : 3 (seconds)<br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                

## OTA update
### Wifi network name (SSID) / Wifi network name
Default value : <br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                
### Wifi password
Default value : <br>
Valid configuration : SmartController / SmartDisplay<br>
<b>Usage : <br></b>
                

