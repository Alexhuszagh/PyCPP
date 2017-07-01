//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief any unittests.
 */

#include <any.h>
#include <gtest/gtest.h>
#include <tuple>

// TESTS
// -----


TEST(any, small)
{
    any v1(1);
    any v2(2.5);

    EXPECT_FALSE(v1.compatible(v2));
    EXPECT_TRUE(v1.compatible(v1));
}


TEST(any, large)
{
    auto x = std::make_tuple(1, 5);
    auto y = std::make_tuple(1.2, 5.3);

    any v1(x);
    any v2(y);

    EXPECT_FALSE(v1.compatible(v2));
    EXPECT_TRUE(v1.compatible(v1));
}


TEST(any, cast)
{
    any value(1);
    EXPECT_EQ(value.cast<int>(), 1);

    try {
        value.cast<double>();
    } catch (bad_any_cast&) {
        return;
    }
    EXPECT_TRUE(false);
}


TEST(any, equality)
{
    any v1;
    any v2;
    any v3(1);
    any v4(2.5);
    any v5(1);

    EXPECT_EQ(v1, v2);
    EXPECT_NE(v1, v3);
    EXPECT_EQ(v3, v3);
    EXPECT_NE(v3, v4);
    EXPECT_EQ(v3, v5);
}
