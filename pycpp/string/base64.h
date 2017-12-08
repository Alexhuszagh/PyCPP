//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Base64 encoding and decoding routines.
 */

#pragma once

#include <pycpp/config.h>
#include <pycpp/view/string.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
  *\brief Encode buffer to base64. Returns number of bytes converted.
 */
size_t base64_encode(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
  *\brief Encode string to base64.
 */
std::string base64_encode(const string_view& str);

/**
  *\brief Decode buffer from base64. Returns number of bytes converted.
 */
size_t base64_decode(const void* src, size_t srclen, void* dst, size_t dstlen);

/**
  *\brief Decode string from base64.
 */
std::string base64_decode(const string_view& str);

PYCPP_END_NAMESPACE
