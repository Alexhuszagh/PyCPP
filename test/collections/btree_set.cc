//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief B-tree set unittests.
 */

#include <pycpp/collections/btree_set.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(btree_set, constructor_null)
{
    using set = btree_set<int>;
    set s1, s2;

    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_TRUE(s1.find(1) == s1.end());
    EXPECT_TRUE(s2.find(1) == s2.end());

    s1.insert(1);
    s1.emplace(2);
    EXPECT_EQ(s1.size(), 2);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_TRUE(s1.find(1) != s1.end());
    EXPECT_TRUE(s2.find(1) == s2.end());
    EXPECT_TRUE(s1.find(2) != s1.end());
    EXPECT_TRUE(s2.find(2) == s2.end());

    s2 = std::move(s1);
    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s2.size(), 2);
    EXPECT_TRUE(s1.find(1) == s1.end());
    EXPECT_TRUE(s2.find(1) != s2.end());
}


TEST(btree_multiset, constructor_null)
{
    using set = btree_multiset<int>;
    set s1, s2;

    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_TRUE(s1.find(1) == s1.end());
    EXPECT_TRUE(s2.find(1) == s2.end());

    s1.insert(1);
    s1.emplace(2);
    EXPECT_EQ(s1.size(), 2);
    EXPECT_EQ(s2.size(), 0);
    EXPECT_TRUE(s1.find(1) != s1.end());
    EXPECT_TRUE(s2.find(1) == s2.end());
    EXPECT_TRUE(s1.find(2) != s1.end());
    EXPECT_TRUE(s2.find(2) == s2.end());

    s2 = std::move(s1);
    EXPECT_EQ(s1.size(), 0);
    EXPECT_EQ(s2.size(), 2);
    EXPECT_TRUE(s1.find(1) == s1.end());
    EXPECT_TRUE(s2.find(1) != s2.end());
}
