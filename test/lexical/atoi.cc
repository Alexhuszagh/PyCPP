//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lexical/atoi.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(u8toa, base10)
{
    EXPECT_EQ(u8toa("0", 10), 0);
    EXPECT_EQ(u8toa("127", 10), 127);
    EXPECT_EQ(u8toa("128", 10), 128);
    EXPECT_EQ(u8toa("255", 10), 255);
    EXPECT_EQ(u8toa("-1", 10), uint8_t(-1));
    EXPECT_EQ(u8toa("1a", 10), 1);
}


TEST(i8toa, base10)
{
    EXPECT_EQ(i8toa("0", 10), 0);
    EXPECT_EQ(i8toa("127", 10), 127);
    EXPECT_EQ(i8toa("128", 10), -128);
    EXPECT_EQ(i8toa("255", 10), -1);
    EXPECT_EQ(i8toa("-1", 10), -1);
    EXPECT_EQ(i8toa("1a", 10), 1);
}


TEST(u16toa, base10)
{
    // TODO: implement...
}


TEST(i16toa, base10)
{
    // TODO: implement...
}


// TODO: 32-bit
// TODO: 64-bit
// TODO: base2 to 36