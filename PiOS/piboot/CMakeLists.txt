cmake_minimum_required(VERSION 3.10)

project(PiBoot C)

set(SOURCES
        src/my-install.c
        src/fatal.c
        src/simple-boot.c
        src/support.c
)

add_executable(piboot ${SOURCES})

target_compile_options(piboot
        PUBLIC -Wall
        PUBLIC -Og
        PUBLIC -ggdb
        PUBLIC -Iinclude
        PUBLIC -g
        PUBLIC -lm)

target_link_libraries(piboot boot)

target_include_directories(piboot
        PRIVATE include
        )

set_target_properties(piboot
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
)