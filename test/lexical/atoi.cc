//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lexical/atoi.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

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

TEST(atou8, base10)
{
    EXPECT_EQ(atou8("0", 10), 0);
    EXPECT_EQ(atou8("127", 10), 127);
    EXPECT_EQ(atou8("128", 10), 128);
    EXPECT_EQ(atou8("255", 10), 255);
    EXPECT_EQ(atou8("-1", 10), uint8_t(-1));
    EXPECT_EQ(atou8("1a", 10), 1);
}


TEST(atoi8, base10)
{
    EXPECT_EQ(atoi8("0", 10), 0);
    EXPECT_EQ(atoi8("127", 10), 127);
    EXPECT_EQ(atoi8("128", 10), -128);
    EXPECT_EQ(atoi8("255", 10), -1);
    EXPECT_EQ(atoi8("-1", 10), -1);
    EXPECT_EQ(atoi8("1a", 10), 1);
}


TEST(atou16, base10)
{
    EXPECT_EQ(atou16("0", 10), 0);
    EXPECT_EQ(atou16("32767", 10), 32767);
    EXPECT_EQ(atou16("32768", 10), 32768);
    EXPECT_EQ(atou16("65535", 10), 65535);
    EXPECT_EQ(atou16("-1", 10), uint16_t(-1));
    EXPECT_EQ(atou16("1a", 10), 1);
}


TEST(atoi16, base10)
{
    EXPECT_EQ(atoi16("0", 10), 0);
    EXPECT_EQ(atoi16("32767", 10), 32767);
    EXPECT_EQ(atoi16("32768", 10), -32768);
    EXPECT_EQ(atoi16("65535", 10), -1);
    EXPECT_EQ(atoi16("-1", 10), -1);
    EXPECT_EQ(atoi16("1a", 10), 1);
}


TEST(atou32, base10)
{
    EXPECT_EQ(atou32("0", 10), 0);
    EXPECT_EQ(atou32("2147483647", 10), 2147483647ULL);
    EXPECT_EQ(atou32("2147483648", 10), 2147483648ULL);
    EXPECT_EQ(atou32("4294967295", 10), 4294967295ULL);
    EXPECT_EQ(atou32("-1", 10), uint32_t(-1));
    EXPECT_EQ(atou32("1a", 10), 1);
}


TEST(atoi32, base10)
{
    EXPECT_EQ(atoi32("0", 10), 0);
    EXPECT_EQ(atoi32("2147483647", 10), 2147483647ULL);
    EXPECT_EQ(atoi32("2147483648", 10), -2147483648ULL);
    EXPECT_EQ(atoi32("4294967295", 10), -1);
    EXPECT_EQ(atoi32("-1", 10), -1);
    EXPECT_EQ(atoi32("1a", 10), 1);
}


TEST(atou64, base10)
{
    EXPECT_EQ(atou64("0", 10), 0);
    EXPECT_EQ(atou64("9223372036854775807", 10), 9223372036854775807ULL);
    EXPECT_EQ(atou64("9223372036854775808", 10), 9223372036854775808ULL);
    EXPECT_EQ(atou64("18446744073709551615", 10), 18446744073709551615ULL);
    EXPECT_EQ(atou64("-1", 10), uint64_t(-1));
    EXPECT_EQ(atou64("1a", 10), 1);
}


TEST(atoi64, base10)
{
    EXPECT_EQ(atoi64("0", 10), 0);
    EXPECT_EQ(atoi64("9223372036854775807", 10), 9223372036854775807ULL);
    EXPECT_EQ(atoi64("9223372036854775808", 10), -9223372036854775808ULL);
    EXPECT_EQ(atoi64("18446744073709551615", 10), -1);
    EXPECT_EQ(atoi64("-1", 10), -1);
    EXPECT_EQ(atoi64("1a", 10), 1);
}


TEST(atoi8, basen)
{
    for (const auto& pair: DATA) {
        EXPECT_EQ(atoi8(pair.second, pair.first), 37);
    }
}


TEST(atoi16, basen)
{
    EXPECT_EQ(atoi16("YA", 36), 1234);
}