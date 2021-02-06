#ifndef TFT_UTIL_H
#define TFT_UTIL_H

#include <TFT_eSPI.h>

#define ILI_DIGIT_DARK_DISABLED TFT_WHITE // 0x3186  // 90 - too much : 0x5ACB // 70 : 0x4228 // 50 : 0x3186
#define ILI_DIGIT_DARK_DIGIT TFT_BLACK

#if (TFT_MODEL == 1) // 2.4"
#define SCALE_FACTOR_X 1
#define SCALE_FACTOR_Y 1
#else // 3.5"
#define SCALE_FACTOR_X 1.5
#define SCALE_FACTOR_Y 1.33
#endif

void tft_util_draw_digit(
    TFT_eSPI *tft, uint8_t digit, uint16_t x, uint16_t y,
    uint16_t fg_color, uint16_t bg_color, uint8_t magnify = 1);

void tft_util_draw_number(
    TFT_eSPI *tft, char *number, uint16_t x, uint16_t y,
    uint16_t fg_color, uint16_t bg_color, uint8_t spacing, uint8_t magnify = 1);

uint16_t progress_to_color(float progress, TFT_eSPI *tft);

#endif //TFT_UTIL_H
