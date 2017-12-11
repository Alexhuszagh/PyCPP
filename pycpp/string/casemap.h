//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Unicode-aware case conversion routines.
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using casemap_lowlevel_callback = std::function<void(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen)
>;

using casemap_highlevel_callback = std::function<std::string(const string_view&)>;

// FUNCTIONS
// ---------

// CHARACTERS

/**
 *  \brief Convert ASCII character to lower-case.
 */
uint8_t ascii_tolower(uint8_t c);

/**
 *  \brief Convert ASCII character to upper-case.
 */
uint8_t ascii_toupper(uint8_t c);

/**
 *  \brief Convert UTF32 character to lower-case.
 */
uint32_t utf32_tolower(uint32_t c);

/**
 *  \brief Convert UTF32 character to upper-case.
 */
uint32_t utf32_toupper(uint32_t c);

// ASCII

/**
 *  \brief Convert ASCII string to lower-case. Returns number of bytes converted.
 */
void ascii_tolower(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert ASCII string to lower-case.
 */
std::string ascii_tolower(const string_view& str);

/**
 *  \brief Convert ASCII string to upper-case. Returns number of bytes converted.
 */
void ascii_toupper(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert ASCII string to upper-case.
 */
std::string ascii_toupper(const string_view& str);

/**
 *  \brief Convert ASCII string to title-case. Returns number of bytes converted.
 */
void ascii_totitle(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert ASCII string to title-case.
 */
std::string ascii_totitle(const string_view& str);

/**
 *  \brief Capitalize ASCII string. Returns number of bytes converted.
 */
void ascii_capitalize(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Capitalize ASCII string.
 */
std::string ascii_capitalize(const string_view& str);

// UTF8

/**
 *  \brief Convert UTF8 string to lower-case. Returns number of bytes converted.
 */
void utf8_tolower(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF8 string to lower-case.
 */
std::string utf8_tolower(const string_view& str);

/**
 *  \brief Convert UTF8 string to upper-case. Returns number of bytes converted.
 */
void utf8_toupper(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF8 string to upper-case.
 */
std::string utf8_toupper(const string_view& str);

/**
 *  \brief Convert UTF8 string to title-case. Returns number of bytes converted.
 */
void utf8_totitle(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF8 string to title-case.
 */
std::string utf8_totitle(const string_view& str);

/**
 *  \brief Capitalize UTF8 string. Returns number of bytes converted.
 */
void utf8_capitalize(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Capitalize UTF8 string.
 */
std::string utf8_capitalize(const string_view& str);

// UTF16

/**
 *  \brief Convert UTF16 string to lower-case. Returns number of bytes converted.
 */
void utf16_tolower(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF16 string to lower-case.
 */
std::string utf16_tolower(const string_view& str);

/**
 *  \brief Convert UTF16 string to lower-case.
 */
std::u16string utf16_tolower(const std::u16string &str);

/**
 *  \brief Convert UTF16 string to upper-case. Returns number of bytes converted.
 */
void utf16_toupper(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF16 string to upper-case.
 */
std::string utf16_toupper(const string_view& str);

/**
 *  \brief Convert UTF16 string to upper-case.
 */
std::u16string utf16_toupper(const std::u16string &str);

/**
 *  \brief Convert UTF16 string to title-case. Returns number of bytes converted.
 */
void utf16_totitle(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF16 string to title-case.
 */
std::string utf16_totitle(const string_view& str);

/**
 *  \brief Convert UTF16 string to title-case.
 */
std::u16string utf16_totitle(const std::u16string &str);

/**
 *  \brief Capitalize UTF16 string. Returns number of bytes converted.
 */
void utf16_capitalize(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Capitalize UTF16 string.
 */
std::string utf16_capitalize(const string_view& str);

/**
 *  \brief Capitalize UTF16 string.
 */
std::u16string utf16_capitalize(const std::u16string &str);

// UTF32

/**
 *  \brief Convert UTF32 string to lower-case. Returns number of bytes converted.
 */
void utf32_tolower(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF32 string to lower-case.
 */
std::string utf32_tolower(const string_view& str);

/**
 *  \brief Convert UTF32 string to lower-case.
 */
std::u32string utf32_tolower(const u32string_view& str);

/**
 *  \brief Convert UTF32 string to upper-case. Returns number of bytes converted.
 */
void utf32_toupper(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF32 string to upper-case.
 */
std::string utf32_toupper(const string_view& str);

/**
 *  \brief Convert UTF32 string to upper-case.
 */
std::u32string utf32_toupper(const u32string_view& str);

/**
 *  \brief Convert UTF32 string to title-case. Returns number of bytes converted.
 */
void utf32_totitle(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Convert UTF32 string to title-case.
 */
std::string utf32_totitle(const string_view& str);

/**
 *  \brief Convert UTF32 string to title-case.
 */
std::u32string utf32_totitle(const u32string_view& str);

/**
 *  \brief Capitalize UTF32 string. Returns number of bytes converted.
 */
void utf32_capitalize(const void*& src, size_t srclen, void*& dst, size_t dstlen);

/**
 *  \brief Capitalize UTF32 string.
 */
std::string utf32_capitalize(const string_view& str);

/**
 *  \brief Capitalize UTF32 string.
 */
std::u32string utf32_capitalize(const u32string_view& str);

PYCPP_END_NAMESPACE
