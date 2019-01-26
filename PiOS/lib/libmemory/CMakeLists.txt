##########
# CS140E: LibMemory
# Jonathan Kula
# Winter 2019
#
# Provides interfaces for Raspberry Pi memory, like
# get32, set32, etc. This prevents GCC from optimizing
# memory code unexpectedly, and allows a drop-in test
# framework that can be run on the computer.
# Also provides memory barriers.
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

project(memory C ASM)

########################################
#> End Project Setup                  <#
########################################

########################################
# Build Variable Setup                 #
########################################

# Source Files
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")
    set(SRC
            arm/mem-access.c
            arm/mem-access.s
            arm/mem-barrier.s
            )

    set(COMPILE_OPTIONS -nostdlib)
    set(LINKER_OPTIONS -nostdlib)
else ()
    set(SRC
            x86/mem-access.c
            x86/mem-barrier.c
            )

    set(COMPILE_OPTIONS)
    set(LINKER_OPTIONS)
endif ()


# Compile Options
set(INCLUDE_DIRECTORIES include-common)


# Dependencies
set(LIBMEMORY_DEPENDENCIES types)

# Output Directories
## Controlled by ${LIB_OUT_DIR} (see root CMakeLists.txt)

########################################
#> End Build Variable Setup           <#
########################################

########################################
# Library Setup                        #
########################################

# Create static library target from predefined source
add_library(memory STATIC ${SRC})

# Add include directories and share them with dependants.
target_include_directories(memory PUBLIC ${INCLUDE_DIRECTORIES})

# Commit compile and link options
target_compile_options(memory PUBLIC ${COMPILE_OPTIONS})
target_link_libraries(memory ${LIBMEMORY_DEPENDENCIES} ${LINKER_OPTIONS})

# Commit output directory
set_target_properties(memory
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
        )

########################################
#> End Library Setup                  <#
########################################