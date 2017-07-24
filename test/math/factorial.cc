//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Factorial unittests.
 */

#include <pycpp/math/factorial.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(math, factorial)
{
    EXPECT_EQ(factorial(3), 6);
    EXPECT_EQ(factorial(4), 24);
    EXPECT_EQ(factorial(0), 1);
    EXPECT_EQ(factorial(-30), 0);
}
