# quicktype is not directly a library,
# but the quicktype code generator is used.
# This file adds cmake commands, so quicktype can be easily used
# in the project.

function(generate_data_models)
    # function argument parsing and checking
    set(OPTIONS)
    set(ONE_VALUE_ARGS TARGET_NAME)
    set(MULTI_VALUE_ARGS SOURCES)
    cmake_parse_arguments(PARSED_ARGS
        "${OPTIONS}" "${ONE_VALUE_ARGS}" "${MULTI_VALUE_ARGS}"
        ${ARGN}
    )

    if(NOT PARSED_ARGS_TARGET_NAME)
        message(FATAL_ERROR "TARGET_NAME argument not provided")
    endif()

    if(NOT PARSED_ARGS_SOURCES)
        message(FATAL_ERROR "SOURCES argument not provided")
    endif()

    # rename function argument variable (just to make it nicer)
    set(SOURCES ${PARSED_ARGS_SOURCES})
    set(TARGET_NAME ${PARSED_ARGS_TARGET_NAME})

    # define some helper variables
    set(INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/${TARGET_NAME})
    set(INCLUDE_PREFIX data-models) # in C++ use #include <data-models/input.hpp> to include
    set(OUT_HEADER_DIR ${INCLUDE_DIR}/${INCLUDE_PREFIX})

    # add code generation command for all sources
    set(OUT_HEADERS)
    foreach(SOURCE_FILE ${SOURCES})
        get_filename_component(SOURCE_STEM ${SOURCE_FILE} NAME_WE)

        set(OUT_HEADER ${OUT_HEADER_DIR}/${SOURCE_STEM}.hpp)
        list(APPEND OUT_HEADERS ${OUT_HEADER})

        add_custom_command(
            OUTPUT ${OUT_HEADER}
            COMMAND
                quicktype
                    --lang c++
                    --src-lang schema
                    --namespace heinerbloch
                    --code-format with-struct
                    --const-style east-const
                    --include-location global-include
                    --type-style pascal-case
                    --member-style camel-case
                    --enumerator-style pascal-case
                    --no-boost
                    --top-level ${SOURCE_STEM}
                    --out ${OUT_HEADER}
                    ${SOURCE_FILE}
            DEPENDS ${SOURCE_FILE}
        )
    endforeach()

    # define target and helper dependency
    add_library(${TARGET_NAME} INTERFACE)
    target_include_directories(${TARGET_NAME} INTERFACE ${INCLUDE_DIR})

    set(HELPER_DEPENDENCY ${TARGET_NAME}_quicktype)
    add_custom_target(${HELPER_DEPENDENCY} DEPENDS ${OUT_HEADERS})
    add_dependencies(${TARGET_NAME} ${HELPER_DEPENDENCY})

endfunction()
