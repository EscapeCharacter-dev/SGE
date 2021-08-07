#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
typedef signed int          bool_t;
typedef unsigned int        uint_t;
typedef void                *handle_t;
#define TRUE                1
#define FALSE               0
#ifdef NULL
#undef NULL
#endif
#define NULL                0

#endif