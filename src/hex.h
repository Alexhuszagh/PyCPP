//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Hexadecimal conversions.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------

/**
 *  \brief Get hex representation. Returns number of bytes converted.
 *
 *  \param width            Size of the input in bytes.
 */
size_t hex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width = 1);

/**
 *  \brief Convert source string to hex representation.
 *
 *  \param width            Size of the input in bytes.
 */
std::string hex(const std::string& str, size_t width = 1);

/**
 *  \brief Get hex representation of 8-bit signed type.
 */
size_t hex_i8(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 8-bit signed type.
 */
std::string hex_i8(const std::string& str);

/**
 *  \brief Get hex representation of 16-bit signed type.
 */
size_t hex_i16(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 16-bit signed type.
 */
std::string hex_i16(const std::string& str);

/**
 *  \brief Get hex representation of 32-bit signed type.
 */
size_t hex_i32(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 32-bit signed type.
 */
std::string hex_i32(const std::string& str);

/**
 *  \brief Get hex representation of 64-bit signed type.
 */
size_t hex_i64(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 64-bit signed type.
 */
std::string hex_i64(const std::string& str);

/**
 *  \brief Undo hex representation. Returns number of bytes converted.
 *
 *  \param width            Size of the input in bytes.
 */
size_t unhex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width = 1);

/**
 *  \brief Convert source string to undo hex representation.
 *
 *  \param width            Size of the input in bytes.
 */
std::string unhex(const std::string& str, size_t width = 1);

/**
 *  \brief Undo hex representation of 8-bit signed type.
 */
size_t unhex_i8(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Undo hex representation of 8-bit signed type.
 */
std::string unhex_i8(const std::string& str);

/**
 *  \brief Undo hex representation of 16-bit signed type.
 */
size_t unhex_i16(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Undo hex representation of 16-bit signed type.
 */
std::string unhex_i16(const std::string& str);

/**
 *  \brief Undo hex representation of 32-bit signed type.
 */
size_t unhex_i32(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Undo hex representation of 32-bit signed type.
 */
std::string unhex_i32(const std::string& str);

/**
 *  \brief Undo hex representation of 64-bit signed type.
 */
size_t unhex_i64(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Undo hex representation of 64-bit signed type.
 */
std::string unhex_i64(const std::string& str);
