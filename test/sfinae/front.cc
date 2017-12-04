//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `front` and `has_front` unittests.
 */

#include <pycpp/sfinae/front.h>
#include <gtest/gtest.h>
#include <deque>
#include <set>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(front, front)
{
    std::deque<int> deq = {2, 3, 4};
    std::set<int> set = {2, 3, 4};
    const auto&d = deq;

    EXPECT_EQ(front()(deq), 2);
    EXPECT_EQ(front()(d), 2);
    front()(deq) = 1;
    EXPECT_EQ(front()(d), 1);
    EXPECT_EQ(front()(set), 2);
}


TEST(front, has_front)
{
    using deq = std::deque<int>;
    using set = std::set<int>;

    static_assert(has_front<deq>::value, "");
    static_assert(!has_front<set>::value, "");
}
