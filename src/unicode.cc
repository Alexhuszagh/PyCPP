//  :copyright: (c) Copyright 2001-2004 Unicode, Inc.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.

#include "unicode.h"

#include <algorithm>
#include <functional>
#include <stdexcept>

// MACROS
// ------

#define utf8_t(t) static_cast<uint8_t>(t)
#define utf16_t(t) static_cast<uint16_t>(t)
#define utf32_t(t) static_cast<uint32_t>(t)

// CONSTANTS
// ---------

const uint8_t FIRST_BYTE_MARK[] = {0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC};
const uint8_t UTF8_BYTES[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};
const uint32_t UTF8_OFFSETS[] = {0x00000000UL, 0x00003080UL, 0x000E2080UL, 0x03C82080UL, 0xFA082080UL, 0x82082080UL};


// HELPERS -- CHARACTERS
// ---------------------


static uint32_t assert_strict(bool strict)
{
    if (strict) {
        throw std::runtime_error("Illegal character encountered.");
    }
    return 0x0000FFFDul;
}


/** \brief Convert UTF-32 character to UTF-16.
 *
 *  Returns the number of characters converted.
 */
template <typename Iter16>
size_t utf32_to_utf16(uint32_t c, Iter16& first, Iter16& last, bool strict)
{
    // limits
    static constexpr uint32_t max_utf32 = 0x0010FFFF;
    static constexpr uint32_t high_begin = 0xD800;
    static constexpr uint32_t low_begin = 0xDC00;
    static constexpr uint32_t max_bmp = 0x0000FFFF;
    static constexpr int shift = 10;
    static constexpr uint32_t base = 0x0010000UL;
    static constexpr uint32_t mask = 0x3FFUL;

    // variables
    if (c <= max_bmp) {
        if (c >= high_begin && c <= low_begin) {
            *first++ = assert_strict(strict);
        } else {
            *first++ = utf16_t(c);
        }
    } else if (c > max_utf32) {
        *first++ = assert_strict(strict);
    } else {
        if (first + 1 > last) {
            return 0;
        }

        c -= base;
        *first++ = utf16_t((c >> shift) + high_begin);
        *first++ = utf16_t((c & mask) + low_begin);
    }

    return 1;
}


/** \brief Convert UTF-32 character to UTF-8.
 *
 *  Returns the number of characters converted.
 */
template <typename Iter8>
size_t utf32_to_utf8(uint32_t c, Iter8& first, Iter8 last, bool strict)
{
    // limits
    static constexpr uint32_t max_utf32 = 0x0010FFFF;
    static constexpr uint32_t bytemark = 0x80;
    static constexpr uint32_t bytemask = 0xBF;

    // calculate bytes to write
    short bytes;
    if (c < 0x80) {
        bytes = 1;
    } else if (c < 0x800) {
        bytes = 2;
    } else if (c < 0x10000) {
        bytes = 3;
    } else if (c <= max_utf32) {
        bytes = 4;
    } else {
        bytes = 3;
        c = assert_strict(strict);
    }

    // check range
    if (first + bytes > last) {
        return 0;
    }

    // write to buffer
    first += bytes;
    switch (bytes) {
        case 4:
            *--first = utf8_t((c | bytemark) & bytemask);
            c >>= 6;
        case 3:
            *--first = utf8_t((c | bytemark) & bytemask);
            c >>= 6;
        case 2:
            *--first = utf8_t((c | bytemark) & bytemask);
            c >>= 6;
        case 1:
            *--first = utf8_t(c | FIRST_BYTE_MARK[bytes]);
    }
    first += bytes;

    return 1;
}


// HELPERS -- ARRAYS
// -----------------


/**
 *  \brief Convert UTF16 to UTF32.
 *
 *  \return     Number of bytes written to dst.
 */
template <typename Iter16, typename Iter32>
size_t utf16_to_utf32_array(Iter16 src_first, Iter16 src_last,
                            Iter32 dst_first, Iter32 dst_last,
                            bool strict = true)
{
    auto src = src_first;
    auto dst = dst_first;
    while (src < src_last && dst < dst_last) {
//        *dst++ = utf16_to_utf32(src, src_last, strict);
    }

    return dst - dst_first;
}


/**
 *  \brief Convert UTF32 to UTF8.
 *
 *  \return     Number of bytes written to dst.
 */
template <typename Iter32, typename Iter8>
size_t utf32_to_utf8_array(Iter32 src_first, Iter32 src_last,
                           Iter8 dst_first, Iter8 dst_last,
                           bool strict = true)
{
    auto src = src_first;
    auto dst = dst_first;
    while (src < src_last && dst < dst_last) {
        utf32_to_utf8(*src++, dst, dst_last, strict);
    }

    return dst - dst_first;
}


/**
 *  \brief Convert UTF32 to UTF16.
 *
 *  \return     Number of bytes written to dst.
 */
template <typename Iter32, typename Iter16>
size_t utf32_to_utf16_array(Iter32 src_first, Iter32 src_last,
                            Iter16 dst_first, Iter16 dst_last,
                            bool strict = true)
{
    auto src = src_first;
    auto dst = dst_first;
    while (src < src_last && dst < dst_last) {
        utf32_to_utf16(*src++, dst, dst_last, strict);
    }

    return dst - dst_first;
}


// HELPERS -- POINTERS
// -------------------


size_t utf16_to_utf32_ptr(const uint16_t *src_first, const uint16_t* src_last,
                          uint32_t* dst_first, uint32_t* dst_last,
                          bool strict = true)
{
    return utf16_to_utf32_array(src_first, src_last, dst_first, dst_last, strict);
}


size_t utf32_to_utf8_ptr(const uint32_t *src_first, const uint32_t* src_last,
                         uint8_t* dst_first, uint8_t* dst_last,
                         bool strict = true)
{
    return utf32_to_utf8_array(src_first, src_last, dst_first, dst_last, strict);
}


size_t utf32_to_utf16_ptr(const uint32_t *src_first, const uint32_t* src_last,
                          uint16_t* dst_first, uint16_t* dst_last,
                          bool strict = true)
{
    return utf32_to_utf16_array(src_first, src_last, dst_first, dst_last, strict);
}


// HELPERS - STL
// -------------


/**
 *  \brief STL wrapper for narrow to wide conversions.
 */
template <typename Char1, typename Char2>
struct to_wide
{
    template <typename Function>
    std::string operator()(const std::string &string, Function function)
    {
        // types
        constexpr size_t size1 = sizeof(Char1);
        constexpr size_t size2 = sizeof(Char2);

        // arguments
        const size_t srclen = string.size() / size1;
        const size_t dstlen = srclen;
        auto *src_first = reinterpret_cast<const Char1*>(string.data());
        auto *src_last = src_first + srclen;
        auto *dst_first = reinterpret_cast<Char2*>(malloc(dstlen * size2));
        auto *dst_last = dst_first + dstlen;

        size_t out = function(src_first, src_last, dst_first, dst_last, true);
        std::string output(reinterpret_cast<const char*>(dst_first), out * size2);
        free(dst_first);

        return output;
    }
};


/**
 *  \brief STL wrapper for wide to narrow conversions.
 */
template <typename Char1, typename Char2>
struct to_narrow
{
    template <typename Function>
    std::string operator()(const std::string &string, Function function)
    {
        // types
        constexpr size_t size1 = sizeof(Char1);
        constexpr size_t size2 = sizeof(Char2);

        // arguments
        const size_t srclen = string.size() / size1;
        const size_t dstlen = srclen * 4;
        auto *src_first = reinterpret_cast<const Char1*>(string.data());
        auto *src_last = src_first + srclen;
        auto *dst_first = reinterpret_cast<Char2*>(malloc(dstlen * size2));
        auto *dst_last = dst_first + dstlen;

        size_t out = function(src_first, src_last, dst_first, dst_last, true);
        std::string output(reinterpret_cast<const char*>(dst_first), out * size2);
        free(dst_first);

        return output;
    }
};

// FUNCTIONS
// ---------


bool is_unicode(const std::string &string)
{
    return std::any_of(string.cbegin(), string.cend(), [](char c) {
        return c < 0;
    });
}


size_t utf8_to_utf16(const void *src, size_t srclen, void* dst, size_t dstlen)
{
    throw std::runtime_error("");
    return 0;
}


std::string utf8_to_utf16(const std::string& str)
{
    throw std::runtime_error("");
    return "";
}


size_t utf8_to_utf32(const void *src, size_t srclen, void* dst, size_t dstlen)
{
    throw std::runtime_error("");
    return 0;
}


std::string utf8_to_utf32(const std::string& str)
{
    throw std::runtime_error("");
    return "";
}


size_t utf16_to_utf8(const void *src, size_t srclen, void* dst, size_t dstlen)
{
    throw std::runtime_error("");
    return 0;
}


std::string utf16_to_utf8(const std::string& str)
{
    throw std::runtime_error("");
    return "";
}


size_t utf16_to_utf32(const void *src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const uint16_t*>(src);
    auto src_last = src_first + (srclen / 2);
    auto dst_first = reinterpret_cast<uint32_t*>(dst);
    auto dst_last = dst_first + (dstlen / 4);

    // TODO: restore
//    return utf16_to_utf32_ptr(src_first, src_last, dst_first, dst_last);
}


std::string utf16_to_utf32(const std::string& str)
{
    // TODO: restore
//    return to_wide<uint16_t, uint32_t>()(str, utf16_to_utf32_ptr);
}


size_t utf32_to_utf8(const void *src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const uint32_t*>(src);
    auto src_last = src_first + (srclen / 2);
    auto dst_first = reinterpret_cast<uint8_t*>(dst);
    auto dst_last = dst_first + (dstlen / 4);

    return utf32_to_utf8_ptr(src_first, src_last, dst_first, dst_last);
}


std::string utf32_to_utf8(const std::string& str)
{
    return to_narrow<uint32_t, uint8_t>()(str, utf32_to_utf8_ptr);
}


size_t utf32_to_utf16(const void *src, size_t srclen, void* dst, size_t dstlen)
{
    auto src_first = reinterpret_cast<const uint32_t*>(src);
    auto src_last = src_first + (srclen / 2);
    auto dst_first = reinterpret_cast<uint16_t*>(dst);
    auto dst_last = dst_first + (dstlen / 4);

    return utf32_to_utf16_ptr(src_first, src_last, dst_first, dst_last);
}


std::string utf32_to_utf16(const std::string& str)
{
    return to_narrow<uint32_t, uint16_t>()(str, utf32_to_utf16_ptr);
}
