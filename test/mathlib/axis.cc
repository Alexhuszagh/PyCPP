//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Numerical array axis unittests.
 */

#include <pycpp/mathlib/arange.h>
#include <pycpp/mathlib/axis.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(ndarray_axis_iterator, constructors)
{
    // TODO: implement
}


TEST(ndarray_axis_iterator, relational)
{
    // TODO: implement
}


TEST(ndarray_axis_iterator, incrementors)
{
    // TODO: implement
}


TEST(ndarray_axis_iterator, dereference)
{
    // TODO: implement
}


TEST(ndarray_axis_iterator, other)
{
    // TODO: implement
}


TEST(ndarray_axis, constructors)
{
    // TODO: implement
}


TEST(ndarray_axis, iterators)
{
    // TODO: implement
}


TEST(ndarray_axis, operations)
{
    // TODO: implement
}


TEST(ndarray_axis, element)
{
    auto array = arange<size_t>(0, 10, 1);
    auto view = array.view();
    EXPECT_EQ(view[0], 0);
    EXPECT_EQ(view[5], 5);
}


TEST(ndarray_axis, other)
{
    // TODO: implement
}
