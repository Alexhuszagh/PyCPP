//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Range coroutine unittests.
 */

#include <pycpp/xrange.h>
#include <gtest/gtest.h>
#include <numeric>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(xrange, xrange)
{
    auto r = xrange(10);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 45);

    r = xrange(0, 10, 1);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 45);
}


TEST(xrange, xrange_mismatch)
{
    // Here we're trying to break xrange.
    auto r = xrange(10, 5, -1);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 40);

    r = xrange(10, 5, 1);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 0);

    r = xrange(5, 10, 2);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 21);

    try {
        r = xrange(5, 10, 0);
    } catch(...) {
        return;
    }
    EXPECT_TRUE(false);
}
