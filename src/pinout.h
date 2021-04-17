#ifndef Pinout_H_
#define Pinout_H_

// PINOUT
#ifndef PCB
#define PCB 142
#endif

#if (PCB == 130)
#define HAS_I2C 0
#define PIN_SERIAL_ESP_TO_LCD 26
#define PIN_SERIAL_ESP_TO_CNTRL 27
#define PIN_SERIAL_LCD_TO_ESP 25
#define PIN_SERIAL_CNTRL_TO_ESP 14
#define PIN_OUT_RELAY 16
#define PIN_IN_VOLTAGE 32
#define PIN_IN_CURRENT 35
#define PIN_IN_BUTTON1 22
#define PIN_IN_BUTTON2 15 // PB was TX
#define PIN_OUT_LED_BUTTON1 3
#define PIN_OUT_BRAKE 13
#define PIN_IN_OUT_DHT 12
#define PIN_IN_ABRAKE 34
#define PIN_IN_ATHROTTLE 39
#define PIN_SPI_BKL 5          // use in LCD  // ok
#define PIN_FREE 19            // ok
#define PIN_OUT_FREE 21        // ok
#endif

#if (PCB == 132)
#define HAS_I2C 1
// LEFT
#define PIN_IN_CURRENT 36          // ok >> need calibration
#define PIN_IN_ATHROTTLE 39        // ~ok >>> missing voltage divider & filter capa
#define PIN_IN_ABRAKE 34           // ok
#define PIN_IN_VOLTAGE 35          // ok
#define PIN_I2C_SDA 32             // ok
#define PIN_I2C_SCL 33             // ok
#define PIN_SERIAL_LCD_TO_ESP 25   // ok
#define PIN_SERIAL_ESP_TO_LCD 26   // ok
#define PIN_SERIAL_ESP_TO_CNTRL 27 // ok
#define PIN_SERIAL_CNTRL_TO_ESP 14 // ok
#define PIN_IN_OUT_DHT 12          // ok
#define PIN_OUT_BRAKE 13           // ok
// RIGHT
#define PIN_SPI_MOSI 23        // use in LCD  // ok
#define PIN_IN_BUTTON1 22      // ok
#define PIN_OUT_POWER_LATCH 21 // ok
#define PIN_SPI_CLK 18         // use in LCD  // ok
#define PIN_SPI_BKL 5          // use in LCD  // ok
#define PIN_SPI_RST 17         // use in LCD  // ok
#define PIN_OUT_RELAY 16       // ok with voltages >> to test with SSR
#define PIN_OUT_LED_BUTTON1 4  // ok
#define PIN_SPI_DC 2           // use in LCD // ok
#define PIN_IN_BUTTON2 15      // ok
#endif

#if (PCB == 142)
#define HAS_I2C 1
// LEFT
#define PIN_IN_CURRENT 36          // ok >> need calibration
#define PIN_IN_ATHROTTLE 39        // ~ok >>> missing voltage divider & filter capa
#define PIN_IN_ABRAKE 34           // ok
#define PIN_IN_VOLTAGE 35          // ok
#define PIN_I2C_SDA 32             // ok
#define PIN_I2C_SCL 33             // ok
#define PIN_SERIAL_LCD_TO_ESP 25   // ok
#define PIN_SERIAL_ESP_TO_LCD 26   // ok
#define PIN_SERIAL_ESP_TO_CNTRL 27 // ok
#define PIN_SERIAL_CNTRL_TO_ESP 14 // ok
#define PIN_IN_OUT_ONEWIRE 12          // ok
#define PIN_OUT_BRAKE 13           // ok
// RIGHT
#define PIN_SPI_MOSI 23        // use in LCD  // ok
#define PIN_IN_BUTTON1 22      // ok
#define PIN_OUT_POWER_LATCH 21 // ok
#define PIN_IN_BUTTON_PWR 19   // ok
#define PIN_SPI_CLK 18         // use in LCD  // ok
#define PIN_SPI_BKL 5          // use in LCD  // ok
#define PIN_SPI_RST 17         // use in LCD  // ok
#define PIN_OUT_RELAY 16       // ok with voltages >> to test with SSR
#define PIN_OUT_LED_BUTTON1 4  // ok
#define PIN_SPI_DC 2           // use in LCD // ok
#define PIN_IN_BUTTON2 15      // ok
#endif

#endif
