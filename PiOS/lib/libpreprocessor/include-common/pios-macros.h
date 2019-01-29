/**
 * Jonathan Kula | CS140E | Winter Quarter 2019
 * File: pios-macros.h
 * --------------
 * Contains useful preprocessor macros.
 */

#ifndef PIOS_PIOS_MACROS_H
#define PIOS_PIOS_MACROS_H

/** Makes the macro parameter into a string. */
#define _STRINGIFY(S) #S
/** Properly resolves variables when joining them with layers of indirection */
#define _VAR_JOIN(X, Y) X ## Y
/** Joins two variables */
#define _JOIN(X, Y) _VAR_JOIN(X, Y)


/** Ignores the given compiler warning until a POP() call. */
#define IGNORE(WARN) _Pragma("GCC diagnostic push") \
                     _Pragma(_STRINGIFY(GCC diagnostic ignored #WARN))

/** Undoes an IGNORE operation. */
#define POP() _Pragma("GCC diagnostic pop")



/** Used allow the same start library to be used with many modules and still link properly. */
#define MODULE_MAIN() _JOIN(MODULE_NAME, _main())

#endif //PIOS_PIOS_MACROS_H
