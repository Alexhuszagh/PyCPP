//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/timeout.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(timeout_t, timeout_t)
{
    timeout_t timeout(1000);
    EXPECT_EQ(timeout.seconds(), 1);
    EXPECT_EQ(timeout.milliseconds(), 1000);
    EXPECT_EQ(timeout.microseconds(), 1000000);
    EXPECT_TRUE(bool(timeout));

    timeout_t empty;
    EXPECT_FALSE(bool(empty));
}
