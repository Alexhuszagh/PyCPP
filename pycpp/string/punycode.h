//  :copyright: (c) 2011 by Ben Noordhuis <info@bnoordhuis.nl>.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Punycode conversions.
 *
 *  Convert Unicode to and from Punycode, a compact ASCII-only
 *  representation of Unicode code points frequently used for
 *  international domain names (IDNAs).
 *
 *  \synopsis
 *      using punycode_lowlevel_callback = void(*)(
 *          const void*& src, size_t srclen,
 *          void*& dst, size_t dstlen,
 *          const byte_allocator& allocator
 *      );
 *
 *      using punycode_highlevel_callback = string(*)(
 *          const string_wrapper&, const byte_allocator&
 *      );
 *
 *      void utf8_to_punycode(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf8_to_punycode(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf16_to_punycode(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf16_to_punycode(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void utf32_to_punycode(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string utf32_to_punycode(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void punycode_to_utf8(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string punycode_to_utf8(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void punycode_to_utf16(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string punycode_to_utf16(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      void punycode_to_utf32(const void*& src,
 *          size_t srclen,
 *          void*& dst,
 *          size_t dstlen,
 *          const byte_allocator& allocator = byte_allocator());
 *
 *      string punycode_to_utf32(const string_wrapper& str,
 *          const byte_allocator& allocator = byte_allocator());
 */

#pragma once

#include <pycpp/stl/functional.h>
#include <pycpp/string/string.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using punycode_lowlevel_callback = void(*)(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    const byte_allocator& allocator
);

using punycode_highlevel_callback = string(*)(
    const string_wrapper&, const byte_allocator&
);

// FUNCTIONS
// ---------

/**
 *  \brief Convert UTF8-encoded data to Punycode.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf8_to_punycode(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF8-encoded data to Punycode.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string
 */
string utf8_to_punycode(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16-encoded data to Punycode.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf16_to_punycode(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF16-encoded data to Punycode.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string
 */
string utf16_to_punycode(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32-encoded data to Punycode.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void utf32_to_punycode(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert UTF32-encoded data to Punycode.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string
 */
string utf32_to_punycode(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert Punycode to UTF-8.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void punycode_to_utf8(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert Punycode to UTF-8.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string
 */
string punycode_to_utf8(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert Punycode to UTF-16.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void punycode_to_utf16(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert Punycode to UTF-16.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string
 */
string punycode_to_utf16(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert Punycode to UTF-32.
 *
 *  \param src              Pointer to source buffer.
 *  \param srclen           Length of source buffer.
 *  \param src              Pointer to destination buffer.
 *  \param dstlen           Length of destination buffer.
 *  \param allocator        Allocator for internal allocations.
 */
void punycode_to_utf32(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator = byte_allocator());

/**
 *  \brief Convert Punycode to UTF-32.
 *
 *  \param str              Source string to convert.
 *  \param allocator        Allocator for output string
 */
string punycode_to_utf32(const string_wrapper& str,
    const byte_allocator& allocator = byte_allocator());

PYCPP_END_NAMESPACE
