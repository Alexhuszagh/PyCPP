//  :copyright: (c) Copyright 2001-2004 Unicode, Inc.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.
//  :copyright: (c) Copyright 2001-2011 Hartmut Kaiser.
//  :copyright: (c) Copyright 2001-2011 Joel de Guzman.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Unicode code point conversions and detection.
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using unicode_lowlevel_callback = std::function<void(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen)
>;

using unicode_highlevel_callback = std::function<std::string(const string_wrapper&)>;

// FUNCTIONS
// ---------

// CHECKERS

/**
 *  \brief Check if a character is ASCII.
 */
bool is_ascii(uint32_t c);

/**
 *  \brief Check if a character is a valid Unicode.
 */
bool is_unicode(uint32_t c);

/**
 *  \brief Check if a character is letter or number.
 */
bool is_alnum(uint32_t c);

/**
 *  \brief Check if a character is letter.
 */
bool is_alpha(uint32_t c);

/**
 *  \brief Check if a character is number.
 */
bool is_digit(uint32_t c);

/**
 *  \brief Check if a character is lowercase.
 */
bool is_lower(uint32_t c);

/**
 *  \brief Check if a character is uppercase.
 */
bool is_upper(uint32_t c);

/**
 *  \brief Check if a character is a space.
 */
bool is_space(uint32_t c);

/**
 *  \brief Check if a character is blank.
 */
bool is_blank(uint32_t c);

/**
 *  \brief Check if a character is punctuation.
 */
bool is_punctuation(uint32_t c);

/**
 *  \brief Check if a byte is an ASCII byte.
 */
bool is_ascii_byte(uint8_t c);

/**
 *  \brief Check if a byte is a UTF8 start byte.
 */
bool is_start_byte(uint8_t c);

/**
 *  \brief Check if a byte is a UTF8 continuation byte.
 */
bool is_continuation_byte(uint8_t c);

/**
 *  \brief Check if string is likely not Unicode.
 */
bool is_ascii(const string_wrapper& str);

/**
 *  \brief Check if string is likely Unicode.
 *
 *  Check if any contents of the string are above '0x7f', the last
 *  non-Unicode identifier in ASCII,  or null. If any characters are,
 *  the data is likely Unicode, if not, the data is almost certainly
 *  ASCII.
 */
bool is_unicode(const string_wrapper& str);

// CONVERSIONS

/**
 *  \brief Convert UTF-8 to UTF-16. Returns number of bytes converted.
 */
void utf8_to_utf16(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF-8 string to UTF-16.
 */
std::string utf8_to_utf16(const string_wrapper& str);

/**
 *  \brief Convert UTF-8 to UTF-32. Returns number of bytes converted.
 */
void utf8_to_utf32(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF-8 string to UTF-32.
 */
std::string utf8_to_utf32(const string_wrapper& str);

/**
 *  \brief Convert UTF-16 to UTF-8. Returns number of bytes converted.
 */
void utf16_to_utf8(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF-16 string to UTF-8.
 */
std::string utf16_to_utf8(const string_wrapper& str);

/**
 *  \brief Convert UTF-16 to UTF-32. Returns number of bytes converted.
 */
void utf16_to_utf32(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF-16 string to UTF-32.
 */
std::string utf16_to_utf32(const string_wrapper& str);

/**
 *  \brief Convert UTF-32 to UTF-8. Returns number of bytes converted.
 */
void utf32_to_utf8(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF-32 string to UTF-8.
 */
std::string utf32_to_utf8(const string_wrapper& str);

/**
 *  \brief Convert UTF-32 to UTF-16. Returns number of bytes converted.
 */
void utf32_to_utf16(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF-32 string to UTF-16.
 */
std::string utf32_to_utf16(const string_wrapper& str);

PYCPP_END_NAMESPACE
