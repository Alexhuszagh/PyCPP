//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Default dict unittests.
 */

#include <pycpp/collections/default_dict.h>
#include <gtest/gtest.h>
#include <memory>

PYCPP_USING_NAMESPACE

// DATA
// ----

using int_ptr = std::shared_ptr<int>;


struct nonconstructable_int
{
public:
    nonconstructable_int() = delete;
    nonconstructable_int(int x): x(x) {};

private:
    int x;
};


int_ptr default_int()
{
    return std::make_shared<int>(0);
}


nonconstructable_int default_nonconstructable_int()
{
    return nonconstructable_int(0);
}

// TESTS
// -----

// DEFAULT MAP

TEST(default_map, constructor)
{
    using intmap = default_map<int, int_ptr>;
    using classmap = default_map<int, nonconstructable_int>;

    intmap m1;
    intmap m2(default_int);
    classmap m3(default_nonconstructable_int);
    intmap m4(m2);
    intmap m5 = m4;
    intmap m6(std::move(m4));
    intmap m7 = std::move(m5);
    classmap m8(m3);
    classmap m9 = m8;
    classmap m10(std::move(m8));
    classmap m11 = std::move(m9);

    intmap m12({{0, default_int()}}, default_int);
    intmap m13(m12.begin(), m12.end(), default_int);
    classmap m14({{0, default_nonconstructable_int()}}, default_nonconstructable_int);
    classmap m15(m14.begin(), m14.end(), default_nonconstructable_int);
}


TEST(default_map, capacity)
{
    using intmap = default_map<int, int_ptr>;

    intmap m1;
    EXPECT_EQ(m1.size(), 0);
    EXPECT_TRUE(m1.empty());
    EXPECT_GT(m1.max_size(), 0);

    intmap m2({{0, default_int()}}, default_int);
    EXPECT_EQ(m2.size(), 1);
    EXPECT_FALSE(m2.empty());
    EXPECT_GT(m2.max_size(), 1);
}


TEST(default_map, iterators)
{
    using intmap = default_map<int, int_ptr>;
    intmap m1({{0, default_int()}}, default_int);

    EXPECT_EQ(m1.begin()->first, 0);
    EXPECT_EQ(m1.cbegin()->first, 0);
    EXPECT_EQ(m1.rbegin()->first, 0);
    EXPECT_EQ(m1.crbegin()->first, 0);
    EXPECT_EQ(std::distance(m1.begin(), m1.end()), 1);
    EXPECT_EQ(std::distance(m1.cbegin(), m1.cend()), 1);
    EXPECT_EQ(std::distance(m1.rbegin(), m1.rend()), 1);
    EXPECT_EQ(std::distance(m1.crbegin(), m1.crend()), 1);
}


TEST(default_map, element_access)
{
    using intmap = default_map<int, int_ptr>;
    intmap m1({{0, default_int()}}, default_int);

    m1.at(0);
    EXPECT_THROW(m1.at(1), std::out_of_range);
    m1[0];
    m1[1];
}


TEST(default_map, operations)
{
    using intmap = default_map<int, int_ptr>;
    intmap m1({{0, default_int()}}, default_int);

    EXPECT_NE(m1.find(0), m1.end());
    EXPECT_EQ(m1.find(1), m1.end());
    EXPECT_EQ(m1.count(0), 1);
    EXPECT_EQ(m1.count(1), 0);
    EXPECT_NE(m1.lower_bound(0), m1.end());
    EXPECT_EQ(m1.upper_bound(0), m1.end());
    auto range = m1.equal_range(0);
    EXPECT_EQ(std::distance(range.first, range.second), 1);
}


TEST(default_map, modifiers)
{
    using intmap = default_map<int, int_ptr>;
    intmap m1;
    intmap m2({{0, default_int()}}, default_int);

    m2.emplace(1, default_int());
    m2.emplace_hint(m2.end(), 2, default_int());
    m2.insert(std::make_pair(3, default_int()));
    m2.insert(m2.end(), std::make_pair(4, default_int()));
    m2.insert({{5, default_int()}});
    m2.erase(m2.begin());
    m2.erase(1);
    m2.erase(m2.begin(), m2.end());
    m2.clear();
    m1.swap(m2);
}


TEST(default_map, observers)
{
    using intmap = default_map<int, int_ptr>;

    intmap m1;
    m1.get_allocator();
    m1.key_comp();
    m1.value_comp();
}


TEST(default_map, operators)
{
    using intmap = default_map<int, int_ptr>;

    intmap m1;
    intmap m2({{0, default_int()}}, default_int);

    EXPECT_EQ(m1, m1);
    EXPECT_EQ(m2, m2);
    EXPECT_NE(m1, m2);
}

// DEFAULT UNORDERED MAP

TEST(default_unordered_map, constructor)
{
    using intmap = default_unordered_map<int, int_ptr>;
    using classmap = default_unordered_map<int, nonconstructable_int>;

    intmap m1;
    intmap m2(default_int);
    classmap m3(default_nonconstructable_int);
    intmap m4(m2);
    intmap m5 = m4;
    intmap m6(std::move(m4));
    intmap m7 = std::move(m5);
    classmap m8(m3);
    classmap m9 = m8;
    classmap m10(std::move(m8));
    classmap m11 = std::move(m9);

    intmap m12({{0, default_int()}}, default_int);
    intmap m13(m12.begin(), m12.end(), default_int);
    classmap m14({{0, default_nonconstructable_int()}}, default_nonconstructable_int);
    classmap m15(m14.begin(), m14.end(), default_nonconstructable_int);
}


TEST(default_unordered_map, capacity)
{
    using intmap = default_unordered_map<int, int_ptr>;

    intmap m1;
    EXPECT_EQ(m1.size(), 0);
    EXPECT_TRUE(m1.empty());
    EXPECT_GT(m1.max_size(), 0);

    intmap m2({{0, default_int()}}, default_int);
    EXPECT_EQ(m2.size(), 1);
    EXPECT_FALSE(m2.empty());
    EXPECT_GT(m2.max_size(), 1);
}


TEST(default_unordered_map, iterators)
{
    using intmap = default_unordered_map<int, int_ptr>;
    intmap m1({{0, default_int()}}, default_int);

    EXPECT_EQ(m1.begin()->first, 0);
    EXPECT_EQ(m1.cbegin()->first, 0);
    EXPECT_EQ(std::distance(m1.begin(), m1.end()), 1);
    EXPECT_EQ(std::distance(m1.cbegin(), m1.cend()), 1);
}


TEST(default_unordered_map, element_access)
{
    using intmap = default_unordered_map<int, int_ptr>;
    intmap m1({{0, default_int()}}, default_int);

    m1.at(0);
    EXPECT_THROW(m1.at(1), std::out_of_range);
    m1[0];
    m1[1];
}


TEST(default_unordered_map, operations)
{
    using intmap = default_unordered_map<int, int_ptr>;
    intmap m1({{0, default_int()}}, default_int);

    EXPECT_NE(m1.find(0), m1.end());
    EXPECT_EQ(m1.find(1), m1.end());
    EXPECT_EQ(m1.count(0), 1);
    EXPECT_EQ(m1.count(1), 0);
    auto range = m1.equal_range(0);
    EXPECT_EQ(std::distance(range.first, range.second), 1);
}


TEST(default_unordered_map, modifiers)
{
    using intmap = default_unordered_map<int, int_ptr>;
    intmap m1;
    intmap m2({{0, default_int()}}, default_int);

    m2.emplace(1, default_int());
    m2.emplace_hint(m2.end(), 2, default_int());
    m2.insert(std::make_pair(3, default_int()));
    m2.insert(m2.end(), std::make_pair(4, default_int()));
    m2.insert({{5, default_int()}});
    m2.erase(m2.begin());
    m2.erase(1);
    m2.erase(m2.begin(), m2.end());
    m2.clear();
    m1.swap(m2);
}


TEST(default_unordered_map, observers)
{
    using intmap = default_unordered_map<int, int_ptr>;

    intmap m1;
    m1.get_allocator();
    m1.hash_function();
    m1.key_eq();
}


TEST(default_unordered_map, buckets)
{
    using intmap = default_unordered_map<int, int_ptr>;
    intmap m1({{0, default_int()}}, default_int);

    m1.bucket_count();
    m1.max_bucket_count();
    m1.bucket_size(0);
    m1.bucket(0);
}


TEST(default_unordered_map, hash_policy)
{
    using intmap = default_unordered_map<int, int_ptr>;
    intmap m1({{0, default_int()}}, default_int);

    m1.load_factor();
    m1.max_load_factor();
    m1.max_load_factor(5);
    m1.rehash(10);
    m1.reserve(10);
}


TEST(default_unordered_map, operators)
{
    using intmap = default_unordered_map<int, int_ptr>;

    intmap m1;
    intmap m2({{0, default_int()}}, default_int);

    EXPECT_EQ(m1, m1);
    EXPECT_EQ(m2, m2);
    EXPECT_NE(m1, m2);
}
