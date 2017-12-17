//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
//  :copyright: (c) 2013-2017 Kim Davies.
//  :license: CC By 4.0, see licenses/cc-by-4.0.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Punycode encoding and decoding unittests.
 */

#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>
#include <pycpp/string/punycode.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// CONSTANTS
// ---------

// 한국어
static const std::string ASCII = {'H', 'a', 'n', 'g', 'u', 'l'};
static const std::string UTF8 = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::string UTF16 = {92, -43, 109, -83, -76, -59};
static const std::string UTF32 = {92, -43, 0, 0, 109, -83, 0, 0, -76, -59, 0, 0};
#else
static const std::string UTF16 = {-43, 92, -83, 109, -59, -76};
static const std::string UTF32 = {0, 0, -43, 92, 0, 0, -83, 109, 0, 0, -59, -76};
#endif

// räksmörgås
static const std::string UTF8_2 = {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115};
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::string UTF16_2 = {114, 0, -28, 0, 107, 0, 115, 0, 109, 0, -10, 0, 114, 0, 103, 0, -27, 0, 115, 0};
static const std::string UTF32_2 = {114, 0, 0, 0, -28, 0, 0, 0, 107, 0, 0, 0, 115, 0, 0, 0, 109, 0, 0, 0, -10, 0, 0, 0, 114, 0, 0, 0, 103, 0, 0, 0, -27, 0, 0, 0, 115, 0, 0, 0};
#else
static const std::string UTF16_2 = {0, 114, 0, -28, 0, 107, 0, 115, 0, 109, 0, -10, 0, 114, 0, 103, 0, -27, 0, 115};
static const std::string UTF32_2 = {0, 0, 0, 114, 0, 0, 0, -28, 0, 0, 0, 107, 0, 0, 0, 115, 0, 0, 0, 109, 0, 0, 0, -10, 0, 0, 0, 114, 0, 0, 0, 103, 0, 0, 0, -27, 0, 0, 0, 115};
#endif

// même
static const std::string UTF8_3 = {109, -61, -86, 109, 101};
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::string UTF16_3 = {109, 0, -22, 0, 109, 0, 101, 0};
static const std::string UTF32_3 = {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0};
#else
static const std::string UTF16_3 = {0, 109, 0, -22, 0, 109, 0, 101};
static const std::string UTF32_3 = {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101};
#endif

// HELPERS
// -------

static void test_lowlevel(const std::string& input, const std::string& expected, punycode_lowlevel_callback cb)
{
    const char* src = input.data();
    char* dst = nullptr;

    try {
        dst = new char[20];
        const void* src_first = src;
        void* dst_first = dst;
        cb(src_first, input.size(), dst_first, 20);
        EXPECT_EQ(distance(dst, (char*) dst_first), expected.size());
        EXPECT_EQ(strncmp(dst, expected.data(), expected.size()), 0);
    } catch (...) {
        delete[] dst;
        throw;
    }

    delete[] dst;
}

// TESTS
// -----


TEST(punycode, punycode)
{
    std::string expected = "Hangul-";
    EXPECT_EQ(utf8_to_punycode(ASCII), expected);

    expected = "3e0bk47br7k";
    EXPECT_EQ(utf8_to_punycode(UTF8), expected);
    EXPECT_EQ(utf16_to_punycode(UTF16), expected);
    EXPECT_EQ(utf32_to_punycode(UTF32), expected);
    EXPECT_EQ(punycode_to_utf8(expected), UTF8);
    EXPECT_EQ(punycode_to_utf16(expected), UTF16);
    EXPECT_EQ(punycode_to_utf32(expected), UTF32);

    expected = "rksmrgs-5wao1o";
    EXPECT_EQ(utf8_to_punycode(UTF8_2), expected);
    EXPECT_EQ(utf16_to_punycode(UTF16_2), expected);
    EXPECT_EQ(utf32_to_punycode(UTF32_2), expected);
    EXPECT_EQ(punycode_to_utf8(expected), UTF8_2);
    EXPECT_EQ(punycode_to_utf16(expected), UTF16_2);
    EXPECT_EQ(punycode_to_utf32(expected), UTF32_2);

    expected = "mme-fma";
    EXPECT_EQ(utf8_to_punycode(UTF8_3), expected);
    EXPECT_EQ(utf16_to_punycode(UTF16_3), expected);
    EXPECT_EQ(utf32_to_punycode(UTF32_3), expected);
    EXPECT_EQ(punycode_to_utf8(expected), UTF8_3);
    EXPECT_EQ(punycode_to_utf16(expected), UTF16_3);
    EXPECT_EQ(punycode_to_utf32(expected), UTF32_3);
}


TEST(punycode, utf8_to_punycode)
{
    test_lowlevel(UTF8, "3e0bk47br7k", [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf8_to_punycode(src, srclen, dst, dstlen);
    });
}


TEST(punycode, utf16_to_punycode)
{
    test_lowlevel(UTF16, "3e0bk47br7k", [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf16_to_punycode(src, srclen, dst, dstlen);
    });
}


TEST(punycode, utf32_to_punycode)
{
    test_lowlevel(UTF32, "3e0bk47br7k", [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf32_to_punycode(src, srclen, dst, dstlen);
    });
}


TEST(punycode, punycode_to_utf8)
{
    test_lowlevel("3e0bk47br7k", UTF8, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        punycode_to_utf8(src, srclen, dst, dstlen);
    });
}


TEST(punycode, punycode_to_utf16)
{
    test_lowlevel("3e0bk47br7k", UTF16, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        punycode_to_utf16(src, srclen, dst, dstlen);
    });
}


TEST(punycode, punycode_to_utf32)
{
    test_lowlevel("3e0bk47br7k", UTF32, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        punycode_to_utf32(src, srclen, dst, dstlen);
    });
}


/**
 *  This comprehensive unittest for Punycode enccoding is under
 *  the BSD 3-clause license and subject to the Unicode license, the rest
 *  of the module is MIT licensed.
 *
 *  The original project can be found here:
 *      https://github.com/kjd/idna
 */
TEST(punycode, sequences)
{
    vector<pair<std::string, std::string>> tests = {
        {
            std::string {-26, -75, -117, -24, -81, -107},
            "0zwm56d",
        },
        {
            std::string {-32, -92, -86, -32, -92, -80, -32, -91, -128, -32, -92, -107, -32, -91, -115, -32, -92, -73, -32, -92, -66},
            "11b5bs3a9aj6g"
        },
        {
            std::string {-19, -107, -100, -22, -75, -83},
            "3e0b707e"
        },
        {
            std::string {-32, -90, -83, -32, -90, -66, -32, -90, -80, -32, -90, -92},
            "45brj9c"
        },
        {
            std::string {-32, -90, -84, -32, -90, -66, -32, -90, -126, -32, -90, -78, -32, -90, -66},
            "54b7fta0cc"
        },
        {
            std::string {-48, -72, -47, -127, -48, -65, -47, -117, -47, -126, -48, -80, -48, -67, -48, -72, -48, -75},
            "80akhbyknj4f",
        },
        {
            std::string {-47, -127, -47, -128, -48, -79},
            "90a3ac",
        },
                {
            std::string {-19, -123, -116, -20, -118, -92, -19, -118, -72},
            "9t4b11yi5a",
        },
        {
            std::string {-32, -82, -102, -32, -82, -65, -32, -82, -103, -32, -81, -115, -32, -82, -107, -32, -82, -86, -32, -81, -115, -32, -82, -86, -32, -81, -126, -32, -82, -80, -32, -81, -115},
            "clchc0ea0b2g2a9gcd",
        },
        {
            std::string {-41, -104, -41, -94, -41, -95, -41, -104},
            "deba0ad",
        },
        {
            std::string {-28, -72, -83, -27, -101, -67},
            "fiqs8s",
        },
        {
            std::string {-28, -72, -83, -27, -100, -117},
            "fiqz9s",
        },
        {
            std::string {-32, -80, -83, -32, -80, -66, -32, -80, -80, -32, -80, -92, -32, -79, -115},
            "fpcrj9c3d",
        },
        {
            std::string {-32, -74, -67, -32, -74, -126, -32, -74, -102, -32, -73, -113},
            "fzc2c9e2c",
        },
        {
            std::string {-26, -72, -84, -24, -87, -90},
            "g6w251d",
        },
        {
            std::string {-32, -86, -83, -32, -86, -66, -32, -86, -80, -32, -86, -92},
            "gecrj9c",
        },
        {
            std::string {-32, -92, -83, -32, -92, -66, -32, -92, -80, -32, -92, -92},
            "h2brj9c",
        },
        {
            std::string {-40, -94, -40, -78, -39, -123, -40, -89, -37, -116, -40, -76, -37, -116},
            "hgbk6aj7f53bba",
        },
        {
            std::string {-32, -82, -86, -32, -82, -80, -32, -82, -65, -32, -82, -97, -32, -81, -115, -32, -82, -102, -32, -81, -120},
            "hlcj6aya9esc7a",
        },
        {
            std::string {-47, -125, -48, -70, -47, -128},
            "j1amh",
        },
        {
            std::string {-23, -90, -103, -26, -72, -81},
            "j6w193g",
        },
        {
            std::string {-50, -76, -50, -65, -50, -70, -50, -71, -50, -68, -50, -82},
            "jxalpdlp",
        },
        {
            std::string {-40, -91, -40, -82, -40, -86, -40, -88, -40, -89, -40, -79},
            "kgbechtv",
        },
        {
            std::string {-27, -113, -80, -26, -71, -66},
            "kprw13d",
        },
        {
            std::string {-27, -113, -80, -25, -127, -93},
            "kpry57d",
        },
        {
            std::string {-40, -89, -39, -124, -40, -84, -40, -78, -40, -89, -40, -90, -40, -79},
            "lgbbat1ad8j",
        },
        {
            std::string {-40, -71, -39, -123, -40, -89, -39, -122},
            "mgb9awbf",
        },
        {
            std::string {-40, -89, -37, -116, -40, -79, -40, -89, -39, -122},
            "mgba3a4f16a",
        },
        {
            std::string {-40, -89, -39, -123, -40, -89, -40, -79, -40, -89, -40, -86},
            "mgbaam7a8h",
        },
        {
            std::string {-39, -66, -40, -89, -38, -87, -40, -77, -40, -86, -40, -89, -39, -122},
            "mgbai9azgqp6j",
        },
        {
            std::string {-40, -89, -39, -124, -40, -89, -40, -79, -40, -81, -39, -122},
            "mgbayh7gpa",
        },
        {
            std::string {-40, -88, -38, -66, -40, -89, -40, -79, -40, -86},
            "mgbbh1a71e",
        },
        {
            std::string {-40, -89, -39, -124, -39, -123, -40, -70, -40, -79, -40, -88},
            "mgbc0a9azcg",
        },
        {
            std::string {-40, -89, -39, -124, -40, -77, -40, -71, -39, -120, -40, -81, -39, -118, -40, -87},
            "mgberp4a5d4ar",
        },
        {
            std::string {-31, -125, -110, -31, -125, -108},
            "node",
        },
        {
            std::string {-32, -71, -124, -32, -72, -105, -32, -72, -94},
            "o3cw4h",
        },
        {
            std::string {-40, -77, -39, -120, -40, -79, -39, -118, -40, -87},
            "ogbpf8fl",
        },
        {
            std::string {-47, -128, -47, -124},
            "p1ai",
        },
        {
            std::string {-40, -86, -39, -120, -39, -122, -40, -77},
            "pgbs0dh",
        },
        {
            std::string {-32, -88, -83, -32, -88, -66, -32, -88, -80, -32, -88, -92},
            "s9brj9c",
        },
        {
            std::string {-39, -123, -40, -75, -40, -79},
            "wgbh1c",
        },
        {
            std::string {-39, -126, -40, -73, -40, -79},
            "wgbl6a",
        },
        {
            std::string {-32, -82, -121, -32, -82, -78, -32, -82, -103, -32, -81, -115, -32, -82, -107, -32, -81, -120},
            "xkc2al3hye2a",
        },
        {
            std::string {-32, -82, -121, -32, -82, -88, -32, -81, -115, -32, -82, -92, -32, -82, -65, -32, -82, -81, -32, -82, -66},
            "xkc2dl3a5ee0h",
        },
        {
            std::string {-26, -106, -80, -27, -118, -96, -27, -99, -95},
            "yfro4i67o",
        },
        {
            std::string {-39, -127, -39, -124, -40, -77, -40, -73, -39, -118, -39, -122},
            "ygbi2ammx",
        },
        {
            std::string {-29, -125, -122, -29, -126, -71, -29, -125, -120},
            "zckzah",
        },
        {
            std::string {-46, -101, -48, -80, -48, -73},
            "80ao21a",
        },
        {
            std::string {-39, -123, -39, -124, -39, -118, -40, -77, -39, -118, -40, -89},
            "mgbx4cd0ab",
        },
        {
            std::string {-48, -68, -48, -66, -48, -67},
            "l1acc",
        },
        {
            std::string {-40, -77, -39, -120, -40, -81, -40, -89, -39, -122},
            "mgbpl2fh",
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_to_punycode(pair.first), pair.second);
        EXPECT_EQ(punycode_to_utf8(pair.second), pair.first);
    }
}
