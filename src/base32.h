//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Base32 encoding and decoding routines.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------

/**
  *\brief Encode buffer to base32. Returns number of bytes converted.
 */
size_t base32_encode(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
  *\brief Encode string to base32.
 */
std::string base32_encode(const std::string& str);

/**
  *\brief Decode buffer from base32. Returns number of bytes converted.
 */
size_t base32_decode(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
  *\brief Decode string from base32.
 */
std::string base32_decode(const std::string& str);
