//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `push_back` and `has_push_back` unittests.
 */

#include <pycpp/sfinae/push_back.h>
#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(push_back, push_back)
{
    std::unordered_set<int> set = {2, 3, 4};
    std::vector<int> vec = {2, 3, 4};

    push_back()(set, 1);
    push_back()(vec, 1);

    ASSERT_EQ(set.size(), 4);
    ASSERT_EQ(vec.size(), 4);
    EXPECT_TRUE(set.find(1) != set.end());
    EXPECT_EQ(vec.at(3), 1);
}


TEST(push_back, has_push_back)
{
    using set = std::unordered_set<int>;
    using vec = std::vector<int>;

    static_assert(!has_push_back<set>::value, "");
    static_assert(has_push_back<vec>::value, "");
}
