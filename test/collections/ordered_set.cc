//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Ordered set unittests.
 */

#include <pycpp/collections/ordered_set.h>
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


TEST(ordered_set, constructor_null)
{
    ordered_set<string> os1;
    EXPECT_EQ(os1.size(), 0);
    os1.emplace("key");
    EXPECT_EQ(os1.size(), 1);
    EXPECT_TRUE(os1.find("key") != os1.end());
}


#if 0           // TODO: restore, MSVC bug fix
TEST(ordered_set, constructor_iterable)
{
    ordered_set<string> os1;
    os1.emplace("key");

    ordered_set<string> os2(os1.begin(), os1.end());
    EXPECT_EQ(os2.size(), 1);
    EXPECT_TRUE(os2.find("key") != os1.end());
}


TEST(ordered_set, constructor_copy)
{
    ordered_set<string> os1;
    os1.emplace("key");

    ordered_set<string> os2(os1);
    EXPECT_EQ(os1.size(), 1);
    EXPECT_EQ(os2.size(), 1);
    EXPECT_TRUE(os2.find("key") != os1.end());
}


TEST(ordered_set, constructor_move)
{
    ordered_set<string> os1;
    os1.emplace("key");

    ordered_set<string> os2(move(os1));
    EXPECT_EQ(os2.size(), 1);
    EXPECT_TRUE(os2.find("key") != os1.end());
}


TEST(ordered_set, constructor_ilist)
{
    ordered_set<int> os1 = {1};
    EXPECT_EQ(os1.size(), 1);
    EXPECT_TRUE(os1.find(1) != os1.end());
}


TEST(ordered_set, iteration)
{
    ordered_set<int> os1 = {-1, 2, 1};
    vector<int> keys = {-1, 2, 1};
    set<int> s1(os1.begin(), os1.end());

    size_t index = 0;
    for (auto it = os1.begin(); it != os1.end(); ++it, ++index) {
        EXPECT_EQ(*it, keys[index]);
    }
}


TEST(ordered_set, emplace)
{
    ordered_set<int> os1;
    os1.emplace(1);
    EXPECT_TRUE(os1.find(0) == os1.end());
    EXPECT_TRUE(os1.find(1) != os1.end());
    EXPECT_EQ(os1.size(), 1);
}


TEST(ordered_set, insert)
{
    ordered_set<int> os1;

    {
        // no hint, copy semantics
        os1.insert(-1);
        EXPECT_TRUE(os1.find(-1) != os1.end());
    }
    {
        // hint, with copy semantics
        int i = 0;
        os1.insert(os1.begin(), i);
        EXPECT_TRUE(os1.find(0) != os1.end());
    }
    {
        // hint, with move semantics
        os1.insert(os1.begin(), 1);
        EXPECT_TRUE(os1.find(1) != os1.end());
    }
}


TEST(ordered_set, erase)
{
    ordered_set<int> os1 = {-1, 1, 2};
    {
        EXPECT_EQ(os1.erase(3), 0);
        EXPECT_EQ(os1.size(), 3);
    }
    {
        auto it = os1.cbegin();
        EXPECT_EQ(os1.erase(*it), 1);
        EXPECT_EQ(os1.size(), 2);
    }
}


TEST(ordered_set, clear)
{
    ordered_set<int> os1;
    os1.emplace(1);

    EXPECT_EQ(os1.size(), 1);
    os1.clear();
    EXPECT_EQ(os1.size(), 0);
}


TEST(ordered_set, swap)
{
    ordered_set<int> os1, os2;
    os1.emplace(1);
    EXPECT_EQ(os1.size(), 1);
    EXPECT_EQ(os2.size(), 0);

    os1.swap(os2);
    EXPECT_EQ(os1.size(), 0);
    EXPECT_EQ(os2.size(), 1);
}


TEST(ordered_set, bad_hash)
{
    using bad_map = ordered_set<int, bad_hash<int>>;
    bad_map os1;
    ASSERT_EQ(os1.size(), 0);
    os1.emplace(1);
    os1.emplace(2);
    ASSERT_EQ(os1.size(), 2);
    EXPECT_TRUE(os1.find(1) != os1.end());
    EXPECT_TRUE(os1.find(2) != os1.end());
    EXPECT_TRUE(os1.find(3) == os1.end());
}
#endif
