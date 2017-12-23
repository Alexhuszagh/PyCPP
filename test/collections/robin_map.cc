//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Robin hood map unittests.
 */

#include <pycpp/collections/robin_map.h>
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


TEST(robin_map, constructor_null)
{
    robin_map<string, std::string> m1;
    EXPECT_EQ(m1.size(), 0);
    m1["key"] = "value";
    EXPECT_EQ(m1.size(), 1);
    EXPECT_EQ(m1.at("key"), "value");
}


TEST(robin_map, constructor_iterable)
{
    robin_map<string, string> m1;
    m1["key"] = "value";

    robin_map<string, string> m2(m1.begin(), m1.end());
    EXPECT_EQ(m2.size(), 1);
    EXPECT_EQ(m2.at("key"), "value");
}


TEST(robin_map, constructor_copy)
{
    robin_map<string, string> m1;
    m1["key"] = "value";

    robin_map<string, string> m2(m1);
    EXPECT_EQ(m1.size(), 1);
    EXPECT_EQ(m2.size(), 1);
    EXPECT_EQ(m2.at("key"), "value");
}


TEST(robin_map, constructor_move)
{
    robin_map<string, string> m1;
    m1["key"] = "value";

    robin_map<string, string> m2(move(m1));
    EXPECT_EQ(m2.size(), 1);
    EXPECT_EQ(m2.at("key"), "value");
}


TEST(robin_map, constructor_ilist)
{
    robin_map<int, int> m1 = {{1, 2},};
    EXPECT_EQ(m1.size(), 1);
    EXPECT_EQ(m1.at(1), 2);
}
