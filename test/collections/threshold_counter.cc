//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Threshold counter unittests.
 */

#include <pycpp/collections/threshold_counter.h>
#include <pycpp/stl/map.h>
#include <gtest/gtest.h>
#include <cmath>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(threshold_counter, ctor)
{
    using t = threshold_counter<int>;

    // empty
    t t1;

    // map
    unordered_map<int, count_t> um = {
        {1, 1},
        {2, 4},
    };
    t t2(um);
    t2 = std::move(um);
    EXPECT_EQ(t2.size(), 2);

    // iter -- map
    map<int, count_t> m = {
        {1, 1},
        {2, 4},
    };
    t t3(m.begin(), m.end());
    EXPECT_EQ(t3.size(), 2);

    // iter -- key
    vector<int> v = {1, 2, 2, 2, 2};
    t t4(v.begin(), v.end());
    EXPECT_EQ(t4.size(), 2);

    // initializer -- map
    t t5 = {
        {1, 1},
        {2, 4},
    };
    EXPECT_EQ(t5.size(), 2);

    // initializer -- key
    t t6 = {1, 2, 2, 2, 2};
    EXPECT_EQ(t6.size(), 2);

    // copy
    t t7(t2);
    EXPECT_EQ(t7.size(), 2);
    t t8;
    t8 = t2;
    EXPECT_EQ(t8.size(), 2);

    // move
    t t9(std::move(t2));
    EXPECT_EQ(t9.size(), 2);
    t t10;
    t10 = std::move(t3);
    EXPECT_EQ(t10.size(), 2);
}


TEST(threshold_counter, capacity)
{
    using t = threshold_counter<int>;
    t t1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    t t2;

    EXPECT_EQ(t1.size(), 3);
    EXPECT_EQ(t2.size(), 0);
    EXPECT_GT(t1.max_size(), 0);
    EXPECT_GT(t2.max_size(), 0);
    EXPECT_FALSE(t1.empty());
    EXPECT_TRUE(t2.empty());
}


TEST(threshold_counter, iterators)
{
    using t = threshold_counter<int>;
    t t1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    t t2;

    auto f1 = t1.begin();
    auto l1 = t1.end();
    auto f2 = t2.begin();
    auto l2 = t2.end();

    EXPECT_EQ(std::distance(f1, l1), 3);
    EXPECT_EQ(std::distance(f2, l2), 0);

    for (auto it = f1; it != l1; ++it) {
        EXPECT_EQ(std::pow(it->first, 2), it->second);
    }
}


TEST(threshold_counter, element_access)
{
    using t = threshold_counter<int>;
    t t1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    t t2;

    // at
    EXPECT_EQ(t1.at(1), 1);
    EXPECT_THROW(t2.at(1), std::out_of_range);
    EXPECT_THROW(t1.at(4), std::out_of_range);
    EXPECT_THROW(t2.at(4), std::out_of_range);

    // get
    EXPECT_EQ(t1.size(), 3);
    EXPECT_EQ(t2.size(), 0);
    EXPECT_EQ(t1.get(1, 0), 1);
    EXPECT_EQ(t2.get(1, 0), 0);
    EXPECT_EQ(t1.get(4, 0), 0);
    EXPECT_EQ(t2.get(4, 0), 0);
    EXPECT_EQ(t1.size(), 3);
    EXPECT_EQ(t2.size(), 0);
}


TEST(threshold_counter, modifiers)
{
    using t = threshold_counter<int>;
    t t1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };

    // add
    EXPECT_EQ(t1.get(4, 0), 0);
    t1.add(4);
    EXPECT_EQ(t1.get(4, 0), 1);
    t1.add(4);
    EXPECT_EQ(t1.get(4, 0), 2);

    // update -- key
    vector<int> v = {5, 5, 6};
    EXPECT_EQ(t1.get(5, 0), 0);
    EXPECT_EQ(t1.get(6, 0), 0);
    t1.update(v.begin(), v.end());
    EXPECT_EQ(t1.get(5, 0), 2);
    EXPECT_EQ(t1.get(6, 0), 1);

    // update -- value
    map<int, count_t> m = {
        {7, 2},
        {8, 1},
    };
    EXPECT_EQ(t1.get(7, 0), 0);
    EXPECT_EQ(t1.get(8, 0), 0);
    t1.update(m.begin(), m.end());
    EXPECT_EQ(t1.get(7, 0), 2);
    EXPECT_EQ(t1.get(8, 0), 1);

    // clear
    t t2(t1);
    EXPECT_FALSE(t1.empty());
    EXPECT_FALSE(t2.empty());
    t2.clear();
    EXPECT_FALSE(t1.empty());
    EXPECT_TRUE(t2.empty());

    // swap
    EXPECT_FALSE(t1.empty());
    EXPECT_TRUE(t2.empty());
    t1.swap(t2);
    EXPECT_TRUE(t1.empty());
    EXPECT_FALSE(t2.empty());
}


TEST(threshold_counter, convenience)
{
    using t = threshold_counter<int>;
    t t1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };

    // most_common
    auto mc0 = t1.most_common();
    auto mc1 = t1.most_common(1);
    auto mc2 = t1.most_common(2);
    auto mc3 = t1.most_common(3);
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
    auto result = t1.elements();
    std::sort(result.begin(), result.end());
    EXPECT_EQ(result, expected);

    // get_common_count
    EXPECT_EQ(t1.get_common_count(), 14);

    // get_uncommon_count
    EXPECT_EQ(t1.get_uncommon_count(), 0);

    // get_commonality
    EXPECT_EQ(t1.get_commonality(), 1.);
}


TEST(threshold_counter, bucket)
{
    using t = threshold_counter<int>;
    t t1;

    t1.bucket_count();
    t1.max_bucket_count();
    t1.bucket_size(t1.bucket(1));
}


TEST(threshold_counter, hash_policy)
{
    using t = threshold_counter<int>;
    t t1;

    t1.load_factor();
    t1.max_load_factor();
    t1.max_load_factor(5.0);
    t1.rehash(5);
    t1.reserve(5);
}


TEST(threshold_counter, observers)
{
    using t = threshold_counter<int>;
    t t1;

    t1.hash_function();
    t1.key_eq();
    t1.get_allocator();
}


TEST(threshold_counter, conversion)
{
    using t = threshold_counter<int>;
    using c = counter<int>;
    t t1 = {
        {1, 1},
        {2, 4},
        {3, 9},
    };
    typename t::counter_type c1(t1);
    typename t::map_type m1(t1);

    EXPECT_TRUE(std::equal(t1.begin(), t1.end(), c1.begin()));
    EXPECT_TRUE(std::equal(t1.begin(), t1.end(), m1.begin()));
}


TEST(threshold_counter, autocompaction)
{
    // ensure counter auto-compacts
    using t = threshold_counter<int>;
    t t1(0.25);

    // no compaction
    t1.add(1);
    t1.add(1);
    t1.add(2);
    t1.add(3);
    EXPECT_EQ(t1.size(), 3);
    EXPECT_EQ(t1.get(1, 0), 2);
    EXPECT_EQ(t1.get(2, 0), 1);
    EXPECT_EQ(t1.get(3, 0), 1);

    // only 1 item remaining
    t1.add(4);
    t1.add(5);
    t1.add(6);
    t1.add(7);
    EXPECT_EQ(t1.size(), 1);
    EXPECT_EQ(t1.get(1, 0), 2);
    EXPECT_EQ(t1.get(2, 0), 0);
    EXPECT_EQ(t1.get(3, 0), 0);
    EXPECT_EQ(t1.get(4, 0), 0);
    EXPECT_EQ(t1.get(5, 0), 0);
    EXPECT_EQ(t1.get(6, 0), 0);
    EXPECT_EQ(t1.get(7, 0), 0);
}
