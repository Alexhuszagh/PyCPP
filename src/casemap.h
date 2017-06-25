//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup crosscxx
 *  \brief Unicode-aware case conversion routines.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------

// ASCII

/** \brief Convert ASCII string to lower-case. Returns number of bytes converted.
 */
size_t ascii_tolower(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert ASCII string to lower-case.
 */
std::string ascii_tolower(const std::string &str);

/** \brief Convert ASCII string to upper-case. Returns number of bytes converted.
 */
size_t ascii_toupper(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert ASCII string to upper-case.
 */
std::string ascii_toupper(const std::string &str);

/** \brief Convert ASCII string to title-case. Returns number of bytes converted.
 */
//size_t ascii_totitle(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert ASCII string to title-case.
 */
// TODO: need to tokenize whitespace/characters...
//std::string ascii_totitle(const std::string &str);

// UTF8

/** \brief Convert UTF8 string to lower-case. Returns number of bytes converted.
 */
size_t utf8_tolower(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF8 string to lower-case.
 */
std::string utf8_tolower(const std::string &str);

/** \brief Convert UTF8 string to upper-case. Returns number of bytes converted.
 */
size_t utf8_toupper(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF8 string to upper-case.
 */
std::string utf8_toupper(const std::string &str);

/** \brief Convert UTF8 string to title-case. Returns number of bytes converted.
 */
//size_t utf8_totitle(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF8 string to title-case.
 */
// TODO: need to tokenize whitespace/characters...
//std::string utf8_totitle(const std::string &str);

// UTF16


// UTF32

/** \brief Convert UTF32 string to lower-case. Returns number of bytes converted.
 */
size_t utf32_tolower(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF32 string to lower-case.
 */
std::string utf32_tolower(const std::string &str);

/** \brief Convert UTF32 string to lower-case.
 */
std::u32string utf32_tolower(const std::u32string &str);

/** \brief Convert UTF32 string to upper-case. Returns number of bytes converted.
 */
size_t utf32_toupper(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF32 string to upper-case.
 */
std::string utf32_toupper(const std::string &str);

/** \brief Convert UTF32 string to upper-case.
 */
std::u32string utf32_toupper(const std::u32string &str);

/** \brief Convert UTF32 string to title-case. Returns number of bytes converted.
 */
//size_t utf32_totitle(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF32 string to title-case.
 */
// TODO: need to tokenize whitespace/characters...
//std::string utf32_totitle(const std::string &str);

/** \brief Convert UTF32 string to title-case.
 */
// TODO: need to tokenize whitespace/characters...
//std::u32string utf32_totitle(const std::u32string &str);
