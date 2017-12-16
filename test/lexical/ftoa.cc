//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lexical/atof.h>
#include <pycpp/lexical/format.h>
#include <pycpp/lexical/ftoa.h>
#include <pycpp/stl/limits.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static std::vector<float> FLOATS = {
    0.,
    0.1,
    1.,
    1.1,
    12.,
    12.1,
    123.,
    123.1,
    1234.,
    1234.1,
    12345.,
    12345.1,
    123456.,
    123456.1,
    1234567.,
    1234567.1,
    12345678.,
    12345678.1,
    123456789.,
    123456789.1,
    123456789.12,
    123456789.123,
    123456789.1234,
    123456789.12345,
    1.2345678912345e8,
    1.2345e+8,
    1.2345e+11,
    1.2345e+38,
    1.2345e-8,
    1.2345e-11,
    1.2345e-38,
};

static std::vector<double> DOUBLES = {
    0.,
    0.1,
    1.,
    1.1,
    12.,
    12.1,
    123.,
    123.1,
    1234.,
    1234.1,
    12345.,
    12345.1,
    123456.,
    123456.1,
    1234567.,
    1234567.1,
    12345678.,
    12345678.1,
    123456789.,
    123456789.1,
    123456789.12,
    123456789.123,
    123456789.1234,
    123456789.12345,
    1.2345678912345e8,
    1.2345e+8,
    1.2345e+11,
    1.2345e+38,
    1.2345e+308,
    1.2345e-8,
    1.2345e-11,
    1.2345e-38,
    1.2345e-308,
};

// TESTS
// -----


TEST(f32toa, base2)
{
    // positive
    EXPECT_EQ(f32toa(1.2345678901234567890e0, 2).substr(0, 20), "1.001111000000110010");
    EXPECT_EQ(f32toa(1.2345678901234567890e1, 2).substr(0, 20), "1100.010110000111111");
    EXPECT_EQ(f32toa(1.2345678901234567890e2, 2).substr(0, 20), "1111011.011101001111");
    EXPECT_EQ(f32toa(1.2345678901234567890e3, 2).substr(0, 20), "10011010010.10010001");

    // negative
    EXPECT_EQ(f32toa(-1.2345678901234567890e0, 2).substr(0, 21), "-1.001111000000110010");
    EXPECT_EQ(f32toa(-1.2345678901234567890e1, 2).substr(0, 21), "-1100.010110000111111");
    EXPECT_EQ(f32toa(-1.2345678901234567890e2, 2).substr(0, 21), "-1111011.011101001111");
    EXPECT_EQ(f32toa(-1.2345678901234567890e3, 2).substr(0, 21), "-10011010010.10010001");

    // special
    EXPECT_EQ(f32toa(std::numeric_limits<float>::quiet_NaN(), 2), NAN_STRING);
    EXPECT_EQ(f32toa(std::numeric_limits<float>::infinity(), 2), INFINITY_STRING);
}


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
    EXPECT_EQ(f32toa(std::numeric_limits<float>::quiet_NaN(), 10), NAN_STRING);
    EXPECT_EQ(f32toa(std::numeric_limits<float>::infinity(), 10), INFINITY_STRING);

    // check parsed value is within 32-bit float error
    for (float f: FLOATS) {
        EXPECT_NEAR(atof32(f32toa(f, 10), 10), f, f*1e-6);
    }
}


TEST(f32toa, basen)
{
    for (float f: FLOATS) {
        for (uint8_t radix = 2; radix <= 36; radix++)  {
            EXPECT_NEAR(atof32(f32toa(f, radix), radix), f, f*1e-6);
        }
    }
}


TEST(f64toa, base2)
{
    // positive
    EXPECT_EQ(f64toa(1.2345678901234567890e0, 2).substr(0, 40), "1.00111100000011001010010000101000110001");
    EXPECT_EQ(f64toa(1.2345678901234567890e1, 2).substr(0, 40), "1100.01011000011111100110100110010111101");
    EXPECT_EQ(f64toa(1.2345678901234567890e2, 2).substr(0, 40), "1111011.01110100111100000001111111101101");
    EXPECT_EQ(f64toa(1.2345678901234567890e3, 2).substr(0, 40), "10011010010.1001000101100001001111110100");

    // negative
    EXPECT_EQ(f64toa(-1.2345678901234567890e0, 2).substr(0, 41), "-1.00111100000011001010010000101000110001");
    EXPECT_EQ(f64toa(-1.2345678901234567890e1, 2).substr(0, 41), "-1100.01011000011111100110100110010111101");
    EXPECT_EQ(f64toa(-1.2345678901234567890e2, 2).substr(0, 41), "-1111011.01110100111100000001111111101101");
    EXPECT_EQ(f64toa(-1.2345678901234567890e3, 2).substr(0, 41), "-10011010010.1001000101100001001111110100");

    // special
    EXPECT_EQ(f64toa(std::numeric_limits<float>::quiet_NaN(), 2), NAN_STRING);
    EXPECT_EQ(f64toa(std::numeric_limits<float>::infinity(), 2), INFINITY_STRING);
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
        EXPECT_NEAR(atof64(f64toa(d, 10), 10), d, d*1e-12);
    }
}


TEST(f64toa, basen)
{
    // known issues
    EXPECT_EQ(f64toa(12.1, 20), "C.2");

    for (double d: DOUBLES) {
        for (uint8_t radix = 2; radix <= 36; radix++)  {
            double dd = atof64(f64toa(d, radix), radix);
            EXPECT_NEAR(atof64(f64toa(d, radix), radix), d, d*1e-12);
        }
    }
}
