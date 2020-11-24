# Run the softwares
Once you have all the electronic, you'll need to flash/install the softwares.

- First : the ESP32 software
- Second : the Android applcation

## Preparation of ESP32 board
- Install Visual Code + Platform.io [tuto](https://platformio.org/install/ide?install=vscode)
- Clone this Github project
- Patch Espressif Framework with patch folder in :
  - C:\Users\<username>\.platformio\packages\framework-arduinoespressif32\cores\esp32
  - C:\Users\<username>\.platformio\packages\framework-arduinoespressif32\libraries\BLE\src
- Burn efuse for GPIO12 (use the script called 'burn_efuse_gpio12.bat' and don't forget to change you COM port)
- Flash the ESP32

## Connect Smartphone
- Enable bluetooth
- Check if bluetooth detect a "SmartContrl-xxxx" device
- Install the APK
- Launch the application
- Accept the connection to the device
- Use the defaul pin code : 147258

=> You can change the pin code in the application settings

Nota : you can modify the Android project from [this github](https://github.com/Koxx3/minimo_android) 

## Settings
TBD

## Update throught Wifi
- From the Android application, in the settings, enter your wifi network and password.
- In the main interface, open the menu with the '...' , tap "Software update (OTA)"
=> The SmartController will siwth from bluetooth to wifi server and you will loose Bluetooth connection.
- Wait the upload and flash end, then it should reboot automatically

