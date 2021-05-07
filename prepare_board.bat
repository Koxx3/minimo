@echo off

SETLOCAL enableextensions
IF "%2"==""  GOTO :Continue
    pip install esptool

    echo ### burn fuse
    python -m espefuse -p %1 --do-not-confirm set_flash_voltage 3.3V

    echo ### upload program
    pio run -s -e %2 -t upload
    
    echo ### upload filesystem
    pio run -s -e %2 -t uploadfs

    echo ### set power supply to 60V and calibrate
    pause

    echo ### set power supply to 30V and calibrate
    pause

    echo ### done

:Continue
IF "%2"=="" echo You must specify COM port (COM19 for exemple)

