//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `fixed_list` unittests.
 */

#include <pycpp/fixed/list.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(fixed_list, ctor)
{
    using list_type = fixed_list<int>;

    // check type properties
    EXPECT_GE(sizeof(list_type), list_type::stack_size());

    // check basic list properties
    list_type d1, d2;
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
