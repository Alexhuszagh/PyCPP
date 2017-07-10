//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Codecs to convert between string types.
 */

#pragma once

#include <pycpp/config.h>
#include <string>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/** \brief Convert UTF-8 string to UTF-16.
 */
std::u16string codec_utf8_utf16(const std::string& str);

/** \brief Convert UTF-8 string to UTF-32.
 */
std::u32string codec_utf8_utf32(const std::string& str);

/** \brief Convert UTF-16 string to UTF-8.
 */
std::string codec_utf16_utf8(const std::u16string& str);

/** \brief Convert UTF-16 string to UTF-32.
 */
std::u32string codec_utf16_utf32(const std::u16string& str);

/** \brief Convert UTF-32 string to UTF-8.
 */
std::string codec_utf32_utf8(const std::u32string& str);

/** \brief Convert UTF-32 string to UTF-16.
 */
std::u16string codec_utf32_utf16(const std::u32string& str);

PYCPP_END_NAMESPACE
