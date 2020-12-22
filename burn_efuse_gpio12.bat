cd c:\temp\
git clone https://github.com/espressif/esptool.git
C:\temp\tool-esptoolpy\espefuse.py -p %1 --do-not-confirm set_flash_voltage 3.3V
