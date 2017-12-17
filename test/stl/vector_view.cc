//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief vector_view unittests.
 */

#include <pycpp/stl/vector_view.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

vector<int> VEC = {0, 1, 2, 3, 4, 5};
vector<int> EMPTY;

// TESTS
// -----


TEST(vector_view, constructors)
{
    vector_view<int> vec(VEC);
    vector_view<int> other;
    EXPECT_EQ(vec.size(), 6);

    vec = vector_view<int>(VEC);
    EXPECT_EQ(vec.size(), 6);

    vec = vector_view<int>(VEC.data(), VEC.size());
    EXPECT_EQ(vec.size(), 6);

    vec = move(other);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(other.size(), 6);

    vec = other;
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(other.size(), 6);
}


TEST(vector_view, swap)
{
    vector_view<int> vec(VEC);
    vector_view<int> other;

    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(other.size(), 0);

    swap(vec, other);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(other.size(), 6);

    vec.swap(other);
    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(other.size(), 0);
}


TEST(vector_view, relational)
{
    vector_view<int> vec(VEC);
    vector_view<int> other;

    // eq
    EXPECT_TRUE(vec == vec);
    EXPECT_FALSE(vec == other);

    // ne
    EXPECT_FALSE(vec != vec);
    EXPECT_TRUE(vec != other);

    // lt
    EXPECT_FALSE(vec < vec);
    EXPECT_FALSE(vec < other);

    // le
    EXPECT_TRUE(vec <= vec);
    EXPECT_FALSE(vec <= other);

    // gt
    EXPECT_FALSE(vec > vec);
    EXPECT_TRUE(vec > other);

    // ge
    EXPECT_TRUE(vec >= vec);
    EXPECT_TRUE(vec >= other);
}


TEST(vector_view, iterator)
{
    vector_view<int> vec(VEC);
    vector<int> reversed(VEC.rbegin(), VEC.rend());
    EXPECT_EQ(vector<int>(vec.begin(), vec.end()), VEC);
    EXPECT_EQ(vector<int>(vec.rbegin(), vec.rend()), reversed);
}


TEST(vector_view, capacity)
{
    vector_view<int> vec(VEC);
    vector_view<int> other;

    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(other.size(), 0);

    EXPECT_FALSE(vec.empty());
    EXPECT_TRUE(other.empty());
}


TEST(vector_view, element)
{
    vector_view<int> vec(VEC);

    EXPECT_EQ(vec[0], 0);
    EXPECT_EQ(vec.at(0), 0);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec.at(1), 1);
    EXPECT_EQ(vec.front(), 0);
    EXPECT_EQ(vec.back(), 5);
}


TEST(vector_view, modifier)
{
    vector_view<int> vec(VEC);
    vector_view<int> other;

    EXPECT_EQ(vec.size(), 6);
    EXPECT_EQ(other.size(), 0);

    vec.swap(other);
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(other.size(), 6);

    other.remove_prefix(1);
    EXPECT_EQ(other.size(), 5);
    EXPECT_EQ(other.at(0), 1);

    other.remove_suffix(2);
    EXPECT_EQ(other.size(), 3);
}


TEST(vector_view, operations)
{
    vector_view<int> vec(VEC);
    int* buffer[6];
}


TEST(vector_view, conversions)
{
    vector_view<int> vec(VEC);
    vector_view<int> other;

    EXPECT_TRUE(bool(vec));
    EXPECT_FALSE(bool(other));

    EXPECT_EQ(vector<int>(vec), VEC);
    EXPECT_EQ(vector<int>(other), EMPTY);
}
