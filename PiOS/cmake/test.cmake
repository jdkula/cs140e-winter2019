##########
# CS140E: Test CMake Snippet
# Jonathan Kula
# Winter 2019
#
# This file provides a list of commands
# that set up a test target.
#
# This file should be included in the project's CMakeLists.txt
# This file expends the following environment variables to be present:
# TEST_NAME - The name of the test
# SRC - List of source files
# SYSTEM_INCLUDE_DIRECTORIES [optional] - List of directories to include as system includes.
# INCLUDE_DIRECTORIES [optional] - List of directories to include
# DEFINES [optional] - List of macro definitions to include.
# COMPILE_OPTIONS [optional] - List of options to give to the copiler
# LINK_OPTIONS [optional] - List of options to give to the linker
# LINKER_SCRIPT [optional] - A linker script to use (if existient).
# DEPENDENCIES [optional] - List of libraries to link to.
# EXE_OUT_DIR - Location to place the output executable file.
# IMAGE_OUTPUT_DIRECTORY - Location to place the output .img, .hex, and .list files.

########################################
# Executable Setup                     #
########################################

if (NOT "${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "ARM")
    # Create executable target with predefined source
    add_executable(${MODULE_NAME}.test ${SRC})

    # Commit compile options
    target_compile_options(${MODULE_NAME}.test PRIVATE ${COMPILE_OPTIONS})
    target_include_directories(${MODULE_NAME}.test SYSTEM PUBLIC ${SYSTEM_INCLUDE_DIRECTORIES})
    target_include_directories(${MODULE_NAME}.test PUBLIC ${INCLUDE_DIRECTORIES})
    target_compile_definitions(${MODULE_NAME}.test PRIVATE ${DEFINES})

    # Link dependencies and add linker arguments (target_link_options only supported in CMake >3.13)
    target_link_libraries(${MODULE_NAME}.test ${DEPENDENCIES} ${LINK_OPTIONS})

    # Set other properties
    set_target_properties(${MODULE_NAME}.test
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
            )

    # Ensure output directory exists
    file(MAKE_DIRECTORY "${IMAGE_OUTPUT_DIRECTORY}")
endif ()


########################################
#> End Executable Setup               <#
########################################