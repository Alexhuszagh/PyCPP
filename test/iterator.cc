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