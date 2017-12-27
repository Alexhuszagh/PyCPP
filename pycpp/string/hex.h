//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hexadecimal conversions.
 *
 *  Convert byte arrays to hexadecimal representations of a desired
 *  width (in bytes). Convenience functions for 8-64 bit width hex
 *  conversions are provided for both hex encoding and decoding.
 *
 *  \synopsis
 *      using hex_lowlevel_callback = void(*)(
 *          const void*& src, size_t srclen,
 *          void*& dst, size_t dstlen,
 *          const byte_allocator& allocator
 *      );
 *
 *      using hex_highlevel_callback = string(*)(
 *          const string_wrapper&, const byte_allocator&
 *      );
 *
 *      void hex(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          size_t width = 1,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string hex(const string_wrapper& str,
 *          size_t width = 1,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void hex_i8(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string hex_i8(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void hex_i16(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string hex_i16(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void hex_i32(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string hex_i32(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void hex_i64(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string hex_i64(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void unhex(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          size_t width = 1,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string unhex(const string_wrapper& str,
 *          size_t width = 1,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void unhex_i8(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string unhex_i8(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void unhex_i16(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string unhex_i16(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void unhex_i32(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string unhex_i32(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void unhex_i64(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string unhex_i64(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using hex_lowlevel_callback = void(*)(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    const byte_allocator& allocator
);

using hex_highlevel_callback = string(*)(
    const string_wrapper&, const byte_allocator&
);

// FUNCTIONS
// ---------

/**
 *  \brief Get hex representation. Increments the src and dst buffers.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param width            Size of the input in bytes.
 *  \param allocator        Allocator for internal allocations.
 */
void hex(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    size_t width = 1,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert source string to hex representation.
 *
 *  \param width            Size of the input in bytes.
 */
string hex(const string_wrapper& str,
    size_t width = 1,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Get hex representation of 8-bit signed type.
 */
void hex_i8(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Get hex representation of 8-bit signed type.
 */
string hex_i8(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Get hex representation of 16-bit signed type.
 */
void hex_i16(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Get hex representation of 16-bit signed type.
 */
string hex_i16(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Get hex representation of 32-bit signed type.
 */
void hex_i32(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Get hex representation of 32-bit signed type.
 */
string hex_i32(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Get hex representation of 64-bit signed type.
 */
void hex_i64(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Get hex representation of 64-bit signed type.
 */
string hex_i64(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation. Increments the src and dst buffers.
 *
 *  \param width            Size of the input in bytes.
 */
void unhex(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    size_t width = 1,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert source string to undo hex representation.
 *
 *  \param width            Size of the input in bytes.
 */
string unhex(const string_wrapper& str,
    size_t width = 1,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation of 8-bit signed type.
 */
void unhex_i8(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation of 8-bit signed type.
 */
string unhex_i8(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation of 16-bit signed type.
 */
void unhex_i16(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation of 16-bit signed type.
 */
string unhex_i16(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation of 32-bit signed type.
 */
void unhex_i32(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation of 32-bit signed type.
 */
string unhex_i32(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation of 64-bit signed type.
 */
void unhex_i64(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Undo hex representation of 64-bit signed type.
 */
string unhex_i64(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

PYCPP_END_NAMESPACE
