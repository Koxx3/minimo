#include <SPI.h>

#include <TFT_eSPI.h>

#include "TFT/tft_main.h"
#include "TFT/tft_util.h"
#include "TFT/text_screen.h"
#include "TFT/color_jauge.h"

#include "SharedData.h"
#include "Settings.h"
#include "FORCED_SQUARE10pt7b.h"
#include "FORCED_SQUARE9pt7b.h"
#include "FORCED_SQUARE6pt7b.h"

#include "TFT/smart_splash.h"

// Stock font and GFXFF reference handle
#define GFXFF 1

// Easily remembered name for the font
#define FONT_FORCED_SQUARE9pt7b &FORCED_SQUARE9pt7b
#define FONT_FORCED_SQUARE6pt7b &FORCED_SQUARE6pt7b
#define FONT_FORCED_SQUARE10pt7b &FORCED_SQUARE10pt7b

#define SEP_LINE 2 * SCALE_FACTOR_X
#define SMALLEST_FONT_SIZE 4 * SCALE_FACTOR_Y
#define SMALL_FONT_SIZE 5 * SCALE_FACTOR_Y
#define BIG_FONT_SIZE 13 * SCALE_FACTOR_Y

#define LINE_1Y 5 * SCALE_FACTOR_Y
#define LINE_2Y 46 * SCALE_FACTOR_Y
#define LINE_3Y 100 * SCALE_FACTOR_Y
#define LINE_4Y 155 * SCALE_FACTOR_Y
#define LINE_5Y 210 * SCALE_FACTOR_Y

#if (TFT_MODEL == 1) // 2.4"
#define SCALE_FACTOR_X 1.5 
#define SCALE_FACTOR_Y 1.33
#define COLUMN1 80 * SCALE_FACTOR_X  // 78 for 3.5
#define COLUMN2 130 * SCALE_FACTOR_X // 120 for 3.5
#define COLUMN3 153 * SCALE_FACTOR_X // 120 for 3.5 // equal COLUMN2
#define COLUMN4 200 * SCALE_FACTOR_X // 192 for 3.5
#define COLUMN5 307 * SCALE_FACTOR_X // 307 for 3.5
#else  // 3.5"
#define COLUMN1 80 * SCALE_FACTOR_X  // 78 for 3.5
#define COLUMN2 115 * SCALE_FACTOR_X // 120 for 3.5
#define COLUMN3 115 * SCALE_FACTOR_X // 120 for 3.5
#define COLUMN4 200 * SCALE_FACTOR_X // 192 for 3.5
#define COLUMN5 307 * SCALE_FACTOR_X // 307 for 3.5
#endif

#define LINE_TEXT_OFFSET 7

#define PIN_SPI_CS -1  // goes to TFT CS -- not used
#define PIN_SPI_DC 2   // goes to TFT DC
#define PIN_SPI_RST 17 // goes to TFT RESET

#define PIN_OUT_BACKLIGHT 5

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
TFT_eSPI tft = TFT_eSPI();

SharedData *_shrd;
Settings *_settings;

float speed = 10.0;

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

  ledcWrite(0, 1023);
}

void tftBacklightLow()
{

  ledcWrite(0, 0);
}

void tftSetup(SharedData *shrd, Settings *settings)
{

  _shrd = shrd;
  _settings = settings;

  // dim screen for init
  tftBacklightLow();
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

  // -1 / show splash screen and init fix datas after
  if (i_loop == -1)
  {

    // init TFT
    tft.begin();
    tft.invertDisplay(1);
    tft.setRotation(1);

    tft.fillScreen(TFT_BLACK);

    tftBacklightFull();

    // Swap the colour byte order when rendering
    tft.setSwapBytes(true);

#if TFT_DISPLAY_SPLASH
    // draw splash screen
    tft.pushImage((TFT_HEIGHT - smart_splash_logoWidth) / 2, (TFT_WIDTH - smart_splash_logoHeight) / 2, smart_splash_logoWidth, smart_splash_logoHeight, smart_splash);

    delay(3000);
    tft.fillScreen(TFT_BLACK);
#endif

    // init TFT settings
    tft.setTextSize(1);

    if (SCALE_FACTOR_Y >= 1.5)
      tft.setFreeFont(FONT_FORCED_SQUARE10pt7b); // Select the font
    else if (SCALE_FACTOR_Y >= 1.3)
      tft.setFreeFont(FONT_FORCED_SQUARE9pt7b); // Select the font
    else
      tft.setFreeFont(FONT_FORCED_SQUARE6pt7b); // Select the font

    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextDatum(BR_DATUM);

    // draw interface
    tft.drawString(txt_mode, COLUMN4, LINE_2Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_volts, COLUMN5, LINE_2Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_time, COLUMN3, LINE_3Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_auton, COLUMN5, LINE_3Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_odo, COLUMN1, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_trip, COLUMN4, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_temp, COLUMN5, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_power, COLUMN1, LINE_5Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_current, COLUMN4, LINE_5Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_hr, COLUMN5, LINE_5Y - LINE_TEXT_OFFSET, GFXFF);

    // draw grid
    tft.fillRect(210 * SCALE_FACTOR_X, 0 * SCALE_FACTOR_Y, SEP_LINE, 253 * SCALE_FACTOR_X, ILI_DIGIT_DARK);
    tft.fillRect(0 * SCALE_FACTOR_X, 79 * SCALE_FACTOR_Y, 210 * SCALE_FACTOR_X, SEP_LINE, ILI_DIGIT_DARK);
    tft.fillRect(0 * SCALE_FACTOR_X, 189 * SCALE_FACTOR_Y, 210 * SCALE_FACTOR_X, SEP_LINE, ILI_DIGIT_DARK);
    tft.fillRect(210 * SCALE_FACTOR_X, 134 * SCALE_FACTOR_Y, 129 * SCALE_FACTOR_X, SEP_LINE, ILI_DIGIT_DARK);
  }
  else
  {
    switch (i_loop % 100)
    {
    case 0:
    {
      sprintf(fmt, "%03.0f", _shrd->speedCurrent);
      tft_util_draw_number(&tft, fmt, COLUMN2, LINE_1Y, TFT_WHITE, TFT_BLACK, 5, BIG_FONT_SIZE);

      speed = speed + 2.2;
      if (speed > 150)
        speed = 0;
      break;
    }

    case 1:
    {
      sprintf(fmt, "%03.0f", _shrd->speedMax);
      tft_util_draw_number(&tft, fmt, COLUMN4, LINE_1Y, TFT_WHITE, TFT_BLACK, 5, SMALLEST_FONT_SIZE);
      break;
    }

    case 2:
    {
      sprintf(fmt, "%01.0f", (float)_shrd->modeOrder);
      tft_util_draw_number(&tft, fmt, COLUMN4, LINE_2Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 3:
    {
      sprintf(fmt, "%02.0f", _shrd->currentTemperature);
      tft_util_draw_number(&tft, fmt, COLUMN5, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 4:
    {
      sprintf(fmt, "%02.0f", _shrd->currentHumidity);
      tft_util_draw_number(&tft, fmt, COLUMN5, LINE_5Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 5:
    {
      double power = (_shrd->currentActual / 1000.0 * _shrd->voltageFilterMean / 1000.0);
      if (power < 0)
        power = 0;
      sprintf(fmt, "%05.0f", power);
      tft_util_draw_number(&tft, fmt, COLUMN1, LINE_5Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 6:
    {
      timeToString().toCharArray(fmt, 9);
      tft_util_draw_number(&tft, fmt, COLUMN3, LINE_3Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 7:
    {
      float voltage = _shrd->voltageFilterMean / 1000.0;
      sprintf(fmt, "%s", Dfmt2_1(voltage));
      tft_util_draw_number(&tft, fmt, COLUMN5, LINE_2Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 8:
    {
      float distance = _shrd->distanceTrip / 10000.0;
      sprintf(fmt, "%s", Dfmt2_1(distance));
      tft_util_draw_number(&tft, fmt, COLUMN4, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 9:
    {
      float odo = _shrd->distanceOdo / 10;
      sprintf(fmt, "%05.0f", (odo));
      tft_util_draw_number(&tft, fmt, COLUMN1, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
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
      tft_util_draw_number(&tft, fmt, COLUMN5, LINE_3Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 11:
    {
      float current = _shrd->currentActual / 1000.0;
      if (current > 99)
        current = 99;
      if (current < 0)
        current = 0;
      sprintf(fmt, "%s", Dfmt2_1(current));
      tft_util_draw_number(&tft, fmt, COLUMN4, LINE_5Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
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
}