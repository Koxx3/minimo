
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

void processAuxEvent(uint8_t buttonId);
void resetPid();