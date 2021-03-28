
#ifndef _TFT_MAIN_h
#define _TFT_MAIN_h

#include <TFT_eSPI.h>

#include "SharedData.h"
#include "Settings.h"

#include "TFT/text_screen.h"
#include "TFT/tft_color_jauge.h"
#include "TFT/tft_main.h"
#include "TFT/tft_util.h"

void tftSetupBacklight();
void tftSetup(SharedData *shrd, Settings *settings);
void tftUpdateData(uint32_t);

#endif