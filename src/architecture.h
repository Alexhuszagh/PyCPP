// :copyright: (c) 2012 Petroules Corporation.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief System architecture detection macros.
 */

#pragma once

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

#ifndef SYSTEM_ARCHITECTURE
#   if (size_t)-1 == 0x0xffffffffffffffffffffffffffffffff
#       define SYSTEM_ARCHITECTURE 128
#   elif (size_t)-1 == 0xffffffffffffffff
#       define SYSTEM_ARCHITECTURE 64
#   elif (size_t)-1 == 0xffffffff
#       define SYSTEM_ARCHITECTURE 32
#   elif (size_t)-1 == 0xffff
#       define SYSTEM_ARCHITECTURE 16
#   else
#       error Unknown system architecture.
#   endif
#endif
