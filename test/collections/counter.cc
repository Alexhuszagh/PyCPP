//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Standard counter unittests.
 */

#include <pycpp/collections/counter.h>
#include <pycpp/stl/map.h>
#include <gtest/gtest.h>
#include <cmath>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(counter, ctor)
{
    using c = counter<int>;

    // empty
    c c1;

    // map
    unordered_map<int, count_t> um = {
        {1, 1},
        {2, 4},
    };
    c c2(um);
    c2 = std::move(um);
    EXPECT_EQ(c2.size(), 2);

    // iter -- map
    map<int, count_t> m = {
        {1, 1},
        {2, 4},
    };
    c c3(m.begin(), m.end());
    EXPECT_EQ(c3.size(), 2);

    // iter -- key
    vector<int> v = {1, 2, 2, 2, 2};
    c c4(v.begin(), v.end());
    EXPECT_EQ(c4.size(), 2);

    // initializer -- map
    c c5 = {
        {1, 1},
        {2, 4},
    };
    EXPECT_EQ(c5.size(), 2);

    // initializer -- key
    c c6 = {1, 2, 2, 2, 2};
    EXPECT_EQ(c6.size(), 2);

    // copy
    c c7(c2);
    EXPECT_EQ(c7.size(), 2);
    c c8;
    c8 = c2;
    EXPECT_EQ(c8.size(), 2);

    // move
    c c9(std::move(c2));
    EXPECT_EQ(c9.size(), 2);
    c c10;
    c10 = std::move(c3);
    EXPECT_EQ(c10.size(), 2);
}


TEST(counter, capacity)
{
    using c = counter<int>;
    c c1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    c c2;

    EXPECT_EQ(c1.size(), 3);
    EXPECT_EQ(c2.size(), 0);
    EXPECT_GT(c1.max_size(), 0);
    EXPECT_GT(c2.max_size(), 0);
    EXPECT_FALSE(c1.empty());
    EXPECT_TRUE(c2.empty());
}


TEST(counter, iterators)
{
    using c = counter<int>;
    c c1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    c c2;

    auto f1 = c1.begin();
    auto l1 = c1.end();
    auto f2 = c2.begin();
    auto l2 = c2.end();

    EXPECT_EQ(std::distance(f1, l1), 3);
    EXPECT_EQ(std::distance(f2, l2), 0);

    for (auto it = f1; it != l1; ++it) {
        EXPECT_EQ(std::pow(it->first, 2), it->second);
    }
}


TEST(counter, element_access)
{
    using c = counter<int>;
    c c1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    c c2;

    // at
    EXPECT_EQ(c1.at(1), 1);
    EXPECT_THROW(c2.at(1), std::out_of_range);
    EXPECT_THROW(c1.at(4), std::out_of_range);
    EXPECT_THROW(c2.at(4), std::out_of_range);

    // get
    EXPECT_EQ(c1.size(), 3);
    EXPECT_EQ(c2.size(), 0);
    EXPECT_EQ(c1.get(1, 0), 1);
    EXPECT_EQ(c2.get(1, 0), 0);
    EXPECT_EQ(c1.get(4, 0), 0);
    EXPECT_EQ(c2.get(4, 0), 0);
    EXPECT_EQ(c1.size(), 3);
    EXPECT_EQ(c2.size(), 0);

    // operator[]
    EXPECT_EQ(c1.size(), 3);
    EXPECT_EQ(c2.size(), 0);
    EXPECT_EQ(c1[1], 1);
    EXPECT_EQ(c2[1], 0);
    EXPECT_EQ(c1[4], 0);
    EXPECT_EQ(c2[4], 0);
    EXPECT_EQ(c1.size(), 4);
    EXPECT_EQ(c2.size(), 2);
}


TEST(counter, modifiers)
{
    using c = counter<int>;
    c c1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };

    // add
    EXPECT_EQ(c1.get(4, 0), 0);
    c1.add(4);
    EXPECT_EQ(c1.get(4, 0), 1);
    c1.add(4);
    EXPECT_EQ(c1.get(4, 0), 2);

    // update -- key
    vector<int> v = {5, 5, 6};
    EXPECT_EQ(c1.get(5, 0), 0);
    EXPECT_EQ(c1.get(6, 0), 0);
    c1.update(v.begin(), v.end());
    EXPECT_EQ(c1.get(5, 0), 2);
    EXPECT_EQ(c1.get(6, 0), 1);

    // update -- map
    map<int, count_t> m = {
        {7, 2},
        {8, 1},
    };
    EXPECT_EQ(c1.get(7, 0), 0);
    EXPECT_EQ(c1.get(8, 0), 0);
    c1.update(m.begin(), m.end());
    EXPECT_EQ(c1.get(7, 0), 2);
    EXPECT_EQ(c1.get(8, 0), 1);

    // erase
    EXPECT_EQ(c1.get(7, 0), 2);
    c1.erase(7);
    EXPECT_EQ(c1.get(7, 0), 0);

    // clear
    c c2(c1);
    EXPECT_FALSE(c1.empty());
    EXPECT_FALSE(c2.empty());
    c2.clear();
    EXPECT_FALSE(c1.empty());
    EXPECT_TRUE(c2.empty());

    // swap
    EXPECT_FALSE(c1.empty());
    EXPECT_TRUE(c2.empty());
    c1.swap(c2);
    EXPECT_TRUE(c1.empty());
    EXPECT_FALSE(c2.empty());
}


TEST(counter, operators)
{
    using c = counter<int>;
    c c1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    c c2 = {4, 5};

    // Note: only check non-assigment operators, they call
    // assignment operators internally

    // +
    auto c3 = c1 + c2;
    EXPECT_EQ(c3.get(1, 0), 1);
    EXPECT_EQ(c3.get(2, 0), 4);
    EXPECT_EQ(c3.get(3, 0), 9);
    EXPECT_EQ(c3.get(4, 0), 1);
    EXPECT_EQ(c3.get(5, 0), 1);

    auto c4 = c1 + 3;
    EXPECT_EQ(c4.get(1, 0), 4);
    EXPECT_EQ(c4.get(2, 0), 7);
    EXPECT_EQ(c4.get(3, 0), 12);
    EXPECT_EQ(c4.get(4, 0), 0);

    // -
    auto c5 = c1 - c2;
    EXPECT_EQ(c5.get(1, 0), 1);
    EXPECT_EQ(c5.get(2, 0), 4);
    EXPECT_EQ(c5.get(3, 0), 9);
    EXPECT_EQ(c5.get(4, 0), -1);
    EXPECT_EQ(c5.get(5, 0), -1);

    auto c6 = c1 - 3;
    EXPECT_EQ(c6.get(1, 0), -2);
    EXPECT_EQ(c6.get(2, 0), 1);
    EXPECT_EQ(c6.get(3, 0), 6);
    EXPECT_EQ(c6.get(4, 0), 0);

    // &
    auto c7 = c1 & c2;
    EXPECT_TRUE(c7.empty());

    // |
    auto c8 = c1 | c2;
    EXPECT_EQ(c8.get(1, 0), 1);
    EXPECT_EQ(c8.get(2, 0), 4);
    EXPECT_EQ(c8.get(3, 0), 9);
    EXPECT_EQ(c8.get(4, 0), 1);
    EXPECT_EQ(c8.get(5, 0), 1);
}


TEST(counter, convenience)
{
    using c = counter<int>;
    c c1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };

    // most_common
    auto mc0 = c1.most_common();
    auto mc1 = c1.most_common(1);
    auto mc2 = c1.most_common(2);
    auto mc3 = c1.most_common(3);
    EXPECT_EQ(mc0.size(), 3);
    EXPECT_EQ(mc1.size(), 1);
    EXPECT_EQ(mc2.size(), 2);
    EXPECT_EQ(mc3.size(), 3);
    EXPECT_EQ(mc0[0].first, 3);
    EXPECT_EQ(mc1[0].first, 3);
    EXPECT_EQ(mc2[0].first, 3);
    EXPECT_EQ(mc3[0].first, 3);
    EXPECT_EQ(mc0[1].first, 2);
    EXPECT_EQ(mc2[1].first, 2);
    EXPECT_EQ(mc3[1].first, 2);
    EXPECT_EQ(mc0[2].first, 1);
    EXPECT_EQ(mc3[2].first, 1);

    // elements
    vector<int> expected = {1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};
    auto result = c1.elements();
    std::sort(result.begin(), result.end());
    EXPECT_EQ(result, expected);
}


TEST(counter, bucket)
{
    using c = counter<int>;
    c c1;

    c1.bucket_count();
    c1.max_bucket_count();
    c1.bucket_size(c1.bucket(1));
}


TEST(counter, hash_policy)
{
    using c = counter<int>;
    c c1;

    c1.load_factor();
    c1.max_load_factor();
    c1.max_load_factor(5.0);
    c1.rehash(5);
    c1.reserve(5);
}


TEST(counter, observers)
{
    using c = counter<int>;
    c c1;

    c1.hash_function();
    c1.key_eq();
    c1.get_allocator();
}


TEST(counter, conversion)
{
    using c = counter<int>;
    c c1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    typename c::map_type m1(c1);

    EXPECT_TRUE(std::equal(c1.begin(), c1.end(), m1.begin()));
}
