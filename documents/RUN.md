# Run the softwares
Once you have all the electronic, you'll need to flash/install the softwares.

- First : the ESP32 software
- Second : the Android applcation

## Preparation of ESP32 board
- Install Visual Code + Platform.io [tuto](https://platformio.org/install/ide?install=vscode)
- Clone this Github project
- Patch Espressif Framework with patch folder in :
  - copy patch-esp\esp32-hal-uart.* in C:\Users\ \<username>\ \.platformio\packages\framework-arduinoespressif32\cores\esp32
  - copy patch-esp\HardwareSerial.* in C:\Users\ \<username>\ \.platformio\packages\framework-arduinoespressif32\cores\esp32
  - copy patch-esp\BLESecurity.* in C:\Users\ \<username>\ \.platformio\packages\framework-arduinoespressif32\libraries\BLE\src
- Burn efuse for GPIO12 with the script called 'burn_efuse_gpio12.bat'
  - burn_efuse_gpio12.bat <COM_PORT>
- Erase the ESP32
- Flash the ESP32

## Configure the device
### Android
- Enable bluetooth
- Check if bluetooth detect a "SmartContrl-xxxx" device
- Pair the device with the default PIN code
- Install the applcation from the Google App Store : https://play.google.com/store/apps/details?id=org.koxx.smartcntrl
- Launch the application
- Accept the connection to the device
- Use the defaul pin code : 147258
Nota : You can change the pin code in the application settings page
- Enter the applicationsettings page
- Configure the device

### iPhone
- Enable wifi
- Search for "SmartContrl-xxxx" wifi
- Connect to the SmartElec hotsport, enter the password 147258147258 and access the hotspot page
  - Nota : The password is the Bluetooth pin code repeated twice. Change the Bluetooth pin code to change the wifi password.
- Enter in the settings menu

### From the SmartDisplay
- Long press the secondary button, you will enter in the settings menu
  - Nota : some settings are not available from this menu (when you have to enter strings)
- Use primary and secondary buttons to move the cursor, validate and escape

## Settings
[See settings descriptions](help_settings_en.md)

## Update throught Wifi
You need to set the Wifi network/password from an android/iphone to make it work :
Then ...

### Android
- In the main interface, open the menu with the '...' , tap "Software update (OTA)"
=> The SmartController will siwth from bluetooth to wifi server and you will loose Bluetooth connection.
- Wait the upload and flash end, then it should reboot automatically

### iPhone
- On the wifi hotspot portal, select "Software update (OTA)"
- Enter the release number and validate !

### From the SmartDisplay
- TBD
