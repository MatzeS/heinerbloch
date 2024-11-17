cmake_minimum_required(VERSION 3.13...3.27)
include(pico_sdk_import.cmake)
project(heinerbloch)
pico_sdk_init()
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs)

add_executable(heinerbloch
    src/main.cpp
    src/sd_hw_config.c
)
target_link_libraries(heinerbloch
    pico_stdlib
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
)
pico_add_extra_outputs(heinerbloch)
