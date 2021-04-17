/*
 * MIT License
 *
 * Copyright (c) 2017-2019 David Antliff
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file ds9990.c
 *
 * Resolution is cached in the DS9990_Info object to avoid querying the hardware
 * every time a adc conversion is required. However this can result in the
 * cached value becoming inconsistent with the hardware value, so care must be taken.
 *
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_system.h"
#include "esp_log.h"

#include "ds9990.h"
#include "owb.h"

static const char *TAG = "ds9990";
static const int T_CONV = 750; // maximum conversion time at 12-bit resolution in milliseconds

#define DS9990_FIMILTY_CODE 0x90

// Function commands
#define DS9990_FUNCTION_READ_MEMORY 0xF0
#define DS9990_FUNCTION_WRITE_MEMORY 0x0F
#define DS9990_FUNCTION_WRITE_READ_MEMORY 0xFF

/// @cond ignore
typedef struct
{
    uint8_t data[8];
} __attribute__((packed)) Memory;
/// @endcond ignore

static void _init(DS9990_Info *ds9990_info, const OneWireBus *bus)
{
    if (ds9990_info != NULL)
    {
        ds9990_info->bus = bus;
        memset(&ds9990_info->rom_code, 0, sizeof(ds9990_info->rom_code));
        ds9990_info->use_crc = false;
        ds9990_info->solo = false; // assume multiple devices unless told otherwise
        ds9990_info->init = true;
    }
    else
    {
        ESP_LOGE(TAG, "ds9990_info is NULL");
    }
}

static bool _is_init(const DS9990_Info *ds9990_info)
{
    bool ok = false;
    if (ds9990_info != NULL)
    {
        if (ds9990_info->init)
        {
            // OK
            ok = true;
        }
        else
        {
            ESP_LOGE(TAG, "ds9990_info is not initialised");
        }
    }
    else
    {
        ESP_LOGE(TAG, "ds9990_info is NULL");
    }
    return ok;
}

static bool _address_device(const DS9990_Info *ds9990_info)
{
    bool present = false;
    if (_is_init(ds9990_info))
    {
        owb_reset(ds9990_info->bus, &present);
        if (present)
        {
            if (ds9990_info->solo)
            {
                // if there's only one device on the bus, we can skip
                // sending the ROM code and instruct it directly
                owb_write_byte(ds9990_info->bus, OWB_ROM_SKIP);
            }
            else
            {
                // if there are multiple devices on the bus, a Match ROM command
                // must be issued to address a specific slave
                owb_write_byte(ds9990_info->bus, OWB_ROM_MATCH);
                owb_write_rom_code(ds9990_info->bus, ds9990_info->rom_code);
            }
        }
        else
        {
            ESP_LOGE(TAG, "ds9990 device not responding");
        }
    }
    return present;
}

static DS9990_ERROR _read_memory(const DS9990_Info *ds9990_info, Memory *memory, size_t count)
{
    // If CRC is enabled, regardless of count, read the entire memory and verify the CRC,
    // otherwise read up to the memory size, or count, whichever is smaller.

    if (!memory)
    {
        return DS9990_ERROR_NULL;
    }

    DS9990_ERROR err = DS9990_ERROR_UNKNOWN;

    // send size
    if (owb_write_byte(ds9990_info->bus, count) == OWB_STATUS_OK)
    {
        // read all data
        if (owb_read_bytes(ds9990_info->bus, (uint8_t *)memory, count) == OWB_STATUS_OK)
        {

            //ESP_LOG_BUFFER_HEX_LEVEL(TAG, memory, count, ESP_LOG_DEBUG);

            err = DS9990_OK;
            uint8_t crc_rcv;
            if (owb_read_byte(ds9990_info->bus, (uint8_t *)&crc_rcv) == OWB_STATUS_OK)
            {
                // With CRC:
                uint8_t crc = owb_crc8_bytes(0, (uint8_t *)memory, count);
                if (crc != crc_rcv)
                {
                    ESP_LOGE(TAG, "CRC failed / computed = %02x / received = %02x !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!", crc, crc_rcv);
                    err = DS9990_ERROR_CRC;

                    /*
                                bool is_present = false;
                                owb_reset(ds9990_info->bus, &is_present); // terminate early
                                */
                }
                else
                {
                    //ESP_LOGE(TAG, "CRC ok / computed = %02x / received = %02x", crc, crc_rcv);nate early

                    //ESP_LOGE(TAG, "owb_read_bytes OK");
                    err = DS9990_OK;
                }
            }
            else
            {
                ESP_LOGE(TAG, "CRC read fail");

                //bool is_present = false;
                //owb_reset(ds9990_info->bus, &is_present); // terminate early
            }
        }
        else
        {
            ESP_LOGE(TAG, "owb_read_bytes failed");
            err = DS9990_ERROR_OWB;
        }
    }
    else
    {
        ESP_LOGE(TAG, "owb_write_byte failed");
        err = DS9990_ERROR_OWB;
    }
    return err;
}

static bool _write_memory(const DS9990_Info *ds9990_info, const uint8_t *values, uint8_t length)
{
    bool result = false;

    owb_write_byte(ds9990_info->bus, length);
    owb_write_bytes(ds9990_info->bus, (uint8_t *)values, length);

    ESP_LOG_BUFFER_HEX_LEVEL(TAG, values, length, ESP_LOG_DEBUG);

    // With CRC:
    uint8_t crc = owb_crc8_bytes(0, (uint8_t *)values, length);
    owb_write_byte(ds9990_info->bus, crc);

    uint8_t crc_rcv;
    if (owb_read_byte(ds9990_info->bus, (uint8_t *)&crc_rcv) == OWB_STATUS_OK)
    {
        if (crc_rcv == crc)
        {
            //ESP_LOGE(TAG, "write ok : %d bytes", length);

            result = true;
        }
        else
        {

            ESP_LOGE(TAG, "write KO : %d bytes !!!!!!!!!!!!!", length);
        }
    }

    return result;
}

// Public API

DS9990_Info *ds9990_malloc(void)
{
    DS9990_Info *ds9990_info = malloc(sizeof(*ds9990_info));
    if (ds9990_info != NULL)
    {
        memset(ds9990_info, 0, sizeof(*ds9990_info));
        ESP_LOGE(TAG, "malloc %p", ds9990_info);
    }
    else
    {
        ESP_LOGE(TAG, "malloc failed");
    }

    return ds9990_info;
}

void ds9990_free(DS9990_Info **ds9990_info)
{
    if (ds9990_info != NULL && (*ds9990_info != NULL))
    {
        ESP_LOGE(TAG, "free %p", *ds9990_info);
        free(*ds9990_info);
        *ds9990_info = NULL;
    }
}

void ds9990_init(DS9990_Info *ds9990_info, const OneWireBus *bus, OneWireBus_ROMCode rom_code)
{
    if (ds9990_info != NULL)
    {
        _init(ds9990_info, bus);
        ds9990_info->rom_code = rom_code;
    }
    else
    {
        ESP_LOGE(TAG, "ds9990_info is NULL");
    }
}

void ds9990_use_crc(DS9990_Info *ds9990_info, bool use_crc)
{
    if (_is_init(ds9990_info))
    {
        ds9990_info->use_crc = use_crc;
        ESP_LOGE(TAG, "use_crc %d", ds9990_info->use_crc);
    }
}

DS9990_ERROR ds9990_read_memory(const DS9990_Info *ds9990_info, uint8_t *value, uint8_t length)
{
    DS9990_ERROR err = DS9990_ERROR_UNKNOWN;
    if (_is_init(ds9990_info))
    {
        //ESP_LOGE(TAG, "memory read: CRC %d, count %d\n", ds9990_info->use_crc, count);
        if (_address_device(ds9990_info))
        {

            if (owb_write_byte(ds9990_info->bus, DS9990_FUNCTION_READ_MEMORY) == OWB_STATUS_OK)
            {

                Memory memory = {0};
                if ((err = _read_memory(ds9990_info, &memory, length)) == DS9990_OK)
                {
                    memcpy(value, memory.data, length);
                }
                else
                {
                    ESP_LOGE(TAG, "ds9990_read_memory : error");
                }
            }
            else
            {
                ESP_LOGE(TAG, "owb_write_byte failed");
                err = DS9990_ERROR_OWB;
            }
        }
    }
    return err;
}

DS9990_ERROR ds9990_write_memory(const DS9990_Info *ds9990_info, uint8_t *value, uint8_t length)
{
    DS9990_ERROR err = DS9990_ERROR_UNKNOWN;
    if (_is_init(ds9990_info))
    {
        //ESP_LOGE(TAG, "memory read: CRC %d, count %d\n", ds9990_info->use_crc, count);
        if (_address_device(ds9990_info))
        {
            // send read order
            if (owb_write_byte(ds9990_info->bus, DS9990_FUNCTION_WRITE_MEMORY) == OWB_STATUS_OK)
            {
                err = (_write_memory(ds9990_info, value, length) == DS9990_OK);
            }
        }
    }
    return err;
}

DS9990_ERROR ds9990_write_read_memory(const DS9990_Info *ds9990_info, uint8_t *value_w, uint8_t length_w, uint8_t *value_r, uint8_t length_r)
{
    DS9990_ERROR err = DS9990_ERROR_UNKNOWN;
    if (_is_init(ds9990_info))
    {
        //ESP_LOGE(TAG, "memory read: CRC %d, count %d\n", ds9990_info->use_crc, count);
        if (_address_device(ds9990_info))
        {

            // send read order
            if (owb_write_byte(ds9990_info->bus, DS9990_FUNCTION_WRITE_READ_MEMORY) == OWB_STATUS_OK)
            {
                Memory memory = {0};

                if ((err = _write_memory(ds9990_info, value_w, length_w)) == DS9990_OK)
                {
                    ESP_LOGE(TAG, "ds9990_write_read_memory : write error");
                    return err;
                }

                if ((err = _read_memory(ds9990_info, &memory, length_r)) == DS9990_OK)
                {
                    memcpy(value_r, memory.data, length_r);
                }
                else
                {
                    ESP_LOGE(TAG, "ds9990_write_read_memory : read error");
                    return err;
                }
            }
            else
            {
                ESP_LOGE(TAG, "ds9990_write_read_memory : err2");
            }
        }
        else
        {
            err = DS9990_ERROR_DEVICE;
            ESP_LOGE(TAG, "ds9990_write_read_memory : err3");
        }
    }
    else
    {
        ESP_LOGE(TAG, "ds9990_write_read_memory : err4");
    }
    return err;
}