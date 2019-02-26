#ifndef __DEMAND_H__
#define __DEMAND_H__

#ifdef __linux__

#include <stdio.h>
#include <stdlib.h>

#else
#include "printf.h"
#define fprintf(ignored, ...) printk(__VA_ARGS__)
#define exit(...) reboot()
#endif

#define _XSTRING(x) #x

// usage: 
//	demand(expr, msg)
// prints <msg> if <expr> is false.
// example:
//	demand(x < 3, x has bogus value!);
// note: 
//	if <_msg> contains a ',' you'll have to put it in quotes.
#define demand(_expr, _msg) do {                    \
    if(!(_expr)) {                            \
        fprintf(stderr, "ERROR:%s:%s:%d: "            \
            "FALSE(<" _XSTRING(_expr) ">): " _XSTRING(_msg) "\n",\
            __FILE__, __FUNCTION__, __LINE__);        \
        exit(1);                        \
    }                                \
} while(0)


#define sys_die(syscall, msg) do {                                      \
        fprintf(stderr, "%s:%s:%d: <%s>\n\tperror=",                    \
                        __FILE__, __FUNCTION__, __LINE__, _XSTRING(msg));               \
        perror(_XSTRING(syscall));                                      \
        exit(1);                                                        \
} while(0)

#ifdef __linux__
#define panic(msg...) do {                        \
        fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, ##msg);                        \
        exit(1);                                                        \
} while(0)
#else
#define panic(msg, args...) do { 					\
    printk("PANIC:%s:%s:%d:" msg "\n", __FILE__, __FUNCTION__, __LINE__, ##args); \
    reboot();							\
} while(0)
#endif

#ifndef NDEBUG
#define debug(msg, args...) \
    (printf)("%s:%s:%d:" msg, __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define debug(msg, args...) do { } while(0)
#endif

#define note(msg...) do {                                              \
        fprintf(stderr, "%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, ##msg);                                            \
} while(0)

#define safe_sys(...) if((__VA_ARGS__) < 0) { sys_die(_XSTRING(__VA_ARGS__), failed); }

#define error(msg...) do { 						\
        fprintf(stderr, "%s:%s:%d:ERROR:", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, ##msg);						\
        exit(1);                                                        \
} while(0)

/* Compile-time assertion used in function. */
#define AssertNow(x) switch(1) { case (x): case 0: ; }

#define unimplemented() panic("implement this function!\n");

#ifndef __linux__
#define assert(bool) do { if((bool) == 0) panic(#bool); } while(0)
#if 0
#undef fprintf
#undef exit
#endif
#endif

#endif /* __DEMAND_H__ */