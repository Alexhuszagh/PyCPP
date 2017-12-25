//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/digest.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(digest_t, encode)
{
    digest_t digest = {"name", "password"};
    EXPECT_EQ(digest.str(), "name:password");
}
