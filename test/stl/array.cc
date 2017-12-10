//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief STL array alias unittests.
 */

#include <pycpp/stl/array.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(array, array)
{
    using array_type = array<int, 4>;
    array_type arr;
}
