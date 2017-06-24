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

/** \brief Get hex representation. Returns number of bytes converted.
 *
 *  \param width            Size of the input in bytes.
 *  \param is_signed        Input data is a signed type.
 */
size_t hex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width = 1, bool is_signed = true);

/** \brief Convert source string to hex representation.
 *
 *  \param width            Size of the input in bytes.
 *  \param is_signed        Input data is a signed type.
 */
std::string hex(const std::string& str, size_t width = 1, bool is_signed = true);
