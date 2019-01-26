cmake_minimum_required(VERSION 3.10)

project(LibPi C ASM)

set(SOURCE
        src/aux.c
        src/gpio.c
        src/helper.c
        src/helpers.s
        src/mem-barrier.s
        src/reboot.c
        src/timer.c
        src/uart.c
)

add_library(pi STATIC ${SOURCE})

target_include_directories(pi
        PUBLIC include
        )
target_compile_options(pi PUBLIC -nostdlib)
target_link_libraries(pi PUBLIC -nostdlib)

set_target_properties(pi
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
)
