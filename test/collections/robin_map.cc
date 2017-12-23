//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Robin hood map unittests.
 */

#include <pycpp/collections/robin_map.h>
#include <pycpp/stl/map.h>
#include <pycpp/stl/string.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TODO: need to test mutable value types...

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


TEST(robin_map, constructor_null)
{
    robin_map<string, string> rm1;
    EXPECT_EQ(rm1.size(), 0);
    rm1["key"] = "value";
    EXPECT_EQ(rm1.size(), 1);
    EXPECT_EQ(rm1.at("key"), "value");
}


TEST(robin_map, constructor_iterable)
{
    robin_map<string, string> rm1;
    rm1["key"] = "value";

    robin_map<string, string> rm2(rm1.begin(), rm1.end());
    EXPECT_EQ(rm2.size(), 1);
    EXPECT_EQ(rm2.at("key"), "value");
}


TEST(robin_map, constructor_copy)
{
    robin_map<string, string> rm1;
    rm1["key"] = "value";

    robin_map<string, string> rm2(rm1);
    EXPECT_EQ(rm1.size(), 1);
    EXPECT_EQ(rm2.size(), 1);
    EXPECT_EQ(rm2.at("key"), "value");
}


TEST(robin_map, constructor_move)
{
    robin_map<string, string> rm1;
    rm1["key"] = "value";

    robin_map<string, string> rm2(move(rm1));
    EXPECT_EQ(rm2.size(), 1);
    EXPECT_EQ(rm2.at("key"), "value");
}


TEST(robin_map, constructor_ilist)
{
    robin_map<int, int> rm1 = {{1, 2},};
    EXPECT_EQ(rm1.size(), 1);
    EXPECT_EQ(rm1.at(1), 2);
}


TEST(robin_map, iteration)
{
    robin_map<int, int> rm1 = {
        {-1, 6},
        {1, 3},
        {2, 5},
    };
    map<int, int> m1(rm1.begin(), rm1.end());

    for (auto it = rm1.begin(); it != rm1.end(); ++it) {
        EXPECT_TRUE(m1.find(it->first) != m1.end());
        EXPECT_EQ(m1.at(it->first), it->second);
    }
}


TEST(robin_map, emplace)
{
    robin_map<int, int> rm1;
    rm1.emplace(1, 1);
    EXPECT_EQ(rm1[1], 1);
    EXPECT_EQ(rm1.size(), 1);
}


TEST(robin_map, insert)
{
    robin_map<int, int> rm1;

    {
        // no hint, copy semantics
        auto pair = make_pair(-1, 4);
        rm1.insert(pair);
        EXPECT_EQ(rm1[-1], 4);
    }
    {
        // hint, with copy semantics
        auto pair = make_pair(0, 3);
        rm1.insert(rm1.begin(), pair);
        EXPECT_EQ(rm1[0], 3);
    }
    {
        // hint, with move semantics
        rm1.insert(rm1.begin(), make_pair(1, 2));
        EXPECT_EQ(rm1[1], 2);
    }
    {
        // initializer list
        rm1.insert({{3, 5}});
        EXPECT_EQ(rm1[3], 5);
    }
}


TEST(robin_map, erase)
{
    robin_map<int, int> rm1 = {
        {-1, 2},
        {1, 2},
        {2, 4},
    };

    {
        EXPECT_EQ(rm1.erase(3), 0);
        EXPECT_EQ(rm1.size(), 3);
    }
    {
        auto it = rm1.cbegin();
        EXPECT_EQ(rm1.erase(it->first), 1);
        EXPECT_EQ(rm1.size(), 2);
    }
}


TEST(robin_map, clear)
{
    robin_map<int, int> rm1;
    rm1[1] = 5;

    EXPECT_EQ(rm1.size(), 1);
    rm1.clear();
    EXPECT_EQ(rm1.size(), 0);
}


TEST(robin_map, swap)
{
    robin_map<int, int> rm1, rm2;
    rm1[1] = 5;
    EXPECT_EQ(rm1.size(), 1);
    EXPECT_EQ(rm2.size(), 0);

    rm1.swap(rm2);
    EXPECT_EQ(rm1.size(), 0);
    EXPECT_EQ(rm2.size(), 1);
}


TEST(robin_map, bad_hash)
{
    using bad_map = robin_map<int, int, bad_hash<int>>;
    bad_map rm1;
    ASSERT_EQ(rm1.size(), 0);
    rm1[1] = 1;
    rm1[2] = 4;
    ASSERT_EQ(rm1.size(), 2);
    EXPECT_EQ(rm1[1], 1);
    EXPECT_EQ(rm1[2], 4);
}
