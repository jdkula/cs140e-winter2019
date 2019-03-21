_For additional details pertaining specifically to the
 final project, please also see [PROJECT.md](./PROJECT.md)_

PiOS
=====

This project is an attempt at bringing together into a
set of libraries all the labs from Winter 2019 CS140E with
Dawson Engler. The act of unifying everything into a single
structure wasn't the easiest thing, but it proved extremely
helpful for my own understanding of the material.

Here we have a bunch of bare metal libraries and programs
designed for the Raspberry Pi 1 A+. Because most of the code
comes from our labs, a fair amount of the code is Dawson's.

Requirements
-------------
Builds on Ubuntu 18.10 using the `arm-gcc-none-eabi` toolchain.

CMake >= 3.10 is required; I used CMake 3.12.3. The ARM GCC CMake toolchain
is included in this project, and should be used when building for ARM.

The `build.sh` script will attempt to build the source for you,
acquiring the latest version of CMake if you lack it.

Project Structure
------------------
`lib/` contains libraries (i.e. any reusable code not containing a `notmain`.)
The most important library here is `libpi`, which contains most of the code
required to interface with the Raspberry Pi and its peripherals.

`modules/` contains bare metal programs intended to run on the Raspberry Pi.

`programs/` contains Unix utilities. Many of them are companions or clients
of RPi modules and communicate with it using UART.

`test/` at one point contained test programs that would emulate
Pi functionality to ideally make debugging easier. As the course
got harder, this became difficult to maintain and has since been 
left by the wayside. I may pick it back up at some point since
debugging on the Pi is Not Easy™.

`cmake/` contains helper files for CMake and the ARM GCC toolchain.

### Target Structure
Each of these subfolders contain targets that are compiled and linked
against each other by CMake. Due to some legacy code (especially for
the `test/` group), much of what is in `lib/` was at one point
cross-compiled on x86 and arm with different implementations. To this
end, each compilation target has this structure:

`CMakeLists.txt` - Contains build definitions for the target

`linker/` - contains linker scripts applied to the target.
> `linker/script-arm` - applied when being compiled to ARM.

`include/` - contains include files for the target.

`src/` - contains source compiled for both x86 and ARM (in `lib/`),
         compiled for just ARM (in `modules/`), and compiled for
         just x86 (in `programs/`). (this is a remnant of the
         aforementioned cross-compilation)

`sysspec/` - contains source compiled only on a particular architecture.
> `sysspec/arm` - just compiled on ARM
>
> `sysspec/x86` - just compiled on x86