
#define EEPROM_SIZE 1024
#define EEPROM_ADDRESS_SETTINGS 0
#define EEPROM_ADDRESS_BLE_LOCK_FORCED 100

void saveBleLockForced();
void restoreBleLockForced();

uint8_t modifyBrakeFromAnalog(char, char *);

void OTA_setup();

void processButton1Click();
void processButton1LpStart();
void processButton1LpDuring();
void processButton1LpStop();

void processButton2Click();
void processButton2DoubleClick();
