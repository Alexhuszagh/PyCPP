//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/format.h>
#include <pycpp/lexical/ftoa.h>
#include <gtest/gtest.h>
#include <limits>
#include <vector>

PYCPP_USING_NAMESPACE

static std::vector<float> FLOATS = {
    0.,
    1.,
    12.,
    123.,
    1234.,
    12345.,
    123456.,
    1234567.,
    12345678.,
    123456789.,
//    123456789.1,
//    123456789.12,
//    123456789.123,
//    123456789.1234,
//    123456789.12345,
//    1.2345678912345e8,
//    1.2345e+8,
//    1.2345e-8,
};

static std::vector<double> DOUBLES = {
    0.,
    1.,
    12.,
    123.,
    1234.,
    12345.,
    123456.,
    1234567.,
    12345678.,
    123456789.,
//    123456789.1,
//    123456789.12,
//    123456789.123,
//    123456789.1234,
//    123456789.12345,
//    1.2345678912345e8,
//    1.2345e+8,
//    1.2345e-8,
};

// TESTS
// -----

TEST(f32toa, base10)
{
    // positive
    EXPECT_EQ(f32toa(1.2345678901234567890e0, 10).substr(0, 8), "1.234567");
    EXPECT_EQ(f32toa(1.2345678901234567890e1, 10).substr(0, 8), "12.34567");
    EXPECT_EQ(f32toa(1.2345678901234567890e2, 10).substr(0, 8), "123.4567");
    EXPECT_EQ(f32toa(1.2345678901234567890e3, 10).substr(0, 8), "1234.567");

    // negative
    EXPECT_EQ(f32toa(-1.2345678901234567890e0, 10).substr(0, 9), "-1.234567");
    EXPECT_EQ(f32toa(-1.2345678901234567890e1, 10).substr(0, 9), "-12.34567");
    EXPECT_EQ(f32toa(-1.2345678901234567890e2, 10).substr(0, 9), "-123.4567");
    EXPECT_EQ(f32toa(-1.2345678901234567890e3, 10).substr(0, 9), "-1234.567");

    // special
    EXPECT_EQ(f32toa(std::numeric_limits<float>::quiet_NaN()), NAN_STRING);
    EXPECT_EQ(f32toa(std::numeric_limits<float>::infinity()), INFINITY_STRING);

    // check parsed value is within 32-bit float error
    for (float f: FLOATS) {
        EXPECT_FLOAT_EQ(atof32(f32toa(f, 10), 10), f);
    }
}


TEST(f32toa, basen)
{
//    for (float f: FLOATS) {
//        for (uint8_t radix = 2; radix <= 36; radix++)  {
//            EXPECT_FLOAT_EQ(atof32(f32toa(f, radix), radix), f);
//        }
//    }
}


TEST(f64toa, base10)
{
    // positive
    EXPECT_EQ(f64toa(1.2345678901234567890e0, 10).substr(0, 16), "1.23456789012345");
    EXPECT_EQ(f64toa(1.2345678901234567890e1, 10).substr(0, 16), "12.3456789012345");
    EXPECT_EQ(f64toa(1.2345678901234567890e2, 10).substr(0, 16), "123.456789012345");
    EXPECT_EQ(f64toa(1.2345678901234567890e3, 10).substr(0, 16), "1234.56789012345");

    // negative
    EXPECT_EQ(f64toa(-1.2345678901234567890e0, 10).substr(0, 17), "-1.23456789012345");
    EXPECT_EQ(f64toa(-1.2345678901234567890e1, 10).substr(0, 17), "-12.3456789012345");
    EXPECT_EQ(f64toa(-1.2345678901234567890e2, 10).substr(0, 17), "-123.456789012345");
    EXPECT_EQ(f64toa(-1.2345678901234567890e3, 10).substr(0, 17), "-1234.56789012345");

    // special
    EXPECT_EQ(f32toa(std::numeric_limits<double>::quiet_NaN()), NAN_STRING);
    EXPECT_EQ(f32toa(std::numeric_limits<double>::infinity()), INFINITY_STRING);


    // check parsed value is within 64-bit float error
    for (double d: DOUBLES) {
        EXPECT_DOUBLE_EQ(atof64(f64toa(d, 10), 10), d);
    }
}

TEST(f64toa, basen)
{
//    for (double d: DOUBLES) {
//        for (uint8_t radix = 2; radix <= 36; radix++)  {
//            EXPECT_DOUBLE_EQ(atof64(f64toa(d, radix), radix), d);
//        }
//    }
}