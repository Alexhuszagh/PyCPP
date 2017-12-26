//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `fixed_deque` unittests.
 */

#include <pycpp/fixed/deque.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(fixed_deque, ctor)
{
    using deque_type = fixed_deque<int>;

    // check type properties
    EXPECT_GE(sizeof(deque_type), deque_type::stack_size());

    // check basic deque properties
    deque_type d1, d2;
    EXPECT_EQ(d1, d2);

    d1.emplace_back(1);
    EXPECT_EQ(d1.size(), 1);
    EXPECT_EQ(d2.size(), 0);
    EXPECT_NE(d1, d2);

    d2 = d1;
    EXPECT_EQ(d1.size(), 1);
    EXPECT_EQ(d2.size(), 1);
    EXPECT_EQ(d1[0], 1);
    EXPECT_EQ(d2[0], 1);
    EXPECT_EQ(d1, d2);

    // check the arena is working well
    EXPECT_NE(&d1.get_arena(), &d2.get_arena());
    EXPECT_LE(d1.stack_used(), d1.stack_size());
    EXPECT_GE(d1.stack_percent_used(), 0);
}
