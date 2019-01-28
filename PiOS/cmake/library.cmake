##########
# CS140E: Library CMake Snippet
# Jonathan Kula
# Winter 2019
#
# This file provides a list of commands
# that set up a static library from a bunch
# of environment variables.
#
# This file should be included in the project's CMakeLists.txt
# This file expends the following environment variables to be present:
# LIBRARY_NAME - The name of the library
# SRC - List of source files
# INCLUDE_DIRECTORIES [optional] - List of directories to include
# SYSTEM_INCLUDE_DIRECTORIES [optional] - List of directories to include as system includes.
# DEFINES [optional] - List of additional compiler definitions.
# COMPILE_OPTIONS [optional] - List of options to give to the copiler
# LINK_OPTIONS [optional] - List of options to give to the linker
# DEPENDENCIES [optional] - List of libraries to link to.
# LIB_OUT_DIR - Location to place the output .a file.

########################################
# Library Setup                        #
########################################

# Create static library target from predefined source
add_library(${LIBRARY_NAME} STATIC ${SRC})

# Add include directories and share them with dependants.
target_include_directories(${LIBRARY_NAME} PUBLIC ${INCLUDE_DIRECTORIES})

# Add target defines.
target_compile_definitions(${LIBRARY_NAME} PRIVATE ${DEFINES})

# Commit compile and link options
target_compile_options(${LIBRARY_NAME} PUBLIC ${COMPILE_OPTIONS})
target_link_libraries(${LIBRARY_NAME} ${DEPENDENCIES} ${LINK_OPTIONS})

# Commit output directory
set_target_properties(${LIBRARY_NAME}
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
        PREFIX ""
        )

########################################
#> End Library Setup                  <#
########################################