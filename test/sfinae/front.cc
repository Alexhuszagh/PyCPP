//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `front` and `has_front` unittests.
 */

#include <pycpp/sfinae/front.h>
#include <pycpp/stl/deque.h>
#include <pycpp/stl/set.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(front, front)
{
    deque<int> d1 = {2, 3, 4};
    set<int> s = {2, 3, 4};
    const auto&d2 = d1;

    EXPECT_EQ(front()(d1), 2);
    EXPECT_EQ(front()(d2), 2);
    front()(d1) = 1;
    EXPECT_EQ(front()(d2), 1);
    EXPECT_EQ(front()(s), 2);
}


TEST(front, has_front)
{
    using deque_type = deque<int>;
    using set_type = set<int>;

    static_assert(has_front<deque_type>::value, "");
    static_assert(!has_front<set_type>::value, "");
}
