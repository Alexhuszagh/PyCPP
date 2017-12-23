//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Type-safe enumeration unittests.
 */

#include <pycpp/misc/enum.h>
#include <gtest/gtest.h>

// ENUM
// ----


enum class enum1_t
{
    A = 1,
    B,
    C,
    D,
    E,
};


enum class enum2_t
{
    F = 1,
    G,
    H,
    I,
    J,
};

PYCPP_ENUM_FLAG(enum1_t)
PYCPP_ENUM_FLAG(enum1_t, enum2_t)

// TESTS
// -----


TEST(enum_flag, enum_flag1)
{
    enum1_t a = enum1_t::A;
    enum1_t b = enum1_t::B;

    EXPECT_EQ(a | b, enum1_t::C);
    EXPECT_EQ(a & b, enum1_t(0));
    EXPECT_EQ(a ^ b, enum1_t::C);

    a |= b;
    EXPECT_EQ(a, enum1_t::C);
    a &= b;
    EXPECT_EQ(a, enum1_t::B);
    a ^= b;
    EXPECT_EQ(a, enum1_t(0));
    EXPECT_EQ(~enum1_t::A, enum1_t(-2));
    EXPECT_EQ(!enum1_t::A, false);
    EXPECT_EQ(!a, true);
}


TEST(enum_flag, enum_flag2)
{
    enum1_t a = enum1_t::A;
    enum2_t g = enum2_t::G;

    EXPECT_EQ(a | g, enum1_t::C);
    EXPECT_EQ(a & g, enum1_t(0));
    EXPECT_EQ(a ^ g, enum1_t::C);

    a |= g;
    EXPECT_EQ(a, enum1_t::C);
    a &= g;
    EXPECT_EQ(a, enum1_t::B);
    a ^= g;
    EXPECT_EQ(a, enum1_t(0));
}
