cmake_minimum_required(VERSION 3.10)

project(PiOS C ASM)


add_executable(bootloader.base src/start.s src/bootloader.c)

get_filename_component(MEMMAP_FILE linker/memmap REALPATH)

message(STATUS "${CMAKE_OBJDUMP}")
message(STATUS "Memmap -- ${MEMMAP_FILE}")

target_link_libraries(bootloader.base pi -T${MEMMAP_FILE})
target_compile_options(bootloader.base PUBLIC -nostdlib -nostartfiles -ffreestanding -fno-builtin)



get_filename_component(OUT_DIR out REALPATH)


target_include_directories(bootloader.base SYSTEM PUBLIC ..)

set_target_properties(bootloader.base
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${LIB_OUT_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${EXE_OUT_DIR}"
        )

message(STATUS "${META_OUT_DIR} || ${IMG_OUT_DIR}")


file(MAKE_DIRECTORY "${IMG_OUT_DIR}/bootloader")

add_custom_target(bootloader "${CMAKE_OBJDUMP}" -D $<TARGET_FILE:bootloader.base> > "${META_OUT_DIR}/bootloader.list"
        COMMAND "${CMAKE_OBJCOPY}" $<TARGET_FILE:bootloader.base> -O ihex "${META_OUT_DIR}/bootloader.hex"
        COMMAND "${CMAKE_OBJCOPY}" $<TARGET_FILE:bootloader.base> -O binary "${IMG_OUT_DIR}/bootloader/kernel.img"
        DEPENDS bootloader.base
        )
