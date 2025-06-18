/* main.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/

#include <pico/stdio.h>
//
#include "pico/stdlib.h"
//
#include "f_util.h"
#include "ff.h"
#include "hw_config.h"

/**
 * @file main.c
 * @brief Minimal example of writing to a file on SD card
 * @details
 * This program demonstrates the following:
 * - Initialization of the stdio
 * - Mounting and unmounting the SD card
 * - Opening a file and writing to it
 * - Closing a file and unmounting the SD card
 */

#include <filesystem>
#include <fstream>
#include <iostream>

int main() {
  // Initialize stdio
  stdio_init_all();

  sleep_ms(500);
  
  puts("Starting up!");

  puts("Enabling power");
  #define HEINERBOX_ENABLE_SENSORS (46)
  gpio_init(HEINERBOX_ENABLE_SENSORS);
  gpio_set_dir(HEINERBOX_ENABLE_SENSORS, GPIO_OUT);
  gpio_put(HEINERBOX_ENABLE_SENSORS, 1);

  puts("Enabling SD Card Power");
  #define HEINERBOX_ENABLE_SDCARD  (31)
  gpio_init(HEINERBOX_ENABLE_SDCARD);
  gpio_set_dir(HEINERBOX_ENABLE_SDCARD, GPIO_OUT);
  gpio_put(HEINERBOX_ENABLE_SDCARD, 0);
  sleep_ms(100);
  gpio_put(HEINERBOX_ENABLE_SDCARD, 1);  
  sleep_ms(100);

  FRESULT fr;

  // See FatFs - Generic FAT Filesystem Module, "Application Interface",
  // http://elm-chan.org/fsw/ff/00index_e.html
  FATFS fs;
  fr = f_mount(&fs, "", 1);
  if (FR_OK != fr) {
    panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
  }

  // Open a file and write to it
  FIL fil;
  const char* const filename = "filename.txt";
  fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
  if (FR_OK != fr && FR_EXIST != fr) {
    panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
  }
  if (f_printf(&fil, "Hello, world!\n") < 0) {
    printf("f_printf failed\n");
  }

  // Close the file
  fr = f_close(&fil);
  if (FR_OK != fr) {
    printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
  }

  // Unmount the SD card
  f_unmount("");

  puts("Goodbye, world!");
  for (;;)
    ;
}
