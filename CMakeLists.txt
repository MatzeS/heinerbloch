add_subdirectory(libs)

add_library(
    heinerbloch
    ${CMAKE_CURRENT_LIST_DIR}/src/calculator.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/BME280.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/I2C.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/Utilities.cpp
)
target_include_directories(
    heinerbloch
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src
)
target_link_libraries(
    heinerbloch
    PUBLIC
    expected
    PRIVATE
    nlohmann_json::nlohmann_json
    DataModels
    fmt::fmt
)
target_compile_options(heinerbloch PRIVATE $<$<COMPILE_LANGUAGE:CXX>:
    -pedantic
    -pedantic-errors
    -Wno-psabi
    -Wall
    -Wextra
    -Wconversion
    -Wsign-conversion
    -Werror
>)

generate_data_models(
    TARGET_NAME DataModels
    SOURCES ${CMAKE_CURRENT_LIST_DIR}/data-models/foo.schema.json
)
