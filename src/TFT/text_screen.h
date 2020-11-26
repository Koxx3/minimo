#ifndef TEXT_SCREEN_H
#define TEXT_SCREEN_H

#define MAX_LINE_LENGTH 30

#include "Adafruit_ILI9341.h"

void write_numeric_line(Adafruit_ILI9341 *tft, float value, const char *units, const char *label, int x, int y, uint16_t color = ILI9341_WHITE);
void write_time_line(Adafruit_ILI9341 *tft, uint32_t seconds, const char *label, int x, int y, uint16_t color = ILI9341_WHITE);
void write_text_line(Adafruit_ILI9341 *tft, const char *value, int x, int y, uint16_t color = ILI9341_WHITE);
void write_line_buffer(Adafruit_ILI9341 *tft, int x, int y, uint16_t color = ILI9341_WHITE);


#endif //TEXT_SCREEN_H
