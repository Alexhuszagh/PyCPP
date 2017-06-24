//  :copyright: (c) 2015 Markus Kuhn.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
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


// This comprehensive unittest for malformed Unicode is under
// CC By 4.0, the rest of the module is MIT licensed.
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
            std::string {},
            std::string {},
        },
        {
            std::string {},
            std::string {},
        },

        // "\xbf"
        // "\x80\xbf"
        // "\x80\xbf\x80"
        // "\x80\xbf\x80\xbf"
        // "\x80\xbf\x80\xbf\x80"
        // "\x80\xbf\x80\xbf\x80\xbf"
        // "\x80\xbf\x80\xbf\x80\xbf\x80"
        // "\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
    };

    for (const auto &pair: tests) {
        if (pair.second.empty()) {
            // throws an exception
            try {
                utf32_to_utf8(utf8_to_utf32(pair.first));
            } catch (std::exception) {
                continue;
            }
            EXPECT_EQ(pair.first, pair.second);
        } else {
            EXPECT_EQ(utf32_to_utf8(utf8_to_utf32(pair.first)), pair.second);
        }
    }
}
