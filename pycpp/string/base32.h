//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Base32 encoding and decoding routines.
 *
 *  \synopsis
 *      size_t base32_encode(const void* src,
 *          size_t srclen,
 *          void* dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator()) noexcept;
 *
 *      string base32_encode(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      size_t base32_decode(const void* src,
 *          size_t srclen,
 *          void* dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator()) noexcept;
 *
 *      string base32_decode(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 */

#pragma once

#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Encode buffer to base32. Returns number of bytes converted.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
size_t base32_encode(const void* src,
    size_t srclen,
    void* dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator()) noexcept;

/**
 *  \brief Encode string to base32.
 *
 *  \param str              Source string to encode.
 *  \param allocator        Allocator for output string.
 */
string base32_encode(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Decode buffer from base32. Returns number of bytes converted.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
size_t base32_decode(const void* src,
    size_t srclen,
    void* dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator()) noexcept;

/**
 *  \brief Decode string from base32.
 *
 *  \param str              Source string to encode.
 *  \param allocator        Allocator for output string.
 */
string base32_decode(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

PYCPP_END_NAMESPACE
