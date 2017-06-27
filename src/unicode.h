//  :copyright: (c) Copyright 2001-2004 Unicode, Inc.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.
//  :copyright: (c) Copyright 2001-2011 Hartmut Kaiser.
//  :copyright: (c) Copyright 2001-2011 Joel de Guzman.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup funxx
 *  \brief Unicode code point conversions and detection.
 */

#pragma once

#include <string>

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
 *  \brief Check if string is likely not Unicode.
 */
bool is_ascii(const std::string &str);

/**
 *  \brief Check if string is likely Unicode.
 *
 *  Check if any contents of the string are above '0x7f', the last
 *  non-Unicode identifier in ASCII,  or null. If any characters are,
 *  the data is likely Unicode, if not, the data is almost certainly
 *  ASCII.
 */
bool is_unicode(const std::string &str);

// CONVERSIONS

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
