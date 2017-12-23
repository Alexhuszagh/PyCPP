//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Ordered map unittests.
 */

#include <pycpp/collections/ordered_map.h>
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


TEST(ordered_map, constructor_null)
{
    ordered_map<string, string> m1;
    EXPECT_EQ(m1.size(), 0);
    m1["key"] = "value";
    EXPECT_EQ(m1.size(), 1);
    EXPECT_EQ(m1.at("key"), "value");
}


TEST(ordered_map, constructor_iterable)
{
    ordered_map<string, string> m1;
    m1["key"] = "value";

    ordered_map<string, string> m2(m1.begin(), m1.end());
    EXPECT_EQ(m2.size(), 1);
    EXPECT_EQ(m2.at("key"), "value");
}


TEST(ordered_map, constructor_copy)
{
    ordered_map<string, string> m1;
    m1["key"] = "value";

    ordered_map<string, string> m2(m1);
    EXPECT_EQ(m1.size(), 1);
    EXPECT_EQ(m2.size(), 1);
    EXPECT_EQ(m2.at("key"), "value");
}


TEST(ordered_map, constructor_move)
{
    ordered_map<string, string> m1;
    m1["key"] = "value";

    ordered_map<string, string> m2(move(m1));
    EXPECT_EQ(m2.size(), 1);
    EXPECT_EQ(m2.at("key"), "value");
}


TEST(ordered_map, constructor_ilist)
{
    ordered_map<int, int> m1 = {{1, 2},};
    EXPECT_EQ(m1.size(), 1);
    EXPECT_EQ(m1.at(1), 2);
}


TEST(ordered_map, iteration)
{
    ordered_map<int, int> m1;
    vector<int> keys = {1, 2, -1};
    vector<int> values = {3, 5, 6};

    for (unsigned int i = 0; i < keys.size(); i++) {
        m1[keys[i]] = values[i];
    }

    {
        // forward iterator tests
        unsigned int index = 0;
        ordered_map<int, int>::iterator it;
        for (it = m1.begin(); it != m1.end(); ++it, index++) {
            EXPECT_EQ(it->first, keys[index]);
            EXPECT_EQ(it->second, values[index]);
        }
    }
    {
        // reverse iterator tests
        unsigned int index = 2;
        ordered_map<int, int>::reverse_iterator it;
        for (it = m1.rbegin(); it != m1.rend(); ++it, index--) {
            EXPECT_EQ(it->first, keys[index]);
            EXPECT_EQ(it->second, values[index]);
        }
    }
}


TEST(ordered_map, incrementors)
{
    ordered_map<int, int> m1;
    m1[1] = 5;
    m1[2] = 4;
    m1[-1] = 3;
    {
        // forward iterators
        auto it = m1.begin();
        EXPECT_EQ(it->first, 1);

        // increment a copy
        EXPECT_EQ((it++)->first, 1);
        EXPECT_EQ(it->first, 2);

        // incrementors/decrementors
        it++;
        EXPECT_EQ(it->first, -1);
        it--;
        EXPECT_EQ(it->first, 2);
        ++it;
        EXPECT_EQ(it->first, -1);
        --it;
        EXPECT_EQ(it->first, 2);
    }

    {
        // reverse iterators
        auto it = m1.rbegin();
        EXPECT_EQ(it->first, -1);

        // increment a copy
        EXPECT_EQ((it++)->first, -1);
        EXPECT_EQ(it->first, 2);

        // incrementors/decrementors
        it++;
        EXPECT_EQ(it->first, 1);
        it--;
        EXPECT_EQ(it->first, 2);
        ++it;
        EXPECT_EQ(it->first, 1);
        --it;
        EXPECT_EQ(it->first, 2);
    }
}


TEST(ordered_map, emplace)
{
    ordered_map<int, int> m1;
    m1.emplace(1, 1);
    EXPECT_EQ(m1[1], 1);
    EXPECT_EQ(m1.size(), 1);
}


TEST(ordered_map, insert)
{
    ordered_map<int, int> m1;

    {
        // no hint, copy semantics
        auto pair = make_pair(-1, 4);
        m1.insert(pair);
        EXPECT_EQ(m1[-1], 4);
    }
    {
        // hint, with copy semantics
        auto pair = make_pair(0, 3);
        m1.insert(m1.begin(), pair);
        EXPECT_EQ(m1[0], 3);
    }
    {
        // hint, with move semantics
        m1.insert(m1.begin(), make_pair(1, 2));
        EXPECT_EQ(m1[1], 2);
    }
    {
        // initializer list
        m1.insert({{3, 5}});
        EXPECT_EQ(m1[3], 5);
    }
}



TEST(ordered_map, erase)
{
    ordered_map<int, int> m1;
    m1[1] = 2;
    m1[3] = 4;
    m1[-1] = 2;

    {
        m1.erase(3);
        EXPECT_EQ(m1.size(), 2);
        auto it = m1.begin();
        EXPECT_EQ(it->first, 1);
        ++it;
        EXPECT_EQ(it->first, -1);
    }
    {
        auto it = m1.cbegin();
        EXPECT_EQ(m1.erase(it)->first, -1);
        EXPECT_EQ(m1.size(), 1);
    }
}


TEST(ordered_map, clear)
{
    ordered_map<int, int> m1;
    m1[1] = 5;

    EXPECT_EQ(m1.size(), 1);
    m1.clear();
    EXPECT_EQ(m1.size(), 0);
}


TEST(ordered_map, swap)
{
    ordered_map<int, int> m1, m2;
    m1[1] = 5;
    EXPECT_EQ(m1.size(), 1);
    EXPECT_EQ(m2.size(), 0);

    m1.swap(m2);
    EXPECT_EQ(m1.size(), 0);
    EXPECT_EQ(m2.size(), 1);
}


TEST(ordered_map, bad_hash)
{
    using bad_map = ordered_map<int, int, bad_hash<int>>;
    bad_map m1;
    ASSERT_EQ(m1.size(), 0);
    m1[1] = 1;
    m1[2] = 4;
    ASSERT_EQ(m1.size(), 2);
    EXPECT_EQ(m1[1], 1);
    EXPECT_EQ(m1[2], 4);
}
