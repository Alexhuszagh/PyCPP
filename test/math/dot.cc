//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Dot product unittests.
 */

#include <pycpp/math/dot.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(math, dot)
{
    std::vector<float> x = {0., 1., 2., 3., 4.};
    std::vector<float> y = {-2., -1., 0., 1., 2.};

    EXPECT_NEAR(dot(x.begin(), x.end(), y.begin(), y.end()), 10., 0.001);
}
