#ifndef DEFINTIONS_H
#define DEFINTIONS_H

#include "Arduino.h"

#define DEBUG_ALL

enum class DebugLevel: uint8_t{
    none,
    important,
    useful,
    devt1,
    devt2
};

#endif