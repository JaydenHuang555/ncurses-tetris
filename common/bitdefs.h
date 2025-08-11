#ifndef COMMON_H
#define COMMON_H

#include "stddef.h"

typedef void u0;
typedef void s0;

typedef char s8;
typedef unsigned char u8;

typedef char byte;
typedef unsigned char ubyte;

typedef short s16;
typedef unsigned short u16;

typedef int s32;
typedef unsigned int u32;

typedef long s64;
typedef unsigned long u64;

typedef float f32;
typedef double f64;

#ifndef bool
#define bool u8
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef NULL
#define NULL 0
#endif

#endif
