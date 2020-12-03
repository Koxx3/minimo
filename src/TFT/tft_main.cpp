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

#include "TFT/smart.c"

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

#define PIN_SPI_CS -1  // goes to TFT CS -- not used
#define PIN_SPI_DC 2   // goes to TFT DC
#define PIN_SPI_RST 17 // goes to TFT RESET

#define PIN_OUT_BACKLIGHT 5

Adafruit_ILI9341 tft = Adafruit_ILI9341(PIN_SPI_CS, PIN_SPI_DC, PIN_SPI_RST);

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

void tftSetupBacklight()
{

  pinMode(PIN_OUT_BACKLIGHT, OUTPUT);
  digitalWrite(PIN_OUT_BACKLIGHT, 1);
  ledcSetup(0, 1000, 10);
  ledcAttachPin(PIN_OUT_BACKLIGHT, 0);
  ledcWrite(0, 0);
}

void tftBacklightFull()
{

  ledcWrite(0, 4095);
}

void tftSetup(SharedData *shrd, Settings *settings)
{

  _shrd = shrd;
  _settings = settings;

  // init TFT
  tft.begin(60000000);
  tft.invertDisplay(1);
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

  tftBacklightFull();
  
  // draw splash scree
  tft.drawRGBBitmap((320 - gimp_image.width) / 2, (240 - gimp_image.height) / 2, (uint16_t *)gimp_image.pixel_data, gimp_image.width, gimp_image.height);
 // delay(3000);
  tft.fillScreen(ILI9341_BLACK);


  // init TFT settings
  tft.setTextSize(1);
  tft.setFont(&FORCED_SQUARE6pt7b);

  // draw interface
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

void tftUpdateData(uint32_t i_loop)
{
  char fmt[10];

  switch (i_loop % 100)
  {
  case 0:
  {
    sprintf(fmt, "%03.0f", _shrd->speedCurrent);
    tft_util_draw_number(&tft, fmt, 5, LINE_1Y, ILI9341_WHITE, ILI9341_BLACK, 5, BIG_FONT_SIZE);
    break;
  }

  case 1:
  {
    sprintf(fmt, "%03.0f", _shrd->speedMax);
    tft_util_draw_number(&tft, fmt, 155, LINE_1Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALLEST_FONT_SIZE);
    break;
  }

  case 2:
  {
    sprintf(fmt, "%01.0f", (float)_shrd->modeOrder);
    tft_util_draw_number(&tft, fmt, 185, LINE_2Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 3:
  {
    sprintf(fmt, "%02.0f", _shrd->currentTemperature);
    tft_util_draw_number(&tft, fmt, 262, LINE_4Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 4:
  {
    sprintf(fmt, "%02.0f", _shrd->currentHumidity);
    tft_util_draw_number(&tft, fmt, 262, LINE_5Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 5:
  {
    double power = (_shrd->currentFilterMean / 1000.0 * _shrd->voltageFilterMean / 1000.0);
    if (power < 0)
      power = 0;
    sprintf(fmt, "%05.0f", power);
    tft_util_draw_number(&tft, fmt, 5, LINE_5Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 6:
  {
    timeToString().toCharArray(fmt, 9);
    tft_util_draw_number(&tft, fmt, 37, LINE_4Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 7:
  {
    float voltage = _shrd->voltageFilterMean / 1000.0;
    sprintf(fmt, "%s", Dfmt2_1(voltage));
    tft_util_draw_number(&tft, fmt, 233, LINE_2Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 8:
  {
    float distance = _shrd->distanceTrip / 10000.0;
    sprintf(fmt, "%s", Dfmt2_1(distance));
    tft_util_draw_number(&tft, fmt, 134, LINE_3Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 9:
  {
    float odo = _shrd->distanceOdo;
    sprintf(fmt, "%05.0f", (odo));
    tft_util_draw_number(&tft, fmt, 5, LINE_3Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 10:
  {
    float autonomy = 65.2;
    if (autonomy > 999)
      autonomy = 999;
    if (autonomy < 0)
      autonomy = 0;
    sprintf(fmt, "%03.0f", autonomy);
    tft_util_draw_number(&tft, fmt, 241, LINE_3Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 11:
  {
    float current = _shrd->currentFilterMean / 1000.0;
    if (current > 99)
      current = 99;
    if (current < 0)
      current = 0;
    sprintf(fmt, "%s", Dfmt2_1(current));
    tft_util_draw_number(&tft, fmt, 134, LINE_5Y, ILI9341_WHITE, ILI9341_BLACK, 5, SMALL_FONT_SIZE);
    break;
  }

  case 12:
  {
    float bat_min = _settings->getS3F().Battery_min_voltage / 10.0;
    float bat_max = _settings->getS3F().Battery_max_voltage / 10.0;
    float batteryPercent = (1 / ((bat_max - bat_min) / ((_shrd->voltageFilterMean / 1000.0) - bat_min)) * 100);
    drawBatteryJauge(&tft, batteryPercent);
    break;
  }
  }
}