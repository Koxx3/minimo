# SmartLCD Minimotors
Warning : this is work in progress. I decline all responsability about using informations from this project.

## What is SmartLCD
It's a combo of electronic and smartphone application. It extend all features from original Minimorots EYE LCD and controller.
The electronic board will interract with most of the e-scooter electronic when you plug it.

### What can be done ?
I suggest you to have a look at all planed features in the image below.
There is almost no limit in custom features.
The controller power cannot exceed the nominal power, but with a shunt, you can go upper and still have a current control loop to limit the current by software (and avoid burning the controller).

Most settings will be configurable by the smartphone, and additionnal hardware buttons will allow you to control specific features.

Some examples : 
- it can lock the escooter with bluetooth proximity (with a beacon or with the smartphone). the power is so reduced that nobody can ride it is you aren't close enouth.
- you find the acceleration trigger to agressive at low speed ? you can change the acceleration curve for smooth trigger at low speed, and still have the beast once you push the trigger harder.
- you want a mode for some weather conditions like 'rainy' with less torque ? no problem. Use the customized "mode Z" in addition to mode 1/2/3 with special P7/P8/P9/PA.

### Main features
![Idea](/SmartLCD.png)

## Electronic
### Software
The software is designed for ESP32 Dev Kit v4.

### Schematics
https://easyeda.com/Koxx3/bettercontroller

### Inspiration to understand controler electronic
http://avdweb.nl/Article_files/Solarbike/Motor-controller/China-BLDC-motor-controller-36V-250W.pdf

## Applications

### Android 
https://github.com/Koxx3/minimo_android

### iPhone
I have no skills in iPhone apps. If anyone wants to develop, let me know.

## TODO
- [X] Serial Minimotors
    - [X] Read/write serial link LCD_TO_CNTRL
    - [X] Read/write serial link CNTRL_TO_LCD
    - [X] Decode speed/mode/brake/regulator from serial link
    - [X] Frame error detection
    - [ ] Error codes processing
- [ ] Serial Kaabo / Zero / Appolo
- [ ] Bluetooth 
    - [X] Communication with Android
    - [X] Anti-theth with smartphone
    - [X] Anti-theth with beacon (scan & rssi detection)
    - [ ] SmartLCD connexion security
    - [ ] Beacon settings
- [ ] Power
    - [ ] Convert 12V to 5V
    - [ ] Convert 80V to 5V
    - [ ] Convert 100V to 5V
- [ ] Other inputs/outputs
    - [X] Read current with WCS1700
    - [X] Read battery voltage
    - [X] Read tempertature/humidity with DHT11/DHT22
    - [X] Read break handle position on serial
    - [ ] Read break handle position on standard brake handle
    - [X] Read break handle position on analog brake handle
    - [X] Read buttons
    - [X] Send break handle position to controler
    - [X] Send LED status
- [ ] Features    
    - [X] Save settings in non volatile memory
    - [X] Disable electric brake with full battery
    - [X] Progressive electric braking (software detection)
    - [ ] Progressive electric braking (hardware on/off detection)
    - [ ] Progressive electric braking (hardware analog detection)
    - [X] OTA update
    - [X] Current measure auto calibration (at startup)
    - [X] Current measure manual calibration
    - [X] LCD Speed adjustment
    - [ ] Speed limiter at startup
    - [ ] Customize modes with different power / max speed
    - [ ] Remote physical button to change mode (or other features)
    - [ ] Speed loop regulation
    - [ ] Current loop regulation
    - [ ] Custom acceleration curve
    - [ ] Wifi connexion for dashboard display and settings
    - [ ] Advanced diagnosis (serial errors, throtle errors ...)
- [ ] Android app
    - [X] display : speed, mode, voltage, current, power, brake mode (PA), max speed, max power, max current, temperature, humidity, time, moving time
    - [X] auto-launch app with NFC tag
    - [X] parameters custom settings
    - [X] Data logging
    - [X] history graphics
    - [ ] display : average speed, distance

- [ ] iPhone app => for someone else

## Serial links data decoding
[Excel](http://github.com/Koxx3/minimo/edit/master/MINIMO.xlsx

## Donate to support
https://www.paypal.com/donate/?cmd=_s-xclick&hosted_button_id=W3KHBZCNL9N2C&source=url
