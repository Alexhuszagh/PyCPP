//  :copyright: (c) Copyright 2001-2004 Unicode, Inc.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.
//  :copyright: (c) Copyright 2001-2011 Hartmut Kaiser.
//  :copyright: (c) Copyright 2001-2011 Joel de Guzman.
//  :license: Boost, see licenses/boost.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Unicode code point conversions and detection.
 *
 *  Routines to detect character type (if the character is numeric,
 *  alphanumeric, alphabetical, lowercase, etc.) and convert between
 *  Unicode code points (UTF-8, UTF-16, and UTF-32). Each conversion
 *  routine takes a memory allocator which allocates memory internally
 *  for the high-level interfaces, and dictate the memory allocation
 *  properties of the output string.
 *
 *  \synopsis
 *      using unicode_lowlevel_callback = void(*)(
 *          const void*& src, size_t srclen,
 *          void*& dst, size_t dstlen,
 *          const byte_allocator& allocator
 *      );
 *
 *      using unicode_highlevel_callback = string(*)(
 *          const string_view&, const byte_allocator& allocator
 *      );
 *
 *      bool is_ascii(uint32_t c);
 *      bool is_unicode(uint32_t c);
 *      bool is_alnum(uint32_t c);
 *      bool is_alpha(uint32_t c);
 *      bool is_digit(uint32_t c);
 *      bool is_lower(uint32_t c);
 *      bool is_upper(uint32_t c);
 *      bool is_space(uint32_t c);
 *      bool is_blank(uint32_t c);
 *      bool is_punctuation(uint32_t c);
 *      bool is_ascii_byte(uint8_t c);
 *      bool is_start_byte(uint8_t c);
 *      bool is_continuation_byte(uint8_t c);
 *      bool is_ascii(const string_wrapper& str);
 *      bool is_unicode(const string_wrapper& str);
 *
 *      void utf8_to_utf16(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf8_to_utf16(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf8_to_utf32(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf8_to_utf32(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf16_to_utf8(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf16_to_utf8(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf16_to_utf32(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf16_to_utf32(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf32_to_utf8(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf32_to_utf8(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf32_to_utf16(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf32_to_utf16(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using unicode_lowlevel_callback = void(*)(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    const byte_allocator& allocator
);

using unicode_highlevel_callback = string(*)(
    const string_view&, const byte_allocator& allocator
);

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
 *  \brief Convert UTF-8 to UTF-16.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf8_to_utf16(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-8 string to UTF-16.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf8_to_utf16(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-8 to UTF-32.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf8_to_utf32(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-8 string to UTF-32.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf8_to_utf32(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-16 to UTF-8.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf16_to_utf8(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-16 string to UTF-8.
 */
string utf16_to_utf8(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-16 to UTF-32.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf16_to_utf32(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-16 string to UTF-32.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf16_to_utf32(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-32 to UTF-8.
 */
void utf32_to_utf8(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-32 string to UTF-8.
 */
string utf32_to_utf8(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-32 to UTF-16.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations (unused).
 */
void utf32_to_utf16(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF-32 string to UTF-16.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string.
 */
string utf32_to_utf16(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

PYCPP_END_NAMESPACE
