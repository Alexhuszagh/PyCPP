//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.

#include "base16.h"
#include "byteorder.h"
#include "hex.h"

#include <cstring>

// HELPERS
// -------


template <typename Iter1, typename Iter2>
static size_t hex_impl(Iter1 src_first, Iter1 src_last,
                       Iter2 dst_first, Iter2 dst_last,
                       size_t width)
{
    char* buffer = new char[width];

    size_t shift = 2 * width;
    auto src = src_first;
    auto dst = dst_first;
    while (src < src_last && dst + shift <= dst_last) {

        memcpy(buffer, src, width);
        bswap(buffer, width);
        base16_encode(buffer, width, dst, shift);

        src += width;
        dst += shift;
    }

    delete[] buffer;

    return dst - dst_first;
}


template <typename Iter1, typename Iter2>
static size_t unhex_impl(Iter1 src_first, Iter1 src_last,
                         Iter2 dst_first, Iter2 dst_last,
                         size_t width)
{
    size_t shift = 2 * width;
    auto src = src_first;
    auto dst = dst_first;
    while (src + shift <= src_last && dst < dst_last) {
        base16_decode(src, shift, dst, width);
        bswap(dst, width);

        src += shift;
        dst += width;
    }

    return dst - dst_first;
}

// FUNCTIONS
// ---------


size_t hex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    return hex_impl(src_first, src_last, dst_first, dst_last, width);
}


std::string hex(const std::string& str, size_t width)
{
    size_t length = 2 * str.size();
    auto *src_first = str.data();
    auto src_last = src_first + str.size();
    char *dst_first = new char[length + 1];
    char *dst_last = dst_first + length + 1;

    // create STL container and return
    auto size = hex_impl(src_first, src_last, dst_first, dst_last, width);
    std::string output(dst_first, size);
    delete[] dst_first;

    return output;
}


size_t hex_i8(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 1);
}


std::string hex_i8(const std::string& str)
{
    return hex(str, 1);
}

size_t hex_i16(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 2);
}


std::string hex_i16(const std::string& str)
{
    return hex(str, 2);
}

size_t hex_i32(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 4);
}


std::string hex_i32(const std::string& str)
{
    return hex(str, 4);
}

size_t hex_i64(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 8);
}


std::string hex_i64(const std::string& str)
{
    return hex(str, 8);
}


size_t unhex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    return unhex_impl(src_first, src_last, dst_first, dst_last, width);
}


std::string unhex(const std::string& str, size_t width)
{
    size_t length = str.size() / 2;
    auto *src_first = str.data();
    auto src_last = src_first + str.size();
    char *dst_first = new char[length + 1];
    char *dst_last = dst_first + length + 1;

    // create STL container and return
    auto size = unhex_impl(src_first, src_last, dst_first, dst_last, width);
    std::string output(dst_first, size);
    delete[] dst_first;

    return output;
}


size_t unhex_i8(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return unhex(src, srclen, dst, dstlen, 1);
}


std::string unhex_i8(const std::string& str)
{
    return unhex(str, 1);
}

size_t unhex_i16(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return unhex(src, srclen, dst, dstlen, 2);
}


std::string unhex_i16(const std::string& str)
{
    return unhex(str, 2);
}

size_t unhex_i32(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return unhex(src, srclen, dst, dstlen, 4);
}


std::string unhex_i32(const std::string& str)
{
    return unhex(str, 4);
}

size_t unhex_i64(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return unhex(src, srclen, dst, dstlen, 8);
}


std::string unhex_i64(const std::string& str)
{
    return unhex(str, 8);
}
