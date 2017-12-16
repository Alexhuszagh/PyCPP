//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Numerical average unittests.
 */

#include <pycpp/math/distribution.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(math, norm_pdf)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    EXPECT_NEAR(norm_pdf(1.0), 0.24197, 0.001);
    EXPECT_EQ(norm_pdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(norm_pdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, gaussian_pdf)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    EXPECT_NEAR(gaussian_pdf(0, 1.0, 1.0), 0.24197, 0.001);
    EXPECT_EQ(gaussian_pdf(0, 1.0, input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(gaussian_pdf(0, 1.0, input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, cauchy_pdf)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    EXPECT_NEAR(cauchy_pdf(1.0), 0.159155, 0.001);
    EXPECT_EQ(cauchy_pdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(cauchy_pdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, lorentzian_pdf)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    EXPECT_NEAR(lorentzian_pdf(1.0, 0.0, 2.0), 0.159155, 0.001);
    EXPECT_EQ(lorentzian_pdf(0.0, 2.0, input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(lorentzian_pdf(0.0, 2.0, input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, norm_cdf)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    EXPECT_NEAR(norm_cdf(1.0), 0.84134, 0.001);
    EXPECT_EQ(norm_cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(norm_cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, gaussian_cdf)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    EXPECT_NEAR(gaussian_cdf(1.0, 0.0, 1.0), 0.84134, 0.001);
    EXPECT_EQ(gaussian_cdf(0.0, 1.0, input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(gaussian_cdf(0.0, 1.0, input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, cauchy_cdf)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    EXPECT_NEAR(cauchy_cdf(1.0), 0.75, 0.001);
    EXPECT_EQ(cauchy_cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(cauchy_cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, lorentzian_cdf)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    EXPECT_NEAR(lorentzian_cdf(1.0, 0.0, 2.0), 0.75, 0.001);
    EXPECT_EQ(lorentzian_cdf(0.0, 2.0, input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(lorentzian_cdf(0.0, 2.0, input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, norm)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    norm inst;
    EXPECT_NEAR(inst.pdf(1.0), 0.24197, 0.001);
    EXPECT_NEAR(inst.cdf(1.0), 0.84134, 0.001);

    // PDF iterator
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
    back.clear();

    // CDF iterator
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, gaussian)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    gaussian inst(0.0, 1.0);
    EXPECT_NEAR(inst.pdf(1.0), 0.24197, 0.001);
    EXPECT_NEAR(inst.cdf(1.0), 0.84134, 0.001);

    // PDF iterator
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
    back.clear();

    // CDF iterator
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, cauchy)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    cauchy inst;
    EXPECT_NEAR(inst.pdf(1.0), 0.159155, 0.001);
    EXPECT_NEAR(inst.cdf(1.0), 0.75, 0.001);

    // PDF iterator
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
    back.clear();

    // CDF iterator
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}


TEST(math, lorentzian)
{
    std::vector<double> input = {-1.0, 0.0, 1.0};
    std::vector<double> back;
    std::vector<double> random(3, 0);

    lorentzian inst(0.0, 2.0);
    EXPECT_NEAR(inst.pdf(1.0), 0.159155, 0.001);
    EXPECT_NEAR(inst.cdf(1.0), 0.75, 0.001);

    // PDF iterator
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
    back.clear();

    // CDF iterator
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), std::back_inserter(back)), 3);
    EXPECT_EQ(inst.cdf(input.begin(), input.end(), random.begin()), 3);
    EXPECT_EQ(back, random);
}
