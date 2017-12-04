//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `pop_front` and `has_pop_front` unittests.
 */

#include <pycpp/sfinae/pop_front.h>
#include <gtest/gtest.h>
#include <deque>
#include <set>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(pop_front, pop_front)
{
    std::deque<int> deq = {2, 3, 4};
    std::set<int> set = {2, 3, 4};

    pop_front()(deq);
    pop_front()(set);

    ASSERT_EQ(deq.size(), 2);
    ASSERT_EQ(set.size(), 2);
    EXPECT_EQ(deq.at(0), 3);
    EXPECT_TRUE(set.find(2) == set.end());
}


TEST(pop_front, has_pop_front)
{
    using deq = std::deque<int>;
    using set = std::set<int>;

    static_assert(has_pop_front<deq>::value, "");
    static_assert(!has_pop_front<set>::value, "");
}
