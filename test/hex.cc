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
}


TEST(hex, hex32)
{
#if BYTE_ORDER == LITTLE_ENDIAN
    // 50462976, 117835012
    std::string expected = "0302010007060504";
#else
    // 66051, 67438087
    std::string expected = "0001020304050607";
#endif

    std::string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(hex(bytes, 4), expected);
}
