//  :copyright: (c) 2014-2016 Mathias Panzenböck.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Public Domain/MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief System endianess detection macros and byteswap routines.
 */

#pragma once

// INTEGERS
// --------

#if _MSC_VER
/* MSVC */
#   define bswap16 _byteswap_ushort
#   define bswap32 _byteswap_ulong
#   define bswap64 _byteswap_uint64
#elif defined(__clang__) || defined(__GNUC__)
/* Clang, GCC */
#   define bswap16 __builtin_bswap16
#   define bswap32 __builtin_bswap32
#   define bswap64 __builtin_bswap64
#else
#   define NEED_BSWAPXX
#endif

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__WINDOWS__)

#   define __WINDOWS__

#endif

#if defined(__linux__) || defined(__CYGWIN__)

#   include <endian.h>

#elif defined(__APPLE__)

#   include <libkern/OSByteOrder.h>

#   define htobe16(x) OSSwapHostToBigInt16(x)
#   define htole16(x) OSSwapHostToLittleInt16(x)
#   define be16toh(x) OSSwapBigToHostInt16(x)
#   define le16toh(x) OSSwapLittleToHostInt16(x)

#   define htobe32(x) OSSwapHostToBigInt32(x)
#   define htole32(x) OSSwapHostToLittleInt32(x)
#   define be32toh(x) OSSwapBigToHostInt32(x)
#   define le32toh(x) OSSwapLittleToHostInt32(x)

#   define htobe64(x) OSSwapHostToBigInt64(x)
#   define htole64(x) OSSwapHostToLittleInt64(x)
#   define be64toh(x) OSSwapBigToHostInt64(x)
#   define le64toh(x) OSSwapLittleToHostInt64(x)

#   define __BYTE_ORDER    BYTE_ORDER
#   define __BIG_ENDIAN    BIG_ENDIAN
#   define __LITTLE_ENDIAN LITTLE_ENDIAN
#   define __PDP_ENDIAN    PDP_ENDIAN

#elif defined(__OpenBSD__)

#   include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#   include <sys/endian.h>

#   define be16toh(x) betoh16(x)
#   define le16toh(x) letoh16(x)

#   define be32toh(x) betoh32(x)
#   define le32toh(x) letoh32(x)

#   define be64toh(x) betoh64(x)
#   define le64toh(x) letoh64(x)

#elif defined(__WINDOWS__)

#   include <winsock2.h>
#   include <windows.h>
#   include <ws2tcpip.h>

#   ifndef BIG_ENDIAN
        /* Windows does not always set byte order, such as MSVC */
#       define LITTLE_ENDIAN REG_DWORD_LITTLE_ENDIAN
#       define BIG_ENDIAN REG_DWORD_BIG_ENDIAN
#       if REG_DWORD == REG_DWORD_BIG_ENDIAN
#           define BYTE_ORDER BIG_ENDIAN
#       else
#           define BYTE_ORDER LITTLE_ENDIAN
#       endif

#   endif

#   if BYTE_ORDER == LITTLE_ENDIAN

#       define htobe16(x) bswap16(x)
#       define htole16(x) (x)
#       define be16toh(x) bswap16(x)
#       define le16toh(x) (x)

#       define htobe32(x) bswap32(x)
#       define htole32(x) (x)
#       define be32toh(x) bswap32(x)
#       define le32toh(x) (x)

#       define htobe64(x) bswap64(x)
#       define htole64(x) (x)
#       define be64toh(x) bswap64(x)
#       define le64toh(x) (x)

#   elif BYTE_ORDER == BIG_ENDIAN

        /* that would be xbox 360 */
#       define htobe16(x) (x)
#       define htole16(x) bswap16(x)
#       define be16toh(x) (x)
#       define le16toh(x) bswap16(x)

#       define htobe32(x) (x)
#       define htole32(x) bswap32(x)
#       define be32toh(x) (x)
#       define le32toh(x) bswap32(x)

#       define htobe64(x) (x)
#       define htole64(x) bswap64(x)
#       define be64toh(x) (x)
#       define le64toh(x) bswap64(x)

#   else

#       error "Byte order not supported."

#   endif

#   define __BYTE_ORDER    BYTE_ORDER
#   define __BIG_ENDIAN    BIG_ENDIAN
#   define __LITTLE_ENDIAN LITTLE_ENDIAN
#   define __PDP_ENDIAN    PDP_ENDIAN

#else

#   error "Platform not supported."

#endif

// FLOATS
// ------

#ifndef __FLOAT_WORD_ORDER
    /* Nearly all systems use little endian floats */
    #define __FLOAT_WORD_ORDER  LITTLE_ENDIAN
#endif

#ifndef FLOAT_WORD_ORDER
    #define FLOAT_WORD_ORDER    __FLOAT_WORD_ORDER
#endif

// FUNCTIONS
// ---------

#include <stdlib.h>

/**
 *  \brief Swap bytes in-place of type sizeof(T) == width.
 */
void bswap(void* buf, int width);

/**
 *  \brief Swap bytes from src into dst of type sizeof(T) == width.
 */
void bswap(void* dst, void* src, int width);

/**
 *  \brief memcpy() with byteswap for each 16-bit type.
 */
void memcpy_bswap16(void* dst, void* src, size_t bytes);

/**
 *  \brief memcpy() with byteswap for each 32-bit type.
 */
void memcpy_bswap32(void* dst, void* src, size_t bytes);

/**
 *  \brief memcpy() with byteswap for each 64-bit type.
 */
void memcpy_bswap64(void* dst, void* src, size_t bytes);

/**
 *  \brief memcpy() with byteswap for type sizeof(T) == width.
 */
void memcpy_bswap(void* dst, void* src, size_t bytes, int width);


#if BYTE_ORDER == LITTLE_ENDIAN

#   define htobe(buf, i) bswap(buf, i)
#   define htole(buf, i)
#   define betoh(buf, i) bswap(buf, i)
#   define letoh(buf, i)

#   define memcpy_htobe16(dst, src, n) memcpy_bswap16(dst, src, n)
#   define memcpy_htole16(dst, src, n) memcpy(dst, src, n)
#   define memcpy_be16toh(dst, src, n) memcpy_bswap16(dst, src, n)
#   define memcpy_le16toh(dst, src, n) memcpy(dst, src, n)

#   define memcpy_htobe32(dst, src, n) memcpy_bswap32(dst, src, n)
#   define memcpy_htole32(dst, src, n) memcpy(dst, src, n)
#   define memcpy_be32toh(dst, src, n) memcpy_bswap32(dst, src, n)
#   define memcpy_le32toh(dst, src, n) memcpy(dst, src, n)

#   define memcpy_htobe64(dst, src, n) memcpy_bswap64(dst, src, n)
#   define memcpy_htole64(dst, src, n) memcpy(dst, src, n)
#   define memcpy_be64toh(dst, src, n) memcpy_bswap64(dst, src, n)
#   define memcpy_le64toh(dst, src, n) memcpy(dst, src, n)

#   define memcpy_htobe(dst, src, n, i) memcpy_bswap(dst, src, n, i)
#   define memcpy_htole(dst, src, n, i) memcpy(dst, src, n)
#   define memcpy_betoh(dst, src, n, i) memcpy_bswap(dst, src, n, i)
#   define memcpy_letoh(dst, src, n, i) memcpy(dst, src, n)

#elif BYTE_ORDER == BIG_ENDIAN

#   define htobe(buf, i)
#   define htole(buf, i) bswap(buf, i)
#   define betoh(buf, i)
#   define letoh(buf, i) bswap(buf, i)

#   define memcpy_htobe16(dst, src, n) memcpy(dst, src, n)
#   define memcpy_htole16(dst, src, n) memcpy_bswap16(dst, src, n)
#   define memcpy_be16toh(dst, src, n) memcpy(dst, src, n)
#   define memcpy_le16toh(dst, src, n) memcpy_bswap16(dst, src, n)

#   define memcpy_htobe32(dst, src, n) memcpy(dst, src, n)
#   define memcpy_htole32(dst, src, n) memcpy_bswap32(dst, src, n)
#   define memcpy_be32toh(dst, src, n) memcpy(dst, src, n)
#   define memcpy_le32toh(dst, src, n) memcpy_bswap32(dst, src, n)

#   define memcpy_htobe64(dst, src, n) memcpy(dst, src, n)
#   define memcpy_htole64(dst, src, n) memcpy_bswap64(dst, src, n)
#   define memcpy_be64toh(dst, src, n) memcpy(dst, src, n)
#   define memcpy_le64toh(dst, src, n) memcpy_bswap64(dst, src, n)

#   define memcpy_htobe(dst, src, n, i) memcpy(dst, src, n)
#   define memcpy_htole(dst, src, n, i) memcpy_bswap(dst, src, n, i)
#   define memcpy_betoh(dst, src, n, i) memcpy(dst, src, n)
#   define memcpy_letoh(dst, src, n, i) memcpy_bswap(dst, src, n, i)

#else

#   error "Byte order not supported."

#endif


#if defined(NEED_BSWAPXX)
#   include <stdint.h>

uint16_t bswap16(uint16_t i);
uint32_t bswap32(uint32_t i);
uint64_t bswap64(uint64_t i);

#endif
