//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `back` and `has_back` unittests.
 */

#include <pycpp/sfinae/back.h>
#include <pycpp/stl/deque.h>
#include <pycpp/stl/set.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(back, back)
{
    deque<int> d1 = {2, 3, 4};
    set<int> s = {2, 3, 4};
    const auto&d2 = d1;

    EXPECT_EQ(back()(d1), 4);
    EXPECT_EQ(back()(d2), 4);
    back()(d1) = 5;
    EXPECT_EQ(back()(d2), 5);
    EXPECT_EQ(back()(s), 4);
}


TEST(back, has_back)
{
    using deque_type = deque<int>;
    using set_type = set<int>;

    static_assert(has_back<deque_type>::value, "");
    static_assert(!has_back<set_type>::value, "");
}
