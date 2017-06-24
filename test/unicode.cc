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


// TESTS
// -----


TEST(unicode, is_unicode)
{
    EXPECT_FALSE(is_unicode(ASCII));
    EXPECT_TRUE(is_unicode(UTF8));
    EXPECT_TRUE(is_unicode(UTF16));
    EXPECT_TRUE(is_unicode(UTF32));
}


TEST(unicode, codepoint_conversions)
{
    // FROM UTF8
    EXPECT_EQ(UTF32, utf8_to_utf32(UTF8));
    EXPECT_EQ(UTF16, utf8_to_utf16(UTF8));

    // FROM UTF16
    EXPECT_EQ(UTF8, utf32_to_utf8(UTF32));
    EXPECT_EQ(UTF16, utf32_to_utf16(UTF32));

    // FROM UTF32
    EXPECT_EQ(UTF8, utf32_to_utf8(UTF32));
    EXPECT_EQ(UTF16, utf32_to_utf16(UTF32));
}
