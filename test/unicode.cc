//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
//  :copyright: (c) 2015 Markus Kuhn.
//  :license: CC By 4.0, see licenses/cc-by-4.0.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Unicode unittests.
 */

#include "unicode.h"
#include <gtest/gtest.h>

#include <utility>
#include <vector>

// CONSTANTS
// ---------

// 한국어
static const std::string ASCII = {'H', 'a', 'n', 'g', 'u', 'l'};
static const std::string HAS_NULL = {'\0', 'a', 'n', 'g', 'u', 'l'};
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

// TESTS
// -----


TEST(unicode, is_ascii)
{
    EXPECT_TRUE(is_ascii(0x00));
    EXPECT_TRUE(is_ascii(0x7f));
    EXPECT_FALSE(is_ascii(0xff));
    EXPECT_FALSE(is_ascii(0x100));
    EXPECT_FALSE(is_ascii(0x10FFFF));
    EXPECT_FALSE(is_ascii(0x1FFFFF));

    EXPECT_TRUE(is_ascii(ASCII));
    EXPECT_FALSE(is_ascii(HAS_NULL));
    EXPECT_FALSE(is_ascii(UTF8));
    EXPECT_FALSE(is_ascii(UTF16));
    EXPECT_FALSE(is_ascii(UTF32));
    EXPECT_FALSE(is_ascii(UTF8_2));
    EXPECT_FALSE(is_ascii(UTF16_2));
    EXPECT_FALSE(is_ascii(UTF32_2));
    EXPECT_FALSE(is_ascii(UTF8_3));
    EXPECT_FALSE(is_ascii(UTF16_3));
    EXPECT_FALSE(is_ascii(UTF32_3));
}


TEST(unicode, is_unicode)
{
    EXPECT_TRUE(is_unicode(0x00));
    EXPECT_TRUE(is_unicode(0x7f));
    EXPECT_TRUE(is_unicode(0xff));
    EXPECT_TRUE(is_unicode(0x100));
    EXPECT_TRUE(is_unicode(0x10FFFF));
    EXPECT_FALSE(is_unicode(0x1FFFFF));

    EXPECT_FALSE(is_unicode(ASCII));
    EXPECT_TRUE(is_unicode(HAS_NULL));
    EXPECT_TRUE(is_unicode(UTF8));
    EXPECT_TRUE(is_unicode(UTF16));
    EXPECT_TRUE(is_unicode(UTF32));
    EXPECT_TRUE(is_unicode(UTF8_2));
    EXPECT_TRUE(is_unicode(UTF16_2));
    EXPECT_TRUE(is_unicode(UTF32_2));
    EXPECT_TRUE(is_unicode(UTF8_3));
    EXPECT_TRUE(is_unicode(UTF16_3));
    EXPECT_TRUE(is_unicode(UTF32_3));
}


TEST(unicode, is_alnum)
{
    EXPECT_FALSE(is_alnum(0x00));       // NULL
    EXPECT_FALSE(is_alnum(0x20));       // " "
    EXPECT_FALSE(is_alnum(0x2e));       // .
    EXPECT_TRUE(is_alnum(0x30));        // 0
    EXPECT_TRUE(is_alnum(0x41));        // A
    EXPECT_FALSE(is_alnum(0x5f));       // _
    EXPECT_FALSE(is_alnum(0x7f));       // control (delete)
    EXPECT_TRUE(is_alnum(0xff));        // ÿ
    EXPECT_TRUE(is_alnum(0x100));       // Ā
    EXPECT_FALSE(is_alnum(0x2008));     // space
    EXPECT_FALSE(is_alnum(0x10FFFF));   // unassigned
}


TEST(unicode, is_alpha)
{
    EXPECT_FALSE(is_alpha(0x00));       // NULL
    EXPECT_FALSE(is_alpha(0x20));       // " "
    EXPECT_FALSE(is_alpha(0x2e));       // .
    EXPECT_FALSE(is_alpha(0x30));       // 0
    EXPECT_TRUE(is_alpha(0x41));        // A
    EXPECT_FALSE(is_alpha(0x5f));       // _
    EXPECT_FALSE(is_alpha(0x7f));       // control (delete)
    EXPECT_TRUE(is_alpha(0xff));        // ÿ
    EXPECT_TRUE(is_alpha(0x100));       // Ā
    EXPECT_FALSE(is_alpha(0x2008));     // space
    EXPECT_FALSE(is_alpha(0x10FFFF));   // unassigned
}


TEST(unicode, is_digit)
{
    EXPECT_FALSE(is_digit(0x00));       // NULL
    EXPECT_FALSE(is_digit(0x20));       // " "
    EXPECT_FALSE(is_digit(0x2e));       // .
    EXPECT_TRUE(is_digit(0x30));        // 0
    EXPECT_FALSE(is_digit(0x41));       // A
    EXPECT_FALSE(is_digit(0x5f));       // _
    EXPECT_FALSE(is_digit(0x7f));       // control (delete)
    EXPECT_FALSE(is_digit(0xff));       // ÿ
    EXPECT_FALSE(is_digit(0x100));      // Ā
    EXPECT_FALSE(is_digit(0x2008));     // space
    EXPECT_FALSE(is_digit(0x10FFFF));   // unassigned
}


TEST(unicode, is_lower)
{
    EXPECT_FALSE(is_lower(0x00));       // NULL
    EXPECT_FALSE(is_lower(0x20));       // " "
    EXPECT_FALSE(is_lower(0x2e));       // .
    EXPECT_FALSE(is_lower(0x30));       // 0
    EXPECT_FALSE(is_lower(0x41));       // A
    EXPECT_FALSE(is_lower(0x5f));       // _
    EXPECT_FALSE(is_lower(0x7f));       // control (delete)
    EXPECT_TRUE(is_lower(0xff));        // ÿ
    EXPECT_FALSE(is_lower(0x100));      // Ā
    EXPECT_FALSE(is_lower(0x2008));     // space
    EXPECT_FALSE(is_lower(0x10FFFF));   // unassigned
}


TEST(unicode, is_upper)
{
    EXPECT_FALSE(is_upper(0x00));       // NULL
    EXPECT_FALSE(is_upper(0x20));       // " "
    EXPECT_FALSE(is_upper(0x2e));       // .
    EXPECT_FALSE(is_upper(0x30));       // 0
    EXPECT_TRUE(is_upper(0x41));        // A
    EXPECT_FALSE(is_upper(0x5f));       // _
    EXPECT_FALSE(is_upper(0x7f));       // control (delete)
    EXPECT_FALSE(is_upper(0xff));       // ÿ
    EXPECT_TRUE(is_upper(0x100));       // Ā
    EXPECT_FALSE(is_upper(0x2008));     // space
    EXPECT_FALSE(is_upper(0x10FFFF));   // unassigned
}


TEST(unicode, is_space)
{
    EXPECT_FALSE(is_space(0x00));       // NULL
    EXPECT_TRUE(is_space(0x20));        // " "
    EXPECT_FALSE(is_space(0x2e));       // .
    EXPECT_FALSE(is_space(0x30));       // 0
    EXPECT_FALSE(is_space(0x41));       // A
    EXPECT_FALSE(is_space(0x5f));       // _
    EXPECT_FALSE(is_space(0x7f));       // control (delete)
    EXPECT_FALSE(is_space(0xff));       // ÿ
    EXPECT_FALSE(is_space(0x100));      // Ā
    EXPECT_TRUE(is_space(0x2008));      // space
    EXPECT_FALSE(is_space(0x10FFFF));   // unassigned
}


TEST(unicode, is_blank)
{
    EXPECT_FALSE(is_blank(0x00));       // NULL
    EXPECT_TRUE(is_blank(0x20));        // " "
    EXPECT_FALSE(is_blank(0x2e));       // .
    EXPECT_FALSE(is_blank(0x30));       // 0
    EXPECT_FALSE(is_blank(0x41));       // A
    EXPECT_FALSE(is_blank(0x5f));       // _
    EXPECT_FALSE(is_blank(0x7f));       // control (delete)
    EXPECT_FALSE(is_blank(0xff));       // ÿ
    EXPECT_FALSE(is_blank(0x100));      // Ā
    EXPECT_TRUE(is_blank(0x2008));      // space
    EXPECT_FALSE(is_blank(0x10FFFF));   // unassigned
}


TEST(unicode, is_punctuation)
{
    EXPECT_FALSE(is_punctuation(0x00));         // NULL
    EXPECT_FALSE(is_punctuation(0x20));         // " "
    EXPECT_TRUE(is_punctuation(0x2e));          // .
    EXPECT_FALSE(is_punctuation(0x30));         // 0
    EXPECT_FALSE(is_punctuation(0x41));         // A
    EXPECT_TRUE(is_punctuation(0x5f));          // _
    EXPECT_FALSE(is_punctuation(0x7f));         // control (delete)
    EXPECT_FALSE(is_punctuation(0xff));         // ÿ
    EXPECT_FALSE(is_punctuation(0x100));        // Ā
    EXPECT_FALSE(is_punctuation(0x2008));       // space
    EXPECT_FALSE(is_punctuation(0x10FFFF));     // unassigned
}


TEST(unicode, codepoint_conversions)
{
    // FROM UTF8
    EXPECT_EQ(utf8_to_utf16(UTF8), UTF16);
    EXPECT_EQ(utf8_to_utf32(UTF8), UTF32);
    EXPECT_EQ(utf8_to_utf16(UTF8_2), UTF16_2);
    EXPECT_EQ(utf8_to_utf32(UTF8_2), UTF32_2);
    EXPECT_EQ(utf8_to_utf16(UTF8_3), UTF16_3);
    EXPECT_EQ(utf8_to_utf32(UTF8_3), UTF32_3);

    // FROM UTF16
    EXPECT_EQ(utf16_to_utf8(UTF16), UTF8);
    EXPECT_EQ(utf16_to_utf32(UTF16), UTF32);
    EXPECT_EQ(utf16_to_utf8(UTF16_2), UTF8_2);
    EXPECT_EQ(utf16_to_utf32(UTF16_2), UTF32_2);
    EXPECT_EQ(utf16_to_utf8(UTF16_3), UTF8_3);
    EXPECT_EQ(utf16_to_utf32(UTF16_3), UTF32_3);

    // FROM UTF32
    EXPECT_EQ(utf32_to_utf8(UTF32), UTF8);
    EXPECT_EQ(utf32_to_utf16(UTF32), UTF16);
    EXPECT_EQ(utf32_to_utf8(UTF32_2), UTF8_2);
    EXPECT_EQ(utf32_to_utf16(UTF32_2), UTF16_2);
    EXPECT_EQ(utf32_to_utf8(UTF32_3), UTF8_3);
    EXPECT_EQ(utf32_to_utf16(UTF32_3), UTF16_3);
}


/**
 *  This comprehensive unittest for malformed Unicode is under
 *  CC By 4.0, the rest of the module is MIT licensed.
 *
 *  The original source can be found here:
 *      https://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
 */
TEST(unicode, sequences)
{
    std::vector<std::pair<std::string, std::string>> tests = {
        {
            std::string {-50, -70, -31, -67, -71, -49, -125, -50, -68, -50, -75},
            std::string {-50, -70, -31, -67, -71, -49, -125, -50, -68, -50, -75},
        },
        {
            std::string {0},
            std::string {0},
        },
        {
            std::string {-62, -128},
            std::string {-62, -128},
        },
        {
            std::string {-32, -96, -128},
            std::string {-32, -96, -128},
        },
        {
            std::string {-16, -112, -128, -128},
            std::string {-16, -112, -128, -128},
        },
        {
            std::string {-8, -120, -128, -128, -128},
            std::string {},
        },
        {
            std::string {127},
            std::string {127},
        },
        {
            std::string {-33, -65},
            std::string {-33, -65},
        },
        {
            std::string {-17, -65, -65},
            std::string {-17, -65, -65},
        },
        {
            std::string {-9, -65, -65, -65},
            std::string {},
        },
        {
            std::string {-5, -65, -65, -65, -65},
            std::string {},
        },
        {
            std::string {-3, -65, -65, -65, -65, -65},
            std::string {},
        },
        {
            std::string {-19, -97, -65},
            std::string {-19, -97, -65},
        },
        {
            std::string {-18, -128, -128},
            std::string {-18, -128, -128},
        },
        {
            std::string {-17, -65, -67},
            std::string {-17, -65, -67},
        },
        {
            std::string {-12, -113, -65, -65},
            std::string {-12, -113, -65, -65},
        },
        {
            std::string {-12, -112, -128, -128},
            std::string {},
        },
        {
            std::string {-128},
            std::string {},
        },
        {
            std::string {-62, -65},
            std::string {-62, -65},
        },
        {
            std::string {-62, -128, -62, -65},
            std::string {-62, -128, -62, -65},
        },
        {
            std::string {-62, -128, -62, -65, -62, -128},
            std::string {-62, -128, -62, -65, -62, -128},
        },
        {
            std::string {-62, -128, -62, -65, -62, -128, -62, -65},
            std::string {-62, -128, -62, -65, -62, -128, -62, -65},
        },
        {
            std::string {-62, -128, -62, -65, -62, -128, -62, -65, -62, -128},
            std::string {-62, -128, -62, -65, -62, -128, -62, -65, -62, -128},
        },
        {
            std::string {-62, -128, -62, -65, -62, -128, -62, -65, -62, -128, -62, -65},
            std::string {-62, -128, -62, -65, -62, -128, -62, -65, -62, -128, -62, -65},
        },
        {
            std::string {-62, -128, -62, -65, -62, -128, -62, -65, -62, -128, -62, -65, -62, -128},
            std::string {-62, -128, -62, -65, -62, -128, -62, -65, -62, -128, -62, -65, -62, -128},
        },
        {
            std::string {-62, -128, -62, -127, -62, -126, -62, -125, -62, -124, -62, -123, -62, -122, -62, -121, -62, -120, -62, -119, -62, -118, -62, -117, -62, -116, -62, -115, -62, -114, -62, -113, -62, -112, -62, -111, -62, -110, -62, -109, -62, -108, -62, -107, -62, -106, -62, -105, -62, -104, -62, -103, -62, -102, -62, -101, -62, -100, -62, -99, -62, -98, -62, -97, -62, -96, -62, -95, -62, -94, -62, -93, -62, -92, -62, -91, -62, -90, -62, -89, -62, -88, -62, -87, -62, -86, -62, -85, -62, -84, -62, -83, -62, -82, -62, -81, -62, -80, -62, -79, -62, -78, -62, -77, -62, -76, -62, -75, -62, -74, -62, -73, -62, -72, -62, -71, -62, -70, -62, -69, -62, -68, -62, -67, -62, -66, -62, -65},
            std::string {-62, -128, -62, -127, -62, -126, -62, -125, -62, -124, -62, -123, -62, -122, -62, -121, -62, -120, -62, -119, -62, -118, -62, -117, -62, -116, -62, -115, -62, -114, -62, -113, -62, -112, -62, -111, -62, -110, -62, -109, -62, -108, -62, -107, -62, -106, -62, -105, -62, -104, -62, -103, -62, -102, -62, -101, -62, -100, -62, -99, -62, -98, -62, -97, -62, -96, -62, -95, -62, -94, -62, -93, -62, -92, -62, -91, -62, -90, -62, -89, -62, -88, -62, -87, -62, -86, -62, -85, -62, -84, -62, -83, -62, -82, -62, -81, -62, -80, -62, -79, -62, -78, -62, -77, -62, -76, -62, -75, -62, -74, -62, -73, -62, -72, -62, -71, -62, -70, -62, -69, -62, -68, -62, -67, -62, -66, -62, -65},
        },
        {
            std::string {-61, -128, 32, -61, -127, 32, -61, -126, 32, -61, -125, 32, -61, -124, 32, -61, -123, 32, -61, -122, 32, -61, -121, 32, -61, -120, 32, -61, -119, 32, -61, -118, 32, -61, -117, 32, -61, -116, 32, -61, -115, 32, -61, -114, 32, -61, -113, 32, -61, -112, 32, -61, -111, 32, -61, -110, 32, -61, -109, 32, -61, -108, 32, -61, -107, 32, -61, -106, 32, -61, -105, 32, -61, -104, 32, -61, -103, 32, -61, -102, 32, -61, -101, 32, -61, -100, 32, -61, -99, 32, -61, -98, 32, -61, -97, 32},
            std::string {-61, -128, 32, -61, -127, 32, -61, -126, 32, -61, -125, 32, -61, -124, 32, -61, -123, 32, -61, -122, 32, -61, -121, 32, -61, -120, 32, -61, -119, 32, -61, -118, 32, -61, -117, 32, -61, -116, 32, -61, -115, 32, -61, -114, 32, -61, -113, 32, -61, -112, 32, -61, -111, 32, -61, -110, 32, -61, -109, 32, -61, -108, 32, -61, -107, 32, -61, -106, 32, -61, -105, 32, -61, -104, 32, -61, -103, 32, -61, -102, 32, -61, -101, 32, -61, -100, 32, -61, -99, 32, -61, -98, 32, -61, -97, 32},
        },
        {
            std::string {-61, -80, 32, -61, -79, 32, -61, -78, 32, -61, -77, 32, -61, -76, 32, -61, -75, 32, -61, -74, 32, -61, -73, 32},
            std::string {-61, -80, 32, -61, -79, 32, -61, -78, 32, -61, -77, 32, -61, -76, 32, -61, -75, 32, -61, -74, 32, -61, -73, 32},
        },
        {
            std::string {-61, -96, 32, -61, -95, 32, -61, -94, 32, -61, -93, 32, -61, -92, 32, -61, -91, 32, -61, -90, 32, -61, -89, 32, -61, -88, 32, -61, -87, 32, -61, -86, 32, -61, -85, 32, -61, -84, 32, -61, -83, 32, -61, -82, 32, -61, -81, 32},
            std::string {-61, -96, 32, -61, -95, 32, -61, -94, 32, -61, -93, 32, -61, -92, 32, -61, -91, 32, -61, -90, 32, -61, -89, 32, -61, -88, 32, -61, -87, 32, -61, -86, 32, -61, -85, 32, -61, -84, 32, -61, -83, 32, -61, -82, 32, -61, -81, 32},
        },
        {
            std::string {-61, -72, 32, -61, -71, 32, -61, -70, 32, -61, -69, 32},
            std::string {-61, -72, 32, -61, -71, 32, -61, -70, 32, -61, -69, 32},
        },
        {
            std::string {-61, -68, 32, -61, -67, 32},
            std::string {-61, -68, 32, -61, -67, 32},
        },
        {
            std::string {},
            std::string {},
        },
        {
            std::string {},
            std::string {},
        },
        {
            std::string {},
            std::string {},
        },

        // LONELY START
        // "\xc0 \xc1 \xc2 \xc3 \xc4 \xc5 \xc6 \xc7 \xc8 \xc9 \xca \xcb \xcc \xcd \xce \xcf \xd0 \xd1 \xd2 \xd3 \xd4 \xd5 \xd6 \xd7 \xd8 \xd9 \xda \xdb \xdc \xdd \xde \xdf "
        // "\xe0 \xe1 \xe2 \xe3 \xe4 \xe5 \xe6 \xe7 \xe8 \xe9 \xea \xeb \xec \xed \xee \xef "
        // "\xf0 \xf1 \xf2 \xf3 \xf4 \xf5 \xf6 \xf7 "
        // "\xf8 \xf9 \xfa \xfb "
        // "\xfc \xfd "

        // LAST BYTE MISSING
        // "\xc0"
        // "\xe0\x80"
        // "\xf0\x80\x80"
        // "\xf8\x80\x80\x80"
        // "\xfc\x80\x80\x80\x80"
        // "\xdf"
        // "\xef\xbf"
        // "\xf7\xbf\xbf"
        // "\xfb\xbf\xbf\xbf"
        //  "\xfd\xbf\xbf\xbf\xbf"

        // CONCATENATION OF INCOMPLETE SEQUENCES
        // "\xc0\xe0\x80\xf0\x80\x80\xf8\x80\x80\x80\xfc\x80\x80\x80\x80\xdf\xef\xbf\xf7\xbf\xbf\xfb\xbf\xbf\xbf\xfd\xbf\xbf\xbf\xbf"

        // Impossible bytes
        // "\xfe"
        // "\xff"
        // "\xfe\xfe\xff\xff"

        // Overlong Sequences
        // "\xc0\xaf"
        // "\xe0\x80\xaf"
        // "\xf0\x80\x80\xaf"
        // "\xf8\x80\x80\x80\xaf"
        // "\xfc\x80\x80\x80\x80\xaf"

        // Maximum overlong sequences
        // "\xc1\xbf"
        // "\xe0\x9f\xbf"
        // "\xf0\x8f\xbf\xbf"
        // "\xf8\x87\xbf\xbf\xbf"
        // "\xfc\x83\xbf\xbf\xbf\xbf"

        // Overlong null
        // \xc0\x80
        // "\xe0\x80\x80"
        // "\xf0\x80\x80\x80"
        // "\xf8\x80\x80\x80\x80"
        // "\xfc\x80\x80\x80\x80\x80"

        // Illegal code positions
        // "\xed\xa0\x80"
        // "\xed\xad\xbf"
        // "\xed\xae\x80"
        // "\xed\xaf\xbf"
        // "\xed\xb0\x80"
        // "\xed\xbe\x80"
        // "\xed\xbf\xbf"

        // Paired Surrogates
        // "\xed\xa0\x80\xed\xb0\x80"
        // "\xed\xa0\x80\xed\xbf\xbf"
        // "\xed\xad\xbf\xed\xb0\x80"
        // "\xed\xad\xbf\xed\xbf\xbf"
        // "\xed\xae\x80\xed\xb0\x80"
        // "\xed\xae\x80\xed\xbf\xbf"
        // "\xed\xaf\xbf\xed\xb0\x80"
        // "\xed\xaf\xbf\xed\xbf\xbf"

        // Noncharacters
        // "\xef\xbf\xbe"
        // "\xef\xbf\xbf"
        // "\xef\xb7\x90\xef\xb7\x91\xef\xb7\x92\xef\xb7\x93\xef\xb7\x94\xef\xb7\x95\xef\xb7\x96\xef\xb7\x97\xef\xb7\x98\xef\xb7\x99\xef\xb7\x9a\xef\xb7\x9b\xef\xb7\x9c\xef\xb7\x9d\xef\xb7\x9e\xef\xb7\x9f\xef\xb7\xa0\xef\xb7\xa1\xef\xb7\xa2\xef\xb7\xa3\xef\xb7\xa4\xef\xb7\xa5\xef\xb7\xa6\xef\xb7\xa7\xef\xb7\xa8\xef\xb7\xa9\xef\xb7\xaa\xef\xb7\xab\xef\xb7\xac\xef\xb7\xad\xef\xb7\xae\xef\xb7\xaf"
        // "\xf0\x9f\xbf\xbe\xf0\x9f\xbf\xbf\xf0\xaf\xbf\xbe\xf0\xaf\xbf\xbf\xf0\xbf\xbf\xbe\xf0\xbf\xbf\xbf\xf1\x8f\xbf\xbe\xf1\x8f\xbf\xbf\xf1\x9f\xbf\xbe\xf1\x9f\xbf\xbf\xf1\xaf\xbf\xbe\xf1\xaf\xbf\xbf\xf1\xbf\xbf\xbe\xf1\xbf\xbf\xbf\xf2\x8f\xbf\xbe\xf2\x8f\xbf\xbf\xf2\x9f\xbf\xbe\xf2\x9f\xbf\xbf\xf2\xaf\xbf\xbe\xf2\xaf\xbf\xbf\xf2\xbf\xbf\xbe\xf2\xbf\xbf\xbf\xf3\x8f\xbf\xbe\xf3\x8f\xbf\xbf\xf3\x9f\xbf\xbe\xf3\x9f\xbf\xbf\xf3\xaf\xbf\xbe\xf3\xaf\xbf\xbf\xf3\xbf\xbf\xbe\xf3\xbf\xbf\xbf\xf4\x8f\xbf\xbe\xf4\x8f\xbf\xbf"
    };

    for (const auto &pair: tests) {
        if (pair.second.empty()) {
            // throws an exception
            std::string result;
            try {
                result = utf32_to_utf8(utf8_to_utf32(pair.first));
            } catch (std::exception) {
                continue;
            }
            EXPECT_EQ(result, pair.second);
        } else {
            EXPECT_EQ(utf32_to_utf8(utf8_to_utf32(pair.first)), pair.second);
        }
    }
}
