//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `fixed_forward_list` unittests.
 */

#include <pycpp/fixed/forward_list.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(fixed_forward_list, ctor)
{
    using forward_list = fixed_forward_list<int>;

    // check type properties
    EXPECT_GE(sizeof(forward_list), forward_list::stack_size());

    // check basic forward_list properties
    forward_list d1, d2;
    EXPECT_EQ(d1, d2);

    d1.emplace_front(1);
    EXPECT_NE(d1, d2);

    d2 = d1;
    EXPECT_EQ(d1, d2);

    // check the arena is working well
    EXPECT_NE(&d1.get_arena(), &d2.get_arena());
    EXPECT_LE(d1.stack_used(), d1.stack_size());
    EXPECT_GE(d1.stack_percent_used(), 0);
}
