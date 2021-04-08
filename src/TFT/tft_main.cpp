#include <SPI.h>

#include "SPIFFS.h"
#include "AnimatedGIF.h"

#include "tft_main.h"
#include "tft_colors.h"

#include "Settings.h"
#include "SharedData.h"
#include "pinout.h"

#include "TFT/fonts/FORCED_SQUARE10pt7b.h"
#include "TFT/fonts/FORCED_SQUARE12pt7b.h"
#include "TFT/fonts/FORCED_SQUARE14pt7b.h"
#include "TFT/fonts/FORCED_SQUARE18pt7b.h"
#include "TFT/fonts/FORCED_SQUARE6pt7b.h"
#include "TFT/fonts/FORCED_SQUARE7pt7b.h"
#include "TFT/fonts/FORCED_SQUARE8pt7b.h"
#include "TFT/fonts/FORCED_SQUARE9pt7b.h"

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

#if ((TFT_MODEL == 2) || (TFT_MODEL == 3)) // 3.5"
#define FONT_RED_LABEL FONT_FORCED_SQUARE10pt7b
#define FONT_UNIT FONT_FORCED_SQUARE10pt7b
#define FONT_UNIT_HEIGH 10
#define FONT_NUMBER FONT_FORCED_SQUARE18pt7b
#define FONT_NUMBER_HEIGHT 18
#else
#define FONT_RED_LABEL FONT_FORCED_SQUARE6pt7b
#define FONT_UNIT FONT_FORCED_SQUARE7pt7b
#define FONT_UNIT_HEIGH 7
#define FONT_NUMBER FONT_FORCED_SQUARE14pt7b
#define FONT_NUMBER_HEIGHT 14
#endif

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
#define NB_BATTERY_BARS 17
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
#define NB_BATTERY_BARS 17
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

#define SPACE_INDICATORS_Y 16

#define COLUMN0 24 * SCALE_FACTOR_X
#define COLUMN1 39 * SCALE_FACTOR_X
#define COLUMN2 82 * SCALE_FACTOR_X
#define COLUMN3 118 * SCALE_FACTOR_X
#define COLUMN4 219 * SCALE_FACTOR_X
#define COLUMN5 225 * SCALE_FACTOR_X
#define COLUMN6 215 * SCALE_FACTOR_X
#define COLUMN7 247 * SCALE_FACTOR_X
#define COLUMN8 278 * SCALE_FACTOR_X //279
#define COLUMN9 292 * SCALE_FACTOR_X

#define LINE_TEXT_OFFSET 6

#define NORMAL_SPEED // Comment out for rame rate for render speed test

#define NB_LOOP_ANIM_GIF 1
#define GIF_PATH_SPLASH "/gifs/splash.gif"
#define GIF_PATH_OTA "/gifs/update.gif"

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
TFT_eSPI tft = TFT_eSPI();

// TODO : clean this...
extern SharedData shrd;
extern Settings settings;

const char *txt_temp = "TMP";
const char *txt_pas = "PAS";
const char *txt_volts = "VOLTS";
const char *txt_err = "ERR";
const char *txt_lck = "LCK";
const char *txt_auton = "AUTON";
const char *txt_time = "TIME";
const char *txt_trip = "TRIP";
const char *txt_hr = "HR";
const char *txt_power = "POWER";
const char *txt_brk = "BRK";
const char *txt_aux = "AUX";
const char *txt_kmh = "Km/h";
const char *txt_km = "Km";
const char *txt_max = "Max";
const char *txt_w = "W ";
const char *txt_v = "V";
const char *txt_c = "C";

uint8_t oldShrdPasEnabled = 255;
uint8_t oldShrdBrakePressedStatus = 255;
uint8_t oldShrdCurrentTemperature = 255;
uint8_t oldShrdCurrentHumidity = 255;
uint8_t oldError = 255;
uint8_t oldShrdIsLocked = 255;
uint8_t oldAuxOrder = 255;

uint8_t old_substate_case3 = 0;
uint8_t old_substate_case7 = 0;
uint8_t old_substate_case6 = 0;
uint8_t old_substate_ota = 0;

bool lock = false;

//-------------------------------------------------------
//
// GIF fonctions
//
//-------------------------------------------------------
#define DISPLAY_WIDTH tft.width()

// rule: loop GIF at least during 3s, maximum 5 times, and don't loop/animate longer than 30s per GIF
const int maxLoopIterations = 5;   // stop after this amount of loops
const int maxLoopsDuration = 5000; // ms, max cumulated time after the GIF will break loop
const int maxGifDuration = 30000;  // ms, max GIF duration

// used to center image based on GIF dimensions
static int xOffset = 0;
static int yOffset = 0;

static int currentFile = 0;
static int lastFile = -1;

static File FSGifFile; // temp gif file holder

static void *GIFOpenFile(const char *fname, int32_t *pSize)
{
  //log_d("GIFOpenFile( %s )\n", fname );
  FSGifFile = SPIFFS.open(fname);
  if (FSGifFile)
  {
    *pSize = FSGifFile.size();
    return (void *)&FSGifFile;
  }
  return NULL;
}

static void GIFCloseFile(void *pHandle)
{
  File *f = static_cast<File *>(pHandle);
  if (f != NULL)
    f->close();
}

static int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{
  int32_t iBytesRead;
  iBytesRead = iLen;
  File *f = static_cast<File *>(pFile->fHandle);
  // Note: If you read a file all the way to the last byte, seek() stops working
  if ((pFile->iSize - pFile->iPos) < iLen)
    iBytesRead = pFile->iSize - pFile->iPos - 1; // <-- ugly work-around
  if (iBytesRead <= 0)
    return 0;
  iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
  pFile->iPos = f->position();
  return iBytesRead;
}

static int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{
  int i = micros();
  File *f = static_cast<File *>(pFile->fHandle);
  f->seek(iPosition);
  pFile->iPos = (int32_t)f->position();
  i = micros() - i;
  //log_d("Seek time = %d us\n", i);
  return pFile->iPos;
}

static void TFTDraw(int x, int y, int w, int h, uint16_t *lBuf)
{
  tft.pushRect(x + xOffset, y + yOffset, w, h, lBuf);
}

// Draw a line of image directly on the LCD
void GIFDraw(GIFDRAW *pDraw)
{
  uint8_t *s;
  uint16_t *d, *usPalette, usTemp[320];
  int x, y, iWidth;

  iWidth = pDraw->iWidth;
  if (iWidth > DISPLAY_WIDTH)
    iWidth = DISPLAY_WIDTH;
  usPalette = pDraw->pPalette;
  y = pDraw->iY + pDraw->y; // current line

  s = pDraw->pPixels;
  if (pDraw->ucDisposalMethod == 2)
  { // restore to background color
    for (x = 0; x < iWidth; x++)
    {
      if (s[x] == pDraw->ucTransparent)
        s[x] = pDraw->ucBackground;
    }
    pDraw->ucHasTransparency = 0;
  }
  // Apply the new pixels to the main image
  if (pDraw->ucHasTransparency)
  { // if transparency used
    uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
    int x, iCount;
    pEnd = s + iWidth;
    x = 0;
    iCount = 0; // count non-transparent pixels
    while (x < iWidth)
    {
      c = ucTransparent - 1;
      d = usTemp;
      while (c != ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent)
        {      // done, stop
          s--; // back up to treat it like transparent
        }
        else
        { // opaque
          *d++ = usPalette[c];
          iCount++;
        }
      } // while looking for opaque pixels
      if (iCount)
      { // any opaque pixels?
        TFTDraw(pDraw->iX + x, y, iCount, 1, (uint16_t *)usTemp);
        x += iCount;
        iCount = 0;
      }
      // no, look for a run of transparent pixels
      c = ucTransparent;
      while (c == ucTransparent && s < pEnd)
      {
        c = *s++;
        if (c == ucTransparent)
          iCount++;
        else
          s--;
      }
      if (iCount)
      {
        x += iCount; // skip these
        iCount = 0;
      }
    }
  }
  else
  {
    s = pDraw->pPixels;
    // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
    for (x = 0; x < iWidth; x++)
      usTemp[x] = usPalette[*s++];
    TFTDraw(pDraw->iX, y, iWidth, 1, (uint16_t *)usTemp);
  }
} /* GIFDraw() */

int gifPlay(AnimatedGIF *gif, char *gifPath, int16_t y_offset)
{ // 0=infinite

  gif->begin(BIG_ENDIAN_PIXELS);

  if (!gif->open(gifPath, GIFOpenFile, GIFCloseFile, GIFReadFile, GIFSeekFile, GIFDraw))
  {
    Serial.printf("Could not open gif %s", gifPath);
    return maxLoopsDuration;
  }

  int frameDelay = 0; // store delay for the last frame
  int then = 0;       // store overall delay

  // center the GIF !!
  int w = gif->getCanvasWidth();
  int h = gif->getCanvasHeight();
  xOffset = (tft.width() - w) / 2;
  yOffset = ((tft.height() - h) / 2) + y_offset;

  if (lastFile != currentFile)
  {
    Serial.printf("Playing %s [%d,%d] with offset [%d,%d]", gifPath, w, h, xOffset, yOffset);
    lastFile = currentFile;
  }

  while (gif->playFrame(true, &frameDelay))
  {
    then += frameDelay;
    if (then > maxGifDuration)
    { // avoid being trapped in infinite GIF's
      //log_w("Broke the GIF loop, max duration exceeded");
      break;
    }
  }

  gif->close();

  return then;
}

//-------------------------------------------------------
//
// Bakclight fonctions
//
//-------------------------------------------------------

void tftSetupBacklight()
{

  pinMode(PIN_SPI_BKL, OUTPUT);
  digitalWrite(PIN_SPI_BKL, 1);
  ledcSetup(0, 1000, 10);
  ledcAttachPin(PIN_SPI_BKL, 0);
  ledcWrite(0, 0);
}

void tftBacklightFull()
{
  if (!lock)
  {
    uint16_t brigtness = map(settings.get_Display_brightness(), 0, 100, 0, 1023);
    ledcWrite(0, brigtness);
  }
}

void tftBacklightLow(bool lock_p)
{
  lock = lock_p;
  ledcWrite(0, 0);
}

//-------------------------------------------------------
//
// Main fonctions
//
//-------------------------------------------------------
void tftSetup(SharedData *shrd_p, Settings *settings_p)
{

  //shrd = shrd_p;
  //settings = settings_p;

  // dim screen for init
  tftBacklightLow(false);

  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS not found !!!");
  }

  tft.begin();
  tft.fillScreen(TFT_BLACK);
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

  if (i_loop == -2) // -2 / show splash screen
  {
    Serial.print("step -2");

    // init TFT
    tft.begin();
    tft.invertDisplay(1);
    tft.setRotation(settings.get_Rotate_screen());

    tft.fillScreen(TFT_BLACK);

    // Initialise SPIFFS
    bool spiffs = SPIFFS.begin();
    if (!spiffs)
    {
      Serial.println("SPIFFS initialisation failed!");
    }
    else
    {

      // Display GIF image
      int loops = maxLoopIterations;          // max loops
      int durationControl = maxLoopsDuration; // force break loop after xxx ms
      AnimatedGIF gif;
      while (loops-- > 0 && durationControl > 0)
      {
        durationControl -= gifPlay(&gif, (char *)GIF_PATH_SPLASH, 0);
        gif.reset();
      }

      tftBacklightFull();

      // Show loading progress
      for (int i = 0; i < 10; i++)
      {
        tft.fillRect(i * (TFT_HEIGHT / 10), 0, (TFT_HEIGHT / 10) - 5, 6 * SCALE_FACTOR_Y, MY_TFT_WHITE);
        delay(250);
      }
    }
  }
  else if (i_loop == -1) // init fix datas after
  {

    // reset indicators status
    oldShrdPasEnabled = 255;
    oldShrdBrakePressedStatus = 255;
    oldShrdCurrentTemperature = 255;
    oldShrdCurrentHumidity = 255;
    oldError = 255;
    oldShrdIsLocked = 255;
    oldAuxOrder = 255;

    tft.fillScreen(TFT_BLACK);

    // init TFT settings
    tft.setTextSize(1);

    tft.setFreeFont(FONT_RED_LABEL);
    // draw interface - labels
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setTextDatum(BR_DATUM);
    tft.drawString(txt_volts, COLUMN4, LINE_2Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_auton, COLUMN3, LINE_2Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_power, COLUMN2, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_time, COLUMN6, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);
    tft.drawString(txt_trip, COLUMN9, LINE_4Y - LINE_TEXT_OFFSET, GFXFF);

    // draw interface - units
    tft.setFreeFont(FONT_UNIT);
    tft.setTextColor(MY_TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(BL_DATUM);
    tft.drawString(txt_km, COLUMN3 + UNIT_LEFT_MARGIN, LINE_2Y_UNIT, GFXFF);
    tft.drawString(txt_v, COLUMN4 + UNIT_LEFT_MARGIN, LINE_2Y_UNIT, GFXFF);
    tft.drawString(txt_kmh, COLUMN8 + UNIT_LEFT_MARGIN, LINE_3Y_UNIT2, GFXFF);

    tft.drawString(txt_w, COLUMN2 + UNIT_LEFT_MARGIN, LINE_4Y_UNIT, GFXFF);
    tft.drawString(txt_km, COLUMN9 + UNIT_LEFT_MARGIN, LINE_4Y_UNIT, GFXFF);

    tft.setFreeFont(FONT_RED_LABEL);
    tft.drawString(txt_max, COLUMN8 + UNIT_LEFT_MARGIN, LINE_3Y_UNIT3, GFXFF);

    tft.setFreeFont(FONT_NUMBER);
    tft.drawString(txt_kmh, COLUMN5 + UNIT_LEFT_MARGIN, LINE_3Y_UNIT1, GFXFF);

    // draw grid
    tft.drawLine(0, LINE_POINT1Y, LINE_POINT1X, LINE_POINT1Y, MY_TFT_DARK_DIGIT_DISABLED);
    tft.drawLine(LINE_POINT1X, LINE_POINT1Y, LINE_POINT2X, LINE_POINT2Y, MY_TFT_DARK_DIGIT_DISABLED);
    tft.drawLine(LINE_POINT2X, LINE_POINT2Y, LINE_POINT3X, LINE_POINT3Y, MY_TFT_DARK_DIGIT_DISABLED);
    tft.drawLine(LINE_POINT3X, LINE_POINT3Y, LINE_POINT4X, LINE_POINT4Y, MY_TFT_DARK_DIGIT_DISABLED);
    tft.drawLine(LINE_POINT4X, LINE_POINT4Y, 0, LINE_POINT4Y, MY_TFT_DARK_DIGIT_DISABLED);

    tft.drawLine(LINE_POINT5X, LINE_POINT5Y, TFT_HEIGHT, LINE_POINT5Y, MY_TFT_DARK_DIGIT_DISABLED);
    tft.drawLine(LINE_POINT6X, LINE_POINT6Y, TFT_HEIGHT, LINE_POINT6Y, MY_TFT_DARK_DIGIT_DISABLED);
  }
  else if (shrd.inOtaMode) // OTA BLE/Wifi
  {

    if (old_substate_ota == 0)
    {
      // Clear screen
      tft.fillScreen(TFT_BLACK);

      old_substate_ota++;
    }
    else if (old_substate_ota == 1)
    {
      // Display GIF image
      int loops = maxLoopIterations;          // max loops
      int durationControl = maxLoopsDuration; // force break loop after xxx ms
      AnimatedGIF gif;
      while (loops-- > 0 && durationControl > 0)
      {
        durationControl -= gifPlay(&gif, (char *)GIF_PATH_OTA, -20);
        gif.reset();
      }

      old_substate_ota++;
    }
    else if (old_substate_ota == 2)
    {

      // Display flashing informations
#if ((TFT_MODEL == 2) || (TFT_MODEL == 3)) // 3.5"
      tft.setFreeFont(FONT_FORCED_SQUARE12pt7b);
#else
      tft.setFreeFont(FONT_FORCED_SQUARE9pt7b);
#endif
      tft.setTextColor(MY_TFT_WHITE, TFT_BLACK);
      tft.setTextDatum(BC_DATUM);
      String str_ota_mode;
      if (shrd.inOtaMode == OTA_IDE)
      {
        str_ota_mode = "Mode : IDE server";
      }
      else
      {
        str_ota_mode = "Mode : Wifi server / Version : " + (String)shrd.inOtaModeVersion;
      }
      tft.drawString("Downloading and flashing", TFT_HEIGHT / 2, 180, GFXFF);
      tft.drawString(str_ota_mode, TFT_HEIGHT / 2, 200, GFXFF);
      tft.drawString("Wifi : " + settings.get_Wifi_ssid(), TFT_HEIGHT / 2, 220, GFXFF);

      old_substate_ota++;
    }
  }
  else
  {
    switch (i_loop % 100)
    {
    case 0:
    {
      int speed = shrd.speedCurrent;
      if (speed > 199)
        speed = 199;
      sprintf(fmt, "%3d", (int)speed);
      tft_util_draw_number(&tft, fmt, COLUMN5, LINE_3Y, MY_TFT_WHITE, TFT_BLACK, 5, BIG_FONT_SIZE);
      break;
    }

    case 1:
    {
      sprintf(fmt, "%02.0f", shrd.speedMax);
      tft_util_draw_number(&tft, fmt, COLUMN8, LINE_3Y, MY_TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 2:
    {
      sprintf(fmt, "%01.0f", (float)shrd.modeOrder);
      tft_util_draw_number(&tft, fmt, COLUMN1, LINE_1Y, MY_TFT_WHITE, TFT_BLACK, 5, MEDIUM_FONT_SIZE);
      break;
    }

    case 3:
    {
      float data;
      String txt_label;
      String txt_unit;
      bool shouldClear = false;

      if ((shrd.currentSensorPresent == 1) && (!(shrd.speedCurrent == 0 && shrd.brakePressedStatus == 1)))
      {
        // CASE POWER (With current sensor)
        txt_label = "  " + (String)txt_power;
        txt_unit = "W ";
        data = ((shrd.currentActual / 1000.0) * (shrd.voltageActual / 1000.0));
        data = constrain(data, -50, 65535);
        sprintf(fmt, "%05.0f", data);

        // check substate change
        if (old_substate_case3 == 1)
        {
          shouldClear = true;
          old_substate_case3 = 0;
        }
      }
      else
      {
        // CASE TEMP (Without current sensor, by default)
        txt_label = "       " + (String)txt_temp;
        txt_unit = "C ";
        data = shrd.currentTemperature;
        sprintf(fmt, "%02.0f", data);

        // check substate change
        if (old_substate_case3 == 0)
        {
          shouldClear = true;
          old_substate_case3 = 1;
        }
      }

      if (shouldClear)
      {
        // Clear number space
        tft.fillRect(0, LINE_4Y, COLUMN2, (SMALLEST_FONT_SIZE * 5), TFT_BLACK);

        // Draw label
        tft.setTextColor(TFT_RED, TFT_BLACK);                                  //LABEL
        tft.setTextDatum(BR_DATUM);                                            //LABEL
        tft.setFreeFont(FONT_RED_LABEL);                                       //LABEL SIZE/FONT
        tft.drawString(txt_label, COLUMN2, LINE_4Y - LINE_TEXT_OFFSET, GFXFF); //DRAW LABEL ON DISPLAY

        // Draw unit
        tft.setTextColor(MY_TFT_WHITE, TFT_BLACK);                                 //UNIT
        tft.setTextDatum(BL_DATUM);                                                //UNIT
        tft.setFreeFont(FONT_UNIT);                                                //UNIT SIZE/FONT
        tft.drawString(txt_unit, COLUMN2 + UNIT_LEFT_MARGIN, LINE_4Y_UNIT, GFXFF); //DRAW UNIT ON DISPLAY

        // Switch font back
        tft.setFreeFont(FONT_NUMBER); //SET FONT FOR DATA
      }

      tft_util_draw_number(&tft, fmt, COLUMN2, LINE_4Y, MY_TFT_WHITE, TFT_BLACK, 5, SMALLEST_FONT_SIZE); //DRAW DATA
      break;
    }

    case 4:
    {
      timeToString().toCharArray(fmt, 9);
      tft_util_draw_number(&tft, fmt, COLUMN6, LINE_4Y, MY_TFT_WHITE, TFT_BLACK, 5, SMALLEST_FONT_SIZE);
      break;
    }

    case 5:
    {
      float voltage = shrd.voltageFilterMean / 1000.0;
      sprintf(fmt, "%s", Dfmt2_1(voltage));
      tft_util_draw_number(&tft, fmt, COLUMN4, LINE_2Y, MY_TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }

    case 6:
    {
      float data;
      String txt_label;
      bool shouldClear = false;

      if (shrd.speedCurrent == 0 && shrd.brakePressedStatus == 1)
      { // CASE ODO
        txt_label = "   ODO";
        data = shrd.distanceOdo / 10;
#if (TFT_MODEL == 1) // 2.4"
        data = constrain(data, 0, 9999);
#else
        data = constrain(data, 0, 99999);
#endif
        sprintf(fmt, "%04.0f", data);

        // check substate change
        if (old_substate_case6 == 1)
        {
          shouldClear = true;
          old_substate_case6 = 0;
        }
      }
      else
      { // CASE TRIP
        txt_label = "  TRIP";
        data = shrd.distanceTrip / 10000.0;
        sprintf(fmt, "%s", Dfmt2_1(data));

        // check substate change
        if (old_substate_case6 == 0)
        {
          shouldClear = true;
          old_substate_case6 = 1;
        }
      }

      if (shouldClear)
      {
        // Clear number space
        tft.fillRect(COLUMN6, LINE_4Y, COLUMN9 - COLUMN6, (SMALLEST_FONT_SIZE * 5), TFT_BLACK);

        // Draw label
        tft.setTextColor(TFT_RED, TFT_BLACK);                                  //LABEL
        tft.setTextDatum(BR_DATUM);                                            //LABEL
        tft.setFreeFont(FONT_RED_LABEL);                                       //LABEL SIZE/FONT
        tft.drawString(txt_label, COLUMN9, LINE_4Y - LINE_TEXT_OFFSET, GFXFF); //DRAW LABEL ON DISPLAY

        // Switch font back
        tft.setFreeFont(FONT_NUMBER); //SET FONT FOR DATA
      }

      tft_util_draw_number(&tft, fmt, COLUMN9, LINE_4Y, MY_TFT_WHITE, TFT_BLACK, 5, SMALLEST_FONT_SIZE); //DRAW DATA
      break;
    }
    case 7:
    {
      float data;
      String txt_unit;
      bool shouldClear = false;

      if (shrd.speedCurrent == 0 && shrd.brakePressedStatus == 1)
      { // CASE PERCENTAGE
        data = shrd.batteryLevel;
        sprintf(fmt, "%03.0f", data);
        txt_unit = "%   ";

        // check substate change
        if (old_substate_case7 == 1)
        {
          shouldClear = true;
          old_substate_case7 = 0;
        }
      }

      else
      { // CASE AUTONOMY
        txt_unit = "Km";
        float autonomy = shrd.autonomyLeft;
        autonomy = constrain(autonomy, 0, 999);
        sprintf(fmt, "%03.0f", autonomy);
        // check substate change
        if (old_substate_case7 == 0)
        {
          shouldClear = true;
          old_substate_case7 = 1;
        }
      }

      if (shouldClear)
      {
        // Clear number space
        //tft.fillRect(COLUMN4, LINE_2Y, COLUMN4 - COLUMN6, (SMALLEST_FONT_SIZE * 5), TFT_BLACK); //broken

        // Draw unit
        tft.setTextColor(MY_TFT_WHITE, TFT_BLACK);                                 //UNIT
        tft.setTextDatum(BL_DATUM);                                                //UNIT
        tft.setFreeFont(FONT_UNIT);                                                //UNIT SIZE/FONT
        tft.drawString(txt_unit, COLUMN3 + UNIT_LEFT_MARGIN, LINE_2Y_UNIT, GFXFF); //DRAW UNIT ON DISPLAY

        // Switch font back
        tft.setFreeFont(FONT_NUMBER); //SET FONT FOR DATA
      }

      tft_util_draw_number(&tft, fmt, COLUMN3, LINE_2Y, MY_TFT_WHITE, TFT_BLACK, 5, SMALL_FONT_SIZE);
      break;
    }
    case 8:
    {
      float bat_min = settings.get_Battery_minimum_voltage();
      float bat_max = settings.get_Battery_maximum_voltage();
      float batteryPercent = (1 / ((bat_max - bat_min) / ((shrd.voltageFilterMean / 1000.0) - bat_min)) * 100);
      drawBatteryJauge(&tft, batteryPercent, COLUMN7, LINE_2Y, 4 * SCALE_FACTOR_X, 24 * SCALE_FACTOR_Y, NB_BATTERY_BARS);
      break;
    }

    case 9:
    {
      // draw interface - indicators
#if ((TFT_MODEL == 2) || (TFT_MODEL == 3)) // 3.5"
      tft.setFreeFont(FONT_FORCED_SQUARE12pt7b);
#else
      tft.setFreeFont(FONT_FORCED_SQUARE9pt7b);
#endif

      tft.setTextDatum(TC_DATUM);

      int i = -10 * SCALE_FACTOR_Y;
      if (oldShrdPasEnabled != shrd.pasEnabled)
      {
        tft.setTextColor(shrd.pasEnabled ? MY_TFT_WHITE : MY_TFT_DARK_DIGIT_DISABLED, TFT_BLACK);
        tft.drawString(txt_pas, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdPasEnabled = shrd.pasEnabled;
      }

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      if (oldShrdBrakePressedStatus != shrd.brakePressedStatus)
      {
        tft.setTextColor(shrd.brakePressedStatus ? MY_TFT_WHITE : MY_TFT_DARK_DIGIT_DISABLED, TFT_BLACK);
        tft.drawString(txt_brk, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdBrakePressedStatus = shrd.brakePressedStatus;
      }

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      if (oldAuxOrder != shrd.auxOrder)
      {
        tft.setTextColor(shrd.auxOrder ? MY_TFT_WHITE : MY_TFT_DARK_DIGIT_DISABLED, TFT_BLACK);
        tft.drawString(txt_aux, COLUMN0, LINE_3Y + i, GFXFF);
        oldAuxOrder = shrd.auxOrder;
      }

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      uint8_t currentTemperatureStatus = (shrd.currentTemperature > settings.get_Temperature_warning());
      if (oldShrdCurrentTemperature != currentTemperatureStatus)
      {
        tft.setTextColor(currentTemperatureStatus ? TFT_RED : MY_TFT_DARK_DIGIT_DISABLED, TFT_BLACK);
        tft.drawString(txt_temp, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdCurrentTemperature = currentTemperatureStatus;
      }

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      uint8_t currentHumidityStatus = (shrd.currentHumidity > settings.get_Humidity_warning());
      if (oldShrdCurrentHumidity != currentHumidityStatus)
      {
        tft.setTextColor(currentHumidityStatus ? TFT_RED : MY_TFT_DARK_DIGIT_DISABLED, TFT_BLACK);
        tft.drawString(txt_hr, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdCurrentHumidity = currentHumidityStatus;
      }

      boolean error = shrd.errorThrottle ||
                      shrd.errorBrake ||
                      shrd.errorSerialFromDisplay ||
                      shrd.errorSerialFromContrl ||
                      shrd.errorContrl;

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      if (oldError != error)
      {
        tft.setTextColor(error ? TFT_RED : MY_TFT_DARK_DIGIT_DISABLED, TFT_BLACK);
        tft.drawString(txt_err, COLUMN0, LINE_3Y + i, GFXFF);
        oldError = error;
      }

      i = i + (SPACE_INDICATORS_Y * SCALE_FACTOR_Y);
      if (oldShrdIsLocked != shrd.isLocked)
      {
        tft.setTextColor(shrd.isLocked ? TFT_RED : MY_TFT_DARK_DIGIT_DISABLED, TFT_BLACK);
        tft.drawString(txt_lck, COLUMN0, LINE_3Y + i, GFXFF);
        oldShrdIsLocked = shrd.isLocked;
      }

      break;
    }
    case 10:
    {
      tftBacklightFull();
      break;
    }
    }
  }
}