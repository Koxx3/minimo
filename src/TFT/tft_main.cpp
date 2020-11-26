#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "TFT/tft_main.h"
#include "TFT/tft_util.h"
#include "TFT/text_screen.h"
#include "TFT/color_jauge.h"

#include "SharedData.h"
#include "Settings.h"
#include "FORCED_SQUARE10pt7b.h"
#include "FORCED_SQUARE9pt7b.h"
#include "FORCED_SQUARE6pt7b.h"

#define SEP_LINE 2
#define SMALLEST_FONT_SIZE 4
#define SMALL_FONT_SIZE 5
#define BIG_FONT_SIZE 13

#define LINE_1Y 5
#define LINE_2Y 46
#define LINE_3Y 100
#define LINE_4Y 155
#define LINE_5Y 210

#define LINE_TEXT_OFFSET 7

#define _cs -1   // 3 goes to TFT CS
#define _dc 5    // 4 goes to TFT DC
#define _mosi 23 // 5 goes to TFT MOSI
#define _sclk 5  // 6 goes to TFT SCK/CLK
#define _rst 17  // ESP RST to TFT RESET
#define _miso -1 // Not connected
Adafruit_ILI9341 tft = Adafruit_ILI9341(_cs, _dc, _rst);

SharedData *_shrd;
Settings *_settings;

const char *txt_mode = "MODE";
const char *txt_temp = "TEMP";
const char *txt_volts = "VOLTS";
const char *txt_current = "CURRENT";
const char *txt_auton = "AUTON";
const char *txt_odo = "ODO";
const char *txt_time = "TIME";
const char *txt_trip = "TRIP";
const char *txt_hr = "HR";
const char *txt_power = "POWER";

void tftSetup(SharedData *shrd, Settings *settings)
{

  _shrd = shrd;
  _settings = settings;

  tft.begin(40000000);

  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.invertDisplay(1);
  tft.setFont(&FORCED_SQUARE6pt7b);

  write_text_line(&tft, txt_mode, 174, LINE_2Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_volts, 266, LINE_2Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_odo, 80, LINE_3Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_trip, 175, LINE_3Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_auton, 263, LINE_3Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_time, 129, LINE_4Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_temp, 269, LINE_4Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_power, 66, LINE_5Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_current, 148, LINE_5Y - LINE_TEXT_OFFSET, ILI9341_RED);
  write_text_line(&tft, txt_hr, 285, LINE_5Y - LINE_TEXT_OFFSET, ILI9341_RED);

  tft.fillRect(210, 0, SEP_LINE, 253, ILI_DIGIT_DARK);
  tft.fillRect(0, 79, 210, SEP_LINE, ILI_DIGIT_DARK);
  tft.fillRect(0, 189, 210, SEP_LINE, ILI_DIGIT_DARK);
  tft.fillRect(210, 134, 129, SEP_LINE, ILI_DIGIT_DARK);
}

static char *Dfmt2_1(double v)
{
  static char x[40];

  /**** This does all the convertion and formatting you need ****/
  sprintf(x, "%02d.%01d", (int)v, (int)(10 * (v - (int)v)));
  return (x);
}

String timeToString()
{
  char str[10];
  unsigned long currentMillis = millis();
  int seconds = currentMillis / 1000;
  int minutes = seconds / 60;
  int hours = minutes / 60;
  seconds %= 60;
  minutes %= 60;
  hours %= 24;
  sprintf(str, "%01d:%02d:%02d", hours, minutes, seconds);
  return String(str);
}

void tftUpdateData()
{
  char fmt[10];

  sprintf(fmt, "%03.0f", _shrd->speedCurrent);
  tft_util_draw_number(&tft, fmt, 5, LINE_1Y, ILI9341_WHITE, ILI9341_BLACK, 5, BIG_FONT_SIZE);

  sprintf(fmt, "%03.0f", _shrd->speedMax);
  tft_util_draw_number(&tft, fmt, 155, LINE_1Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALLEST_FONT_SIZE);

  sprintf(fmt, "%01.0f", (float)_shrd->modeOrder);
  tft_util_draw_number(&tft, fmt, 185, LINE_2Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  sprintf(fmt, "%02.0f", _shrd->currentTemperature);
  tft_util_draw_number(&tft, fmt, 262, LINE_4Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  sprintf(fmt, "%02.0f", _shrd->currentHumidity);
  tft_util_draw_number(&tft, fmt, 262, LINE_5Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  double power = (_shrd->currentFilterMean / 1000.0 * _shrd->voltageFilterMean / 1000.0);
  if (power < 0)
    power = 0;
  sprintf(fmt, "%05.0f", power);
  tft_util_draw_number(&tft, fmt, 5, LINE_5Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  timeToString().toCharArray(fmt, 9);
  tft_util_draw_number(&tft, fmt, 37, LINE_4Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  float voltage = _shrd->voltageFilterMean / 1000.0;
  sprintf(fmt, "%s", Dfmt2_1(voltage));
  tft_util_draw_number(&tft, fmt, 233, LINE_2Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  float distance = _shrd->distanceTrip / 10000.0;
  sprintf(fmt, "%s", Dfmt2_1(distance));
  tft_util_draw_number(&tft, fmt, 134, LINE_3Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  float odo = _shrd->distanceOdo;
  sprintf(fmt, "%05.0f", (odo));
  tft_util_draw_number(&tft, fmt, 5, LINE_3Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  float autonomy = 65.2;
  if (autonomy > 999)
    autonomy = 999;
  if (autonomy < 0)
    autonomy = 0;
  sprintf(fmt, "%03.0f", autonomy);
  tft_util_draw_number(&tft, fmt, 241, LINE_3Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  float current = _shrd->currentFilterMean / 1000.0;
  if (current > 99)
    current = 99;
  if (current < 0)
    current = 0;
  sprintf(fmt, "%s", Dfmt2_1(current));
  tft_util_draw_number(&tft, fmt, 134, LINE_5Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);

  float bat_min = _settings->getS3F().Battery_min_voltage / 10.0;
  float bat_max = _settings->getS3F().Battery_max_voltage / 10.0;
  float batteryPercent = (1 / ((bat_max - bat_min) / ((_shrd->voltageFilterMean / 1000.0) - bat_min)) * 100);
  drawBatteryJauge(&tft, batteryPercent);
}