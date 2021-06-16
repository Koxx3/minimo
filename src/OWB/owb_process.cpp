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
#include "pinout.h"

#include "owb.h"
#include "owb_process.h"
#include "owb_rmt.h"
#include "ds9990.h"
#include "SharedData.h"
#include "tools/buffer.h"

#define GPIO_OWB (PIN_IN_OUT_ONEWIRE)
#define MAX_DEVICES 4
#define SAMPLE_PERIOD 50 // milliseconds
#define SINGLE_ATTEMPT 1

int num_devices = 0;
DS9990_Info *devices[MAX_DEVICES] = {0};
OneWireBus *owb;
owb_rmt_driver_info rmt_driver_info;
OneWireBus_ROMCode device_rom_codes[MAX_DEVICES] = {0};

SharedData *owb_shrd;

void owb_setup(SharedData *shrd)
{

    owb_shrd = shrd;

    // Override global log level
    //esp_log_level_set("*", ESP_LOG_INFO);

    // To debug, use 'make menuconfig' to set default Log level to DEBUG, then uncomment:
    esp_log_level_set("owb", ESP_LOG_DEBUG);
    esp_log_level_set("ds9990", ESP_LOG_DEBUG);
    esp_log_level_set("owb_rmt", ESP_LOG_DEBUG);

    // Create a 1-Wire bus, using the RMT timeslot driver
    owb = owb_rmt_initialize(&rmt_driver_info, GPIO_OWB, RMT_CHANNEL_1, RMT_CHANNEL_0);
    owb_use_crc(owb, true); // enable CRC check for ROM code

    // Find all connected devices
    printf("OW - Find devices:\n");
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
            printf("OW - device %d is DS9990\n", num_devices);
            owb_shrd->owb_ds9990_present = true;
        }
        else
        {
            printf("OW - device %d is UNKNONWN\n", num_devices);
        }

        num_devices++;
        owb_search_next(owb, &search_state, &found);
    }
    printf("OW - Found %d device%s\n", num_devices, num_devices == 1 ? "" : "s");
}

void owb_loop()
{

    // Stable readings require a brief period before communication
    vTaskDelay(2000.0 / portTICK_PERIOD_MS);

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
            DS9990_ERROR errors_ds9990[MAX_DEVICES] = {DS9990_OK};

            for (int i = 0; i < num_devices; ++i)
            {
                if (device_rom_codes[i].fields.family[0] == DS9990_FAMILY_CODE)
                {

                    // -------------
                    // WRITE & READ
                    // first attempt

                    writings[0] = owb_shrd->brakePressedStatus;

                    bool isDataOk = false;
                    errors_ds9990[i] = ds9990_write_read_memory(devices_ds9990[i], writings, 1, readings, 7);
                    if (errors_ds9990[i] != DS9990_OK)
                    {
#if SINGLE_ATTEMPT
                        ++errors_ds9990_count[i];
                        printf("OW %d: %d errors (%lu ms)\n", i, errors_ds9990_count[i], millis());
#else
                        // second attemp
                        errors_ds9990[i] = ds9990_write_read_memory(devices_ds9990[i], writings, 1, readings, 7);
                        if (errors_ds9990[i] != DS9990_OK)
                        {
                            ++errors_ds9990_count[i];
                            printf("OW %d: %d errors => still error => increase counter (%d ms)\n", i, errors_ds9990_count[i], millis());
                        }
                        else
                        {
                            isDataOk = true;
                        }
#endif
                    }
                    else
                    {
                        isDataOk = true;
                    }

                    if (isDataOk)
                    {
                        //printf("  %d: %02x %02x %02x %02x %02x %02x %02x    %d errors\n", i, readings[0], readings[1], readings[2], readings[3], readings[4], readings[5], readings[6], errors_ds9990_count[i]);
                        int32_t index = 1;
                        int16_t tempInf = buffer_get_int16_inv(readings, &index);
                        int16_t humInt = buffer_get_int16_inv(readings, &index);
                        uint16_t curInt = buffer_get_uint16_inv(readings, &index);
                        owb_shrd->currentTemperature = tempInf / 10.0;
                        owb_shrd->currentHumidity = humInt / 10.0;
                        owb_shrd->currentActual = curInt;
                        //printf("  %d: t = %d / h = %d / c = %d    %d errors\n", i, tempInf, humInt, curInt, errors_ds9990_count[i]);
                    }
                }
            }

            i_loop++;

            vTaskDelayUntil(&last_wake_time, SAMPLE_PERIOD / portTICK_PERIOD_MS);
        }
    }
    else
    {
        while (1)
        {
            vTaskDelay(100);
            //printf("OW - no device found\n");
        }
    }
}
