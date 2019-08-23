#ifndef DEFINTIONS_H
#define DEFINTIONS_H

#include "Arduino.h"

#define LC_DEBUG_ALL

enum class DebugLevel: uint8_t{
    none,
    important,
    useful,
    devt1,
    devt2
};

#define LC_PORT_DEBUG Serial

#define LC_ASSERT_ALL
#ifdef LC_ASSERT_ALL
    // choose one only    
    #define LC_ASSERT_STRICT // program stops when assertions fail
    #ifndef LC_ASSERT_STRICT
    #define LC_ASSERT_RELAXED // when assertions fail, the offending condition is 'neutralized' and program continues
    #endif
#endif

#ifdef LC_ASSERT_STRICT
static void assert_custom(const char *__file, int __lineno, const char *__sexp ) {
    // transmit diagnostic information through serial link. 
    LC_PORT_DEBUG.println(F("Fatal error!"));
    LC_PORT_DEBUG.print(F("  File: ")); LC_PORT_DEBUG.println(__file);
    LC_PORT_DEBUG.print(F("  Line no.: ")); LC_PORT_DEBUG.println(__lineno, DEC);
    LC_PORT_DEBUG.print(F("  Test condition: ")); LC_PORT_DEBUG.println(__sexp);
    LC_PORT_DEBUG.println(F("Program aborted!"));
    LC_PORT_DEBUG.flush();
    // abort program execution.
    abort();
}

#define lcAssert( a )  if (!(a)) { assert_custom(__FILE__, __LINE__,  #a); } 
#endif

#define INTERVAL_PROCESS_DIGITALOUTPUTS 50

#endif