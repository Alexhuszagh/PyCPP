//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/cookie.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(cookies_t, encode)
{
    cookies_t cookies = {
        {"name", "value"},
    };
    EXPECT_EQ(cookies.encode(), "name=value; ");
}
