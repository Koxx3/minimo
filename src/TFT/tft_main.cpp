#include <SPI.h>

#include <TFT_eSPI.h>

#include "TFT/tft_main.h"
#include "TFT/tft_util.h"
#include "TFT/text_screen.h"
#include "TFT/color_jauge.h"

#include "SharedData.h"
#include "Settings.h"
#include "FORCED_SQUARE6pt7b.h"
#include "FORCED_SQUARE7pt7b.h"
#include "FORCED_SQUARE8pt7b.h"
#include "FORCED_SQUARE9pt7b.h"
#include "FORCED_SQUARE10pt7b.h"
#include "FORCED_SQUARE12pt7b.h"
#include "FORCED_SQUARE14pt7b.h"
#include "FORCED_SQUARE18pt7b.h"

#include "TFT/smart_splash.h"

// Stock font and GFXFF reference handle
#define GFXFF 1

// Easily remembered name for the font
#define FONT_FORCED_SQUARE6pt7b &FORCED_SQUARE6pt7b
#define FONT_FORCED_SQUARE7pt7b &FORCED_SQUARE7pt7b
#define FONT_FORCED_SQUARE8pt7b &FORCED_SQUARE8pt7b
#define FONT_FORCED_SQUARE9pt7b &FORCED_SQUARE9pt7b
#define FONT_FORCED_SQUARE10pt7b &FORCED_SQUARE10pt7b
#define FONT_FORCED_SQUARE12pt7b &FORCED_SQUARE12pt7b
#define FONT_FORCED_SQUARE14pt7b &FORCED_SQUARE14pt7b
#define FONT_FORCED_SQUARE18pt7b &FORCED_SQUARE18pt7b

#define SEP_LINE 2 * SCALE_FACTOR_X
#define UNIT_LEFT_MARGIN 5 * SCALE_FACTOR_X

#define SMALLEST_FONT_SIZE 4 * SCALE_FACTOR_Y
#define SMALL_FONT_SIZE 5 * SCALE_FACTOR_Y
#define MEDIUM_FONT_SIZE 8 * SCALE_FACTOR_Y
#define BIG_FONT_SIZE 18 * SCALE_FACTOR_Y

#if (TFT_MODEL == 1) // 2.4"
#define LINE_1Y 9 * SCALE_FACTOR_Y
#define LINE_2Y 23 * SCALE_FACTOR_Y
#define LINE_2Y_UNIT LINE_2Y + (27 * SCALE_FACTOR_Y)
#define LINE_3Y 79 * SCALE_FACTOR_Y
#define LINE_3Y_UNIT1 LINE_3Y + (95 * SCALE_FACTOR_Y) // Km/m speed
#define LINE_3Y_UNIT2 LINE_3Y + (26 * SCALE_FACTOR_Y) // speed max
#define LINE_3Y_UNIT3 LINE_3Y + (15 * SCALE_FACTOR_Y) // Km/h speed max
#define LINE_4Y 210 * SCALE_FACTOR_Y
#define LINE_4Y_UNIT LINE_4Y + (22 * SCALE_FACTOR_Y)
#else // 3.5"
#define LINE_1Y 9 * SCALE_FACTOR_Y
#define LINE_2Y 23 * SCALE_FACTOR_Y
#define LINE_2Y_UNIT LINE_2Y + (25 * SCALE_FACTOR_Y)
#define LINE_3Y 79 * SCALE_FACTOR_Y
#define LINE_3Y_UNIT1 LINE_3Y + (91 * SCALE_FACTOR_Y) // Km/m speed
#define LINE_3Y_UNIT2 LINE_3Y + (25 * SCALE_FACTOR_Y) // speed max
#define LINE_3Y_UNIT3 LINE_3Y + (14 * SCALE_FACTOR_Y) // Km/h speed max
#define LINE_4Y 210 * SCALE_FACTOR_Y
#define LINE_4Y_UNIT LINE_4Y + (21 * SCALE_FACTOR_Y)
#endif

// separation lines
// for left indicators
#define LINE_POINT1X 38 * SCALE_FACTOR_X
#define LINE_POINT1Y 58 * SCALE_FACTOR_Y
#define LINE_POINT2X (38 + 15) * SCALE_FACTOR_X
#define LINE_POINT2Y (58 + 15) * SCALE_FACTOR_Y
#define LINE_POINT3X (38 + 15) * SCALE_FACTOR_X
#define LINE_POINT3Y 173 * SCALE_FACTOR_Y
#define LINE_POINT4X 38 * SCALE_FACTOR_X
#define LINE_POINT4Y (173 + 15) * SCALE_FACTOR_Y
// upper
#define LINE_POINT5X 70 * SCALE_FACTOR_X
#define LINE_POINT5Y LINE_POINT1Y
// lower
#define LINE_POINT6X 70 * SCALE_FACTOR_X
#define LINE_POINT6Y LINE_POINT4Y



#define SPACE_INDICATORS_Y 19

#define COLUMN0 24 * SCALE_FACTOR_X
#define COLUMN1 39 * SCALE_FACTOR_X
#define COLUMN2 82 * SCALE_FACTOR_X
#define COLUMN3 118 * SCALE_FACTOR_X
#define COLUMN4 219 * SCALE_FACTOR_X
#define COLUMN5 225 * SCALE_FACTOR_X
#define COLUMN6 215 * SCALE_FACTOR_X
#define COLUMN7 247 * SCALE_FACTOR_X
#define COLUMN8 279 * SCALE_FACTOR_X
#define COLUMN9 292 * SCALE_FACTOR_X

#define LINE_TEXT_OFFSET 6

#define PIN_SPI_CS -1  // goes to TFT CS -- not used
#define PIN_SPI_DC 2   // goes to TFT DC
#define PIN_SPI_RST 17 // goes to TFT RESET

#define PIN_OUT_BACKLIGHT 5

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
TFT_eSPI tft = TFT_eSPI();

SharedData *_shrd;
Settings *_settings;

float speed = 10.0;

const char *txt_temp = "TMP";
const char *txt_pas = "PAS";
const char *txt_volts = "VOLTS";
const char *txt_err = "ERR";
const char *txt_auton = "AUTON";
const char *txt_time = "TIME";
const char *txt_trip = "TRIP";
const char *txt_hr = "HR";
const char *txt_power = "POWER";
const char *txt_brk = "BRK";
const char *txt_kmh = "Km/h";
const char *txt_km = "Km";
const char *txt_max = "Max";
const char *txt_w = "W";
const char *txt_v = "V";

uint8_t oldShrdPasEnabled = 255;
uint8_t oldShrdBrakePressedStatus = 255;
uint8_t oldShrdCurrentTemperature = 255;
uint8_t oldShrdCurrentHumidity = 255;
uint8_t oldError = 255;

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
  uint16_t brigtness = map(_settings->getS6F().Display_brightness, 0, 100, 0, 1023);
  ledcWrite(0, brigtness);
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

#if (TFT_MODEL == 2) // 3.5"
    tft.setFreeFont(FONT_FORCED_SQUARE10pt7b);
#else
    tft.setFreeFont(FONT_FORCED_SQUARE6pt7b);
#endif

    // draw interface - labels
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextDatum(BR_DATUM);
    tft.drawString(txt_volts, COLUMN4, LINE_2Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_auton, COLUMN3, LINE_2Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_power, COLUMN2, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_time, COLUMN6, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_trip, COLUMN9, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);

    // draw interface - units
#if (TFT_MODEL == 2) // 3.5"
    tft.setFreeFont(FONT_FORCED_SQUARE10pt7b);
#else
    tft.setFreeFont(FONT_FORCED_SQUARE7pt7b);
#endif
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(BL_DATUM);
    tft.drawString(txt_km, COLUMN3 + UNIT_LEFT_MARGIN, LINE_2Y_UNIT, GFXFF);
    tft.drawString(txt_v, COLUMN4 + UNIT_LEFT_MARGIN, LINE_2Y_UNIT, GFXFF);
    tft.drawString(txt_kmh, COLUMN8 + UNIT_LEFT_MARGIN, LINE_3Y_UNIT2, GFXFF);
    tft.drawString(txt_max, COLUMN8 + UNIT_LEFT_MARGIN, LINE_3Y_UNIT3, GFXFF);
    tft.drawString(txt_w, COLUMN2 + UNIT_LEFT_MARGIN, LINE_4Y_UNIT, GFXFF);
    tft.drawString(txt_km, COLUMN9 + UNIT_LEFT_MARGIN, LINE_4Y_UNIT, GFXFF);

#if (TFT_MODEL == 2) // 3.5"
    tft.setFreeFont(FONT_FORCED_SQUARE18pt7b);
#else
    tft.setFreeFont(FONT_FORCED_SQUARE14pt7b);
#endif
    tft.drawString(txt_kmh, COLUMN5 + UNIT_LEFT_MARGIN, LINE_3Y_UNIT1, GFXFF);

    // draw grid
    tft.drawLine(0, LINE_POINT1Y, LINE_POINT1X, LINE_POINT1Y, ILI_DIGIT_DARK_DISABLED);
    tft.drawLine(LINE_POINT1X, LINE_POINT1Y, LINE_POINT2X, LINE_POINT2Y, ILI_DIGIT_DARK_DISABLED);
    tft.drawLine(LINE_POINT2X, LINE_POINT2Y, LINE_POINT3X, LINE_POINT3Y, ILI_DIGIT_DARK_DISABLED);
    tft.drawLine(LINE_POINT3X, LINE_POINT3Y, LINE_POINT4X, LINE_POINT4Y, ILI_DIGIT_DARK_DISABLED);
    tft.drawLine(LINE_POINT4X, LINE_POINT4Y, 0, LINE_POINT4Y, ILI_DIGIT_DARK_DISABLED);

    tft.drawLine(LINE_POINT5X, LINE_POINT5Y, TFT_HEIGHT, LINE_POINT5Y, ILI_DIGIT_DARK_DISABLED);
    tft.drawLine(LINE_POINT6X, LINE_POINT6Y, TFT_HEIGHT, LINE_POINT6Y, ILI_DIGIT_DARK_DISABLED);
  }
  else
  {
    switch (i_loop % 100)
    {
    case 0:
    {
      sprintf(fmt, "%02.0f", _shrd->speedCurrent);
      tft_util_draw_number(&tft, fmt, COLUMN5, LINE_3Y, TFT_WHITE, TFT_BLACK, 5, BIG_FONT_SIZE);

      speed = speed + 2.2;
      if (speed > 150)
        speed = 0;
      break;
    }
    case 1:
    {
      sprintf(fmt, "%02.0f", _shrd->speedMax);
      tft_util_draw_number(&tft, fmt, COLUMN8, LINE_3Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 2:
    {
      sprintf(fmt, "%01.0f", (float)_shrd->modeOrder);
      tft_util_draw_number(&tft, fmt, COLUMN1, LINE_1Y, TFT_WHITE, TFT_BLACK, 5, MEDIUM_FONT_SIZE);
      break;
    }
      /*
    case 3:
    {
      sprintf(fmt, "%02.0f", _shrd->currentTemperature);
      tft_util_draw_number(&tft, fmt, COLUMN5, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 4:
    {
      sprintf(fmt, "%02.0f", _shrd->currentHumidity);
      tft_util_draw_number(&tft, fmt, COLUMN5, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }
*/
    case 5:
    {
      double power = (_shrd->currentActual / 1000.0 * _shrd->voltageFilterMean / 1000.0);
      if (power < 0)
        power = 0;
      sprintf(fmt, "%05.0f", power);
      tft_util_draw_number(&tft, fmt, COLUMN2, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALLEST_FONT_SIZE);
      break;
    }

    case 6:
    {
      timeToString().toCharArray(fmt, 9);
      tft_util_draw_number(&tft, fmt, COLUMN6, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALLEST_FONT_SIZE);
      break;
    }

    case 7:
    {
      float voltage = _shrd->voltageFilterMean / 1000.0;
      sprintf(fmt, "%s", Dfmt2_1(voltage));
      tft_util_draw_number(&tft, fmt, COLUMN4, LINE_2Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 8:
    {
      float distance = _shrd->distanceTrip / 10000.0;
      sprintf(fmt, "%s", Dfmt2_1(distance));
      tft_util_draw_number(&tft, fmt, COLUMN9, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALLEST_FONT_SIZE);
      break;
    }
      /*
    case 9:
    {
      float odo = _shrd->distanceOdo / 10;
      sprintf(fmt, "%05.0f", (odo));
      tft_util_draw_number(&tft, fmt, COLUMN1, LINE_4Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }
*/
    case 10:
    {
      float autonomy = 65.2;
      if (autonomy > 999)
        autonomy = 999;
      if (autonomy < 0)
        autonomy = 0;
      sprintf(fmt, "%03.0f", autonomy);
      tft_util_draw_number(&tft, fmt, COLUMN3, LINE_2Y, TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }
      /*
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
*/
    case 12:
    {
      float bat_min = _settings->getS3F().Battery_min_voltage / 10.0;
      float bat_max = _settings->getS3F().Battery_max_voltage / 10.0;
      float batteryPercent = (1 / ((bat_max - bat_min) / ((_shrd->voltageFilterMean / 1000.0) - bat_min)) * 100);
      drawBatteryJauge(&tft, batteryPercent, COLUMN7 * SCALE_FACTOR_X, LINE_2Y * SCALE_FACTOR_Y, 4 * SCALE_FACTOR_Y, 25 * SCALE_FACTOR_Y, 17);
      break;
    }

    case 13:
    {

      // draw interface - indicators
#if (TFT_MODEL == 2) // 3.5"
      tft.setFreeFont(FONT_FORCED_SQUARE12pt7b);
#else
      tft.setFreeFont(FONT_FORCED_SQUARE9pt7b);
#endif
      tft.setTextDatum(TC_DATUM);

      int i = 0;
      if (oldShrdPasEnabled != _shrd->pasEnabled)
      {
        tft.setTextColor(_shrd->pasEnabled ? TFT_WHITE : ILI_DIGIT_DARK_DISABLED, TFT_BLACK);
        tft.drawString(txt_pas, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdPasEnabled = _shrd->pasEnabled;
      }

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      if (oldShrdBrakePressedStatus != _shrd->brakePressedStatus)
      {
        tft.setTextColor(_shrd->brakePressedStatus ? TFT_WHITE : ILI_DIGIT_DARK_DISABLED, TFT_BLACK);
        tft.drawString(txt_brk, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdBrakePressedStatus = _shrd->brakePressedStatus;
      }

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      uint8_t currentTemperatureStatus = (_shrd->currentTemperature > _settings->getS6F().Temperature_warning);
      if (oldShrdCurrentTemperature != currentTemperatureStatus)
      {
        tft.setTextColor(currentTemperatureStatus ? TFT_RED : ILI_DIGIT_DARK_DISABLED, TFT_BLACK);
        tft.drawString(txt_temp, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdCurrentTemperature = currentTemperatureStatus;
      }

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      uint8_t currentHumidityStatus = (_shrd->currentHumidity > _settings->getS6F().Humidity_warning);
      if (oldShrdCurrentHumidity != currentHumidityStatus)
      {
        tft.setTextColor(currentHumidityStatus ? TFT_RED : ILI_DIGIT_DARK_DISABLED, TFT_BLACK);
        tft.drawString(txt_hr, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdCurrentHumidity = currentHumidityStatus;
      }

      boolean error = _shrd->errorThrottle ||
                      _shrd->errorBrake ||
                      _shrd->errorSerialFromDisplay ||
                      _shrd->errorSerialFromContrl ||
                      _shrd->errorSerialContrlDataFeedback;

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      if (oldError != error)
      {
        tft.setTextColor(error ? TFT_RED : ILI_DIGIT_DARK_DISABLED, TFT_BLACK);
        tft.drawString(txt_err, COLUMN0, LINE_3Y + i, GFXFF);
        oldError = error;
      }

      break;
    }
    case 14:
    {
      tftBacklightFull();
      break;
    }
    }
  }
}