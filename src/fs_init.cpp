/*
 * Copyright 2024, Hiroyuki OYAMA. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <blockdevice/sd.h>
#include <filesystem/fat.h>
#include <filesystem/vfs.h>
#include <hardware/clocks.h>
#include <stdio.h>
#include <string.h>

bool fs_init(void) {
  printf("fs_init FAT on SD card\n");
  blockdevice_t* sd =
      blockdevice_sd_create(spi1, 11, 8, 10, 9, 24 * MHZ, false);
  filesystem_t* fat = filesystem_fat_create();
  int err = fs_mount("/", fat, sd);
  if (err == -1) {
    printf("format / with FAT\n");
    err = fs_format(fat, sd);
    if (err == -1) {
      printf("fs_format error: %s", strerror(errno));
      return false;
    }
    err = fs_mount("/", fat, sd);
    if (err == -1) {
      printf("fs_mount error: %s", strerror(errno));
      return false;
    }
  }
  return true;
}
