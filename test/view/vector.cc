//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief vector_view unittests.
 */

#include <pycpp/view/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

std::vector<int> VEC = {0, 1, 2, 3, 4, 5};
std::vector<int> EMPTY;

// TESTS
// -----


TEST(vector_view, constructors)
{
    vector_view<int> vector(VEC);
    vector_view<int> other;
    EXPECT_EQ(vector.size(), 6);

    vector = vector_view<int>(VEC);
    EXPECT_EQ(vector.size(), 6);

    vector = vector_view<int>(VEC.data(), VEC.size());
    EXPECT_EQ(vector.size(), 6);

    vector = std::move(other);
    EXPECT_EQ(vector.size(), 0);
    EXPECT_EQ(other.size(), 6);

    vector = other;
    EXPECT_EQ(vector.size(), 6);
    EXPECT_EQ(other.size(), 6);
}


TEST(vector_view, swap)
{
    vector_view<int> vector(VEC);
    vector_view<int> other;

    EXPECT_EQ(vector.size(), 6);
    EXPECT_EQ(other.size(), 0);

    std::swap(vector, other);
    EXPECT_EQ(vector.size(), 0);
    EXPECT_EQ(other.size(), 6);

    vector.swap(other);
    EXPECT_EQ(vector.size(), 6);
    EXPECT_EQ(other.size(), 0);
}


TEST(vector_view, relational)
{
    vector_view<int> vector(VEC);
    vector_view<int> other;

    // eq
    EXPECT_TRUE(vector == vector);
    EXPECT_FALSE(vector == other);

    // ne
    EXPECT_FALSE(vector != vector);
    EXPECT_TRUE(vector != other);

    // lt
    EXPECT_FALSE(vector < vector);
    EXPECT_FALSE(vector < other);

    // le
    EXPECT_TRUE(vector <= vector);
    EXPECT_FALSE(vector <= other);

    // gt
    EXPECT_FALSE(vector > vector);
    EXPECT_TRUE(vector > other);

    // ge
    EXPECT_TRUE(vector >= vector);
    EXPECT_TRUE(vector >= other);
}


TEST(vector_view, iterator)
{
    vector_view<int> vector(VEC);
    std::vector<int> reversed(VEC.rbegin(), VEC.rend());
    EXPECT_EQ(std::vector<int>(vector.begin(), vector.end()), VEC);
    EXPECT_EQ(std::vector<int>(vector.rbegin(), vector.rend()), reversed);
}


TEST(vector_view, capacity)
{
    vector_view<int> vector(VEC);
    vector_view<int> other;

    EXPECT_EQ(vector.size(), 6);
    EXPECT_EQ(other.size(), 0);

    EXPECT_FALSE(vector.empty());
    EXPECT_TRUE(other.empty());
}


TEST(vector_view, element)
{
    vector_view<int> vector(VEC);

    EXPECT_EQ(vector[0], 0);
    EXPECT_EQ(vector.at(0), 0);
    EXPECT_EQ(vector[1], 1);
    EXPECT_EQ(vector.at(1), 1);
    EXPECT_EQ(vector.front(), 0);
    EXPECT_EQ(vector.back(), 5);
}


TEST(vector_view, modifier)
{
    vector_view<int> vector(VEC);
    vector_view<int> other;

    EXPECT_EQ(vector.size(), 6);
    EXPECT_EQ(other.size(), 0);

    vector.swap(other);
    EXPECT_EQ(vector.size(), 0);
    EXPECT_EQ(other.size(), 6);
}


TEST(vector_view, conversions)
{
    vector_view<int> vector(VEC);
    vector_view<int> other;

    EXPECT_TRUE(bool(vector));
    EXPECT_FALSE(bool(other));

    EXPECT_EQ(std::vector<int>(vector), VEC);
    EXPECT_EQ(std::vector<int>(other), EMPTY);
}
