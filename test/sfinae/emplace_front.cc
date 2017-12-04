//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `emplace_front` and `has_emplace_front` unittests.
 */

#include <pycpp/sfinae/emplace_front.h>
#include <gtest/gtest.h>
#include <deque>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(emplace_front, emplace_front)
{
    std::deque<int> deq = {2, 3, 4};
    std::vector<int> vec = {2, 3, 4};

    emplace_front()(deq, 1);
    emplace_front()(vec, 1);

    ASSERT_EQ(deq.size(), 4);
    ASSERT_EQ(vec.size(), 4);
    EXPECT_EQ(deq.at(0), 1);
    EXPECT_EQ(vec.at(0), 1);
}


TEST(emplace_front, has_emplace_front)
{
    using deq = std::deque<int>;
    using vec = std::vector<int>;

    static_assert(has_emplace_front<deq>::value, "");
    static_assert(!has_emplace_front<vec>::value, "");
}
