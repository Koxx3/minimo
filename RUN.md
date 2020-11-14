### Requirements

• A e-scooter with minimotors controllers (single or dual)

• A constant 12V ouput from the regulator (not only powered when brakes are pressed like some Kaabo)... OR an external DC-DC providing 12V or 5V
• If you want to use progressive brake feature, you must ensure no more than 60V pass in the brake handle (issue noted only for Kaabo)
• The electronics (ESP32 Dev KitC V4 + SmartController shield + JST SM/PH connectors)
• Some electronic skills

### Preparation of ESP32 board

• Install Visual Code + Platform.io
• Clone Github project
• Patch Espressif Framework with patch folder
• Burn efuse for GPIO12
• Flash the ESP32

### Connect Smartphone

• Enable bluetooth
• Check bluetooth detect a "SmartContrl-xxxx" device
• Install the APK
• Launch the application
• Accept the connection to the device
• Use the defaul pin code : 147258

=> You can change the pin code in the application settings

### Settings

TBD

### Update throught Wifi

• From the application, in the '...' menu, tap "Software update"
=> The SmartController will siwth from bluetooth to wifi server
• On the smartphone, search for wifi called 'SmartCntr-xxx' and connect to it
• Open a web brower, and tap : http://192.168.1.2
• Once on the web page : login "SmartController" / password "SmartController$"
• Select the firmware, then click 'upload'.
• Wait the upload end, then it should reboot automatically

