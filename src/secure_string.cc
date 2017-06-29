//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.

#include "secure_string.h"
#include <gtest/gtest.h>


// TESTS
// -----


TEST(secure_string, secure_string)
{
    secure_string str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_GT(str.capacity(), 0);
    EXPECT_GT(str.max_size(), 0);
}


// TODO:
