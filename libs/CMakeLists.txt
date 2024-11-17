add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/no-OS-FatFS-SD-SDIO-SPI-RPi-Pico/src build)

set(EXPECTED_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/expected)
