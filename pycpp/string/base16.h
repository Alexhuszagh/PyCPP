//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Base16 encoding and decoding routines.
 *
 *  \synopsis
 *      void base16_encode(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator()) noexcept;
 *
 *      string base16_encode(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void base16_decode(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator()) noexcept;
 *
 *      string base16_decode(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 */

#pragma once

#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
  *\brief Encode buffer to base16.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void base16_encode(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator()) noexcept;

/**
  *\brief Encode string to base16.
 *
 *  \param str              Source string to encode.
 *  \param allocator        Allocator for output string.
 */
string base16_encode(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
  *\brief Decode buffer from base16.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void base16_decode(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator()) noexcept;

/**
  *\brief Decode string from base16.
 *
 *  \param str              Source string to encode.
 *  \param allocator        Allocator for output string.
 */
string base16_decode(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

PYCPP_END_NAMESPACE
