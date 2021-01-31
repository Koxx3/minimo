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
    TFT_eSPI *tft, uint8_t digit, uint16_t x, uint16_t y,
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
    TFT_eSPI *tft, char *number, uint16_t x, uint16_t y,
    uint16_t fg_color, uint16_t bg_color, uint8_t spacing, uint8_t magnify)
{
    uint16_t cursor_x = x;
    int number_len = strlen(number);
    bool activeDigit = true;
    //Serial.println("number_len = " + (String)number_len + " / number = " + number);
    int width = 0;

    for (int i = number_len; i >= 0; i--)
    {
        char ch = number[i];
        if (ch >= '0' and ch <= '9')
        {
            bool nextCharsHasOnlyZeros = true;
            for (int j = i; j >= 0; j--)
            {
                //Serial.println("j = " + (String)j + " / number[j] = " + number[j]);
                if (number[j] > '0' and number[j] <= '9')
                {
                    nextCharsHasOnlyZeros = false;
                }
            }

            // check this is the first left caracter and the next characters are only zero
            if ((nextCharsHasOnlyZeros) && (i != number_len - 1))
                activeDigit = false;
            else
                activeDigit = true;

            //Serial.println("i = " + (String)i + " / ch = " + (String)ch + " / nextCharsHasOnlyZeros = " + nextCharsHasOnlyZeros + " / activeDigit = " + activeDigit);

            width = 3 * magnify;
            tft_util_draw_digit(tft, ch - '0', cursor_x - width, y, activeDigit ? fg_color : ILI_DIGIT_DARK, bg_color, magnify);
            cursor_x -= width + spacing;
        }
        else if (ch == '.')
        {
            width = magnify;
            tft->fillRect(cursor_x - width, y, magnify - 1, 5 * magnify - 1, bg_color);
            tft->fillRect(cursor_x - width, y + 4 * magnify, magnify - 1, magnify - 1, TFT_RED);
            cursor_x -= width + spacing;
        }
        else if (ch == ':')
        {
            width = magnify;
            tft->fillRect(cursor_x - width, y, magnify - 1, 5 * magnify - 1, bg_color);
            tft->fillRect(cursor_x - width, y + 4 * magnify, magnify - 1, magnify - 1, activeDigit ? TFT_RED : ILI_DIGIT_DARK);
            tft->fillRect(cursor_x - width, y + 2 * magnify, magnify - 1, magnify - 1, activeDigit ? TFT_RED : ILI_DIGIT_DARK);
            cursor_x -= magnify + spacing;
        }
        else if (ch == '-')
        {
            width = 3 * magnify;
            //            tft->fillRect(cursor_x - width, y, 3 * magnify - 1, 5 * magnify - 1, bg_color);
            tft->fillRect(cursor_x - width, y + 2 * magnify, 3 * magnify, magnify - 1, fg_color);
            cursor_x -= width + spacing;
        }
        else if (ch == ' ')
        {
            width = 3 * magnify;
            tft->fillRect(cursor_x - width, y, 3 * magnify, 5 * magnify, bg_color);
            cursor_x -= width + spacing;
        }
    }
}

/*
uint16_t progress_to_color(float progress, TFT_eSPI* tft) {
    float brightness = 255.0 * (1.0 - progress);
    return  tft->setColor(brightness, brightness, brightness);
}
*/
