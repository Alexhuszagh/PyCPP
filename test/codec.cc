//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Codec unittests.
 */

#include "byteorder.h"
#include "codec.h"
#include <gtest/gtest.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>

// CONSTANTS
// ---------

// 한국어
static const std::string UTF8 = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::u16string UTF16 = {-10916, -21139, -14924};
static const std::u32string UTF32 = {54620, 44397, 50612};
#else
static const std::u16string UTF16 = {23765,  28077, -19259};
static const std::u32string UTF32 = {1557463040,  1840054272, -1262157824};
#endif

// räksmörgås
static const std::string UTF8_2 = {114, -61, -92, 107, 115, 109, -61, -74, 114, 103, -61, -91, 115};
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::u16string UTF16_2 = {114,  228,  107,  115,  109,  246,  114,  103,  229,  115};
static const std::u32string UTF32_2 = {114, 228, 107, 115, 109, 246, 114, 103, 229, 115};
#else
static const std::u16string UTF16_2 = {29184, -7168, 27392, 29440, 27904, -2560, 29184, 26368, -6912, 29440};
static const std::u32string UTF32_2 = {1912602624, -469762048, 1795162112, 1929379840, 1828716544, -167772160, 1912602624, 1728053248, -452984832, 1929379840};
#endif

// "Même"
static const std::string UTF8_3 = {77, -61, -86, 109, 101};
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::u16string UTF16_3 = {77, 234, 109, 101};
static const std::u32string UTF32_3 = {77, 234, 109, 101};
#else
static const std::u16string UTF16_3 = {19712, -5632, 27904, 25856};
static const std::u32string UTF32_3 = {1291845632, -369098752, 1828716544, 1694498816};
#endif

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

#include <warnings/pop.h>
