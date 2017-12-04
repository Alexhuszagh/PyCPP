//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `push_front` and `has_push_front` unittests.
 */

#include <pycpp/sfinae/push_front.h>
#include <gtest/gtest.h>
#include <deque>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(push_front, push_front)
{
    std::deque<int> deq = {2, 3, 4};
    std::vector<int> vec = {2, 3, 4};

    push_front()(deq, 1);
    push_front()(vec, 1);

    ASSERT_EQ(deq.size(), 4);
    ASSERT_EQ(vec.size(), 4);
    EXPECT_EQ(deq.at(0), 1);
    EXPECT_EQ(vec.at(0), 1);
}


TEST(push_front, has_push_front)
{
    using deq = std::deque<int>;
    using vec = std::vector<int>;

    static_assert(has_push_front<deq>::value, "");
    static_assert(!has_push_front<vec>::value, "");
}
