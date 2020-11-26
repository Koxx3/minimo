#ifndef TFT_UTIL_H
#define TFT_UTIL_H

#include "Adafruit_ILI9341.h"

#define ILI_DIGIT_DARK 0x10A2

void tft_util_draw_digit(
        Adafruit_ILI9341* tft, uint8_t digit, uint16_t x, uint16_t y,
        uint16_t fg_color, uint16_t bg_color, uint8_t magnify = 1);

void tft_util_draw_number(
        Adafruit_ILI9341* tft, char *number, uint16_t x, uint16_t y,
        uint16_t fg_color, uint16_t bg_color, uint8_t spacing, uint8_t magnify = 1);

uint16_t progress_to_color(float progress, Adafruit_ILI9341* tft);

#endif //TFT_UTIL_H
