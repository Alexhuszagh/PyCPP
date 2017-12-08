//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `fixed_vector` unittests.
 */

#include <pycpp/fixed/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(fixed_vector, ctor)
{
    using vector = fixed_vector<int>;

    // check type properties
    EXPECT_GE(sizeof(vector), vector::stack_size());

    // check basic vector properties
    vector v1, v2;
    EXPECT_EQ(v1, v2);

    v1.emplace_back(1);
    EXPECT_EQ(v1.size(), 1);
    EXPECT_EQ(v2.size(), 0);
    EXPECT_NE(v1, v2);

    v2 = v1;
    EXPECT_EQ(v1.size(), 1);
    EXPECT_EQ(v2.size(), 1);
    EXPECT_EQ(v1, v2);

    // check the arena is working well
    EXPECT_NE(&v1.get_arena(), &v2.get_arena());
    EXPECT_LE(v1.stack_used(), v1.stack_size());
    EXPECT_GE(v1.stack_percent_used(), 0);
}
