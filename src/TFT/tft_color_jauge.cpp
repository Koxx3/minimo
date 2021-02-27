#include "TFT_eSPI.h"
#include "tft_util.h"

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

void drawColorBar(TFT_eSPI *tft, int value, int spotx, int spoty, int pix, uint16_t height, uint8_t nbBars)
{
    uint16_t color;
    for (int i = 0; i < nbBars; i++)
    {
        if (i <= value)
        {
            color = tft->color565(rgbtableInv[i][0], rgbtableInv[i][1], rgbtableInv[i][2]);
        }
        else
        {
            color = ILI_DIGIT_DARK_DISABLED;
        }
        tft->fillRect(spotx + (i * pix), spoty, pix - 2 , height, color);
    }
}

void drawBatteryJauge(TFT_eSPI *tft, uint8_t batteryPercent, uint16_t xStart, uint16_t yStart, uint16_t witdh, uint16_t height, uint8_t nbBars)
{
    int8_t bars = batteryPercent * nbBars / 100.0;    
    drawColorBar(tft, bars , xStart, yStart, witdh, height, nbBars);
}