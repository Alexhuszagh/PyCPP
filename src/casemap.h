//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Unicode-aware case conversion routines.
 */

#pragma once

#include <string>

// FUNCTIONS
// ---------

// CHARACTERS

/** \brief Convert ASCII character to lower-case.
 */
uint8_t ascii_tolower(uint8_t c);

/** \brief Convert ASCII character to upper-case.
 */
uint8_t ascii_toupper(uint8_t c);

/** \brief Convert UTF32 character to lower-case.
 */
uint32_t utf32_tolower(uint32_t c);

/** \brief Convert UTF32 character to upper-case.
 */
uint32_t utf32_toupper(uint32_t c);

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
size_t ascii_totitle(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert ASCII string to title-case.
 */
std::string ascii_totitle(const std::string &str);

/** \brief Capitalize ASCII string. Returns number of bytes converted.
 */
size_t ascii_capitalize(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Capitalize ASCII string.
 */
std::string ascii_capitalize(const std::string &str);

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
size_t utf8_totitle(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF8 string to title-case.
 */
std::string utf8_totitle(const std::string &str);

/** \brief Capitalize UTF8 string. Returns number of bytes converted.
 */
size_t utf8_capitalize(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Capitalize UTF8 string.
 */
std::string utf8_capitalize(const std::string &str);

// UTF16

/** \brief Convert UTF16 string to lower-case. Returns number of bytes converted.
 */
size_t utf16_tolower(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF16 string to lower-case.
 */
std::string utf16_tolower(const std::string &str);

/** \brief Convert UTF16 string to lower-case.
 */
std::u16string utf16_tolower(const std::u16string &str);

/** \brief Convert UTF16 string to upper-case. Returns number of bytes converted.
 */
size_t utf16_toupper(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF16 string to upper-case.
 */
std::string utf16_toupper(const std::string &str);

/** \brief Convert UTF16 string to upper-case.
 */
std::u16string utf16_toupper(const std::u16string &str);

/** \brief Convert UTF16 string to title-case. Returns number of bytes converted.
 */
size_t utf16_totitle(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF16 string to title-case.
 */
std::string utf16_totitle(const std::string &str);

/** \brief Convert UTF16 string to title-case.
 */
std::u16string utf16_totitle(const std::u16string &str);

/** \brief Capitalize UTF16 string. Returns number of bytes converted.
 */
size_t utf16_capitalize(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Capitalize UTF16 string.
 */
std::string utf16_capitalize(const std::string &str);

/** \brief Capitalize UTF16 string.
 */
std::u16string utf16_capitalize(const std::u16string &str);

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
size_t utf32_totitle(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Convert UTF32 string to title-case.
 */
std::string utf32_totitle(const std::string &str);

/** \brief Convert UTF32 string to title-case.
 */
std::u32string utf32_totitle(const std::u32string &str);

/** \brief Capitalize UTF32 string. Returns number of bytes converted.
 */
size_t utf32_capitalize(const void *src, size_t srclen, void* dst, size_t dstlen);

/** \brief Capitalize UTF32 string.
 */
std::string utf32_capitalize(const std::string &str);

/** \brief Capitalize UTF32 string.
 */
std::u32string utf32_capitalize(const std::u32string &str);
