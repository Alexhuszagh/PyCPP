//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Lexical integer conversion unittests.
 */

#include <pycpp/lexical/int.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----
// Don't test extensively: the back-end is
// already extensively tested.

TEST(lexical_int_formatter, unsigned_int)
{
    auto TEST_USHORT = [](const unsigned short value, const string_view& expected) {
        EXPECT_EQ(lexical_int_formatter(value).string(), expected);
    };
    auto TEST_UINT = [](const unsigned int value, const string_view& expected) {
        EXPECT_EQ(lexical_int_formatter(value).string(), expected);
    };
    auto TEST_ULONG = [](const unsigned long value, const string_view& expected) {
        EXPECT_EQ(lexical_int_formatter(value).string(), expected);
    };
    auto TEST_ULONGLONG = [](const unsigned long long value, const string_view& expected) {
        EXPECT_EQ(lexical_int_formatter(value).string(), expected);
    };

    TEST_USHORT(1, "1");
    TEST_UINT(1, "1");
    TEST_ULONG(1, "1");
    TEST_ULONGLONG(1, "1");
}


TEST(lexical_int_formatter, signed_int)
{
    auto TEST_SHORT = [](const short value, const string_view& expected) {
        EXPECT_EQ(lexical_int_formatter(value).string(), expected);
    };
    auto TEST_INT = [](const int value, const string_view& expected) {
        EXPECT_EQ(lexical_int_formatter(value).string(), expected);
    };
    auto TEST_LONG = [](const long value, const string_view& expected) {
        EXPECT_EQ(lexical_int_formatter(value).string(), expected);
    };
    auto TEST_LONGLONG = [](const long long value, const string_view& expected) {
        EXPECT_EQ(lexical_int_formatter(value).string(), expected);
    };

    TEST_SHORT(-1, "-1");
    TEST_INT(-1, "-1");
    TEST_LONG(-1, "-1");
    TEST_LONGLONG(-1, "-1");
    TEST_SHORT(1, "1");
    TEST_INT(1, "1");
    TEST_LONG(1, "1");
    TEST_LONGLONG(1, "1");
}


TEST(lexical_int_extractor, lexical_int_extractor)
{
    // signed only
    EXPECT_EQ(int8_t(lexical_int_extractor("-1")), -1);
    EXPECT_EQ(short(lexical_int_extractor("-1")), -1);
    EXPECT_EQ(int(lexical_int_extractor("-1")), -1);
    EXPECT_EQ(long(lexical_int_extractor("-1")), -1);
    EXPECT_EQ((long long)(lexical_int_extractor("-1")), -1);

    // signed and unsigned
    EXPECT_EQ(int8_t(lexical_int_extractor("1")), 1);
    EXPECT_EQ(uint8_t(lexical_int_extractor("1")), 1);
    EXPECT_EQ(short(lexical_int_extractor("1")), 1);
    EXPECT_EQ((unsigned short)(lexical_int_extractor("1")), 1);
    EXPECT_EQ(int(lexical_int_extractor("1")), 1);
    EXPECT_EQ((unsigned int)(lexical_int_extractor("1")), 1);
    EXPECT_EQ(long(lexical_int_extractor("1")), 1);
    EXPECT_EQ((unsigned long)(lexical_int_extractor("1")), 1);
    EXPECT_EQ((long long)(lexical_int_extractor("1")), 1);
    EXPECT_EQ((unsigned long long)(lexical_int_extractor("1")), 1);
}
