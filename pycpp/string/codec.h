//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Codecs to convert between string types.
 */

#pragma once

#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Convert UTF-8 string to UTF-16.
 */
u16string codec_utf8_utf16(const string_view& str);

/**
 *  \brief Convert UTF-8 string to UTF-32.
 */
u32string codec_utf8_utf32(const string_view& str);

/**
 *  \brief Convert UTF-16 string to UTF-8.
 */
string codec_utf16_utf8(const u16string_view& str);

/**
 *  \brief Convert UTF-16 string to UTF-32.
 */
u32string codec_utf16_utf32(const u16string_view& str);

/**
 *  \brief Convert UTF-32 string to UTF-8.
 */
string codec_utf32_utf8(const u32string_view& str);

/**
 *  \brief Convert UTF-32 string to UTF-16.
 */
u16string codec_utf32_utf16(const u32string_view& str);

PYCPP_END_NAMESPACE
