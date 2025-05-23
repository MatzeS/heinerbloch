cmake_minimum_required(VERSION 3.13...3.27)
set(PICO_PLATFORM rp2350)
set(PICO_BOARD solderparty_rp2350_stamp_xl)

include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

project(heinerbloch-rp2350)
set(CMAKE_CXX_STANDARD 20)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
pico_sdk_init()

# Getting clang-tidy to work with the pico sdk includes was extremly difficult.
# There are still issues regarding C compilation, but it is not worth fixing this at the moment.
# Also see cmake forum where this solution originated:
# https://discourse.cmake.org/t/configure-clang-tidy-when-using-an-arm-none-eabi-gcc-g-toolchain/10862
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

add_executable(
    heinerbloch-rp2350
    src/main.cpp
    src/I2C.cpp
)
pico_enable_filesystem(heinerbloch-rp2350 FS_INIT src/fs_init.cpp)
target_link_libraries(heinerbloch-rp2350
    PRIVATE
    heinerbloch
    pico_stdlib
    pico_filesystem
    pico_filesystem_filesystem_fat
    pico_filesystem_blockdevice_sd
    hardware_i2c
    expected
    FreeRTOS-Kernel-Heap4
    FreeRTOS-Config
    FreeRTOS-POSIX
)
pico_add_extra_outputs(heinerbloch-rp2350)

add_subdirectory(../../ build-heinerbloch)
