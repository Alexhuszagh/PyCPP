//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief Pimpl unittests.
 */

#include <pimpl.h>
#include <gtest/gtest.h>

// FORWARD
// -------

struct structure;
typedef pimpl<structure, 8> pstructure;

// OBJECTS
// -------

struct structure
{
    uint64_t x;
};

// TESTS
// -----


TEST(pimpl, integer)
{
    typedef pimpl<int> pint;

    pint x(2);
    pint y;
    EXPECT_EQ(x, 2);

    x = 5;
    EXPECT_EQ(x, 5);

    y = x;
    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 5);

    y = 2;
    EXPECT_EQ(x, 5);
    EXPECT_EQ(y, 2);

    std::swap(x, y);
    EXPECT_EQ(x, 2);
    EXPECT_EQ(y, 5);
}


TEST(pimpl, structure)
{
    pstructure a = structure {2};
    pstructure b;
    EXPECT_EQ(a->x, 2);

    a->x = 5;
    EXPECT_EQ(a->x, 5);

    b = a;
    EXPECT_EQ(a->x, 5);
    EXPECT_EQ(b->x, 5);

    b->x = 2;
    EXPECT_EQ(a->x, 5);
    EXPECT_EQ(b->x, 2);

    std::swap(a, b);
    EXPECT_EQ(a->x, 2);
    EXPECT_EQ(b->x, 5);
}
