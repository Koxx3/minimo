
#define EEPROM_SIZE 1024
#define EEPROM_ADDRESS_SETTINGS 0
#define EEPROM_ADDRESS_BLE_LOCK_FORCED 100

void saveBleLockForced();
void restoreBleLockForced();
uint8_t modifyBrakeFromAnalog(char, char *);
void OTA_setup();