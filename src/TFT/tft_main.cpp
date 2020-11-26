#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "TFT/tft_main.h"
#include "TFT/tft_util.h"
#include "TFT/text_screen.h"
#include "TFT/color_jauge.h"

#include "SharedData.h"
#include "Settings.h"

#define _cs -1   // 3 goes to TFT CS
#define _dc 5    // 4 goes to TFT DC
#define _mosi 23 // 5 goes to TFT MOSI
#define _sclk 5  // 6 goes to TFT SCK/CLK
#define _rst 17  // ESP RST to TFT RESET
#define _miso -1 // Not connected
Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _rst);

SharedData *_shrd;
Settings *_settings;

const char *txt1 = "MODE";
const char *txt2 = "TEMP";
const char *txt3 = "VOLTS";
const char *txt4 = "CURRENT";

void tftSetup(SharedData* shrd, Settings* settings)
{

  _shrd = shrd;
  _settings = settings;

  tft.begin(40000000);

  tft.setRotation(0);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.invertDisplay(1);

  write_text_line(&tft, txt1, 48, 150, ILI9341_RED);
  write_text_line(&tft, txt2, 175, 150, ILI9341_RED);
  write_text_line(&tft, txt3, 35, 230, ILI9341_RED);
  write_text_line(&tft, txt4, 135, 230, ILI9341_RED);
}

void tftUpdateData()
{
  char fmt[10];
  // draw voltage
  dtostrf(_shrd->speedCurrent, 2, 0, fmt);
  tft_util_draw_number(&tft, fmt, 105, 20, ILI9341_WHITE, ILI9341_BLACK, 5, 20);

  dtostrf(_shrd->modeOrder, 2, 0, fmt);
  tft_util_draw_number(&tft, fmt, 50, 160, ILI9341_WHITE, ILI9341_BLACK, 5, 8);

  dtostrf(_shrd->currentTemperature, 2, 0, fmt);
  tft_util_draw_number(&tft, fmt, 175, 160, ILI9341_WHITE, ILI9341_BLACK, 5, 8);

  dtostrf(_shrd->voltageFilterMean / 1000.0, 4, 1, fmt);
  tft_util_draw_number(&tft, fmt, 10, 240, ILI9341_WHITE, ILI9341_BLACK, 5, 8);

  float current = _shrd->currentFilterMean / 1000.0;
  if (current > 99)
    current = 99;
  if (current < 0)
    current = 0;
  dtostrf(current, 4, 1, fmt);
  tft_util_draw_number(&tft, fmt, 135, 240, ILI9341_WHITE, ILI9341_BLACK, 5, 8);

  float bat_min = _settings->getS3F().Battery_min_voltage / 10.0;
  float bat_max = _settings->getS3F().Battery_max_voltage / 10.0;
  float batteryPercent = (1 / ((bat_max - bat_min) / ((_shrd->voltageFilterMean / 1000.0) - bat_min)) * 100);
  drawBatteryJauge(&tft, batteryPercent);
}