//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `emplace_back` and `has_emplace_back` unittests.
 */

#include <pycpp/sfinae/emplace_back.h>
#include <gtest/gtest.h>
#include <unordered_set>
#include <vector>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(emplace_back, emplace_back)
{
    std::unordered_set<int> set = {2, 3, 4};
    std::vector<int> vec = {2, 3, 4};

    emplace_back()(set, 1);
    emplace_back()(vec, 1);

    ASSERT_EQ(set.size(), 4);
    ASSERT_EQ(vec.size(), 4);
    EXPECT_TRUE(set.find(1) != set.end());
    EXPECT_EQ(vec.at(3), 1);
}


TEST(emplace_back, has_emplace_back)
{
    using set = std::unordered_set<int>;
    using vec = std::vector<int>;

    static_assert(!has_emplace_back<set>::value, "");
    static_assert(has_emplace_back<vec>::value, "");
}
