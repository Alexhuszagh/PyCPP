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
    using pint = unique_heap_pimpl<int>;
    static_assert(sizeof(pint) > sizeof(int*), "std::allocator shouldn have overhead.");

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

    swap(x, y);
    EXPECT_EQ(x, 2);
    EXPECT_EQ(y, 5);
}


TEST(unique_heap_pimpl, std_allocator)
{
    using pint = unique_heap_pimpl<int, std::allocator<int>>;
    static_assert(sizeof(pint) == sizeof(int*), "std::allocator shouldn't have overhead.");
}


TEST(shared_heap_pimpl, integer)
{
    using pint = shared_heap_pimpl<int>;

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
