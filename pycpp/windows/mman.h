//  :copyright: (c) 2012-2017 Steven Lee.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \brief `sys/mman.h` implementation for MSVC/MinGW.
 *
 *  Adapted from https://github.com/witwall/mman-win32
 */

#pragma once

// MACROS
// ------

// Force Windows XP or later features.
#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501
#endif


#include <stdint.h>
#include <sys/types.h>
#ifndef _MSC_VER
#   include <_mingw.h>
#endif

// ALIASES
// -------

#if defined(_WIN64)         // WIN64
    using offset_t = int64_t;
#else                       // WIN32
    using offset_t = uint32_t;
#endif                      // WIN64

// CONSTANTS
// ---------

#define PROT_NONE       0
#define PROT_READ       1
#define PROT_WRITE      2
#define PROT_EXEC       4

#define MAP_FILE        0
#define MAP_SHARED      1
#define MAP_PRIVATE     2
#define MAP_TYPE        0xf
#define MAP_FIXED       0x10
#define MAP_ANONYMOUS   0x20
#define MAP_ANON        MAP_ANONYMOUS

#define MAP_FAILED      ((void*)-1)

/* Flags for msync. */
#define MS_ASYNC        1
#define MS_SYNC         2
#define MS_INVALIDATE   4

// FUNCTIONS
// ---------

void* mmap(void* addr, size_t len, int prot, int flags, int fildes, offset_t off);
int munmap(void* addr, size_t len);
int mprotect(void* addr, size_t len, int prot);
int msync(void* addr, size_t len, int flags);
int mlock(const void* addr, size_t len);
int munlock(const void* addr, size_t len);
