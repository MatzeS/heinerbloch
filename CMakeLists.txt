cmake_minimum_required(VERSION 3.13...3.27)
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

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
)
target_include_directories(heinerbloch
    PRIVATE
    src
    libs/FreeRTOS/gthr
)
pico_enable_filesystem(heinerbloch FS_INIT src/fs_init.cpp)
target_link_libraries(heinerbloch
    PRIVATE
    pico_stdlib
    # pico_filesystem
    # pico_filesystem_filesystem_fat
    # pico_filesystem_blockdevice_sd
    expected
    FreeRTOS-Kernel-Heap4
    FreeRTOS-Config
    FreeRTOS-POSIX
)
pico_add_extra_outputs(heinerbloch)

target_compile_definitions(heinerbloch PUBLIC 
    -D_GLIBCXX_HAS_GTHREADS=1 
    -D_POSIX_THREADS=1 
    -D_POSIX_TIMEOUTS=1
    -D_UNIX98_THREAD_MUTEX_ATTRIBUTES=1
)