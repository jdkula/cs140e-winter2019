cmake_minimum_required(VERSION 3.10)

project(PiOS C ASM)

include("${CMAKE_ROOT}/Modules/ExternalProject.cmake")

file(MAKE_DIRECTORY "./out")
file(MAKE_DIRECTORY "./out/lib")
file(MAKE_DIRECTORY "./out/bin")
file(MAKE_DIRECTORY "./out/img")
file(MAKE_DIRECTORY "./out/meta")

get_filename_component(LIB_OUT_DIR out/lib REALPATH)
get_filename_component(EXE_OUT_DIR out/bin REALPATH)
get_filename_component(IMG_OUT_DIR out/img REALPATH)
get_filename_component(META_OUT_DIR out/meta REALPATH)
get_filename_component(PIBOOT_DIR PiBoot REALPATH)

ExternalProject_Add(piboot
        PREFIX "${PIBOOT_DIR}"
        SOURCE_DIR "${PIBOOT_DIR}"
        CMAKE_ARGS ""
        BUILD_ALWAYS TRUE
        INSTALL_COMMAND cp "${PIBOOT_DIR}/out/piboot" "${EXE_OUT_DIR}"
        )

add_subdirectory(modules/libpi)
add_subdirectory(modules/bootloader)

add_custom_target(build_all ALL DEPENDS bootloader pi piboot)

add_custom_target(clean_out rm "${LIB_OUT_DIR}/*"
            COMMAND rm "${EXE_OUT_DIR}/*"
            COMMAND rm -rf "${IMG_OUT_DIR}/*"
            COMMAND rm "${META_OUT_DIR}/*"
        )
