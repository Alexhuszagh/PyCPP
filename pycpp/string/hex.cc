//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.

#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/string/base16.h>
#include <pycpp/string/hex.h>
#include <cstring>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


template <typename Iter1, typename Iter2>
static void hex_impl(Iter1 &src_first, Iter1 src_last,
                     Iter2 &dst_first, Iter2 dst_last,
                     size_t width)
{
    char* buffer = new char[width];

    size_t shift = 2 * width;
    while (src_first < src_last && dst_first + shift <= dst_last) {

        memcpy(buffer, src_first, width);
        bswap(buffer, width);
        base16_encode(buffer, width, dst_first, shift);

        src_first += width;
        dst_first += shift;
    }

    delete[] buffer;
}


template <typename Iter1, typename Iter2>
static void unhex_impl(Iter1 &src_first, Iter1 src_last,
                       Iter2 &dst_first, Iter2 dst_last,
                       size_t width)
{
    size_t shift = 2 * width;
    while (src_first + shift <= src_last && dst_first < dst_last) {
        base16_decode(src_first, shift, dst_first, width);
        bswap(dst_first, width);

        src_first += shift;
        dst_first += width;
    }
}

// FUNCTIONS
// ---------


void hex(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t width)
{
    // convert to preferred formats
    auto src_first = (const char*) src;
    auto src_last = src_first + srclen;
    auto dst_first = (char*) dst;
    auto dst_last = dst_first + dstlen;

    // convert
    hex_impl(src_first, src_last, dst_first, dst_last, width);

    // store buffer
    src = src_first;
    dst = dst_first;
}


std::string hex(const string_view& str, size_t width)
{
    size_t dstlen = 2 * str.size();
    const char* src = str.data();
    char* dst = new char[dstlen + 1];
    const void* src_first = (const void*) src;
    void* dst_first = (void*) dst;

    // create STL container and return
    hex(src_first, str.size(), dst_first, dstlen, width);
    size_t size = std::distance(dst, (char*) dst_first);
    std::string output(dst, size);
    delete[] dst;

    return output;
}


void hex_i8(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    hex(src, srclen, dst, dstlen, 1);
}


std::string hex_i8(const string_view& str)
{
    return hex(str, 1);
}

void hex_i16(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    hex(src, srclen, dst, dstlen, 2);
}


std::string hex_i16(const string_view& str)
{
    return hex(str, 2);
}

void hex_i32(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    hex(src, srclen, dst, dstlen, 4);
}


std::string hex_i32(const string_view& str)
{
    return hex(str, 4);
}

void hex_i64(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    hex(src, srclen, dst, dstlen, 8);
}


std::string hex_i64(const string_view& str)
{
    return hex(str, 8);
}


void unhex(const void*& src, size_t srclen, void*& dst, size_t dstlen, size_t width)
{
    // convert to preferred formats
    auto src_first = (const char*) src;
    auto src_last = src_first + srclen;
    auto dst_first = (char*) dst;
    auto dst_last = dst_first + dstlen;

    // convert
    unhex_impl(src_first, src_last, dst_first, dst_last, width);

    // store buffer
    src = src_first;
    dst = dst_first;
}


std::string unhex(const string_view& str, size_t width)
{
    size_t dstlen = str.size() / 2;
    const char* src = str.data();
    char* dst = new char[dstlen + 1];
    const void* src_first = (const void*) src;
    void* dst_first = (void*) dst;

    // create STL container and return
    unhex(src_first, str.size(), dst_first, dstlen, width);
    size_t size = std::distance(dst, (char*) dst_first);
    std::string output(dst, size);
    delete[] dst;

    return output;
}


void unhex_i8(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    unhex(src, srclen, dst, dstlen, 1);
}


std::string unhex_i8(const string_view& str)
{
    return unhex(str, 1);
}

void unhex_i16(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    unhex(src, srclen, dst, dstlen, 2);
}


std::string unhex_i16(const string_view& str)
{
    return unhex(str, 2);
}

void unhex_i32(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    unhex(src, srclen, dst, dstlen, 4);
}


std::string unhex_i32(const string_view& str)
{
    return unhex(str, 4);
}

void unhex_i64(const void*& src, size_t srclen, void*& dst, size_t dstlen)
{
    unhex(src, srclen, dst, dstlen, 8);
}


std::string unhex_i64(const string_view& str)
{
    return unhex(str, 8);
}

PYCPP_END_NAMESPACE
