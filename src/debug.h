#ifndef _debug_h
#define _debug_h

// Bluetooth debugs
#define DEBUG_BLE_DISPLAY_VOLTAGE                   0
#define DEBUG_BLE_DISPLAY_ANALOG_BRAKE              0
#define DEBUG_BLE_DISPLAY_FRAME                     0
#define DEBUG_BLE_DISPLAY_COMMANDSFEEDBACK          0
#define DEBUG_BLE_DISPLAY_SETTINGSCHANGED           0
#define DEBUG_BLE_DISPLAY_VOLTAGE_TOO_HIGH          0
#define DEBUG_BLE_DISPLAY_SCAN                      0

// Perf debugs
#define DEBUG_TIMELOOP_MS                           0
#define DEBUG_TIMELOOP_NS                           0
#define DEBUG_HEAP                                  0

// General debugs
#define DEBUG_DISPLAY_BLE_NOTIFY                    0
#define DEBUG_DISPLAY_MODE                          0
#define DEBUG_DISPLAY_DIGITAL_BRAKE                 0
#define DEBUG_DISPLAY_ANALOG_BRAKE                  0
#define DEBUG_DISPLAY_DAC_OUTPUT                    0
#define DEBUG_DISPLAY_THROTTLE                      0
#define DEBUG_DISPLAY_THROTTLE_6_POINTS             0
#define DEBUG_DISPLAY_ECO                           0
#define DEBUG_DISPLAY_ACCEL                         0
#define DEBUG_DISPLAY_NITRO                         0
#define DEBUG_DISPLAY_BUTTON1                       0
#define DEBUG_DISPLAY_BUTTON2                       0
#define DEBUG_DISPLAY_VOLTAGE                       0
#define DEBUG_DISPLAY_CURRENT                       0
#define DEBUG_DISPLAY_DHT                           0
#define DEBUG_DISPLAY_MINIMO_MOD_POWER              0
#define DEBUG_DISPLAY_MINIMO_MOD_PAS                0
#define DEBUG_DISPLAY_BRAKE_FORBIDEN                0
#define DEBUG_DISPLAY_DISTANCE                      0
#define DEBUG_DISPLAY_AUTONOMY                      0
#define DEBUG_DISPLAY_SHTC3                         0
#define DEBUG_DISPLAY_SPEED                         0
#define DEBUG_DISPLAY_SAVE_ODO                      0
#define DEBUG_DISPLAY_BATTERY                       0

#define DEBUG_BRAKE_SENT_ORDER                      0


// Serial frame deconding debugs
#define DEBUG_DISPLAY_FRAME_LCD_TO_CNTRL            0
#define DEBUG_DISPLAY_MODIFIED_FRAME_LCD_TO_CNTRL   0
#define DEBUG_DISPLAY_FRAME_CNTRL_TO_LCD            0
#define DEBUG_DISPLAY_DECODED_FRAME_CNTRL_TO_LCD    0
#define DEBUG_SERIAL_CHECKSUM_LCD_TO_CNTRL          0
#define DEBUG_SERIAL_CHECKSUM_CNTRL_TO_LCD          0
#define DEBUG_DISPLAY_VESC_FRAME                    0

#define SERIAL_FAKE_CURRENT                         0
#define DEBUG_FAKE_SPEED                            0
#define DEBUG_FAKE_VOLTAGE                          0
#define DEBUG_FAKE_TEMPERATURE                      0

#endif