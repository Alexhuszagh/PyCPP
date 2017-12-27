//  :copyright: (c) 2017 Alex Huszagh.
//  :license: Unicode, see licenses/unicode.md for more details.

#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/string/base16.h>
#include <pycpp/string/hex.h>
#include <string.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


template <typename Iter1, typename Iter2>
static void hex_impl(Iter1 &src_first,
    Iter1 src_last,
    Iter2 &dst_first,
    Iter2 dst_last,
    const byte_allocator& allocator,
    size_t width)
{
    // get allocator
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char>;
    char_allocator alloc(allocator);

    void* buffer = nullptr;
    try {
        buffer = static_cast<void*>(alloc.allocate(width));
        const void* b16_src = buffer;
        void* b16_dst = static_cast<void*>(dst_first);
        size_t shift = 2 * width;
        while (src_first < src_last && dst_first + shift <= dst_last) {
            memcpy(buffer, src_first, width);
            bswap(buffer, static_cast<int>(width));
            base16_encode(b16_src, width, b16_dst, shift);

            src_first += width;
            dst_first += shift;
            b16_src = buffer;
        }
        alloc.deallocate(static_cast<char*>(buffer), width);
    } catch (...) {
        if (buffer) {
            alloc.deallocate(static_cast<char*>(buffer), width);
        }
        throw;
    }
}


template <typename Iter1, typename Iter2>
static void unhex_impl(Iter1 &src_first,
    Iter1 src_last,
    Iter2 &dst_first,
    Iter2 dst_last,
    const byte_allocator&,
    size_t width)
{
    size_t shift = 2 * width;
    const void* b16_src = static_cast<const void*>(src_first);
    void* b16_dst = static_cast<void*>(dst_first);
    while (src_first + shift <= src_last && dst_first < dst_last) {
        base16_decode(b16_src, shift, b16_dst, width);
        bswap(dst_first, static_cast<int>(width));

        src_first += shift;
        dst_first += width;
    }
}

// FUNCTIONS
// ---------


void hex(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator,
    size_t width)
{
    // convert to preferred formats
    auto src_first = (const char*) src;
    auto src_last = src_first + srclen;
    auto dst_first = (char*) dst;
    auto dst_last = dst_first + dstlen;

    // convert
    hex_impl(src_first, src_last, dst_first, dst_last, allocator, width);

    // store buffer
    src = src_first;
    dst = dst_first;
}


string hex(const string_wrapper& str,
    const byte_allocator& allocator,
    size_t width)
{
    // get allocator
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char>;
    char_allocator alloc(allocator);

    // get params
    size_t srclen = str.size();
    size_t dstlen = 2 * str.size();
    const char* src = str.data();
    char* dst = nullptr;

    try {
        // initialize memory for output
        dst = alloc.allocate(dstlen+1);
        const void* src_first = static_cast<const void*>(src);
        void* dst_first = static_cast<void*>(dst);

        // create STL container and return
        hex(src_first, srclen, dst_first, dstlen, allocator, width);
        size_t size = distance(dst, static_cast<char*>(dst_first));
        string output(dst, size, alloc);
        alloc.deallocate(dst, dstlen+1);

        return output;
    } catch (...) {
        if (dst) {
            alloc.deallocate(dst, dstlen+1);
        }
        throw;
    }
}


void hex_i8(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator)
{
    hex(src, srclen, dst, dstlen, allocator, 1);
}


string hex_i8(const string_wrapper& str,
    const byte_allocator& allocator)
{
    return hex(str, allocator, 1);
}


void hex_i16(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator)
{
    hex(src, srclen, dst, dstlen, allocator, 2);
}


string hex_i16(const string_wrapper& str,
    const byte_allocator& allocator)
{
    return hex(str, allocator, 2);
}

void hex_i32(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator)
{
    hex(src, srclen, dst, dstlen, allocator, 4);
}


string hex_i32(const string_wrapper& str,
    const byte_allocator& allocator)
{
    return hex(str, allocator, 4);
}

void hex_i64(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator)
{
    hex(src, srclen, dst, dstlen, allocator, 8);
}


string hex_i64(const string_wrapper& str,
    const byte_allocator& allocator)
{
    return hex(str, allocator, 8);
}


void unhex(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator,
    size_t width)
{
    // convert to preferred formats
    auto src_first = (const char*) src;
    auto src_last = src_first + srclen;
    auto dst_first = (char*) dst;
    auto dst_last = dst_first + dstlen;

    // convert
    unhex_impl(src_first, src_last, dst_first, dst_last, allocator, width);

    // store buffer
    src = src_first;
    dst = dst_first;
}


string unhex(const string_wrapper& str,
    const byte_allocator& allocator,
    size_t width)
{
    // get allocator
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char>;
    char_allocator alloc(allocator);

    // get params
    size_t srclen = str.size();
    size_t dstlen = str.size() / 2;
    const char* src = str.data();
    char* dst = nullptr;

    try {
        // initialize memory for output
        dst = alloc.allocate(dstlen+1);;
        const void* src_first = static_cast<const void*>(src);
        void* dst_first = static_cast<void*>(dst);

        // create STL container and return
        unhex(src_first, srclen, dst_first, dstlen, allocator, width);
        size_t size = distance(dst, static_cast<char*>(dst_first));
        string output(dst, size, alloc);
        alloc.deallocate(dst, dstlen+1);

        return output;
    } catch (...) {
        if (dst) {
            alloc.deallocate(dst, dstlen+1);
        }
        throw;
    }
}


void unhex_i8(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator)
{
    unhex(src, srclen, dst, dstlen, allocator, 1);
}


string unhex_i8(const string_wrapper& str,
    const byte_allocator& allocator)
{
    return unhex(str, allocator, 1);
}


void unhex_i16(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator)
{
    unhex(src, srclen, dst, dstlen, allocator, 2);
}


string unhex_i16(const string_wrapper& str,
    const byte_allocator& allocator)
{
    return unhex(str, allocator, 2);
}

void unhex_i32(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator)
{
    unhex(src, srclen, dst, dstlen, allocator, 4);
}


string unhex_i32(const string_wrapper& str,
    const byte_allocator& allocator)
{
    return unhex(str, allocator, 4);
}


void unhex_i64(const void*& src,
    size_t srclen,
    void*& dst,
    size_t dstlen,
    const byte_allocator& allocator)
{
    unhex(src, srclen, dst, dstlen, allocator, 8);
}


string unhex_i64(const string_wrapper& str,
    const byte_allocator& allocator)
{
    return unhex(str, allocator, 8);
}

PYCPP_END_NAMESPACE
