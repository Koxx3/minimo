#ifndef TEXT_SCREEN_H
#define TEXT_SCREEN_H

#define MAX_LINE_LENGTH 30

//#include "Adafruit_ILI9341.h"
#include <TFT_eSPI.h>

void write_numeric_line(TFT_eSPI *tft, float value, const char *units, const char *label, int x, int y, uint16_t color = TFT_WHITE);
void write_time_line(TFT_eSPI *tft, uint32_t seconds, const char *label, int x, int y, uint16_t color = TFT_WHITE);
void write_text_line(TFT_eSPI *tft, const char *value, int x, int y, uint16_t color = TFT_WHITE);
void write_line_buffer(TFT_eSPI *tft, int x, int y, uint16_t color = TFT_WHITE);


#endif //TEXT_SCREEN_H
