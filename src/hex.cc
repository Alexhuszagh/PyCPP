//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.

#include "base16.h"
#include "byteorder.h"
#include "hex.h"

#include <cstdio>


// HELPERS
// -------

#if BYTE_ORDER == LITTLE_ENDIAN

template <typename Iter1>
static int64_t signed_value(Iter1 src, size_t width)
{
    int64_t value = 0;
    for (size_t i = 0; i < width; ++i) {
        value += *src++ << 8 * i;
    }

    return value;
}


template <typename Iter1>
static uint64_t unsigned_value(Iter1 src, size_t width)
{
    uint64_t value = 0;
    for (size_t i = 0; i < width; ++i) {
        value += *src++ << 8 * i;
    }

    return value;
}

#else

template <typename Iter1>
static int64_t signed_value(Iter1 src, size_t width)
{
    int64_t value = 0;
        while (width--) {
        value += *src++ << 8 * width;
    }

    return value;
}


template <typename Iter1>
static uint64_t unsigned_value(Iter1 src, size_t width)
{
    uint64_t value = 0;
    while (width--) {
        value += *src++ << 8 * width;
    }

    return value;
}

#endif


/** \brief Get number of digits from number.
 */
static size_t digits(size_t number)
{
    size_t digits = 0;
    while (number) {
        number /= 10;
        ++digits;
    }
    return digits;
}


/** \brief Get the snprintf formatting string.
 *
 *  \code
 *      formatter(8) -> "%08x"
 */
static std::string formatter(size_t width)
{
    const size_t length = digits(width);
    const size_t size = length + 4;
    char *buffer = new char[size];

    snprintf(buffer, size, "%%0%dx", static_cast<int>(width));
    std::string output(buffer, size - 1);
    delete[] buffer;

    return output;
}


template <typename Iter1, typename Iter2, typename Function>
static size_t hex_impl(Iter1 src_first, Iter1 src_last,
                       Iter2 dst_first, Iter2 dst_last,
                       size_t width, Function function)
{
    size_t shift = 2 * width;
    auto src = src_first;
    auto dst = dst_first;
    auto format = formatter(shift);
    while (src < src_last && dst + shift <= dst_last) {

        auto num = function(src, width);
        snprintf(dst, shift+1, format.data(), num);

#if BYTE_ORDER == BIG_ENDIAN
        // need to byteswap the values
//        bswap64();
#endif
        // base16_encode(&num, width, dst, shift);
        // TODO: this could likely remove snprintf...
        // Need to make sure this is big and little endian...
        // This is just base16 all over again...
        src += width;
        dst += shift;
    }

    return dst - dst_first;
}


template <typename Iter1, typename Iter2>
static size_t hex_impl(Iter1 src_first, Iter1 src_last,
                       Iter2 dst_first, Iter2 dst_last,
                       size_t width, bool is_signed)
{
    if (is_signed) {
        return hex_impl(src_first, src_last, dst_first, dst_last, width, signed_value<Iter1>);
    } else {
        return hex_impl(src_first, src_last, dst_first, dst_last, width, unsigned_value<Iter1>);
    }
}


template <typename Iter1, typename Iter2, typename Function>
static size_t unhex_impl(Iter1 src_first, Iter1 src_last,
                         Iter2 dst_first, Iter2 dst_last,
                         size_t width, Function function)
{
    size_t shift = 2 * width;
    auto src = src_first;
    auto dst = dst_first;
// TODO: here....
//    auto format = formatter(shift);
//    while (src < src_last && dst + shift <= dst_last) {
//        snprintf(dst, shift+1, format.data(), function(src, width));
//        src += width;
//        dst += shift;
//    }
//
//    return dst - dst_first;
}

// FUNCTIONS
// ---------


size_t hex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width, bool is_signed)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

    return hex_impl(src_first, src_last, dst_first, dst_last, width, is_signed);
}


std::string hex(const std::string& str, size_t width, bool is_signed)
{
    size_t length = 2 * str.size();
    auto *src_first = str.data();
    auto src_last = src_first + str.size();
    char *dst_first = new char[length + 1];
    char *dst_last = dst_first + length + 1;

    // create STL container and return
    auto size = hex_impl(src_first, src_last, dst_first, dst_last, width, is_signed);
    std::string output(dst_first, size);
    delete[] dst_first;

    return output;
}


size_t hex_int8(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 1, true);
}


std::string hex_int8(const std::string& str)
{
    return hex(str, 1, true);
}


size_t hex_uint8(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 1, false);
}


std::string hex_uint8(const std::string& str)
{
    return hex(str, 1, false);
}


size_t hex_int16(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 2, true);
}


std::string hex_int16(const std::string& str)
{
    return hex(str, 2, true);
}


size_t hex_uint16(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 2, false);
}


std::string hex_uint16(const std::string& str)
{
    return hex(str, 2, false);
}


size_t hex_int32(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 4, true);
}


std::string hex_int32(const std::string& str)
{
    return hex(str, 4, true);
}


size_t hex_uint32(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 4, false);
}


std::string hex_uint32(const std::string& str)
{
    return hex(str, 4, false);
}


size_t hex_int64(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 8, true);
}


std::string hex_int64(const std::string& str)
{
    return hex(str, 8, true);
}


size_t hex_uint64(const void* src, size_t srclen, void* dst, size_t dstlen)
{
    return hex(src, srclen, dst, dstlen, 8, false);
}


std::string hex_uint64(const std::string& str)
{
    return hex(str, 8, false);
}


size_t unhex(const void* src, size_t srclen, void* dst, size_t dstlen, size_t width, bool is_signed)
{
    auto src_first = reinterpret_cast<const char*>(src);
    auto src_last = src_first + srclen;
    auto dst_first = reinterpret_cast<char*>(dst);
    auto dst_last = dst_first + dstlen;

//    return unhex_impl(src_first, src_last, dst_first, dst_last, width, is_signed);
    return 0;
}


std::string unhex(const std::string& str, size_t width, bool is_signed)
{
    size_t length = str.size() / 2;
    auto *src_first = str.data();
    auto src_last = src_first + str.size();
    char *dst_first = new char[length + 1];
    char *dst_last = dst_first + length + 1;

    // create STL container and return
//    auto size = unhex_impl(src_first, src_last, dst_first, dst_last, width, is_signed);
    size_t size = 0;
    std::string output(dst_first, size);
    delete[] dst_first;

    return output;
}
