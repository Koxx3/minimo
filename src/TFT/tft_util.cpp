#include "tft_util.h"

// TODO: PROGMEM
const bool FONT_DIGITS_3x5[10][5][3] = {
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {0, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 0, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 0},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
        {0, 0, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
    },
    {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1},
        {0, 0, 1},
        {1, 1, 1},
    }};

void tft_util_draw_digit(
    Adafruit_ILI9341 *tft, uint8_t digit, uint16_t x, uint16_t y,
    uint16_t fg_color, uint16_t bg_color, uint8_t magnify)
{
    for (int xx = 0; xx < 3; xx++)
    {
        for (int yy = 0; yy < 5; yy++)
        {
            uint16_t color = FONT_DIGITS_3x5[digit][yy][xx] ? fg_color : bg_color;
            int x1 = x + xx * magnify;
            int y1 = y + yy * magnify;
            tft->fillRect(x1, y1, magnify, magnify, color);
        }
    }
}

void tft_util_draw_number(
    Adafruit_ILI9341 *tft, char *number, uint16_t x, uint16_t y,
    uint16_t fg_color, uint16_t bg_color, uint8_t spacing, uint8_t magnify)
{
    uint16_t cursor_x = x;
    int number_len = strlen(number);
    bool activeDigit = false;
    for (int i = 0; i < number_len; i++)
    {
        char ch = number[i];
        if (ch >= '0' and ch <= '9')
        {
            if ((ch > '0') || (i == number_len -1))
                activeDigit = true;

            tft_util_draw_digit(tft, ch - '0', cursor_x, y, activeDigit ? fg_color : ILI_DIGIT_DARK, bg_color, magnify);
            cursor_x += 3 * magnify + spacing;
        }
        else if (ch == '.')
        {
            tft->fillRect(cursor_x, y, magnify - 1, 5 * magnify - 1, bg_color);
            tft->fillRect(cursor_x, y + 4 * magnify, magnify - 1, magnify - 1, ILI9341_RED);
            cursor_x += magnify + spacing;
        }
        else if (ch == ':')
        {
            tft->fillRect(cursor_x, y, magnify - 1, 5 * magnify - 1, bg_color);
            tft->fillRect(cursor_x, y + 4 * magnify, magnify - 1, magnify - 1,  activeDigit ? ILI9341_RED : ILI_DIGIT_DARK);
            tft->fillRect(cursor_x, y + 2 * magnify, magnify - 1, magnify - 1,  activeDigit ? ILI9341_RED : ILI_DIGIT_DARK);
            cursor_x += magnify + spacing;
        }
        else if (ch == '-')
        {
            tft->fillRect(cursor_x, y, 3 * magnify - 1, 5 * magnify - 1, bg_color);
            tft->fillRect(cursor_x, y + 2 * magnify, 3 * magnify - 1, 3 * magnify - 1, fg_color);
            cursor_x += 3 * magnify + spacing;
        }
        else if (ch == ' ')
        {
            tft->fillRect(cursor_x, y, 3 * magnify, 5 * magnify, bg_color);
            cursor_x += 3 * magnify + spacing;
        }
    }
}

/*
uint16_t progress_to_color(float progress, TFT_eSPI* tft) {
    float brightness = 255.0 * (1.0 - progress);
    return  tft->setColor(brightness, brightness, brightness);
}
*/
