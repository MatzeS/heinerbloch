cmake_minimum_required(VERSION 3.13...3.27)
include(pico_sdk_import.cmake)
project(heinerbloch)
pico_sdk_init()
set(CMAKE_CXX_STANDARD 20)

add_executable(heinerbloch
    src/main.cpp
)
target_link_libraries(heinerbloch
    pico_stdlib
)
pico_add_extra_outputs(heinerbloch)
