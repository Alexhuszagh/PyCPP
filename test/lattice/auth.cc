//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/auth.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(authentication_t, encode)
{
    authentication_t auth = {"name", "password"};
    EXPECT_EQ(auth.string(), "name:password");
}
