//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Codecs to convert between string types.
 *
 *  Convert between single-byte and multi-byte Unicode code points,
 *  using a high-level interface. Each allocator controls internal
 *  allocations necessary for the conversion, and dictates the memory
 *  allocation properties of the output string.
 *
 *  \synopsis
 *      u16string codec_utf8_utf16(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u32string codec_utf8_utf32(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string codec_utf16_utf8(const u16string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u32string codec_utf16_utf32(const u16string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string codec_utf32_utf8(const u32string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u16string codec_utf32_utf16(const u32string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 */

#pragma once

#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

// FUNCTIONS
// ---------

/**
 *  \brief Convert UTF-8 string to UTF-16.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u16string codec_utf8_utf16(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-8 string to UTF-32.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u32string codec_utf8_utf32(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-16 string to UTF-8.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string codec_utf16_utf8(const u16string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-16 string to UTF-32.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u32string codec_utf16_utf32(const u16string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-32 string to UTF-8.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string codec_utf32_utf8(const u32string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-32 string to UTF-16.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u16string codec_utf32_utf16(const u32string_view& str,
    const byte_allocator& allocator = byte_allocator());

PYCPP_END_NAMESPACE
