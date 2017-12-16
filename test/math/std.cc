//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Numerical variance and standard deviations.
 */

#include <pycpp/math/std.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(math, variance)
{
    vector<double> x = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    vector<double> y = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6};
    vector<double> y0 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_NEAR(variance(x.begin(), x.end()), 8.25, 0.001);
    EXPECT_NEAR(variance(x.begin(), x.end(), y0.begin(), y0.end()), 8.25, 0.001);

    // custom predicates
    double result = variance(x.begin(), x.end(), [](const double x) {
        return x + 2;
    });
    EXPECT_NEAR(result, 8.25, 0.001);

    result = variance(x.begin(), x.end(), y.begin(), y.end(), [](const double x) {
        return x + 2;
    }, [](const double y) {
        return y + 1;
    });
    EXPECT_NEAR(result, 7.73535, 0.001);
}


TEST(math, std)
{
    vector<double> x = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    vector<double> y = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6};
    vector<double> y0 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    EXPECT_NEAR(stdev(x.begin(), x.end()), 2.87228, 0.001);
    EXPECT_NEAR(stdev(x.begin(), x.end(), y0.begin(), y0.end()), 2.87228, 0.001);

    // custom predicates
    double result = stdev(x.begin(), x.end(), [](const double x) {
        return x + 2;
    });
    EXPECT_NEAR(result, 2.87228, 0.001);

    result = stdev(x.begin(), x.end(), y.begin(), y.end(), [](const double x) {
        return x + 2;
    }, [](const double y) {
        return y + 1;
    });
    EXPECT_NEAR(result, 2.781249, 0.001);
}
