//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Punycode encoding and decoding unittests.
 */

#include "punycode.h"
#include <gtest/gtest.h>

// CONSTANTS
// ---------

// 한국어
static const std::string UTF8 = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
static const std::string UTF16 = {92, -43, 109, -83, -76, -59};
static const std::string UTF32 = {92, -43, 0, 0, 109, -83, 0, 0, -76, -59, 0, 0};

// räksmörgås
static const std::string UTF8_2 = {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115};
static const std::string UTF16_2 = {114, 0, -28, 0, 107, 0, 115, 0, 109, 0, -10, 0, 114, 0, 103, 0, -27, 0, 115, 0};
static const std::string UTF32_2 = {114, 0, 0, 0, -28, 0, 0, 0, 107, 0, 0, 0, 115, 0, 0, 0, 109, 0, 0, 0, -10, 0, 0, 0, 114, 0, 0, 0, 103, 0, 0, 0, -27, 0, 0, 0, 115, 0, 0, 0};

// TESTS
// -----


TEST(punycode, punycode)
{
    std::string expected = "3e0bk47br7k";
    EXPECT_EQ(utf8_to_punycode(UTF8), expected);
    EXPECT_EQ(utf16_to_punycode(UTF16), expected);
    EXPECT_EQ(utf32_to_punycode(UTF32), expected);
//    EXPECT_EQ(punycode_to_utf8(expected), UTF8);
//    EXPECT_EQ(punycode_to_utf16(expected), UTF16);
//    EXPECT_EQ(punycode_to_utf32(expected), UTF32);

    expected = "rksmrgs-5wao1o";
    EXPECT_EQ(utf8_to_punycode(UTF8_2), expected);
    EXPECT_EQ(utf16_to_punycode(UTF16_2), expected);
    EXPECT_EQ(utf32_to_punycode(UTF32_2), expected);
}
