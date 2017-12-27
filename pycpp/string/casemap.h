//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Unicode-aware case conversion routines.
 *
 *  Case conversion routines for ASCII and Unicode codepoints. Convert
 *  text to lowercase (all code points are converted to a normalized
 *  lowercase format), uppercase (all code points are converted to a
 *  normalized uppercase format), titlecase (the first letter at each
 *  word boundary is capitalized), or capital case (only the first
 *  letter is capitalized).
 *
 *  \synopsis
 *      using casemap_lowlevel_callback = void(*)(
 *          const void*& src, size_t srclen,
 *          void*& dst, size_t dstlen,
 *          const byte_allocator& allocator
 *      );
 *
 *      using casemap_highlevel_callback = string(*)(
 *          const string_view&, const byte_allocator& allocator
 *      );
 *
 *      uint8_t ascii_tolower(uint8_t c);
 *      uint8_t ascii_toupper(uint8_t c);
 *      uint32_t utf32_tolower(uint32_t c);
 *      uint32_t utf32_toupper(uint32_t c);
 *
 *      void ascii_tolower(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string ascii_tolower(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void ascii_toupper(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string ascii_toupper(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void ascii_totitle(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string ascii_totitle(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void ascii_capitalize(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string ascii_capitalize(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf8_tolower(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf8_tolower(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf8_toupper(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf8_toupper(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf8_totitle(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf8_totitle(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf8_capitalize(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf8_capitalize(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf16_tolower(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf16_tolower(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u16string utf16_tolower(const u16string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf16_toupper(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf16_toupper(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u16string utf16_toupper(const u16string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf16_totitle(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf16_totitle(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u16string utf16_totitle(const u16string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf16_capitalize(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf16_capitalize(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u16string utf16_capitalize(const u16string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf32_tolower(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf32_tolower(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u32string utf32_tolower(const u32string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf32_toupper(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf32_toupper(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u32string utf32_toupper(const u32string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf32_totitle(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf32_totitle(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u32string utf32_totitle(const u32string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf32_capitalize(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf32_capitalize(const string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      u32string utf32_capitalize(const u32string_view& str,
 *          const byte_allocator& allocator = byte_allocator());
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/stl/string_view.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using casemap_lowlevel_callback = void(*)(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    const byte_allocator& allocator
);

using casemap_highlevel_callback = string(*)(
    const string_view&, const byte_allocator& allocator
);

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
 *  \brief Convert ASCII string to lower-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void ascii_tolower(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert ASCII string to lower-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string ascii_tolower(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert ASCII string to upper-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void ascii_toupper(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert ASCII string to upper-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string ascii_toupper(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert ASCII string to title-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void ascii_totitle(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert ASCII string to title-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string ascii_totitle(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize ASCII string.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void ascii_capitalize(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize ASCII string.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string ascii_capitalize(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

// UTF8

/**
 *  \brief Convert UTF8 string to lower-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf8_tolower(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF8 string to lower-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf8_tolower(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF8 string to upper-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf8_toupper(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF8 string to upper-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf8_toupper(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF8 string to title-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf8_totitle(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF8 string to title-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf8_totitle(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize UTF8 string.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf8_capitalize(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize UTF8 string.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf8_capitalize(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

// UTF16

/**
 *  \brief Convert UTF16 string to lower-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf16_tolower(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16 string to lower-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf16_tolower(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16 string to lower-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u16string utf16_tolower(const u16string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16 string to upper-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf16_toupper(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16 string to upper-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf16_toupper(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16 string to upper-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u16string utf16_toupper(const u16string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16 string to title-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf16_totitle(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16 string to title-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf16_totitle(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16 string to title-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u16string utf16_totitle(const u16string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize UTF16 string.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf16_capitalize(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize UTF16 string.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf16_capitalize(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize UTF16 string.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u16string utf16_capitalize(const u16string_view& str,
    const byte_allocator& allocator = byte_allocator());

// UTF32

/**
 *  \brief Convert UTF32 string to lower-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf32_tolower(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32 string to lower-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf32_tolower(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32 string to lower-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u32string utf32_tolower(const u32string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32 string to upper-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf32_toupper(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32 string to upper-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf32_toupper(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32 string to upper-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u32string utf32_toupper(const u32string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32 string to title-case.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf32_totitle(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32 string to title-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf32_totitle(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32 string to title-case.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u32string utf32_totitle(const u32string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize UTF32 string.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf32_capitalize(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize UTF32 string.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf32_capitalize(const string_view& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Capitalize UTF32 string.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
u32string utf32_capitalize(const u32string_view& str,
    const byte_allocator& allocator = byte_allocator());

PYCPP_END_NAMESPACE
