//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Unicode unittests.
 */

#include "unicode.h"
#include <gtest/gtest.h>


// CONSTANTS
// ---------

// 한국어
static const std::string ASCII = {'H', 'a', 'n', 'g', 'u', 'l'};
static const std::string UTF8 = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
static const std::string UTF16 = {92, -43, 109, -83, -76, -59};
static const std::string UTF32 = {92, -43, 0, 0, 109, -83, 0, 0, -76, -59, 0, 0};

// räksmörgås
static const std::string UTF8_2 = {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115};
static const std::string UTF16_2 = {114, 0, -28, 0, 107, 0, 115, 0, 109, 0, -10, 0, 114, 0, 103, 0, -27, 0, 115, 0};
static const std::string UTF32_2 = {114, 0, 0, 0, -28, 0, 0, 0, 107, 0, 0, 0, 115, 0, 0, 0, 109, 0, 0, 0, -10, 0, 0, 0, 114, 0, 0, 0, 103, 0, 0, 0, -27, 0, 0, 0, 115, 0, 0, 0};

// TESTS
// -----


TEST(unicode, is_unicode)
{
    EXPECT_FALSE(is_unicode(ASCII));
    EXPECT_TRUE(is_unicode(UTF8));
    EXPECT_TRUE(is_unicode(UTF16));
    EXPECT_TRUE(is_unicode(UTF32));
    EXPECT_TRUE(is_unicode(UTF8_2));
    EXPECT_TRUE(is_unicode(UTF16_2));
    EXPECT_TRUE(is_unicode(UTF32_2));
}


TEST(unicode, codepoint_conversions)
{
    // FROM UTF8
    EXPECT_EQ(utf8_to_utf16(UTF8), UTF16);
    EXPECT_EQ(utf8_to_utf32(UTF8), UTF32);
    EXPECT_EQ(utf8_to_utf16(UTF8_2), UTF16_2);
    EXPECT_EQ(utf8_to_utf32(UTF8_2), UTF32_2);

    // FROM UTF16
    EXPECT_EQ(utf16_to_utf8(UTF16), UTF8);
    EXPECT_EQ(utf16_to_utf32(UTF16), UTF32);
    EXPECT_EQ(utf16_to_utf8(UTF16_2), UTF8_2);
    EXPECT_EQ(utf16_to_utf32(UTF16_2), UTF32_2);

    // FROM UTF32
    EXPECT_EQ(utf32_to_utf8(UTF32), UTF8);
    EXPECT_EQ(utf32_to_utf16(UTF32), UTF16);
    EXPECT_EQ(utf32_to_utf8(UTF32_2), UTF8_2);
    EXPECT_EQ(utf32_to_utf16(UTF32_2), UTF16_2);
}
