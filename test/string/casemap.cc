//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Casemap conversion unittests.
 */

#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>
#include <pycpp/string/casemap.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// HELPERS
// -------

static void test_lowlevel(const string& input,
    const string& expected,
    casemap_lowlevel_callback cb)
{
    byte_allocator alloc;
    const char* src = input.data();
    char* dst = nullptr;
    size_t dstlen = 20;

    try {
        dst = (char*) alloc.allocate(dstlen);
        const void* src_first = src;
        void* dst_first = dst;
        cb(src_first, input.size(), dst_first, dstlen, alloc);
        EXPECT_EQ(distance(dst, (char*) dst_first), expected.size());
        EXPECT_EQ(strncmp(dst, expected.data(), expected.size()), 0);
    } catch (...) {
        if (dst) {
            alloc.deallocate((byte*) dst, dstlen);
        }
        throw;
    }

    alloc.deallocate((byte*) dst, dstlen);
}

// TESTS
// -----

TEST(casemap, ascii_tolower)
{
    // high-level
    EXPECT_EQ(ascii_tolower("lower"), "lower");
    EXPECT_EQ(ascii_tolower("LOWER"), "lower");
    EXPECT_EQ(ascii_tolower("LOWER-/"), "lower-/");
    EXPECT_EQ(ascii_tolower("-/LOW+"), "-/low+");

    // low-level
    test_lowlevel("LOWER", "lower", casemap_lowlevel_callback(ascii_tolower));
}


TEST(casemap, ascii_toupper)
{
    // high-level
    EXPECT_EQ(ascii_toupper("LOWER"), "LOWER");
    EXPECT_EQ(ascii_toupper("lower"), "LOWER");
    EXPECT_EQ(ascii_toupper("lower-/"), "LOWER-/");
    EXPECT_EQ(ascii_toupper("-/low+"), "-/LOW+");

    // low-level
    test_lowlevel("lower", "LOWER", casemap_lowlevel_callback(ascii_toupper));
}


TEST(casemap, ascii_totitle)
{
    // high-level
    EXPECT_EQ(ascii_totitle("LOWER"), "Lower");
    EXPECT_EQ(ascii_totitle("lower"), "Lower");
    EXPECT_EQ(ascii_totitle("lower-/"), "Lower-/");
    EXPECT_EQ(ascii_totitle("-/low+"), "-/Low+");
    EXPECT_EQ(ascii_totitle("aaaAA0aa"), "Aaaaa0aa");
    EXPECT_EQ(ascii_totitle("aaaAA.aa"), "Aaaaa.Aa");

    // low-level
    test_lowlevel("lower", "Lower", casemap_lowlevel_callback(ascii_totitle));
}


TEST(casemap, ascii_capitalize)
{
    // high-level
    EXPECT_EQ(ascii_capitalize("LOWER"), "Lower");
    EXPECT_EQ(ascii_capitalize("lower"), "Lower");
    EXPECT_EQ(ascii_capitalize("lower-/"), "Lower-/");
    EXPECT_EQ(ascii_capitalize("-/low+"), "-/low+");
    EXPECT_EQ(ascii_capitalize("aaaAA0aa"), "Aaaaa0aa");
    EXPECT_EQ(ascii_capitalize("aaaAA.aa"), "Aaaaa.aa");

    // low-level
    test_lowlevel("aaaAA.aa", "Aaaaa.aa", casemap_lowlevel_callback(ascii_capitalize));
}


TEST(casemap, utf8_tolower)
{
    vector<pair<string, string>> tests = {
        {
            string {109, -61, -86, 109, 101},
            string {109, -61, -86, 109, 101},
        },
        {
            string {77, -61, -118, 77, 69},
            string {109, -61, -86, 109, 101},
        },
        {
            string {77, -61, -86, 109, 101},
            string {109, -61, -86, 109, 101},
        },
        {
            string {-31, -70, -98},
            string {-61, -97},
        },
        {
            string {-60, -80},
            string {105},
        },
        {
            string {-56, -70},
            string {-30, -79, -91},
        },
        {
            string {-56, -66},
            string {-30, -79, -90},
        },
        {
            string {-30, -124, -90},
            string {-49, -119},
        },
        {
            string {-30, -124, -86},
            string {107},
        },
        {
            string {-30, -124, -85},
            string {-61, -91},
        },
        {
            string {-30, -79, -94},
            string {-55, -85},
        },
        {
            string {-30, -79, -92},
            string {-55, -67},
        },
        {
            string {-30, -79, -83},
            string {-55, -111},
        },
        {
            string {-30, -79, -82},
            string {-55, -79},
        },
        {
            string {-30, -79, -81},
            string {-55, -112},
        },
// TODO: these don't work
//        {
//            string {-30, -79, -80},
//            string {-55, -110},
//        },
//        {
//            string {-30, -79, -66},
//            string {-56, -65},
//        },
//        {
//            string {-30, -79, -65},
//            string {-55, -128},
//        },
//        {
//            string {-22, -98, -115},
//            string {-55, -91},
//        },
//        {
//            string {-22, -98, -86},
//            string {-55, -90},
//        },
//        {
//            string {-22, -98, -85},
//            string {-55, -100},
//        },
//        {
//            string {-22, -98, -84},
//            string {-55, -95},
//        },
//        {
//            string {-22, -98, -83},
//            string {-55, -84},
//        },
//        {
//            string {-22, -98, -82},
//            string {-55, -86},
//        },
//        {
//            string {-22, -98, -80},
//            string {-54, -98},
//        },
//        {
//            string {-22, -98, -79},
//            string {-54, -121},
//        },
//        {
//            string {-22, -98, -78},
//            string {-54, -99},
//        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_tolower(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf8_tolower));
}


TEST(casemap, utf8_toupper)
{
    vector<pair<string, string>> tests = {
        {
            string {109, -61, -86, 109, 101},
            string {77, -61, -118, 77, 69},
        },
        {
            string {77, -61, -118, 77, 69},
            string {77, -61, -118, 77, 69},
        },
        {
            string {77, -61, -86, 109, 101},
            string {77, -61, -118, 77, 69},
        },
        {
            string {-60, -79},
            string {73},
        },
        {
            string {-59, -65},
            string {83},
        },
        {
            string {-55, -112},
            string {-30, -79, -81},
        },
        {
            string {-55, -111},
            string {-30, -79, -83},
        },
        {
            string {-55, -85},
            string {-30, -79, -94},
        },
        {
            string {-55, -79},
            string {-30, -79, -82},
        },
        {
            string {-55, -67},
            string {-30, -79, -92},
        },
        {
            string {-31, -66, -66},
            string {-50, -103},
        },
        {
            string {-30, -79, -91},
            string {-56, -70},
        },
        {
            string {-30, -79, -90},
            string {-56, -66},
        },
// TODO: These don't work
//        {
//            string {-61, -97},
//            string {-31, -70, -98},
//        },
//        {
//            string {-56, -65},
//            string {-30, -79, -66},
//        },
//        {
//            string {-55, -128},
//            string {-30, -79, -65},
//        },
//        {
//            string {-55, -110},
//            string {-30, -79, -80},
//        },
//        {
//            string {-55, -100},
//            string {-22, -98, -85},
//        },
//        {
//            string {-55, -95},
//            string {-22, -98, -84},
//        },
//        {
//            string {-55, -91},
//            string {-22, -98, -115},
//        },
//        {
//            string {-55, -90},
//            string {-22, -98, -86},
//        },
//        {
//            string {-55, -86},
//            string {-22, -98, -82},
//        },
//        {
//            string {-55, -84},
//            string {-22, -98, -83},
//        },
//        {
//            string {-54, -121},
//            string {-22, -98, -79},
//        },
//        {
//            string {-54, -99},
//            string {-22, -98, -78},
//        },
//        {
//            string {-54, -98},
//            string {-22, -98, -80},
//        },
//        {
//            string {-31, -78, -128},
//            string {-48, -110},
//        },
//        {
//            string {-31, -78, -127},
//            string {-48, -108},
//        },
//        {
//            string {-31, -78, -126},
//            string {-48, -98},
//        },
//        {
//            string {-31, -78, -125},
//            string {-48, -95},
//        },
//        {
//            string {-31, -78, -124},
//            string {-48, -94},
//        },
//        {
//            string {-31, -78, -123},
//            string {-48, -94},
//        },
//        {
//            string {-31, -78, -122},
//            string {-48, -86},
//        },
//        {
//            string {-31, -78, -121},
//            string {-47, -94},
//        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_toupper(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf8_toupper));
}


TEST(casemap, utf8_totitle)
{
    vector<pair<string, string>> tests = {
        {
            string {109, -61, -86, 109, 101},
            string {77, -61, -86, 109, 101},
        },
        {
            string {77, -61, -118, 77, 69},
            string {77, -61, -86, 109, 101},
        },
        {
            string {77, -61, -86, 109, 101},
            string {77, -61, -86, 109, 101},
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_totitle(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf8_totitle));
}


TEST(casemap, utf8_capitalize)
{
    vector<pair<string, string>> tests = {
        {
            string {109, -61, -86, 109, 101},
            string {77, -61, -86, 109, 101},
        },
        {
            string {77, -61, -118, 77, 69},
            string {77, -61, -86, 109, 101},
        },
        {
            string {77, -61, -86, 109, 101},
            string {77, -61, -86, 109, 101},
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_capitalize(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf8_capitalize));
}


TEST(casemap, utf16_tolower)
{
    vector<pair<string, string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            string {109, 0, -22, 0, 109, 0, 101, 0},
            string {109, 0, -22, 0, 109, 0, 101, 0},
        },
        {
            string {77, 0, -54, 0, 77, 0, 69, 0},
            string {109, 0, -22, 0, 109, 0, 101, 0},
        },
        {
            string {77, 0, -22, 0, 109, 0, 101, 0},
            string {109, 0, -22, 0, 109, 0, 101, 0},
        },
#else
        {
            string {0, 109, 0, -22, 0, 109, 0, 101},
            string {0, 109, 0, -22, 0, 109, 0, 101},
        },
        {
            string {0, 77, 0, -54, 0, 77, 0, 69},
            string {0, 109, 0, -22, 0, 109, 0, 101},
        },
        {
            string {0, 77, 0, -22, 0, 109, 0, 101},
            string {0, 109, 0, -22, 0, 109, 0, 101},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf16_tolower(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf16_tolower));
}


TEST(casemap, utf16_toupper)
{
    vector<pair<string, string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            string {109, 0, -22, 0, 109, 0, 101, 0},
            string {77, 0, -54, 0, 77, 0, 69, 0},
        },
        {
            string {77, 0, -54, 0, 77, 0, 69, 0},
            string {77, 0, -54, 0, 77, 0, 69, 0},
        },
        {
            string {77, 0, -22, 0, 109, 0, 101, 0},
            string {77, 0, -54, 0, 77, 0, 69, 0},
        },
#else
        {
            string {0, 109, 0, -22, 0, 109, 0, 101},
            string {0, 77, 0, -54, 0, 77, 0, 69},
        },
        {
            string {0, 77, 0, -54, 0, 77, 0, 69},
            string {0, 77, 0, -54, 0, 77, 0, 69},
        },
        {
            string {0, 77, 0, -22, 0, 109, 0, 101},
            string {0, 77, 0, -54, 0, 77, 0, 69},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf16_toupper(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf16_toupper));
}


TEST(casemap, utf16_totitle)
{
    vector<pair<string, string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            string {109, 0, -22, 0, 109, 0, 101, 0},
            string {77, 0, -22, 0, 109, 0, 101, 0},
        },
        {
            string {77, 0, -54, 0, 77, 0, 69, 0},
            string {77, 0, -22, 0, 109, 0, 101, 0},
        },
        {
            string {77, 0, -22, 0, 109, 0, 101, 0},
            string {77, 0, -22, 0, 109, 0, 101, 0},
        },
#else
        {
            string {0, 109, 0, -22, 0, 109, 0, 101},
            string {0, 77, 0, -22, 0, 109, 0, 101},
        },
        {
            string {0, 77, 0, -54, 0, 77, 0, 69},
            string {0, 77, 0, -22, 0, 109, 0, 101},
        },
        {
            string {0, 77, 0, -22, 0, 109, 0, 101},
            string {0, 77, 0, -22, 0, 109, 0, 101},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf16_totitle(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf16_totitle));
}


TEST(casemap, utf32_tolower)
{
    // high-level
    vector<pair<string, string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
        {
            string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
            string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
        {
            string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
#else
        {
            string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
        {
            string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
            string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
        {
            string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf32_tolower(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf32_tolower));
}


TEST(casemap, utf32_toupper)
{
    // high-level
    vector<pair<string, string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
        },
        {
            string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
            string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
        },
        {
            string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
        },
#else
        {
            string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
        },
        {
            string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
            string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
        },
        {
            string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf32_toupper(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf32_toupper));
}


TEST(casemap, utf32_totitle)
{
    // high-level
    vector<pair<string, string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
        {
            string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
            string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
        {
            string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
#else
        {
            string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
        {
            string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
            string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
        {
            string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf32_totitle(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, casemap_lowlevel_callback(utf32_totitle));
}
