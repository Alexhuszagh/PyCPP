//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Codec unittests.
 */

#include <pycpp/byteorder.h>
#include <pycpp/codec.h>
#include <gtest/gtest.h>

// CONSTANTS
// ---------

// 한국어
static const std::string UTF8 = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
static const std::u16string UTF16 = {54620, 44397, 50612};
static const std::u32string UTF32 = {54620, 44397, 50612};

// räksmörgås
static const std::string UTF8_2 = {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115};
static const std::u16string UTF16_2 = {114,  228,  107,  115,  109,  246,  114,  103,  229,  115};
static const std::u32string UTF32_2 = {114, 228, 107, 115, 109, 246, 114, 103, 229, 115};

// "Même"
static const std::string UTF8_3 = {77, -61, -86, 109, 101};
static const std::u16string UTF16_3 = {77, 234, 109, 101};
static const std::u32string UTF32_3 = {77, 234, 109, 101};

// TESTS
// -----


TEST(codec, codepoint_conversions)
{
    // FROM UTF8
    EXPECT_EQ(codec_utf8_utf16(UTF8), UTF16);
    EXPECT_EQ(codec_utf8_utf16(UTF8_2), UTF16_2);
    EXPECT_EQ(codec_utf8_utf16(UTF8_3), UTF16_3);
    EXPECT_EQ(codec_utf8_utf32(UTF8), UTF32);
    EXPECT_EQ(codec_utf8_utf32(UTF8_2), UTF32_2);
    EXPECT_EQ(codec_utf8_utf32(UTF8_3), UTF32_3);

    // FROM UTF16
    EXPECT_EQ(codec_utf16_utf8(UTF16), UTF8);
    EXPECT_EQ(codec_utf16_utf8(UTF16_2), UTF8_2);
    EXPECT_EQ(codec_utf16_utf8(UTF16_3), UTF8_3);
    EXPECT_EQ(codec_utf16_utf32(UTF16), UTF32);
    EXPECT_EQ(codec_utf16_utf32(UTF16_2), UTF32_2);
    EXPECT_EQ(codec_utf16_utf32(UTF16_3), UTF32_3);

    // FROM UTF32
    EXPECT_EQ(codec_utf32_utf8(UTF32), UTF8);
    EXPECT_EQ(codec_utf32_utf8(UTF32_2), UTF8_2);
    EXPECT_EQ(codec_utf32_utf8(UTF32_3), UTF8_3);
    EXPECT_EQ(codec_utf32_utf16(UTF32), UTF16);
    EXPECT_EQ(codec_utf32_utf16(UTF32_2), UTF16_2);
    EXPECT_EQ(codec_utf32_utf16(UTF32_3), UTF16_3);
}
