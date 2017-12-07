//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Heap Pimpl unittests.
 */

#include <pycpp/misc/heap_pimpl.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(unique_heap_pimpl, integer)
{
    typedef unique_heap_pimpl<int> pint;

    pint x(2);
    pint y;
    EXPECT_EQ(x, 2);

    x = 5;
    EXPECT_EQ(x, 5);

    y = x;
    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 5);

    // modifying a copy does not modify the original
    y = 2;
    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 2);

    std::swap(x, y);
    EXPECT_EQ(x, 2);
    EXPECT_EQ(y, 5);
}


TEST(shared_heap_pimpl, integer)
{
    typedef shared_heap_pimpl<int> pint;

    pint x(2);
    pint y;
    EXPECT_EQ(x, 2);

    x = 5;
    EXPECT_EQ(x, 5);

    y = x;
    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 5);

    // modifying a copy modifies the original
    y = 2;
    EXPECT_EQ(x, 2);
    EXPECT_EQ(y, 2);
}
