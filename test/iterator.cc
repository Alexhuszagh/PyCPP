//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Iterator unittests.
 */

#include <pycpp/iterator.h>
#include <gtest/gtest.h>
#include <vector>

PYCPP_USING_NAMESPACE

// HELPERS
// -------


int mul2(int x)
{
    return x * 2;
}


struct int_generator
{
    using value_type = int;
    int counter;

    int_generator(int c = 5):
        counter(c)
    {}

    int operator()()
    {
        return counter++;
    }

    explicit operator bool() const
    {
        return counter < 5;
    }
};

// TESTS
// -----


TEST(iterator, transform_iterator)
{
    std::vector<int> list = {0, 1, 2, 3, 4};
    auto first = make_transform_iterator(list.begin(), mul2);
    auto last = make_transform_iterator(list.end(), mul2);
    EXPECT_TRUE(std::equal(first, last, list.begin(), [](int l, int r) {
        return l == r * 2;
    }));

    // incrementors
    EXPECT_EQ(*first, 0);
    ++first;
    EXPECT_EQ(*first, 2);
    --first;

    // indexing
    EXPECT_EQ(first[4], 8);

    // arithmetic
    first += 4;
    EXPECT_EQ(*first, 8);
    first -= 2;
    EXPECT_EQ(*first, 4);
    first -= 2;
    EXPECT_EQ(*first, 0);

    // comparison
    EXPECT_EQ(first, first);
    EXPECT_NE(first, last);
    EXPECT_LT(first, last);
    EXPECT_LE(first, last);
    EXPECT_GT(last, first);
    EXPECT_GE(last, first);
}


TEST(iterator, input_iterator_facade)
{
    using iterator = input_iterator_facade<int_generator>;
    int_generator g(0);
    auto v = std::vector<int>(iterator(g), iterator());
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 0);
    EXPECT_EQ(v[4], 4);
}


TEST(iterator, unique_range)
{
    using vector = std::vector<int>;
    using vector_range = unique_range<typename vector::const_iterator>;
    using input_iterator = input_iterator_facade<int_generator>;
    using input_range = unique_range<input_iterator>;

    // forward+ iterators
    vector v = {1, 2, 3, 1, 4, 2, 5};
    vector_range r1(v.begin(), v.end());
    vector_range r2(v.begin(), v.end());
    vector v1(r1.begin(), r1.end());
    vector v2(r2.begin(), r2.end());
    ASSERT_EQ(v1.size(), 5);
    ASSERT_EQ(v2.size(), 5);
    EXPECT_EQ(v1[0], 1);
    EXPECT_EQ(v2[0], 1);
    EXPECT_EQ(v1[4], 5);
    EXPECT_EQ(v2[4], 5);

    // input iterators
    int_generator g(0);
    auto r3 = input_range(input_iterator(g), input_iterator());
    vector v3(r3.begin(), r3.end());
    ASSERT_EQ(v3.size(), 5);
    EXPECT_EQ(v3[0], 0);
    EXPECT_EQ(v3[4], 4);
}

