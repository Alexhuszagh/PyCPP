//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Ordering unittests.
 */

#include <pycpp/ordering.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(ordering, not_equal_to)
{
    EXPECT_TRUE(not_equal_to(1, 3));
    EXPECT_FALSE(not_equal_to(1, 1));
}


TEST(ordering, less_equal)
{
    EXPECT_TRUE(less_equal(1, 3));
    EXPECT_TRUE(less_equal(1, 1));
    EXPECT_FALSE(less_equal(1, 0));
}


TEST(ordering, greater)
{
    EXPECT_FALSE(greater(1, 3));
    EXPECT_FALSE(greater(1, 1));
    EXPECT_TRUE(greater(1, 0));
}


TEST(ordering, greater_equal)
{
    EXPECT_FALSE(greater_equal(1, 3));
    EXPECT_TRUE(greater_equal(1, 1));
    EXPECT_TRUE(greater_equal(1, 0));
}
