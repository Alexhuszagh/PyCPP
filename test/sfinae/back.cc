//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `back` and `has_back` unittests.
 */

#include <pycpp/sfinae/back.h>
#include <gtest/gtest.h>
#include <deque>
#include <set>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(back, back)
{
    std::deque<int> deq = {2, 3, 4};
    std::set<int> set = {2, 3, 4};
    const auto&d = deq;

    EXPECT_EQ(back()(deq), 4);
    EXPECT_EQ(back()(d), 4);
    back()(deq) = 5;
    EXPECT_EQ(back()(d), 5);
    EXPECT_EQ(back()(set), 4);
}


TEST(back, has_back)
{
    using deq = std::deque<int>;
    using set = std::set<int>;

    static_assert(has_back<deq>::value, "");
    static_assert(!has_back<set>::value, "");
}
