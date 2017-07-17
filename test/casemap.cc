//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Casemap conversion unittests.
 */

#include <pycpp/byteorder.h>
#include <pycpp/casemap.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// HELPERS
// -------

static void test_lowlevel(const std::string& input, const std::string& expected, casemap_lowlevel_callback cb)
{
    const char* src = input.data();
    char* dst = new char[20];
    const void* src_first = src;
    void* dst_first = dst;

    try {
        cb(src_first, input.size(), dst_first, 20);
        EXPECT_EQ(std::distance(dst, (char*) dst_first), expected.size());
        EXPECT_EQ(strncmp(dst, expected.data(), expected.size()), 0);
    } catch (...) {
        delete[] dst;
        throw;
    }

    delete[] dst;
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
    test_lowlevel("LOWER", "lower", [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        ascii_tolower(src, srclen, dst, dstlen);
    });
}


TEST(casemap, ascii_toupper)
{
    // high-level
    EXPECT_EQ(ascii_toupper("LOWER"), "LOWER");
    EXPECT_EQ(ascii_toupper("lower"), "LOWER");
    EXPECT_EQ(ascii_toupper("lower-/"), "LOWER-/");
    EXPECT_EQ(ascii_toupper("-/low+"), "-/LOW+");

    // low-level
    test_lowlevel("lower", "LOWER", [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        ascii_toupper(src, srclen, dst, dstlen);
    });
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
    test_lowlevel("lower", "Lower", [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        ascii_totitle(src, srclen, dst, dstlen);
    });
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
    test_lowlevel("aaaAA.aa", "Aaaaa.aa", [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        ascii_capitalize(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf8_tolower)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            std::string {109, -61, -86, 109, 101},
            std::string {109, -61, -86, 109, 101},
        },
        {
            std::string {77, -61, -118, 77, 69},
            std::string {109, -61, -86, 109, 101},
        },
        {
            std::string {77, -61, -86, 109, 101},
            std::string {109, -61, -86, 109, 101},
        },
        {
            std::string {-31, -70, -98},
            std::string {-61, -97},
        },
        {
            std::string {-60, -80},
            std::string {105},
        },
        {
            std::string {-56, -70},
            std::string {-30, -79, -91},
        },
        {
            std::string {-56, -66},
            std::string {-30, -79, -90},
        },
        {
            std::string {-30, -124, -90},
            std::string {-49, -119},
        },
        {
            std::string {-30, -124, -86},
            std::string {107},
        },
        {
            std::string {-30, -124, -85},
            std::string {-61, -91},
        },
        {
            std::string {-30, -79, -94},
            std::string {-55, -85},
        },
        {
            std::string {-30, -79, -92},
            std::string {-55, -67},
        },
        {
            std::string {-30, -79, -83},
            std::string {-55, -111},
        },
        {
            std::string {-30, -79, -82},
            std::string {-55, -79},
        },
        {
            std::string {-30, -79, -81},
            std::string {-55, -112},
        },
// TODO: these don't work
//        {
//            std::string {-30, -79, -80},
//            std::string {-55, -110},
//        },
//        {
//            std::string {-30, -79, -66},
//            std::string {-56, -65},
//        },
//        {
//            std::string {-30, -79, -65},
//            std::string {-55, -128},
//        },
//        {
//            std::string {-22, -98, -115},
//            std::string {-55, -91},
//        },
//        {
//            std::string {-22, -98, -86},
//            std::string {-55, -90},
//        },
//        {
//            std::string {-22, -98, -85},
//            std::string {-55, -100},
//        },
//        {
//            std::string {-22, -98, -84},
//            std::string {-55, -95},
//        },
//        {
//            std::string {-22, -98, -83},
//            std::string {-55, -84},
//        },
//        {
//            std::string {-22, -98, -82},
//            std::string {-55, -86},
//        },
//        {
//            std::string {-22, -98, -80},
//            std::string {-54, -98},
//        },
//        {
//            std::string {-22, -98, -79},
//            std::string {-54, -121},
//        },
//        {
//            std::string {-22, -98, -78},
//            std::string {-54, -99},
//        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_tolower(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf8_tolower(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf8_toupper)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            std::string {109, -61, -86, 109, 101},
            std::string {77, -61, -118, 77, 69},
        },
        {
            std::string {77, -61, -118, 77, 69},
            std::string {77, -61, -118, 77, 69},
        },
        {
            std::string {77, -61, -86, 109, 101},
            std::string {77, -61, -118, 77, 69},
        },
        {
            std::string {-60, -79},
            std::string {73},
        },
        {
            std::string {-59, -65},
            std::string {83},
        },
        {
            std::string {-55, -112},
            std::string {-30, -79, -81},
        },
        {
            std::string {-55, -111},
            std::string {-30, -79, -83},
        },
        {
            std::string {-55, -85},
            std::string {-30, -79, -94},
        },
        {
            std::string {-55, -79},
            std::string {-30, -79, -82},
        },
        {
            std::string {-55, -67},
            std::string {-30, -79, -92},
        },
        {
            std::string {-31, -66, -66},
            std::string {-50, -103},
        },
        {
            std::string {-30, -79, -91},
            std::string {-56, -70},
        },
        {
            std::string {-30, -79, -90},
            std::string {-56, -66},
        },
// TODO: These don't work
//        {
//            std::string {-61, -97},
//            std::string {-31, -70, -98},
//        },
//        {
//            std::string {-56, -65},
//            std::string {-30, -79, -66},
//        },
//        {
//            std::string {-55, -128},
//            std::string {-30, -79, -65},
//        },
//        {
//            std::string {-55, -110},
//            std::string {-30, -79, -80},
//        },
//        {
//            std::string {-55, -100},
//            std::string {-22, -98, -85},
//        },
//        {
//            std::string {-55, -95},
//            std::string {-22, -98, -84},
//        },
//        {
//            std::string {-55, -91},
//            std::string {-22, -98, -115},
//        },
//        {
//            std::string {-55, -90},
//            std::string {-22, -98, -86},
//        },
//        {
//            std::string {-55, -86},
//            std::string {-22, -98, -82},
//        },
//        {
//            std::string {-55, -84},
//            std::string {-22, -98, -83},
//        },
//        {
//            std::string {-54, -121},
//            std::string {-22, -98, -79},
//        },
//        {
//            std::string {-54, -99},
//            std::string {-22, -98, -78},
//        },
//        {
//            std::string {-54, -98},
//            std::string {-22, -98, -80},
//        },
//        {
//            std::string {-31, -78, -128},
//            std::string {-48, -110},
//        },
//        {
//            std::string {-31, -78, -127},
//            std::string {-48, -108},
//        },
//        {
//            std::string {-31, -78, -126},
//            std::string {-48, -98},
//        },
//        {
//            std::string {-31, -78, -125},
//            std::string {-48, -95},
//        },
//        {
//            std::string {-31, -78, -124},
//            std::string {-48, -94},
//        },
//        {
//            std::string {-31, -78, -123},
//            std::string {-48, -94},
//        },
//        {
//            std::string {-31, -78, -122},
//            std::string {-48, -86},
//        },
//        {
//            std::string {-31, -78, -121},
//            std::string {-47, -94},
//        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_toupper(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf8_toupper(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf8_totitle)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            std::string {109, -61, -86, 109, 101},
            std::string {77, -61, -86, 109, 101},
        },
        {
            std::string {77, -61, -118, 77, 69},
            std::string {77, -61, -86, 109, 101},
        },
        {
            std::string {77, -61, -86, 109, 101},
            std::string {77, -61, -86, 109, 101},
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_totitle(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf8_totitle(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf8_capitalize)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            std::string {109, -61, -86, 109, 101},
            std::string {77, -61, -86, 109, 101},
        },
        {
            std::string {77, -61, -118, 77, 69},
            std::string {77, -61, -86, 109, 101},
        },
        {
            std::string {77, -61, -86, 109, 101},
            std::string {77, -61, -86, 109, 101},
        },
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_capitalize(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf8_capitalize(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf16_tolower)
{
    std::vector<std::pair<std::string, std::string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            std::string {109, 0, -22, 0, 109, 0, 101, 0},
            std::string {109, 0, -22, 0, 109, 0, 101, 0},
        },
        {
            std::string {77, 0, -54, 0, 77, 0, 69, 0},
            std::string {109, 0, -22, 0, 109, 0, 101, 0},
        },
        {
            std::string {77, 0, -22, 0, 109, 0, 101, 0},
            std::string {109, 0, -22, 0, 109, 0, 101, 0},
        },
#else
        {
            std::string {0, 109, 0, -22, 0, 109, 0, 101},
            std::string {0, 109, 0, -22, 0, 109, 0, 101},
        },
        {
            std::string {0, 77, 0, -54, 0, 77, 0, 69},
            std::string {0, 109, 0, -22, 0, 109, 0, 101},
        },
        {
            std::string {0, 77, 0, -22, 0, 109, 0, 101},
            std::string {0, 109, 0, -22, 0, 109, 0, 101},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf16_tolower(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf16_tolower(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf16_toupper)
{
    std::vector<std::pair<std::string, std::string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            std::string {109, 0, -22, 0, 109, 0, 101, 0},
            std::string {77, 0, -54, 0, 77, 0, 69, 0},
        },
        {
            std::string {77, 0, -54, 0, 77, 0, 69, 0},
            std::string {77, 0, -54, 0, 77, 0, 69, 0},
        },
        {
            std::string {77, 0, -22, 0, 109, 0, 101, 0},
            std::string {77, 0, -54, 0, 77, 0, 69, 0},
        },
#else
        {
            std::string {0, 109, 0, -22, 0, 109, 0, 101},
            std::string {0, 77, 0, -54, 0, 77, 0, 69},
        },
        {
            std::string {0, 77, 0, -54, 0, 77, 0, 69},
            std::string {0, 77, 0, -54, 0, 77, 0, 69},
        },
        {
            std::string {0, 77, 0, -22, 0, 109, 0, 101},
            std::string {0, 77, 0, -54, 0, 77, 0, 69},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf16_toupper(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf16_toupper(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf16_totitle)
{
    std::vector<std::pair<std::string, std::string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            std::string {109, 0, -22, 0, 109, 0, 101, 0},
            std::string {77, 0, -22, 0, 109, 0, 101, 0},
        },
        {
            std::string {77, 0, -54, 0, 77, 0, 69, 0},
            std::string {77, 0, -22, 0, 109, 0, 101, 0},
        },
        {
            std::string {77, 0, -22, 0, 109, 0, 101, 0},
            std::string {77, 0, -22, 0, 109, 0, 101, 0},
        },
#else
        {
            std::string {0, 109, 0, -22, 0, 109, 0, 101},
            std::string {0, 77, 0, -22, 0, 109, 0, 101},
        },
        {
            std::string {0, 77, 0, -54, 0, 77, 0, 69},
            std::string {0, 77, 0, -22, 0, 109, 0, 101},
        },
        {
            std::string {0, 77, 0, -22, 0, 109, 0, 101},
            std::string {0, 77, 0, -22, 0, 109, 0, 101},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf16_totitle(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf16_totitle(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf32_tolower)
{
    // high-level
    std::vector<std::pair<std::string, std::string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            std::string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            std::string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
        {
            std::string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
            std::string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
        {
            std::string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            std::string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
#else
        {
            std::string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            std::string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
        {
            std::string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
            std::string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
        {
            std::string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            std::string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf32_tolower(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf32_tolower(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf32_toupper)
{
    // high-level
    std::vector<std::pair<std::string, std::string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            std::string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            std::string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
        },
        {
            std::string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
            std::string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
        },
        {
            std::string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            std::string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
        },
#else
        {
            std::string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            std::string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
        },
        {
            std::string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
            std::string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
        },
        {
            std::string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            std::string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf32_toupper(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf32_toupper(src, srclen, dst, dstlen);
    });
}


TEST(casemap, utf32_totitle)
{
    // high-level
    std::vector<std::pair<std::string, std::string>> tests = {
#if BYTE_ORDER == LITTLE_ENDIAN
        {
            std::string {109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            std::string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
        {
            std::string {77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69, 0, 0, 0},
            std::string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
        {
            std::string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
            std::string {77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101, 0, 0, 0},
        },
#else
        {
            std::string {0, 0, 0, 109, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            std::string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
        {
            std::string {0, 0, 0, 77, 0, 0, 0, -54, 0, 0, 0, 77, 0, 0, 0, 69},
            std::string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
        {
            std::string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
            std::string {0, 0, 0, 77, 0, 0, 0, -22, 0, 0, 0, 109, 0, 0, 0, 101},
        },
#endif
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf32_totitle(pair.first), pair.second);
    }

    // low-level
    test_lowlevel(tests[0].first, tests[0].second, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        utf32_totitle(src, srclen, dst, dstlen);
    });
}
