//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Ordered map unittests.
 */

#include <pycpp/collections/ordered_map.h>
#include <pycpp/stl/string.h>
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


TEST(ordered_map, constructor_null)
{
    ordered_map<std::string, std::string> map;
    EXPECT_EQ(map.size(), 0);
    map["key"] = "value";
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map.at("key"), "value");
}


TEST(ordered_map, constructor_iterable)
{
    ordered_map<std::string, std::string> map;
    map["key"] = "value";

    ordered_map<std::string, std::string> it(map.begin(), map.end());
    EXPECT_EQ(it.size(), 1);
    EXPECT_EQ(it.at("key"), "value");
}


TEST(ordered_map, constructor_copy)
{
    ordered_map<std::string, std::string> map;
    map["key"] = "value";

    ordered_map<std::string, std::string> copy(map);
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(copy.size(), 1);
    EXPECT_EQ(copy.at("key"), "value");
}


TEST(ordered_map, constructor_move)
{
    ordered_map<std::string, std::string> map;
    map["key"] = "value";

    ordered_map<std::string, std::string> move(std::move(map));
    EXPECT_EQ(move.size(), 1);
    EXPECT_EQ(move.at("key"), "value");
}


TEST(ordered_map, constructor_ilist)
{
    ordered_map<int, int> initializer = {{1, 2},};
    EXPECT_EQ(initializer.size(), 1);
    EXPECT_EQ(initializer.at(1), 2);
}


TEST(ordered_map, iteration)
{
    ordered_map<int, int> map;
    std::vector<int> keys = {1, 2, -1};
    std::vector<int> values = {3, 5, 6};

    for (unsigned int i = 0; i < keys.size(); i++) {
        map[keys[i]] = values[i];
    }

    {
        // forward iterator tests
        unsigned int index = 0;
        ordered_map<int, int>::iterator it;
        for (it = map.begin(); it != map.end(); ++it, index++) {
            EXPECT_EQ(it->first, keys[index]);
            EXPECT_EQ(it->second, values[index]);
        }
    }
    {
        // reverse iterator tests
        unsigned int index = 2;
        ordered_map<int, int>::reverse_iterator it;
        for (it = map.rbegin(); it != map.rend(); ++it, index--) {
            EXPECT_EQ(it->first, keys[index]);
            EXPECT_EQ(it->second, values[index]);
        }
    }
}


TEST(ordered_map, incrementors)
{
    ordered_map<int, int> map;
    map[1] = 5;
    map[2] = 4;
    map[-1] = 3;
    {
        // forward iterators
        auto it = map.begin();
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
        auto it = map.rbegin();
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
    ordered_map<int, int> map;
    map.emplace(1, 1);
    EXPECT_EQ(map[1], 1);
    EXPECT_EQ(map.size(), 1);
}


TEST(ordered_map, insert)
{
    ordered_map<int, int> map;

    {
        // no hint, copy semantics
        auto pair = std::make_pair(-1, 4);
        map.insert(pair);
        EXPECT_EQ(map[-1], 4);
    }
    {
        // hint, with copy semantics
        auto pair = std::make_pair(0, 3);
        map.insert(map.begin(), pair);
        EXPECT_EQ(map[0], 3);
    }
    {
        // hint, with move semantics
        map.insert(map.begin(), std::make_pair(1, 2));
        EXPECT_EQ(map[1], 2);
    }
    {
        // initializer list
        map.insert({{3, 5}});
        EXPECT_EQ(map[3], 5);
    }
}



TEST(ordered_map, erase)
{
    ordered_map<int, int> map;
    map[1] = 2;
    map[3] = 4;
    map[-1] = 2;

    {
        map.erase(3);
        EXPECT_EQ(map.size(), 2);
        auto it = map.begin();
        EXPECT_EQ(it->first, 1);
        ++it;
        EXPECT_EQ(it->first, -1);
    }
    {
        auto it = map.cbegin();
        EXPECT_EQ(map.erase(it)->first, -1);
        EXPECT_EQ(map.size(), 1);
    }
}


TEST(ordered_map, clear)
{
    ordered_map<int, int> map;
    map[1] = 5;

    EXPECT_EQ(map.size(), 1);
    map.clear();
    EXPECT_EQ(map.size(), 0);
}


TEST(ordered_map, swap)
{
    ordered_map<int, int> first, second;
    first[1] = 5;
    EXPECT_EQ(first.size(), 1);
    EXPECT_EQ(second.size(), 0);

    first.swap(second);
    EXPECT_EQ(first.size(), 0);
    EXPECT_EQ(second.size(), 1);
}


TEST(ordered_map, bad_hash)
{
    using bad_map = ordered_map<int, int, bad_hash<int>>;
    bad_map map;
    ASSERT_EQ(map.size(), 0);
    map[1] = 1;
    map[2] = 4;
    ASSERT_EQ(map.size(), 2);
    EXPECT_EQ(map[1], 1);
    EXPECT_EQ(map[2], 4);
}
