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

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "owb.h"
#include "owb_process.h"
#include "owb_rmt.h"
#include "ds9990.h"
#include "SharedData.h"

#define GPIO_OWB (GPIO_NUM_12)
#define MAX_DEVICES (8)
#define SAMPLE_PERIOD (50) // milliseconds
#define SINGLE_ATTEMPT 0

int num_devices = 0;
DS9990_Info *devices[MAX_DEVICES] = {0};
OneWireBus *owb;

SharedData *owb_shrd;

void owb_setSharedData(SharedData *shrd)
{
    owb_shrd = shrd;
}

void owb_loop()
{

    // Override global log level
    //esp_log_level_set("*", ESP_LOG_INFO);

    // To debug, use 'make menuconfig' to set default Log level to DEBUG, then uncomment:
    esp_log_level_set("owb", ESP_LOG_DEBUG);
    esp_log_level_set("ds9990", ESP_LOG_DEBUG);
    esp_log_level_set("owb_rmt", ESP_LOG_DEBUG);

    // Stable readings require a brief period before communication
    vTaskDelay(2000.0 / portTICK_PERIOD_MS);

    // Create a 1-Wire bus, using the RMT timeslot driver
    OneWireBus *owb;
    owb_rmt_driver_info rmt_driver_info;
    owb = owb_rmt_initialize(&rmt_driver_info, GPIO_OWB, RMT_CHANNEL_1, RMT_CHANNEL_0);
    owb_use_crc(owb, true); // enable CRC check for ROM code

    // Find all connected devices
    printf("Find devices:\n");
    OneWireBus_ROMCode device_rom_codes[MAX_DEVICES] = {0};
    int num_devices = 0;
    OneWireBus_SearchState search_state = {0};
    bool found = false;
    owb_search_first(owb, &search_state, &found);
    while (found)
    {
        char rom_code_s[17];
        owb_string_from_rom_code(search_state.rom_code, rom_code_s, sizeof(rom_code_s));
        device_rom_codes[num_devices] = search_state.rom_code;
        if (search_state.rom_code.fields.family[0] == DS9990_FAMILY_CODE)
        {
            printf("  %d devices : is DS9990\n", num_devices);
        }

        num_devices++;
        owb_search_next(owb, &search_state, &found);
    }
    printf("Found %d device%s\n", num_devices, num_devices == 1 ? "" : "s");

    // Create DS18B20 devices on the 1-Wire bus
    DS9990_Info *devices_ds9990[MAX_DEVICES] = {0};
    for (int i = 0; i < num_devices; ++i)
    {
        if (device_rom_codes[i].fields.family[0] == DS9990_FAMILY_CODE)
        {
            DS9990_Info *ds9990_info = ds9990_malloc(); // heap allocation
            devices_ds9990[i] = ds9990_info;

            ds9990_init(ds9990_info, owb, device_rom_codes[i]); // associate with bus and device
            ds9990_use_crc(ds9990_info, true);                  // enable CRC check on all reads
        }
    }

#ifdef CONFIG_ENABLE_STRONG_PULLUP_GPIO
    // An external pull-up circuit is used to supply extra current to OneWireBus devices
    // during temperature conversions.
    owb_use_strong_pullup_gpio(owb, CONFIG_STRONG_PULLUP_GPIO);
#endif

    vTaskDelay(3000);

    // Read temperatures more efficiently by starting conversions on all devices at the same time
    int errors_ds9990_count[MAX_DEVICES] = {0};
    if (num_devices > 0)
    {

        TickType_t last_wake_time = xTaskGetTickCount();
        uint8_t i_loop = 0;
        while (1)
        {

            //--------------------
            // DS 9990

            // Read the results immediately after conversion otherwise it may fail
            // (using printf before reading may take too long)
            uint8_t readings[8] = {0};
            uint8_t writings[1] = {0};
            writings[0] = owb_shrd->brakePressedStatus;
            DS9990_ERROR errors_ds9990[MAX_DEVICES] = {DS9990_OK};

            for (int i = 0; i < num_devices; ++i)
            {
                if (device_rom_codes[i].fields.family[0] == DS9990_FAMILY_CODE)
                {
                    /*
                    // -------------
                    // READ
                    errors_ds9990[i] = ds9990_read_memory(devices_ds9990[i], readings, 7);

                    if (errors_ds9990[i] != DS9990_OK)
                    {
                        ++errors_ds9990_count[i];
                    }
                    //printf("  %d: %02x %02x %02x %02x %02x %02x %02x %02x    %d errors\n", i, readings[0], readings[1], readings[2], readings[3], readings[4], readings[5], readings[6], readings[7], errors_ds9990_count[i]);

                    // -------------
                    // WRITE
                    readings[0] = (uint8_t)i_loop % 256;
                    errors_ds9990[i] = ds9990_write_memory(devices_ds9990[i], readings, 1);

                    if (errors_ds9990[i] != DS9990_OK)
                    {
                        ++errors_ds9990_count[i];
                    }
                    */

                    // -------------
                    // WRITE & READ
                    // first attempt
                    errors_ds9990[i] = ds9990_write_read_memory(devices_ds9990[i], writings, 1, readings, 7);
                    if (errors_ds9990[i] != DS9990_OK)
                    {
#if SINGLE_ATTEMPT
                        ++errors_ds9990_count[i];
#else
                        // second attemp
                        errors_ds9990[i] = ds9990_write_read_memory(devices_ds9990[i], writings, 1, readings, 7);
                        if (errors_ds9990[i] != DS9990_OK)
                        {
                            ++errors_ds9990_count[i];
                            printf("  %d: %d errors => still error => increase counter\n", i, errors_ds9990_count[i]);
                        }
                        // printf("  %d: %02x %02x %02x %02x %02x %02x %02x    %d errors\n", i, readings[0], readings[1], readings[2], readings[3], readings[4], readings[5], readings[6], errors_ds9990_count[i]);
#endif
                        printf("  %d: %d errors => retry\n", i, errors_ds9990_count[i]);
                    }
                }
            }

            i_loop++;

            vTaskDelayUntil(&last_wake_time, SAMPLE_PERIOD / portTICK_PERIOD_MS);

            //Serial.println();
        }
    }
}
