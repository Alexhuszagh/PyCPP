//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Iterator unittests.
 */

#include <pycpp/iterator.h>
#include <gtest/gtest.h>
#include <vector>

// FUNCTIONS
// ---------


int mul2(int x)
{
    return x * 2;
}

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

