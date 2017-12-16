//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Trapezoidal integration unittests.
 */

#include <pycpp/math/trapz.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(math, trapz)
{
    vector<double> x = {4.5, 6.5, 8.5, 9.3};
    vector<double> y = {0.1, 200.45, 175.6, 12.3};
    EXPECT_NEAR(trapz(y.begin(), y.end(), 1.0), 382.25, 0.001);
    EXPECT_NEAR(trapz(y.begin(), y.end(), x.begin(), x.end()), 651.76, 0.001);

    // custom predicates
    double result = trapz(y.begin(), y.end(), 1.0, [](const double x) {
        return x;
    });
    EXPECT_NEAR(result, 382.25, 0.001);

    result = trapz(y.begin(), y.end(), x.begin(), x.end(), [](const double x) {
        return x;
    }, [](const double y) {
        return y;
    });
    EXPECT_NEAR(result, 651.76, 0.001);
}
