/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: start.c [x86 Only]
 * --------------
 * The counterpart to start.s -- runs module_main.
 * Needed to properly build an executable on x86.
 */

int module_main();

int main() {
    return module_main();
}