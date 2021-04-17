/*
 * MIT License
 *
 * Copyright (c) 2017 David Antliff
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
 * @file ds9990.h
 * @brief Interface definitions for the Maxim Integrated DS9990 Programmable
 *        Resolution 1-Wire Digital Thermometer device.
 *
 * This component provides structures and functions that are useful for communicating
 * with DS9990 devices connected via a Maxim Integrated 1-Wire® bus.
 */

#ifndef DS9990_H
#define DS9990_H

#include "owb.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DS9990_FAMILY_CODE 0x09

/**
 * @brief Success and error codes.
 */
typedef enum
{
    DS9990_ERROR_UNKNOWN = -1,  ///< An unknown error occurred, or the value was not set
    DS9990_OK = 0,        ///< Success
    DS9990_ERROR_DEVICE,  ///< A device error occurred
    DS9990_ERROR_CRC,     ///< A CRC error occurred
    DS9990_ERROR_OWB,     ///< A One Wire Bus error occurred
    DS9990_ERROR_NULL,    ///< A parameter or value is NULL
} DS9990_ERROR;

/**
 * @brief Structure containing information related to a single DS9990 device connected
 * via a 1-Wire bus.
 */
typedef struct
{
    bool init;                     ///< True if struct has been initialised, otherwise false
    bool solo;                     ///< True if device is intended to be the only one connected to the bus, otherwise false
    bool use_crc;                  ///< True if CRC checks are to be used when retrieving information from a device on the bus
    const OneWireBus * bus;        ///< Pointer to 1-Wire bus information relevant to this device
    OneWireBus_ROMCode rom_code;   ///< The ROM code used to address this device on the bus
} DS9990_Info;

DS9990_Info * ds9990_malloc(void);

void ds9990_free(DS9990_Info ** ds9990_info);

void ds9990_init(DS9990_Info * ds9990_info, const OneWireBus * bus, OneWireBus_ROMCode rom_code);

void ds9990_use_crc(DS9990_Info * ds9990_info, bool use_crc);

OneWireBus_ROMCode ds9990_read_rom(DS9990_Info * ds9990_info);

DS9990_ERROR ds9990_read_memory(const DS9990_Info * ds9990_info, uint8_t * value, uint8_t length);

DS9990_ERROR ds9990_write_memory(const DS9990_Info * ds9990_info, uint8_t * value, uint8_t length);

DS9990_ERROR ds9990_write_read_memory(const DS9990_Info * ds9990_info, uint8_t * value_w, uint8_t length_w, uint8_t * value_r, uint8_t length_r);

#ifdef __cplusplus
}
#endif

#endif  // DS9990_H
