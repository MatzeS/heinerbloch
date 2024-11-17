cmake_minimum_required(VERSION 3.13...3.27)
include(pico_sdk_import.cmake)

set(CMAKE_C_COMPILER "/opt/arm-gnu-toolchain/bin/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "/opt/arm-gnu-toolchain/bin/arm-none-eabi-g++")

project(heinerbloch)
pico_sdk_init()
set(CMAKE_CXX_STANDARD 20)

# Getting clang-tidy to work with the pico sdk includes was extremly difficult.
# There are still issues regarding C compilation, but it is not worth fixing this at the moment.
# Also see cmake forum where this solution originated:
# https://discourse.cmake.org/t/configure-clang-tidy-when-using-an-arm-none-eabi-gcc-g-toolchain/10862
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})

add_subdirectory(libs)

add_executable(heinerbloch
    src/main.cpp
    src/sd_hw_config.cpp
)
target_link_libraries(heinerbloch
    pico_stdlib
    no-OS-FatFS-SD-SDIO-SPI-RPi-Pico
    expected
)
pico_add_extra_outputs(heinerbloch)
