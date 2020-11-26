#include "tft_util.h"
#include "Adafruit_ILI9341.h"
#include "text_screen.h"

#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMono18pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMono12pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

char _line_buffer[MAX_LINE_LENGTH + 1];

/*
void reset(Adafruit_ILI9341* tft) {
    tft->fillRectangle(0, 0, tft->maxX() - 1, tft->maxY() - 1, COLOR_BLACK);
}

void heartbeat(Adafruit_ILI9341* tft, uint32_t duration_ms, bool successful_vesc_read) {
    uint16_t color = successful_vesc_read ? tft->setColor(0, 150, 0) : tft->setColor(150, 0, 0);
    tft->fillRectangle(167, 5, 171, 9, color);
    delay(duration_ms);
    tft->fillRectangle(167, 5, 171, 9, COLOR_BLACK);
}

*/

void write_text_line(Adafruit_ILI9341 *tft, const char *value, int x, int y, uint16_t color)
{
    for (int i = 0; i < strlen(value); i++)
        _line_buffer[i] = value[i];
    _line_buffer[strlen(value)] = '\0';

    write_line_buffer(tft, x, y, color);
}

void write_line_buffer(Adafruit_ILI9341 *tft, int x, int y, uint16_t color)
{
    // space padding
    for (int i = strlen(_line_buffer); i < MAX_LINE_LENGTH; i++)
        _line_buffer[i] = ' ';
    _line_buffer[MAX_LINE_LENGTH] = '\0';

    //    tft->drawText(5, y, _line_buffer, color);
    tft->setCursor(x, y);
    tft->setTextColor(color);
    tft->print(_line_buffer);
}
