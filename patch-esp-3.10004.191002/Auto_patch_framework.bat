@echo off
xcopy /v/y esp32-hal-uart.c %HOMEPATH%\.platformio\packages\framework-arduinoespressif32\cores\esp32
xcopy /v/y esp32-hal-uart.h %HOMEPATH%\.platformio\packages\framework-arduinoespressif32\cores\esp32
xcopy /v/y HardwareSerial.cpp %HOMEPATH%\.platformio\packages\framework-arduinoespressif32\cores\esp32
xcopy /v/y HardwareSerial.h %HOMEPATH%\.platformio\packages\framework-arduinoespressif32\cores\esp32

xcopy /v/y BLESecurity.cpp %HOMEPATH%\.platformio\packages\framework-arduinoespressif32\libraries\BLE\src
xcopy /v/y BLESecurity.h %HOMEPATH%\.platformio\packages\framework-arduinoespressif32\libraries\BLE\src
pause
