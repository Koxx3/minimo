# Run the softwares
Once you have all the electronic, you'll need to flash/install the softwares.

First : the ESP32 software
Second : the Android applcation

## Preparation of ESP32 board
- Install Visual Code + Platform.io [tuto](https://platformio.org/install/ide?install=vscode)
- Clone this Github project
- Patch Espressif Framework with patch folder
- Burn efuse for GPIO12 (use the script called 'burn_efuse_gpio12.bat' and don't forget to change you COM port)
- Flash the ESP32

## Connect Smartphone
- Enable bluetooth
- Check bluetooth detect a "SmartContrl-xxxx" device
- Install the APK
- Launch the application
- Accept the connection to the device
- Use the defaul pin code : 147258

=> You can change the pin code in the application settings

Nota : you can modify the Android project from [this github](https://github.com/Koxx3/minimo_android) 

## Settings
TBD

## Update throught Wifi
- From the application, in the '...' menu, tap "Software update"
=> The SmartController will siwth from bluetooth to wifi server
- On the smartphone, search for wifi called 'SmartCntr-xxx' and connect to it
- Open a web brower, and tap : http://192.168.1.2
- Once on the web page : login "SmartController" / password "SmartController$"
- Select the firmware, then click 'upload'.
- Wait the upload end, then it should reboot automatically

