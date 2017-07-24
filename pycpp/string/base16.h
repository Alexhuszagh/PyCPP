//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Base16 encoding and decoding routines.
 */

#pragma once

#include <pycpp/config.h>
#include <string>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
  *\brief Encode buffer to base16. Returns number of bytes converted.
 */
size_t base16_encode(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
  *\brief Encode string to base16.
 */
std::string base16_encode(const std::string& str);

/**
  *\brief Decode buffer from base16. Returns number of bytes converted.
 */
size_t base16_decode(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
  *\brief Decode string from base16.
 */
std::string base16_decode(const std::string& str);

PYCPP_END_NAMESPACE
