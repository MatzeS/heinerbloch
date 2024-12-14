set(EXPECTED_BUILD_TESTS OFF)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/expected)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/nlohmann_json)

add_subdirectory(FreeRTOS)

add_subdirectory(fmt)

include(quicktype.cmake)
