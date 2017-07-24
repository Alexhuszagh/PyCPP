//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Numerical average unittests.
 */

#include <pycpp/math/average.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(math, average)
{
    std::vector<double> x = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    std::vector<double> y = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6};
    EXPECT_NEAR(average(x.begin(), x.end()), 9.5, 0.001);
    EXPECT_NEAR(average(x.begin(), x.end(), y.begin(), y.end()), 8.7143, 0.001);

    // custom predicates
    double result = average(x.begin(), x.end(), [](const double x) {
        return x + 2;
    });
    EXPECT_NEAR(result, 11.5, 0.001);

    result = average(x.begin(), x.end(), y.begin(), y.end(), [](const double x) {
        return x + 2;
    }, [](const double y) {
        return y + 1;
    });
    EXPECT_NEAR(result, 10.782609, 0.001);
}
