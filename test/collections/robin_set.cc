//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Robin hood set unittests.
 */

#include <pycpp/collections/robin_set.h>
#include <pycpp/stl/set.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// HELPERS
// -------

// Simulate a bad hash function with a static hash
template <typename T>
struct bad_hash
{
    constexpr size_t operator()(const T& t) const
    {
        return 1;
    }
};

// TESTS
// -----


TEST(robin_set, constructor_null)
{
    robin_set<string> rs1;
    EXPECT_EQ(rs1.size(), 0);
    rs1.emplace("key");
    EXPECT_EQ(rs1.size(), 1);
    EXPECT_TRUE(rs1.find("key") != rs1.end());
}


#if 0           // TODO: restore, MSVC bug
TEST(robin_set, constructor_iterable)
{
    robin_set<string> rs1;
    rs1.emplace("key");

    robin_set<string> rs2(rs1.begin(), rs1.end());
    EXPECT_EQ(rs2.size(), 1);
    EXPECT_TRUE(rs2.find("key") != rs1.end());
}


TEST(robin_set, constructor_copy)
{
    robin_set<string> rs1;
    rs1.emplace("key");

    robin_set<string> rs2(rs1);
    EXPECT_EQ(rs1.size(), 1);
    EXPECT_EQ(rs2.size(), 1);
    EXPECT_TRUE(rs2.find("key") != rs1.end());
}


TEST(robin_set, constructor_move)
{
    robin_set<string> rs1;
    rs1.emplace("key");

    robin_set<string> rs2(move(rs1));
    EXPECT_EQ(rs2.size(), 1);
    EXPECT_TRUE(rs2.find("key") != rs1.end());
}


TEST(robin_set, constructor_ilist)
{
    robin_set<int> rs1 = {1};
    EXPECT_EQ(rs1.size(), 1);
    EXPECT_TRUE(rs1.find(1) != rs1.end());
}


TEST(robin_set, iteration)
{
    robin_set<int> rs1 = {-1, 1, 2};
    set<int> s1(rs1.begin(), rs1.end());

    for (auto it = rs1.begin(); it != rs1.end(); ++it) {
        EXPECT_TRUE(s1.find(*it) != s1.end());
    }
}


TEST(robin_set, emplace)
{
    robin_set<int> rs1;
    rs1.emplace(1);
    EXPECT_TRUE(rs1.find(0) == rs1.end());
    EXPECT_TRUE(rs1.find(1) != rs1.end());
    EXPECT_EQ(rs1.size(), 1);
}


TEST(robin_set, insert)
{
    robin_set<int> rs1;

    {
        // no hint, copy semantics
        rs1.insert(-1);
        EXPECT_TRUE(rs1.find(-1) != rs1.end());
    }
    {
        // hint, with copy semantics
        int i = 0;
        rs1.insert(rs1.begin(), i);
        EXPECT_TRUE(rs1.find(0) != rs1.end());
    }
    {
        // hint, with move semantics
        rs1.insert(rs1.begin(), 1);
        EXPECT_TRUE(rs1.find(1) != rs1.end());
    }
}


TEST(robin_set, erase)
{
    robin_set<int> rs1 = {-1, 1, 2};
    {
        EXPECT_EQ(rs1.erase(3), 0);
        EXPECT_EQ(rs1.size(), 3);
    }
    {
        auto it = rs1.cbegin();
        EXPECT_EQ(rs1.erase(*it), 1);
        EXPECT_EQ(rs1.size(), 2);
    }
}


TEST(robin_set, clear)
{
    robin_set<int> rs1;
    rs1.emplace(1);

    EXPECT_EQ(rs1.size(), 1);
    rs1.clear();
    EXPECT_EQ(rs1.size(), 0);
}


TEST(robin_set, swap)
{
    robin_set<int> rs1, rs2;
    rs1.emplace(1);
    EXPECT_EQ(rs1.size(), 1);
    EXPECT_EQ(rs2.size(), 0);

    rs1.swap(rs2);
    EXPECT_EQ(rs1.size(), 0);
    EXPECT_EQ(rs2.size(), 1);
}


TEST(robin_set, bad_hash)
{
    using bad_map = robin_set<int, bad_hash<int>>;
    bad_map rs1;
    ASSERT_EQ(rs1.size(), 0);
    rs1.emplace(1);
    rs1.emplace(2);
    ASSERT_EQ(rs1.size(), 2);
    EXPECT_TRUE(rs1.find(1) != rs1.end());
    EXPECT_TRUE(rs1.find(2) != rs1.end());
    EXPECT_TRUE(rs1.find(3) == rs1.end());
}
#endif
