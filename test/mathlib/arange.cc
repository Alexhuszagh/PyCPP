//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief NumPy-like arange unittests.
 */

#include <pycpp/mathlib/arange.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(mathlib, arange)
{
    auto range = arange<int>(0, 5);
    EXPECT_EQ(std::vector<int>(range.begin(), range.end()), std::vector<int>({0, 1, 2, 3, 4}));

    range = arange<int>(0, 5, 2);
    EXPECT_EQ(std::vector<int>(range.begin(), range.end()), std::vector<int>({0, 2, 4}));

    range = arange<int>(5, 0, -2);
    EXPECT_EQ(std::vector<int>(range.begin(), range.end()), std::vector<int>({5, 3, 1}));
}
