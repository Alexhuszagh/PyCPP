//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/string/codec.h>
#include <pycpp/string/unicode.h>

PYCPP_BEGIN_NAMESPACE

// ALIAS
// -----

using lowlevel_callback = void(*)(
    const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    const byte_allocator& allocator
);

using highlevel_callback = string(*)(
    const string_view&, const byte_allocator& allocator
);

// HELPERS
// -------

/**
 *  \brief Convert narrow to wide encoding.
 */
template <typename S1, typename S2, typename Function>
static S2& to_wide(const S1& s1,
    S2& s2,
    const byte_allocator& allocator,
    Function function)
{
    // types
    using char_type1 = typename S1::value_type;
    using char_type2 = typename S2::value_type;

    // allocator
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char>;
    char_allocator alloc(allocator);

    // get parameters
    size_t srclen = s1.size() * sizeof(char_type1);
    size_t dstlen = srclen * 4 / sizeof(char_type1);
    const char* src = (const char*) s1.data();
    char* dst = nullptr;

    try {
        // initialize variables
        dst = alloc.allocate(dstlen);
        const void* src_first = (const void*) src;
        void* dst_first = (void*) dst;

        // convert string
        function(src_first, srclen, dst_first, dstlen, allocator);
        size_t length = distance(dst, (char*) dst_first);
        s2 = S2((char_type2*) dst, length / sizeof(char_type2));
        alloc.deallocate(dst, dstlen);
        return s2;
    } catch (...) {
        alloc.deallocate(dst, dstlen);
        throw;
    }
}


/**
 *  \brief Convert wide to narrow encoding.
 */
template <typename S1, typename S2, typename Function>
static S2& to_narrow(const S1& s1,
    S2& s2,
    const byte_allocator& allocator,
    Function function)
{
    // types
    using char_type1 = typename S1::value_type;
    using char_type2 = typename S2::value_type;

    // allocator
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char>;
    char_allocator alloc(allocator);

    // get parameters
    // each utf16 to utf8 conversion can increase the buffer size,
    // while each utf32 to utf16/utf8 at most keeps the buffer size
    // constant.
    size_t srclen = s1.size() * sizeof(char_type1);
    size_t dstlen = sizeof(char_type1) == 2 ? static_cast<size_t>(srclen * 1.5) : srclen;
    const char* src = (const char*) s1.data();
    char* dst = nullptr;

    try {
        dst = alloc.allocate(dstlen);
        const void* src_first = (const void*) src;
        void* dst_first = (void*) dst;

        // convert string
        function(src_first, srclen, dst_first, dstlen, allocator);
        size_t length = distance(dst, (char*) dst_first);
        s2 = S2((char_type2*) dst, length / sizeof(char_type2));
        alloc.deallocate(dst, dstlen);
        return s2;
    } catch (...) {
        alloc.deallocate(dst, dstlen);
        throw;
    }
}

// FUNCTIONS
// ---------


u16string codec_utf8_utf16(const string_view& str,
    const byte_allocator& allocator)
{
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char16_t>;
    char_allocator alloc(allocator);

    u16string u16(alloc);
    lowlevel_callback cb(utf8_to_utf16);
    return to_wide(str, u16, allocator, cb);
}


u32string codec_utf8_utf32(const string_view& str,
    const byte_allocator& allocator)
{
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char32_t>;
    char_allocator alloc(allocator);

    u32string u32(alloc);
    lowlevel_callback cb(utf8_to_utf32);
    return to_wide(str, u32, allocator, cb);
}


string codec_utf16_utf8(const u16string_view& str,
    const byte_allocator& allocator)
{
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char>;
    char_allocator alloc(allocator);

    string u8(alloc);
    lowlevel_callback cb(utf16_to_utf8);
    return to_narrow(str, u8, allocator, cb);
}


u32string codec_utf16_utf32(const u16string_view& str,
    const byte_allocator& allocator)
{
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char32_t>;
    char_allocator alloc(allocator);

    u32string u32(alloc);
    lowlevel_callback cb(utf16_to_utf32);
    return to_wide(str, u32, allocator, cb);
}


string codec_utf32_utf8(const u32string_view& str,
    const byte_allocator& allocator)
{
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char>;
    char_allocator alloc(allocator);

    string u8(alloc);
    lowlevel_callback cb(utf32_to_utf8);
    return to_narrow(str, u8, allocator, cb);
}


u16string codec_utf32_utf16(const u32string_view& str,
    const byte_allocator& allocator)
{
    using char_allocator = allocator_traits<byte_allocator>::template rebind_alloc<char16_t>;
    char_allocator alloc(allocator);

    u16string u16(alloc);
    lowlevel_callback cb(utf32_to_utf16);
    return to_narrow(str, u16, allocator, cb);
}

PYCPP_END_NAMESPACE
