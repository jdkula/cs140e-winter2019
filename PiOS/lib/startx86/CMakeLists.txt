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

if (NOT ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "ARM")

    ########################################
    # Project Setup                        #
    ########################################

    project(start C)

    ########################################
    #> End Project Setup                  <#
    ########################################

    ########################################
    # Build Variable Setup                 #
    ########################################

    # Source Files
    set(SRC
            x86/start.c
            )


    # Compile Options
    set(COMPILE_OPTIONS)
    set(INCLUDE_DIRECTORIES include-common)


    # Linker Options
    set(LINKER_OPTIONS)

    # Dependencies

    set(LIBMEMORY_DEPENDENCIES)

    # Output Directories
    ## Controlled by ${LIB_OUT_DIR} (see root CMakeLists.txt)

    ########################################
    #> End Build Variable Setup           <#
    ########################################

    ########################################
    # Library Setup                        #
    ########################################

    # Create static library target from predefined source
    add_library(start STATIC ${SRC})

    # Add include directories and share them with dependants.
    target_include_directories(start PUBLIC ${INCLUDE_DIRECTORIES})

    # Commit compile and link options
    target_compile_options(start PUBLIC ${COMPILE_OPTIONS})
    target_link_libraries(start ${LIBMEMORY_DEPENDENCIES} ${LINKER_OPTIONS})

    # Commit output directory
    set_target_properties(start
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
            )

    ########################################
    #> End Library Setup                  <#
    ########################################

else ()

    add_library(start INTERFACE)

endif ()