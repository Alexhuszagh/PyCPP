//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `pop_front` and `has_pop_front` unittests.
 */

#include <pycpp/sfinae/pop_front.h>
#include <pycpp/stl/deque.h>
#include <pycpp/stl/set.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(pop_front, pop_front)
{
    deque<int> d = {2, 3, 4};
    set<int> s = {2, 3, 4};

    pop_front()(d);
    pop_front()(s);

    ASSERT_EQ(d.size(), 2);
    ASSERT_EQ(s.size(), 2);
    EXPECT_EQ(d.at(0), 3);
    EXPECT_TRUE(s.find(2) == s.end());
}


TEST(pop_front, has_pop_front)
{
    using deque_type = deque<int>;
    using set_type = set<int>;

    static_assert(has_pop_front<deque_type>::value, "");
    static_assert(!has_pop_front<set_type>::value, "");
}
