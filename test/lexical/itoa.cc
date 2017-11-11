//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lexical/itoa.h>
#include <gtest/gtest.h>
#include <vector>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::vector<std::pair<uint8_t, std::string>> DATA = {
    {2, "100101"},
    {3, "1101"},
    {4, "211"},
    {5, "122"},
    {6, "101"},
    {7, "52"},
    {8, "45"},
    {9, "41"},
    {10, "37"},
    {11, "34"},
    {12, "31"},
    {13, "2B"},
    {14, "29"},
    {15, "27"},
    {16, "25"},
    {17, "23"},
    {18, "21"},
    {19, "1I"},
    {20, "1H"},
    {21, "1G"},
    {22, "1F"},
    {23, "1E"},
    {24, "1D"},
    {25, "1C"},
    {26, "1B"},
    {27, "1A"},
    {28, "19"},
    {29, "18"},
    {30, "17"},
    {31, "16"},
    {32, "15"},
    {33, "14"},
    {34, "13"},
    {35, "12"},
    {36, "11"},
};

// TESTS
// -----

TEST(u8toa, base10)
{
    EXPECT_EQ(u8toa(0, 10), "0");
    EXPECT_EQ(u8toa(127, 10), "127");
    EXPECT_EQ(u8toa(128, 10), "128");
    EXPECT_EQ(u8toa(255, 10), "255");
    EXPECT_EQ(u8toa(-1, 10), "255");
}


TEST(i8toa, base10)
{
    EXPECT_EQ(i8toa(0, 10), "0");
    EXPECT_EQ(i8toa(127, 10), "127");
    EXPECT_EQ(i8toa(128, 10), "-128");
    EXPECT_EQ(i8toa(255, 10), "-1");
    EXPECT_EQ(i8toa(-1, 10), "-1");
}


TEST(u16toa, base10)
{
    EXPECT_EQ(u16toa(0, 10), "0");
    EXPECT_EQ(u16toa(32767, 10), "32767");
    EXPECT_EQ(u16toa(32768, 10), "32768");
    EXPECT_EQ(u16toa(65535, 10), "65535");
    EXPECT_EQ(u16toa(-1, 10), "65535");
}


TEST(i16toa, base10)
{
    EXPECT_EQ(i16toa(0, 10), "0");
    EXPECT_EQ(i16toa(32767, 10), "32767");
    EXPECT_EQ(i16toa(32768, 10), "-32768");
    EXPECT_EQ(i16toa(65535, 10), "-1");
    EXPECT_EQ(i16toa(-1, 10), "-1");
}


TEST(u32toa, base10)
{
    EXPECT_EQ(u32toa(0, 10), "0");
    EXPECT_EQ(u32toa(2147483647ULL, 10), "2147483647");
    EXPECT_EQ(u32toa(2147483648ULL, 10), "2147483648");
    EXPECT_EQ(u32toa(4294967295ULL, 10), "4294967295");
    EXPECT_EQ(u32toa(-1, 10), "4294967295");
}


TEST(i32toa, base10)
{
    EXPECT_EQ(i32toa(0, 10), "0");
    EXPECT_EQ(i32toa(2147483647ULL, 10), "2147483647");
    EXPECT_EQ(i32toa(2147483648ULL, 10), "-2147483648");
    EXPECT_EQ(i32toa(4294967295ULL, 10), "-1");
    EXPECT_EQ(i32toa(-1, 10), "-1");
}


TEST(u64toa, base10)
{
    EXPECT_EQ(u64toa(0, 10), "0");
    EXPECT_EQ(u64toa(9223372036854775807ULL, 10), "9223372036854775807");
    EXPECT_EQ(u64toa(9223372036854775808ULL, 10), "9223372036854775808");
    EXPECT_EQ(u64toa(18446744073709551615ULL, 10), "18446744073709551615");
    EXPECT_EQ(u64toa(-1, 10), "18446744073709551615");
}


TEST(i64toa, base10)
{
    EXPECT_EQ(i64toa(0, 10), "0");
    EXPECT_EQ(i64toa(9223372036854775807ULL, 10), "9223372036854775807");
    EXPECT_EQ(i64toa(9223372036854775808ULL, 10), "-9223372036854775808");
    EXPECT_EQ(i64toa(18446744073709551615ULL, 10), "-1");
    EXPECT_EQ(i64toa(-1, 10), "-1");
}


TEST(i8toa, basen)
{
    for (const auto& pair: DATA) {
        EXPECT_EQ(i8toa(37, pair.first), pair.second);
    }
}