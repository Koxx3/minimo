#ifndef _ZERO_h
#define _ZERO_h

#include <Arduino.h>
#include "Settings.h"
#include "SharedData.h"
#include "BLE/BluetoothHandler.h"

#define ZERO_BAUD_RATE 1200

#define ZERO_DATA_BUFFER_SIZE 30

#define ZERO_MODE_LCD_TO_CNTRL_START_BYTE 0x01
#define ZERO_MODE_CNTRL_TO_LCD_START_BYTE 0x36

// MINIMO CONFIG
#define ZERO_ALLOW_LCD_TO_CNTRL_MODIFICATIONS false
#define ZERO_ALLOW_CNTRL_TO_LCD_MODIFICATIONS false

class ZeroUart {

public:
    ZeroUart();

    void setSettings(Settings *);
    void setSharedData(SharedData *);
    void setBluetoothHandler(BluetoothHandler *);

    void setLcdSerialPort(Stream *port);
    void setControllerSerialPort(Stream *port);

    uint8_t getCheckSum(char *string);
    void displayFrame(int mode, char data_buffer[], byte checksum);
    uint8_t modifyModeOld(char var, char data_buffer[]);
    uint8_t getMode(char var, char data_buffer[]);
    uint8_t modifyMode(char var, char data_buffer[]);
    uint8_t getBrakeFromCntrlFrame(char var, char data_buffer[]);
    void displayDecodedFrame(int mode, char data_buffer[], byte checksum);
    double getSpeed();
    uint8_t modifyAccel(char var, char data_buffer[]);
    uint16_t generateSpeedRawValue(double speed);
    uint8_t modifySpeed(char var, char data_buffer[], uint8_t byte);
    uint8_t modifyPower(char var, char data_buffer[]);
    uint8_t modifyBrakeFromDisplay(char var, char data_buffer[]);
    uint8_t modifyBrakeFromAnalog(char var, char data_buffer[]);
    uint8_t modifyEco(char var, char data_buffer[]);
    uint8_t modifyPas(char var, char data_buffer[]);

    bool getSerialStatusOkFromContrl();
    bool getSerialStatusOkFromLcd();
    bool getContrlStatusOk();
    bool isContrlInError(char var, char data_buffer[]);

    void readHardSerial(int mode, int *i, Stream *hwSerCntrl, Stream *hwSerLcd, int serialMode, char *data_buffer_ori, char *data_buffer_mod);
    void processSerial(uint32_t i_loop, boolean simulatedDisplay);

private:
    Stream *hwSerCntrl;
    Stream *hwSerLcd;
    BluetoothHandler *blh;

    SharedData *shrd;
    Settings *settings;

    uint32_t timeLastBrake = 0;

    int begin_LcdToCntrl = 1;
    int begin_CntrlToLcd = 1;

    int isModified_LcdToCntrl = 0;
    int isModified_CntrlToLcd = 0;

    int i_LcdRcv = 0;
    int i_CntrlRcv = 0;

    uint32_t timeLastValidFrameFromCntrl = 0;
    uint32_t timeLastValidFrameFromLcd = 0;

    bool cntrlInError = false;

    char data_buffer_lcd_mod[ZERO_DATA_BUFFER_SIZE];
    char data_buffer_cntrl_mod[ZERO_DATA_BUFFER_SIZE];
    char data_buffer_lcd_ori[ZERO_DATA_BUFFER_SIZE];
    char data_buffer_cntrl_ori[ZERO_DATA_BUFFER_SIZE];
};

#endif
