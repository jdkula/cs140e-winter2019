/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: start.c [x86 Only]
 * --------------
 * The counterpart to start.s -- runs module_main.
 * Needed to properly build an executable on x86.
 */

#include <pios-macros.h>

#ifndef MODULE_NAME
#error "MODULE_NAME must be defined for startx86"
#endif

extern int MODULE_MAIN();

int main() {
    return MODULE_MAIN();
}