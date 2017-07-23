//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include <pycpp/codec.h>
#include <pycpp/unicode.h>

PYCPP_BEGIN_NAMESPACE

// HELPERS
// -------


/**
 *  \brief Convert narrow to wide encoding.
 */
template <typename S1, typename S2, typename Function>
static S2& to_wide(const S1& s1, S2& s2, Function function)
{
    // types
    typedef typename S1::value_type Char1;
    typedef typename S2::value_type Char2;

    // get parameters
    size_t srclen = s1.size() * sizeof(Char1);
    size_t dstlen = srclen * 4 / sizeof(Char1);
    const char* src = (const char*) s1.data();
    char* dst = new char[dstlen];
    const void* src_first = (const void*) src;
    void* dst_first = (void*) dst;

    // convert string
    function(src_first, srclen, dst_first, dstlen);
    size_t length = std::distance(dst, (char*) dst_first);
    s2 = S2((Char2*) dst, length / sizeof(Char2));
    delete[] dst;

    return s2;
}


/**
 *  \brief Convert wide to narrow encoding.
 */
template <typename S1, typename S2, typename Function>
static S2& to_narrow(const S1& s1, S2& s2, Function function)
{
    // types
    typedef typename S1::value_type Char1;
    typedef typename S2::value_type Char2;

    // get parameters
    // each utf16 to utf8 conversion can increase the buffer size,
    // while each utf32 to utf16/utf8 at most keeps the buffer size
    // constant.
    size_t srclen = s1.size() * sizeof(Char1);
    size_t dstlen = sizeof(Char1) == 2 ? static_cast<size_t>(srclen * 1.5) : srclen;
    const char* src = (const char*) s1.data();
    char* dst = new char[dstlen];
    const void* src_first = (const void*) src;
    void* dst_first = (void*) dst;

    // convert string
    function(src_first, srclen, dst_first, dstlen);
    size_t length = std::distance(dst, (char*) dst_first);
    s2 = S2((Char2*) dst, length / sizeof(Char2));
    delete[] dst;

    return s2;
}


// FUNCTIONS
// ---------


std::u16string codec_utf8_utf16(const std::string& str)
{
    std::u16string u16;
    return to_wide(str, u16, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf8_to_utf16(src, srclen, dst, dstlen);
    });
}


std::u32string codec_utf8_utf32(const std::string& str)
{
    std::u32string u32;
    return to_wide(str, u32, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf8_to_utf32(src, srclen, dst, dstlen);
    });
}


std::string codec_utf16_utf8(const std::u16string& str)
{
    std::string u8;
    return to_narrow(str, u8, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf16_to_utf8(src, srclen, dst, dstlen);
    });
}


std::u32string codec_utf16_utf32(const std::u16string& str)
{
    std::u32string u32;
    return to_wide(str, u32, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf16_to_utf32(src, srclen, dst, dstlen);
    });
}


std::string codec_utf32_utf8(const std::u32string& str)
{
    std::string u8;
    return to_narrow(str, u8, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf32_to_utf8(src, srclen, dst, dstlen);
    });
}


std::u16string codec_utf32_utf16(const std::u32string& str)
{
    std::u16string u16;
    return to_narrow(str, u16, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf32_to_utf16(src, srclen, dst, dstlen);
    });
}

PYCPP_END_NAMESPACE
