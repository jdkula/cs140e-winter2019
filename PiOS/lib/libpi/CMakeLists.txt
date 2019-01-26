##########
# CS140E: LibPi
# Jonathan Kula
# Winter 2019
#
# Provides interfaces for Raspberry Pi hardware, like
# GPIO, UART, timing, rebooting, and so on.
#
# This build file was last updated on January 25th, 2019.
# If you're unfamiliar with CMake, you can just use the
# build.sh file present at the project root to build all.
#
# This project should not be generated directly; use the
# CMakeLists.txt file located at the root of the project.

cmake_minimum_required(VERSION 3.10)

########################################
# Project Setup                        #
########################################

project(libpi C ASM)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
set(SRC
        common/aux.c
        common/gpio.c
        common/reboot.c
        common/timer.c
        common/uart.c
        )

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    list(APPEND SRC arm/dummy.s)
else ()
    list(APPEND SRC x86/dummy.c)
endif ()

# Compile & Link Options

set(INCLUDE_DIRECTORIES include-common)

if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(COMPILE_OPTIONS -nostdlib)
    set(LINKER_OPTIONS -nostdlib)
else ()
    set(COMPILE_OPTIONS)
    set(LINKER_OPTIONS)
endif ()

# Dependencies
set(DEPENDENCIES memory types)

# Output Directories
## Controlled by ${LIB_OUT_DIR} (see root CMakeLists.txt)

########################################
#> End Build Variable Setup           <#
########################################

########################################
# Library Setup                        #
########################################

# Create static library target from predefined source
add_library(pi STATIC ${SRC})

# Add include directories and share them with dependants.
target_include_directories(pi PUBLIC ${INCLUDE_DIRECTORIES})

# Commit compile and link options
target_compile_options(pi PUBLIC ${COMPILE_OPTIONS})
target_link_libraries(pi ${DEPENDENCIES} ${LINKER_OPTIONS})

# Commit output directory
set_target_properties(pi
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
        )

########################################
#> End Library Setup                  <#
########################################