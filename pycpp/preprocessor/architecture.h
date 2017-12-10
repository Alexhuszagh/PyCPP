// :copyright: (c) 2012 Petroules Corporation.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief System architecture detection macros.
 */

#pragma once

#include <cstdint>
#include <climits>

// WINDOWS
// -------

#if _WIN32 || _WIN64
#   if _WIN128
#       define SYSTEM_ARCHITECTURE 128
#   elif _WIN64
#       define SYSTEM_ARCHITECTURE 64
#   elif _WIN32
#       define SYSTEM_ARCHITECTURE 32
#   elif _WIN16
#       define SYSTEM_ARCHITECTURE 16
#   endif
#endif

// GCC
// ---

#if __GNUC__
#   if defined(__x86_64__) || defined(__ppc64__) || defined(__amd64__) || defined(__LP64__) || defined(_M_X64) || defined(__ia64) || defined(_M_IA64) || defined(__aarch64__) || defined(__powerpc64__)
#       define SYSTEM_ARCHITECTURE 64
#   elif defined(__i386__) || defined(_M_IX86) || defined(_M_PPC) || defined(__LP32__) || defined(__POWERPC__)
#       define SYSTEM_ARCHITECTURE 32
#   else
#       define SYSTEM_ARCHITECTURE 32
#   endif
#endif

// OTHER
// -----

// Use uintptr_trather than size_t, since we might be on a segmented
// architecture. Go smallest-to-largest, since we don't want
// to check values too large for type.
#ifndef SYSTEM_ARCHITECTURE
#   if UINTPTR_MAX == 0xffff
#       define SYSTEM_ARCHITECTURE 16
#   elif UINTPTR_MAX == 0xffffffff
#       define SYSTEM_ARCHITECTURE 32
#   elif UINTPTR_MAX == 0xffffffffffffffff
#       define SYSTEM_ARCHITECTURE 64
#   elif UINTPTR_MAX == 0xffffffffffffffffffffffffffffffff
#       define SYSTEM_ARCHITECTURE 128
#   else
#       error "Unknown system architecture."
#   endif
#endif

// MEMORY ARCHITECTURE
// -------------------

// We may be using multiple memory segments, so `sizeof(size_t) <
// sizeof(uintptr_t)`. For example, a 16-bit integer may the max
// object size, but there may be multiple 16-bit memory segments,
// requiring 32-bit pointers. Go smallest-to-largest, since we don't
// want to check values too large for type.
#ifndef MEMORY_ARCHITECTURE
#   if SIZE_MAX == 0xffff
#       define MEMORY_ARCHITECTURE 16
#   elif SIZE_MAX == 0xffffffff
#       define MEMORY_ARCHITECTURE 32
#   elif SIZE_MAX == 0xffffffffffffffff
#       define MEMORY_ARCHITECTURE 64
#   elif SIZE_MAX == 0xffffffffffffffffffffffffffffffff
#       define MEMORY_ARCHITECTURE 128
#   else
#       error "Unknown memory architecture."
#   endif
#endif

// ALIGNMENT
// ---------

#define IS_ALIGNED_16(p)    (0 == (((uintptr_t) p) & 1))
#define IS_ALIGNED_32(p)    (0 == (((uintptr_t) p) & 3))
#define IS_ALIGNED_64(p)    (0 == (((uintptr_t) p) & 7))
#define IS_ALIGNED_128(p)   (0 == (((uintptr_t) p) & 15))
