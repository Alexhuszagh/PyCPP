//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `pop_back` and `has_pop_back` unittests.
 */

#include <pycpp/sfinae/pop_back.h>
#include <gtest/gtest.h>
#include <deque>
#include <set>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(pop_back, pop_back)
{
    std::deque<int> deq = {2, 3, 4};
    std::set<int> set = {2, 3, 4};

    pop_back()(deq);
    pop_back()(set);

    ASSERT_EQ(deq.size(), 2);
    ASSERT_EQ(set.size(), 2);
    EXPECT_EQ(deq.at(1), 3);
    EXPECT_TRUE(set.find(4) == set.end());
}


TEST(pop_back, has_pop_back)
{
    using deq = std::deque<int>;
    using set = std::set<int>;

    static_assert(has_pop_back<deq>::value, "");
    static_assert(!has_pop_back<set>::value, "");
}
