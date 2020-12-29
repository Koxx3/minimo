;//@echo off

SETLOCAL enableextensions

IF "%1"=="" GOTO :Continue
    pip install esptool
    espefuse.exe -p %1 --do-not-confirm set_flash_voltage 3.3V
:Continue
IF "%1"=="" echo You must specify COM port (COM19 for exemple)

pause
