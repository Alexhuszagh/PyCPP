//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief URL encoding and decoding unittests.
 */

#include <pycpp/url.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(url, url_encode)
{
    EXPECT_EQ("%3C%5ESpecial%3E", url_encode("<^Special>"));
}


TEST(url, url_decode)
{
    EXPECT_EQ("<^Special>", url_decode("%3C%5ESpecial%3E"));
}
