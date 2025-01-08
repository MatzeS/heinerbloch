

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

  puts("Hello, world!");

  // std::filesystem::path path{"/well/x.txt"};
  // // std::filesystem::create_directories(path.parent_path());
  // std::ofstream ofs(path);
  // ofs << "this is some text in the new file\n";
  // ofs.close();

    // size_t au_size_bytes;
    // bool ok = sd_allocation_unit(sd_get_by_num(0), &au_size_bytes);
    // if (!ok || !au_size_bytes)
    //     au_size_bytes = 4194304; // Default to 4 MiB
    // UINT n_align = au_size_bytes / sd_block_size;

    // MKFS_PARM opt = {
    //     FM_ANY,  /* Format option (FM_FAT, FM_FAT32, FM_EXFAT and FM_SFD) */
    //     2,       /* Number of FATs */
    //     n_align, /* Data area alignment (sector) */
    //     0,       /* Number of root directory entries */
    //     0        /* Cluster size (byte) */
    // };
    // /* Format the drive */
    // FRESULT fr = f_mkfs("", &opt, 0, FF_MAX_SS * 2);
    // if (FR_OK != fr) {
    //   panic("mkfs error: %s (%d)\n", FRESULT_str(fr), fr);
    // }

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
