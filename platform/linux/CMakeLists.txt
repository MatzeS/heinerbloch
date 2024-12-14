cmake_minimum_required(VERSION 3.25.0)
project(heinerbloch-linux)
set(CMAKE_CXX_STANDARD 20)

add_executable(
    heinerbloch-linux
    src/main.cpp
)
target_link_libraries(
    heinerbloch-linux
    PRIVATE
    heinerbloch
)

add_subdirectory(../../ build-heinerbloch)
