//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Hex representation unittests.
 */

#include "byteorder.h"
#include "hex.h"
#include <gtest/gtest.h>


// TESTS
// -----


TEST(hex, hex8)
{
    std::string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    std::string expected = "0001020304050607";
    EXPECT_EQ(hex(bytes, 1), expected);
    EXPECT_EQ(hex_i8(bytes), expected);
}


TEST(hex, hex32)
{
std::string expected = "0302010007060504";
    std::string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(hex(bytes, 4), expected);
    EXPECT_EQ(hex_i32(bytes), expected);
}


TEST(unhex, unhex8)
{
    std::string bytes = "0001020304050607";
    std::string expected = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(unhex(bytes, 1), expected);
    EXPECT_EQ(unhex_i8(bytes), expected);
}


TEST(unhex, unhex32)
{
    std::string bytes = "0302010007060504";
    std::string expected = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(unhex(bytes, 4), expected);
    EXPECT_EQ(unhex_i32(bytes), expected);
}
