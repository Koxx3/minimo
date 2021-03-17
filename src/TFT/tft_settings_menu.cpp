/***************************************************************************************************************************************
LILYGO TTGO T-DISPLAY DEMO
Created at August 29th, 2019 by Jeroen Maathuis (j [dot] maathuis [at] gmail [dot] com) to support the LilyGo TTGO T-display

Based on:
- ArduinoMenu TFT_eSPI example on https://github.com/neu-rah/ArduinoMenu/blob/master/examples/adafruitGfx/eTFT/TFT_eSPI/TFT_eSPI.ino.
- TTGO T-display example on https://github.com/Xinyuan-LilyGO/TTGO-T-Display/blob/master/TTGO-T-Display.ino.
- Several Button2 examples on https://github.com/LennartHennigs/Button2/tree/master/examples

External needed library's:
- TFT_eSPI
- Buttons2

Preparations:
- Create TFT_eSPI User Setup file as described on https://github.com/Xinyuan-LilyGO/TTGO-T-Display/blob/master/README.MD and
  https://sites.google.com/site/jmaathuis/arduino/lilygo-ttgo-t-display-esp32/arduinomenu-demo-ttgo-t-display
- Attach a led (and a resistor if needed). This is optional.

This demo can also work on other setups with displays supported by the TFT_eSPI library. This demo is explicitly for the
TTGO T-display but I had a slightly modifies version working on a Wemos D1 mini with attached ST7735 display and some image
and text modifications as well another User Setup file for TFT_eSPI.

For a more detailed description about this demo please visit:
https://sites.google.com/site/jmaathuis/arduino/lilygo-ttgo-t-display-esp32/arduinomenu-demo-ttgo-t-display

This sketch may freely redistributed as long as you mention the sources / authors mentioned above. I would be very happy to here from
you when you have improved the sketch. You can mail me at j [dot] maathuis [at] gmail [dot] com.

****************************************************************************************************************************************/

#include <Arduino.h>

#include <TFT_eSPI.h>

#include "tft_settings_menu.h"

#include "TFT/fonts/FORCED_SQUARE6pt7b.h"
#include "TFT/fonts/FORCED_SQUARE7pt7b.h"
#include "TFT/fonts/FORCED_SQUARE8pt7b.h"
#include "TFT/fonts/FORCED_SQUARE9pt7b.h"
#include "TFT/fonts/FORCED_SQUARE10pt7b.h"
#include "TFT/fonts/FORCED_SQUARE12pt7b.h"
#include "TFT/fonts/FORCED_SQUARE14pt7b.h"
#include "TFT/fonts/FORCED_SQUARE18pt7b.h"

#include "main.h"
#include "Settings.h"
#include "SharedData.h"

// Easily remembered name for the font
#define FONT_FORCED_SQUARE6pt7b &FORCED_SQUARE6pt7b
#define FONT_FORCED_SQUARE7pt7b &FORCED_SQUARE7pt7b
#define FONT_FORCED_SQUARE8pt7b &FORCED_SQUARE8pt7b
#define FONT_FORCED_SQUARE9pt7b &FORCED_SQUARE9pt7b
#define FONT_FORCED_SQUARE10pt7b &FORCED_SQUARE10pt7b
#define FONT_FORCED_SQUARE12pt7b &FORCED_SQUARE12pt7b
#define FONT_FORCED_SQUARE14pt7b &FORCED_SQUARE14pt7b
#define FONT_FORCED_SQUARE18pt7b &FORCED_SQUARE18pt7b

#define MAX_DEPTH 5

using namespace Menu;
extern TFT_eSPI tft;
serialIn serial(Serial);
Settings *TFT_menu_settings;
SharedData *TFT_menu_shrd;

result doAlert(eventMask e, prompt &item);

#include "tft_settings_menu_specs.h"

//define serial output device
idx_t serialTops[MAX_DEPTH] = {0};

#define GFX_WIDTH TFT_HEIGHT
#define GFX_HEIGHT TFT_WIDTH
#define fontW 10
#define fontH 26

const panel panels[] MEMMODE = {{0, 0, GFX_WIDTH / fontW, GFX_HEIGHT / fontH}};
navNode *nodes[sizeof(panels) / sizeof(panel)]; //navNodes to store navigation status
panelsList pList(panels, nodes, 1);             //a list of panels and nodes
idx_t eSpiTops[MAX_DEPTH] = {0};
TFT_eSPIOut eSpiOut(tft, colors, eSpiTops, pList, fontW, fontH);
menuOut *constMEM outputs[] MEMMODE = {&eSpiOut};              //list of output devices
outputsList out(outputs, sizeof(outputs) / sizeof(menuOut *)); //outputs list controller

NAVROOT(nav, mainMenu, MAX_DEPTH, serial, out);

int nTasks = 2;
SemaphoreHandle_t barrierSemaphore = xSemaphoreCreateMutex();;

void settings_menu_btn_click(uint8_t pressType, uint8_t btnNum)
{

  xSemaphoreTake(barrierSemaphore, portMAX_DELAY);


  if ((btnNum == 1) && (pressType == 0))
  {
    nav.doNav(upCmd);
  }
  if ((btnNum == 2) && (pressType == 0))
  {
    nav.doNav(downCmd);
  }
  if ((btnNum == 1) && (pressType == 1))
  {
    nav.doNav(enterCmd);
  }
  if ((btnNum == 2) && (pressType == 1))
  {
    if (nav.level > 0)
      nav.doNav(escCmd);
  }

  xSemaphoreGive(barrierSemaphore);

  // Serial.println("settings_menu_btn_click : btnNum " + (String)btnNum + " / pressType = " + (String)pressType);
}

void settings_menu_setup()
{

  //barrierSemaphore = xSemaphoreCreateMutex();

  //options=&myOptions;//can customize options

  //nav.idleTask = idle; //point a function to be used when menu is suspended
  nav.canExit = true;
  //nav.idleOff();
  nav.inputBurst = 1;

  //mainMenu[1].disable();
  //outGfx.usePreview=true;//reserve one panel for preview?
  //nav.showTitle=false;//show menu title?

  tft.setTextSize(1);
  tft.setTextDatum(BL_DATUM);
  tft.setFreeFont(FONT_FORCED_SQUARE10pt7b);

  tft.fillScreen(Black);

  mainMenu.dirty = true; // Force the main menu to redraw itself

  eSpiOut.fontMarginY = 17;

  Serial.println("settings_menu_set_settings");
  settings_menu_init_from_settings();
  
  xSemaphoreGive(barrierSemaphore);
}

void settings_menu_loop()
{
  xSemaphoreTake(barrierSemaphore, portMAX_DELAY);

  nav.poll();
  
  xSemaphoreGive(barrierSemaphore);

}

void settings_menu_set_settings(Settings *set)
{
  TFT_menu_settings = set;
}
void settings_menu_set_shared_datas(SharedData *set)
{
  TFT_menu_shrd = set;
}
