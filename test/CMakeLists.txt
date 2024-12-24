enable_testing()
find_package(GTest REQUIRED)

add_executable(
    heinerbloch_tests
    ${CMAKE_CURRENT_LIST_DIR}/calculator_tests.cpp
    ${CMAKE_CURRENT_LIST_DIR}/I2C.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UtilitiesTest.cpp
)
target_link_libraries(
    heinerbloch_tests
    heinerbloch
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(heinerbloch_tests)
