//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/format.h>
#include <gtest/gtest.h>
#include <cmath>

PYCPP_USING_NAMESPACE

// HELPERS
// -------

// EXPECT_FLOAT_EQ and EXPECT_DOUBLE_EQ are a little over-precise
// for serializing/deserializing error, since the check with
// 4 deltas, where delta is `(d - next_float(d)) / 2`.
// We just want a relative ballpark, for floats, within 1e-6, for doubles,
// within 1e-12.
//
// For values close to 0, just check the are within reasonable range of
// FLT_MIN and DBL_MIN.

#define EXPECT_FLOAT_NEAR(f1, f2)                                       \
    {                                                                   \
        float f1_val = f1;                                              \
        float f2_val = f2;                                              \
        if (f2_val == 0) {                                              \
            EXPECT_TRUE(std::abs(f1_val) < 1.176e-38);                  \
        } else {                                                        \
            EXPECT_TRUE(std::abs((f1_val - f2_val) / f2_val) < 1e-6);   \
        }                                                               \
    }                                                                   \
    EXPECT_TRUE(true)

#define EXPECT_DOUBLE_NEAR(f1, f2)                                      \
    {                                                                   \
        double f1_val = f1;                                             \
        double f2_val = f2;                                             \
        if (f2_val == 0) {                                              \
            EXPECT_TRUE(std::abs(f1_val) < 2.226e-308);                 \
        } else {                                                        \
            EXPECT_TRUE(std::abs((f1_val - f2_val) / f2_val) < 1e-12);  \
        }                                                               \
    }                                                                   \
    EXPECT_TRUE(true)


// TESTS
// -----

TEST(atof32, base10)
{
    EXPECT_FLOAT_NEAR(atof32("0", 10), 0);
    EXPECT_FLOAT_NEAR(atof32("1", 10), 1);
    EXPECT_FLOAT_NEAR(atof32("12", 10), 12);
    EXPECT_FLOAT_NEAR(atof32("123", 10), 123);
    EXPECT_FLOAT_NEAR(atof32("1234", 10), 1234);
    EXPECT_FLOAT_NEAR(atof32("12345", 10), 12345);
    EXPECT_FLOAT_NEAR(atof32("123456", 10), 123456);
    EXPECT_FLOAT_NEAR(atof32("1234567", 10), 1234567);
    EXPECT_FLOAT_NEAR(atof32("12345678", 10), 12345678);
    EXPECT_FLOAT_NEAR(atof32("123456789", 10), 123456789);
    EXPECT_FLOAT_NEAR(atof32("123456789.1", 10), 123456789.1);
    EXPECT_FLOAT_NEAR(atof32("123456789.12", 10), 123456789.12);
    EXPECT_FLOAT_NEAR(atof32("123456789.123", 10), 123456789.123);
    EXPECT_FLOAT_NEAR(atof32("123456789.1234", 10), 123456789.1234);
    EXPECT_FLOAT_NEAR(atof32("123456789.12345", 10), 123456789.12345);
    EXPECT_FLOAT_NEAR(atof32("1.2345678912345e8", 10), 123456789.12345);
    EXPECT_FLOAT_NEAR(atof32("1.2345e+8", 10), 123450000);
    EXPECT_FLOAT_NEAR(atof32("1.2345e+11", 10), 1.2345e+11);
    EXPECT_FLOAT_NEAR(atof32("123450000000", 10), 1.2345e+11);
    EXPECT_FLOAT_NEAR(atof32("1.2345e+38", 10), 1.2345e+38);
    EXPECT_FLOAT_NEAR(atof32("123450000000000000000000000000000000000", 10), 1.2345e+38);
    EXPECT_FLOAT_NEAR(atof32("1.2345e-8", 10), 1.2345e-8);
    EXPECT_FLOAT_NEAR(atof32("0.000000012345", 10), 1.2345e-8);
    EXPECT_FLOAT_NEAR(atof32("1.2345e-38", 10), 1.2345e-38);
    EXPECT_FLOAT_NEAR(atof32("0.000000000000000000000000000000000000012345", 10), 1.2345e-38);
    EXPECT_TRUE(std::isnan(atof32(NAN_STRING, 10)));
    EXPECT_TRUE(std::isinf(atof32(INFINITY_STRING, 10)));
    EXPECT_TRUE(std::isinf(atof32("-" + INFINITY_STRING, 10)));
}


TEST(atof32, basen)
{
    EXPECT_FLOAT_NEAR(atof32("YA", 36), 1234.);
}


TEST(atof64, base10)
{
    EXPECT_DOUBLE_NEAR(atof64("0", 10), 0);
    EXPECT_DOUBLE_NEAR(atof64("1", 10), 1);
    EXPECT_DOUBLE_NEAR(atof64("12", 10), 12);
    EXPECT_DOUBLE_NEAR(atof64("123", 10), 123);
    EXPECT_DOUBLE_NEAR(atof64("1234", 10), 1234);
    EXPECT_DOUBLE_NEAR(atof64("12345", 10), 12345);
    EXPECT_DOUBLE_NEAR(atof64("123456", 10), 123456);
    EXPECT_DOUBLE_NEAR(atof64("1234567", 10), 1234567);
    EXPECT_DOUBLE_NEAR(atof64("12345678", 10), 12345678);
    EXPECT_DOUBLE_NEAR(atof64("123456789", 10), 123456789);
    EXPECT_DOUBLE_NEAR(atof64("123456789.1", 10), 123456789.1);
    EXPECT_DOUBLE_NEAR(atof64("123456789.12", 10), 123456789.12);
    EXPECT_DOUBLE_NEAR(atof64("123456789.123", 10), 123456789.123);
    EXPECT_DOUBLE_NEAR(atof64("123456789.1234", 10), 123456789.1234);
    EXPECT_DOUBLE_NEAR(atof64("123456789.12345", 10), 123456789.12345);
    EXPECT_DOUBLE_NEAR(atof64("1.2345678912345e8", 10), 123456789.12345);
    EXPECT_DOUBLE_NEAR(atof64("1.2345e+8", 10), 123450000);
    EXPECT_DOUBLE_NEAR(atof64("123450000000", 10), 1.2345e+11);
    EXPECT_DOUBLE_NEAR(atof64("1.2345e+11", 10), 1.2345e+11);
    EXPECT_DOUBLE_NEAR(atof64("1.2345e+38", 10), 1.2345e+38);
    EXPECT_DOUBLE_NEAR(atof64("123450000000000000000000000000000000000", 10), 1.2345e+38);
    EXPECT_DOUBLE_NEAR(atof64("1.2345e+308", 10), 1.2345e+308);
    EXPECT_DOUBLE_NEAR(atof64("123450000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 10), 1.2345e+308);
    EXPECT_DOUBLE_NEAR(atof64("1.2345e-8", 10), 0.000000012345);
    EXPECT_DOUBLE_NEAR(atof64("0.000000012345", 10), 1.2345e-8);
    EXPECT_DOUBLE_NEAR(atof64("1.2345e-38", 10), 1.2345e-38);
    EXPECT_DOUBLE_NEAR(atof64("0.000000000000000000000000000000000000012345", 10), 1.2345e-38);
    EXPECT_DOUBLE_NEAR(atof64("1.2345e-308", 10), 1.2345e-308);
    // due to issues in how the data is parsed, manually extracting
    // non-exponents of 1.<e-299 is prone to error
    // test the limit of our ability
    EXPECT_DOUBLE_NEAR(atof64("0.000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000012345", 10), 1.2345e-299);
    EXPECT_TRUE(std::isnan(atof64(NAN_STRING, 10)));
    EXPECT_TRUE(std::isinf(atof64(INFINITY_STRING, 10)));
    EXPECT_TRUE(std::isinf(atof64("-" + INFINITY_STRING, 10)));
}


TEST(atof64, basen)
{
    EXPECT_FLOAT_NEAR(atof64("YA", 36), 1234.);
}
