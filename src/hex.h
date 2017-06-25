//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.
/**
 *  \addtogroup crosscxx
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
 *  \param is_signed        Input data is a signed type.
 */
size_t hex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width = 1, bool is_signed = true);

/**
 *  \brief Convert source string to hex representation.
 *
 *  \param width            Size of the input in bytes.
 *  \param is_signed        Input data is a signed type.
 */
std::string hex(const std::string& str, size_t width = 1, bool is_signed = true);

/**
 *  \brief Get hex representation of 8-bit signed type.
 */
size_t hex_int8(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 8-bit signed type.
 */
std::string hex_int8(const std::string& str);

/**
 *  \brief Get hex representation of 8-bit unsigned type.
 */
size_t hex_uint8(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 8-bit unsigned type.
 */
std::string hex_uint8(const std::string& str);

/**
 *  \brief Get hex representation of 16-bit signed type.
 */
size_t hex_int16(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 16-bit signed type.
 */
std::string hex_int16(const std::string& str);

/**
 *  \brief Get hex representation of 16-bit unsigned type.
 */
size_t hex_uint16(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 16-bit unsigned type.
 */
std::string hex_uint16(const std::string& str);

/**
 *  \brief Get hex representation of 32-bit signed type.
 */
size_t hex_int32(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 32-bit signed type.
 */
std::string hex_int32(const std::string& str);

/**
 *  \brief Get hex representation of 32-bit unsigned type.
 */
size_t hex_uint32(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 32-bit unsigned type.
 */
std::string hex_uint32(const std::string& str);

/**
 *  \brief Get hex representation of 64-bit signed type.
 */
size_t hex_int64(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 64-bit signed type.
 */
std::string hex_int64(const std::string& str);

/**
 *  \brief Get hex representation of 64-bit unsigned type.
 */
size_t hex_uint64(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
 *  \brief Get hex representation of 64-bit unsigned type.
 */
std::string hex_uint64(const std::string& str);

/**
 *  \brief Undo hex representation. Returns number of bytes converted.
 *
 *  \param width            Size of the input in bytes.
 *  \param is_signed        Input data is a signed type.
 */
size_t unhex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width = 1, bool is_signed = true);

/**
 *  \brief Convert source string to undo hex representation.
 *
 *  \param width            Size of the input in bytes.
 *  \param is_signed        Input data is a signed type.
 */
std::string unhex(const std::string& str, size_t width = 1, bool is_signed = true);
