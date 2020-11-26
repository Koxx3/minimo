#include "Adafruit_ILI9341.h"

byte rgbtable[34][3]{
    {0, 255, 192},
    {0, 255, 160},
    {0, 255, 128},
    {0, 255, 96},
    {0, 255, 64},
    {0, 255, 32},
    {0, 255, 0},
    {32, 255, 0},
    {64, 255, 0},
    {96, 255, 0},
    {128, 255, 0},
    {160, 255, 0},
    {192, 255, 0},
    {224, 255, 0},
    {255, 255, 0},
    {255, 224, 0},
    {255, 192, 0},
    {255, 160, 0},
    {255, 128, 0},
    {255, 96, 0},
    {255, 64, 0},
    {255, 32, 0},
    {255, 0, 0}
    };

byte rgbtableInv[34][3]{
    {255, 0, 0},
    {255, 32, 0},
    {255, 64, 0},
    {255, 96, 0},
    {255, 128, 0},
    {255, 160, 0},
    {255, 192, 0},
    {255, 224, 0},
    {255, 255, 0},
    {224, 255, 0},
    {192, 255, 0},
    {160, 255, 0},
    {128, 255, 0},
    {96, 255, 0},
    {64, 255, 0},
    {32, 255, 0},
    {0, 255, 0},
    {0, 255, 32},
    {0, 255, 64},
    {0, 255, 96},
    {0, 255, 128},
    {0, 255, 160},
    {0, 255, 192}
    };

void drawColorBar(Adafruit_ILI9341 *tft, int value, int spotx, int spoty, int pix)
{
    uint16_t color;
    for (int i = 0; i < 23; i++)
    {
        if (i <= value)
        {
            color = tft->color565(rgbtableInv[i][0], rgbtableInv[i][1], rgbtableInv[i][2]);
        }
        else
        {
            color = ILI9341_DARKGREY;
        }
        tft->fillRect(spotx + (i * pix), spoty, pix - 2 , 18, color);
    }
}

void drawBatteryJauge(Adafruit_ILI9341 *tft, uint8_t batteryPercent)
{
    int8_t bars = batteryPercent * 24.0 / 100;    
    drawColorBar(tft, bars , 222, 5, 4);
}