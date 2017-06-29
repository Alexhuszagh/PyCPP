//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "secure_string.h"
#include <gtest/gtest.h>


// TESTS
// -----


TEST(secure_string, secure_string)
{
    // default initializer
    secure_string str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_GT(str.capacity(), 0);
    EXPECT_GT(str.max_size(), 0);

    // move + secure_string(const_pointer)
    str = secure_string("This is some data");
    EXPECT_EQ(str.size(), 17);
    EXPECT_GT(str.capacity(), 17);

    // move + secure_string(const_pointer)
    str = secure_string("This is some data", 8);
    EXPECT_EQ(str.size(), 8);
    EXPECT_GT(str.capacity(), 8);

    // move + fill
    str = secure_string('0', 15);
    EXPECT_EQ(str.size(), 15);
    EXPECT_GT(str.capacity(), 15);

    // iterator construct
    secure_string str2(str.begin(), str.end());
    EXPECT_EQ(str2.size(), 15);
    EXPECT_GT(str2.capacity(), 15);
//    EXPECT_EQ(str, str2);
}
