//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Hexadecimal conversions.
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using hex_lowlevel_callback = function<void(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen)
>;

using hex_highlevel_callback = function<string(const string_wrapper&)>;

// FUNCTIONS
// ---------

/**
 *  \brief Get hex representation. Increments the src and dst buffers.
 *
 *  \param width            Size of the input in bytes.
 */
void hex(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t width = 1);

/**
 *  \brief Convert source string to hex representation.
 *
 *  \param width            Size of the input in bytes.
 */
string hex(const string_wrapper& str, size_t width = 1);

/**
 *  \brief Get hex representation of 8-bit signed type.
 */
void hex_i8(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Get hex representation of 8-bit signed type.
 */
string hex_i8(const string_wrapper& str);

/**
 *  \brief Get hex representation of 16-bit signed type.
 */
void hex_i16(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Get hex representation of 16-bit signed type.
 */
string hex_i16(const string_wrapper& str);

/**
 *  \brief Get hex representation of 32-bit signed type.
 */
void hex_i32(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Get hex representation of 32-bit signed type.
 */
string hex_i32(const string_wrapper& str);

/**
 *  \brief Get hex representation of 64-bit signed type.
 */
void hex_i64(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Get hex representation of 64-bit signed type.
 */
string hex_i64(const string_wrapper& str);

/**
 *  \brief Undo hex representation. Increments the src and dst buffers.
 *
 *  \param width            Size of the input in bytes.
 */
void unhex(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t width = 1);

/**
 *  \brief Convert source string to undo hex representation.
 *
 *  \param width            Size of the input in bytes.
 */
string unhex(const string_wrapper& str, size_t width = 1);

/**
 *  \brief Undo hex representation of 8-bit signed type.
 */
void unhex_i8(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Undo hex representation of 8-bit signed type.
 */
string unhex_i8(const string_wrapper& str);

/**
 *  \brief Undo hex representation of 16-bit signed type.
 */
void unhex_i16(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Undo hex representation of 16-bit signed type.
 */
string unhex_i16(const string_wrapper& str);

/**
 *  \brief Undo hex representation of 32-bit signed type.
 */
void unhex_i32(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Undo hex representation of 32-bit signed type.
 */
string unhex_i32(const string_wrapper& str);

/**
 *  \brief Undo hex representation of 64-bit signed type.
 */
void unhex_i64(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Undo hex representation of 64-bit signed type.
 */
string unhex_i64(const string_wrapper& str);

PYCPP_END_NAMESPACE
