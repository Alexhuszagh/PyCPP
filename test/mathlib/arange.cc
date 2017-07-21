//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief NumPy-like arange unittests.
 */

// TODO: restore
//#include <pycpp/mathlib/arange.h>
#include <gtest/gtest.h>

//PYCPP_USING_NAMESPACE

// TESTS
// -----


// TODO: restore
#if 0
TEST(mathlib, arange)
{
    auto r1 = arange<int>(0, 5);
    EXPECT_EQ(std::vector<int>(r1.begin(), r1.end()), std::vector<int>({0, 1, 2, 3, 4}));

    r1 = arange<int>(0, 5, 2);
    EXPECT_EQ(std::vector<int>(r1.begin(), r1.end()), std::vector<int>({0, 2, 4}));

    r1 = arange<int>(5, 0, -2);
    EXPECT_EQ(std::vector<int>(r1.begin(), r1.end()), std::vector<int>({5, 3, 1}));

    auto r2 = arange<size_t>(0, 5, 2);
    EXPECT_EQ(std::vector<size_t>(r2.begin(), r2.end()), std::vector<size_t>({0, 2, 4}));
}
#endif
