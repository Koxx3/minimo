
void saveBleLockForced();
void restoreBleLockForced();
void saveBrakeMaxPressure();

uint8_t modifyBrakeFromAnalog(char, char *);

void processButton1Click();
void processButton1LpStart();
void processButton1LpDuring();
void processButton1LpStop();

void processButton2Click();
void processButton2LpStart();
void processButton2LpDuring();
void processButton2LpStop();

void processSpeedLimiterEvent(uint8_t, bool);
void processLockEvent(uint8_t, bool);
void processAuxEvent(uint8_t, bool);

void resetPid();

bool isElectricBrakeForbiden();

void disableWatchdog();
