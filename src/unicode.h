//  :copyright: (c) Copyright 2001-2004 Unicode, Inc.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Unicode code point conversions.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------


/**
 *  \brief Check if string is likely Unicode.
 *
 *  Check if any contents of the string are above '0x7f', the last
 *  non-Unicode identifier in ASCII. If any characters are, the
 *  data is likely Unicode, if not, the data is almost certainly
 *  ASCII.
 */
bool is_unicode(const std::string &string);

/** \brief Convert UTF-8 to UTF-16. Returns number of bytes converted.
 */
size_t utf8_to_utf16(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF-8 string to UTF-16.
 */
std::string utf8_to_utf16(const std::string& str);

/** \brief Convert UTF-8 to UTF-32. Returns number of bytes converted.
 */
size_t utf8_to_utf32(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF-8 string to UTF-32.
 */
std::string utf8_to_utf32(const std::string& str);

/** \brief Convert UTF-16 to UTF-8. Returns number of bytes converted.
 */
size_t utf16_to_utf8(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF-16 string to UTF-8.
 */
std::string utf16_to_utf8(const std::string& str);

/** \brief Convert UTF-16 to UTF-32. Returns number of bytes converted.
 */
size_t utf16_to_utf32(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF-16 string to UTF-32.
 */
std::string utf16_to_utf32(const std::string& str);

/** \brief Convert UTF-32 to UTF-8. Returns number of bytes converted.
 */
size_t utf32_to_utf8(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF-32 string to UTF-8.
 */
std::string utf32_to_utf8(const std::string& str);

/** \brief Convert UTF-32 to UTF-16. Returns number of bytes converted.
 */
size_t utf32_to_utf16(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF-32 string to UTF-16.
 */
std::string utf32_to_utf16(const std::string& str);
