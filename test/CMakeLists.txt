enable_testing()
find_package(GTest REQUIRED)

add_executable(
    heinerbloch_tests
    ${CMAKE_CURRENT_LIST_DIR}/calculator_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/I2C.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Sht35Test.cpp
)
target_include_directories(
    heinerbloch_tests
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../src
)
target_link_libraries(
    heinerbloch_tests
    heinerbloch
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(heinerbloch_tests)
