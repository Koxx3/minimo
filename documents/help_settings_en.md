
# Manual

## Escooter characteristics
### Wheel size
Valid configuration : SmartController / SmartDisplay<br>
Default value : 10.0 (inches)<br>
<br>
Usage : Wheel size (in inches) is used to adjust the speed and distance when riding.<br>

Usage 2 : 
    Wheel size (in inches) is used to adjust the speed and distance when riding.
                
### Number of poles pairs
Valid configuration : SmartController / SmartDisplay<br>
Default value : 15<br>
<br>
Usage : Number of poles are used to compute speed/distance. Check you manual to set the correct value. If the manual specify '30 poles', set 15.<br>

Usage 2 : 
    Number of poles are used to compute speed/distance. Check you manual to set the correct value. If the manual specify '30 poles', set 15.
                
### Battery nominal voltage
Valid configuration : SmartController / SmartDisplay<br>
Possible values : <br>
    - 36V (42V full / 10S) (value = 0)<br>
    - 46,8 (54,6V full / 13S) (value = 1)<br>
    - 50,4V (58,8V full / 14S) (value = 2)<br>
    - 57.6V (67,2V full / 16S) (value = 3)<br>
    - 72V (84V full / 20S) (value = 4)<br>
    - 86V (100,8V full / 24S) (value = 5)<br>
Default value : 2<br>
<br>
Usage : Battery voltage is used to compute the autonomy left and charge indicator. Check your manuel or charger (it indicate full charge voltage) to set the correct setting.<br>

Usage 2 : 
    Battery voltage is used to compute the autonomy left and charge indicator. Check your manuel or charger (it indicate full charge voltage) to set the correct setting.
                
### Battery maximum distance
Valid configuration : SmartController / SmartDisplay<br>
Default value : 40 (kilometers)<br>
<br>
Usage : Use to compute autonomy. Set your regular max distance with one charge.<br>

Usage 2 : 
    Use to compute autonomy. Set your regular max distance with one charge.
                

## General
### Speed limiter at startup
Valid configuration : SmartController / SmartDisplay<br>
Default value : 0<br>
<br>
Usage : Enable speed limit after power on by setting a power limit (Dualtron/Speedway/Zeero …) or speed limit (VESC).<br>

Usage 2 : 
    Enable speed limit after power on by setting a power limit (Dualtron/Speedway/Zeero …) or speed limit (VESC).
                
### Speed limiter max speed
Valid configuration : SmartController / SmartDisplay<br>
Default value : 37<br>
<br>
Usage : Power limit is expressed in percent (same power setting as in your manual). For VESC, the speed limit is set to 25 Km/h.<br>

Usage 2 : 
    Power limit is expressed in percent (same power setting as in your manual). For VESC, the speed limit is set to 25 Km/h.
                
### Bluetooth lock mode
Valid configuration : SmartController / SmartDisplay<br>
Possible values : <br>
    - None (value = 0)<br>
    - Smartphone connected (value = 1)<br>
    - Smartphone connected or beacon visible (value = 2)<br>
    - Beacon visible (value = 3)<br>
Default value : 0<br>
<br>
Usage : Lock the scooter if bluetooth device check is not successful.<br>- None : no bluetooth peripheral check<br>- Smartphone connected : your smartphone needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>- Smartphone connected or beacon visible : your smartphone OR a BLE beacon needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>- Beacon visible : BLE beacon needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>Those modes are use as anti-theft technics. If an thief try to still you escooter, as soon as the escooter is out of range of your bluetooth device, it will cut power of the escooter.
Beacon visible<br>

Usage 2 : 
    Lock the scooter if bluetooth device check is not successful.<br>- None : no bluetooth peripheral check<br>- Smartphone connected : your smartphone needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>- Smartphone connected or beacon visible : your smartphone OR a BLE beacon needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>- Beacon visible : BLE beacon needs to be connected to the SmartElec device in Bluetooth to unlock the scooter.<br>Those modes are use as anti-theft technics. If an thief try to still you escooter, as soon as the escooter is out of range of your bluetooth device, it will cut power of the escooter.<br>Beacon visible
                
### PIN code
Valid configuration : SmartController / SmartDisplay<br>
Default value : 147258<br>
<br>
Usage : Bluetooth PIN code is used when you want to connect a smartphone to your SmartEelc device. Change it to protect your SmartElec device from any anybody connection.<br>

Usage 2 : 
    Bluetooth PIN code is used when you want to connect a smartphone to your SmartEelc device. Change it to protect your SmartElec device from any anybody connection.
                
### Beacon Mac Address
Valid configuration : SmartController / SmartDisplay<br>
Default value : aa:bb:cc:dd:ee:ff<br>
<br>
Usage : To search a beacon you must specify the beacon mac address. Search on your smartphone store for a 'BLE Scanner', it will scan all Bluetooth devices and the MAC address (an unique identifier) for the beacon, then enter this MAC address in this setting.<br>
You can use any Bluetooth Low Energy device as beacon (any BLE band for example) or use a BLE beacon (10$ on aliexpress).<br>

Usage 2 : 
    To search a beacon you must specify the beacon mac address. Search on your smartphone store for a 'BLE Scanner', it will scan all Bluetooth devices and the MAC address (an unique identifier) for the beacon, then enter this MAC address in this setting.<br><br>You can use any Bluetooth Low Energy device as beacon (any BLE band for example) or use a BLE beacon (10$ on aliexpress).
                
### Beacon range
Valid configuration : SmartController / SmartDisplay<br>
Default value : -80 (dB)<br>
<br>
Usage : Minimal reception power of the beacon to unlock the escooter. This is in dB, it means a -100 value will be a longer distance than -50.<br>

Usage 2 : 
    Minimal reception power of the beacon to unlock the escooter. This is in dB, it means a -100 value will be a longer distance than -50.
                
### Original display speed adjustment (in percent) / Speed adjustment
Valid configuration : SmartController / SmartDisplay<br>
Default value : 0 (percent)<br>
<br>
Usage : Use this value to correct the speed on the escooter display.<br>

Usage 2 : 
    Use this value to correct the speed on the escooter display.
                
### Temperature warning
Valid configuration : SmartController / SmartDisplay<br>
Default value : 70 (celcuis)<br>
<br>
Usage : Trigger a error indicator on the display or the smartphone application if this temperature threshold is reached. If you place the temperature sensor inside the deck, it will warn you if the controller or the battery are too hot.<br>

Usage 2 : 
    Trigger a error indicator on the display or the smartphone application if this temperature threshold is reached. If you place the temperature sensor inside the deck, it will warn you if the controller or the battery are too hot.
                
### Humidity warning
Valid configuration : SmartController / SmartDisplay<br>
Default value : 80 (RH)<br>
<br>
Usage : Trigger a error indicator on the display or the smartphone application if this humidy threshold is reached. If you place the humidity sensor inside the deck, it will warn you if the water is entered inside the deck.<br>

Usage 2 : 
    Trigger a error indicator on the display or the smartphone application if this humidy threshold is reached. If you place the humidity sensor inside the deck, it will warn you if the water is entered inside the deck.
                
### Automatic power off / Auto power off
Valid configuration : SmartDisplay<br>
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
<br>
Usage : Automatically shutdown the escooter at a fixed time at 0 speed.<br>

Usage 2 : 
    Automatically shutdown the escooter at a fixed time at 0 speed.
                

## Smartphone display
### Speed adjustment (in percent) / Speed adjustment
Valid configuration : SmartController / SmartDisplay<br>
Default value : 0<br>
<br>
Usage : Use this value to correct the speed on the smartphone display.<br>

Usage 2 : 
    Use this value to correct the speed on the smartphone display.
                
### Aux relay name
Valid configuration : SmartController / SmartDisplay<br>
Possible values : <br>
    - AUX (value = 0)<br>
    - DUAL (value = 1)<br>
    - LIGHT (value = 2)<br>
    - HORN (value = 3)<br>
    - RELAY (value = 4)<br>
Default value : 0<br>
<br>
Usage : Change the name on the display and smartphone for the integrated relay action. Exemple : if you connect a light, you can display "light" on the dashboard instead of "aux".<br>

Usage 2 : 
    Change the name on the display and smartphone for the integrated relay action. Exemple : if you connect a light, you can display "light" on the dashboard instead of "aux".
                
### Display GPS speed instead of escooter speed / Display GPS speed 
Valid configuration : SmartController / SmartDisplay<br>
Default value : 0<br>
<br>
Usage : Display GPS speed instead of escooter speed on the smartphone display.<br>

Usage 2 : 
    Display GPS speed instead of escooter speed on the smartphone display.
                

## SmartDisplay
### Default mode at startup / Startup mode
Valid configuration : SmartDisplay<br>
Default value : 3<br>
<br>
Usage : Default speed mode after escooter power up.<br>

Usage 2 : 
    Default speed mode after escooter power up.
                
### Default eco mode at startup / Startup eco mode
Valid configuration : SmartDisplay<br>
Possible values : <br>
    - Max (value = 0)<br>
    - Medium (value = 1)<br>
    - None (value = 2)<br>
Default value : 2<br>
<br>
Usage : Default eco mode after escooter power up.<br>

Usage 2 : 
    Default eco mode after escooter power up.
                
### Default acceleration / Startup acceleration
Valid configuration : SmartDisplay<br>
Possible values : <br>
    - Maximum (value = 0)<br>
    - Very fast (value = 1)<br>
    - Fast (value = 2)<br>
    - Medium (value = 3)<br>
    - Slow (value = 4)<br>
    - Very slow (value = 5)<br>
Default value : 0<br>
<br>
Usage : Default acceleration mode after escooter power up.<br>

Usage 2 : 
    Default acceleration mode after escooter power up.
                
### Default electric brake at startup / Startup  ebrake
Valid configuration : SmartDisplay<br>
Default value : 3<br>
<br>
Usage : Default electric brake power mode after escooter power up.<br>

Usage 2 : 
    Default electric brake power mode after escooter power up.
                
### PAS enabled
Valid configuration : SmartDisplay<br>
Default value : 1<br>
<br>
Usage : The 'pedal assitance switch' will for you to push the escooter to start the engine. If you don't push the scooter, the thottle is not effective.<br>

Usage 2 : 
    The 'pedal assitance switch' will for you to push the escooter to start the engine. If you don't push the scooter, the thottle is not effective.
                
### ABS enabled
Valid configuration : SmartDisplay<br>
Default value : 0<br>
<br>
Usage : Enable the Anti-lock braking system (not advised)<br>

Usage 2 : 
    Enable the Anti-lock braking system (not advised)
                
### Display brightness
Valid configuration : SmartDisplay<br>
Default value : 100<br>
<br>
Usage : Set the escooter display brightness<br>

Usage 2 : 
    Set the escooter display brightness
                
### Display splash screen
Valid configuration : SmartDisplay<br>
Default value : 1<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Rotate screen
Valid configuration : SmartDisplay<br>
Possible values : <br>
    - 0 (value = 0)<br>
    - 1 (value = 1)<br>
    - 2 (value = 2)<br>
    - 3 (value = 3)<br>
Default value : 1<br>
<br>
Usage : <br>

Usage 2 : 
    
                

## Electric brake
### Progressive mode
Valid configuration : SmartController / SmartDisplay<br>
Default value : 0<br>
<br>
Usage : Enable  electric brake progressive mode.<br>

Usage 2 : 
    Enable  electric brake progressive mode.
                
### Smart brake type / Type
Valid configuration : SmartController / SmartDisplay<br>
Possible values : <br>
    - Controller (value = 0)<br>
    - Smart - Digital brake lever (value = 1)<br>
    - Smart - Analog brake lever (value = 2)<br>
    - Smart - Digital brake + throttle (exp) (value = 3)<br>
Default value : 0<br>
<br>
Usage : If you enable the electric progressive mode, the braking feedback must arrive to the SmartElec device. There is mulitple possibilities to cable the brake on the SmartElec and you *MUST* select the brake type according to your wiring :
- Controller : no specific wiring required, the brake feedback will come from the controller. This is slower and will add a delay of 200/300ms between the brake pressure and the electric brake to starts to kick. The brake force will be determined by the brake lever pressure time. Longer you press, stronger the electric brake will be.
- Smart - Digital brake lever : a standard on/off brake lever is wired directly to the SmartElec device. This wiring has a faster reaction. The brake force will be determined by the brake lever pressure time. Longer you press, stronger the electric brake will be.
- Smart - Analog brake lever : an analog brake lever (xiaomi type) is wired to directly to the SmartElec device. This wiring is the best and fast. The brake force will be determined by the brake lever pressure force.
- Smart - Digital brake + throttle (expermental) : a standard on/off brake lever is wired directly to the SmartElec device and the throttle pressure determine the brake force. The user will need time to use this mode when riding.<br>

Usage 2 : 
    If you enable the electric progressive mode, the braking feedback must arrive to the SmartElec device. There is mulitple possibilities to cable the brake on the SmartElec and you *MUST* select the brake type according to your wiring :<br>- Controller : no specific wiring required, the brake feedback will come from the controller. This is slower and will add a delay of 200/300ms between the brake pressure and the electric brake to starts to kick. The brake force will be determined by the brake lever pressure time. Longer you press, stronger the electric brake will be.<br>- Smart - Digital brake lever : a standard on/off brake lever is wired directly to the SmartElec device. This wiring has a faster reaction. The brake force will be determined by the brake lever pressure time. Longer you press, stronger the electric brake will be.<br>- Smart - Analog brake lever : an analog brake lever (xiaomi type) is wired to directly to the SmartElec device. This wiring is the best and fast. The brake force will be determined by the brake lever pressure force.<br>- Smart - Digital brake + throttle (expermental) : a standard on/off brake lever is wired directly to the SmartElec device and the throttle pressure determine the brake force. The user will need time to use this mode when riding.
                
### Min power value / Min value
Valid configuration : SmartController / SmartDisplay<br>
Default value : 1<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Max power value / Max value
Valid configuration : SmartController / SmartDisplay<br>
Default value : 5<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Time between mode shift
Valid configuration : SmartController / SmartDisplay<br>
Default value : 500 (milliseconds)<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Disabled on high battery voltage / Disabled with bat, HV
Valid configuration : SmartController / SmartDisplay<br>
Default value : 0<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Disabled percent limit / Disabled with bat. HV
Valid configuration : SmartController / SmartDisplay<br>
Default value : 100 (percent)<br>
<br>
Usage : <br>

Usage 2 : 
    
                

## Throttle
### Throttle regeneration
Valid configuration : SmartController / SmartDisplay<br>
Default value : 0<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Input min voltage (in millivolts) / Input min voltage
Valid configuration : SmartController / SmartDisplay<br>
Default value : 650 (millivolts)<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Input max voltage (in millivolts) / Input max voltage
Valid configuration : SmartController / SmartDisplay<br>
Default value : 3950 (millivolts)<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Output min voltage (in millivolts) / Output min voltage
Valid configuration : SmartController / SmartDisplay<br>
Default value : 800 (millivolts)<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Output max voltage (in millivolts) / Output max voltage
Valid configuration : SmartController / SmartDisplay<br>
Default value : 3350 (millivolts)<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Output curve
Valid configuration : SmartController / SmartDisplay<br>
Possible values : <br>
    - Linear (value = 0)<br>
    - Exponential 1 (value = 1)<br>
    - Exponential 2 (value = 2)<br>
    - Exponential 3 (value = 3)<br>
    - Exponential 4 (value = 4)<br>
    - Custom 6 points (value = 5)<br>
Default value : 2<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Custom output curve points
Valid configuration : SmartController / SmartDisplay<br>
Default value : 20,40,60,80<br>
<br>
Usage : <br>

Usage 2 : 
    
                

## Escooter buttons
### Button 1 short press action / B1 short press 
Valid configuration : SmartController / SmartDisplay<br>
Possible values : <br>
    - None (value = 0)<br>
    - Speed limit ON/OFF (value = 1)<br>
    - Aux ON/OFF (value = 2)<br>
    - Mode switch 1/2/3 (value = 3)<br>
    - Mode switch 2/3 (value = 4)<br>
    - Eco switch NONE/MED/MAX (value = 5)<br>
    - Eco switch NONE/MED (value = 6)<br>
Default value : 0<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Button 1 long press action / B1 long press 
Valid configuration : SmartController / SmartDisplay<br>
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
<br>
Usage : <br>

Usage 2 : 
    
                
### Button 2 short press action / B2 short press 
Valid configuration : SmartController / SmartDisplay<br>
Possible values : <br>
    - None (value = 0)<br>
    - Speed limitation ON/OFF (value = 1)<br>
    - Aux ON/OFF (value = 2)<br>
    - Mode switch 1/2/3 (value = 3)<br>
    - Mode switch 2/3 (value = 4)<br>
    - Eco switch NONE/MED/MAX (value = 5)<br>
    - Eco switch NONE/MED (value = 6)<br>
Default value : 0<br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Button long press duration
Valid configuration : SmartController / SmartDisplay<br>
Default value : 3 (seconds)<br>
<br>
Usage : <br>

Usage 2 : 
    
                

## OTA update
### Wifi network name (SSID) / Wifi network name
Valid configuration : SmartController / SmartDisplay<br>
Default value : <br>
<br>
Usage : <br>

Usage 2 : 
    
                
### Wifi password
Valid configuration : SmartController / SmartDisplay<br>
Default value : <br>
<br>
Usage : <br>

Usage 2 : 
    
                

