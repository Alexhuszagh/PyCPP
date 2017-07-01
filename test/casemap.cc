//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Casemap conversion unittests.
 */

#include <byteorder.h>
#include <casemap.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(casemap, ascii_tolower)
{
    EXPECT_EQ(ascii_tolower("lower"), "lower");
    EXPECT_EQ(ascii_tolower("LOWER"), "lower");
    EXPECT_EQ(ascii_tolower("LOWER-/"), "lower-/");
    EXPECT_EQ(ascii_tolower("-/LOW+"), "-/low+");
}


TEST(casemap, ascii_toupper)
{
    EXPECT_EQ(ascii_toupper("LOWER"), "LOWER");
    EXPECT_EQ(ascii_toupper("lower"), "LOWER");
    EXPECT_EQ(ascii_toupper("lower-/"), "LOWER-/");
    EXPECT_EQ(ascii_toupper("-/low+"), "-/LOW+");
}


TEST(casemap, ascii_totitle)
{
    EXPECT_EQ(ascii_totitle("LOWER"), "Lower");
    EXPECT_EQ(ascii_totitle("lower"), "Lower");
    EXPECT_EQ(ascii_totitle("lower-/"), "Lower-/");
    EXPECT_EQ(ascii_totitle("-/low+"), "-/Low+");
    EXPECT_EQ(ascii_totitle("aaaAA0aa"), "Aaaaa0aa");
    EXPECT_EQ(ascii_totitle("aaaAA.aa"), "Aaaaa.Aa");
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
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_tolower(pair.first), pair.second);
    }
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
    };

    for (const auto &pair: tests) {
        EXPECT_EQ(utf8_toupper(pair.first), pair.second);
    }
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
}


TEST(casemap, utf32_tolower)
{
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
}


TEST(casemap, utf32_toupper)
{
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
}


TEST(casemap, utf32_totitle)
{
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
}
