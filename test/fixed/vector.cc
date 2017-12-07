//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief `fixed_vector` unittests.
 */

#include <pycpp/fixed/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(fixed_vector, ctor)
{
    using vector = fixed_vector<int>;
    vector v1, v2;
    EXPECT_EQ(v1, v2);

    v1.emplace_back(1);
    EXPECT_NE(v1, v2);

    v2 = v1;
    EXPECT_EQ(v1, v2);
}
