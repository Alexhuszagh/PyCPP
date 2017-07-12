//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Range coroutine unittests.
 */

#include <pycpp/range.h>
#include <gtest/gtest.h>
#include <numeric>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(range, range)
{
    auto r = range(10);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 45);

    r = range(0, 10, 1);
    EXPECT_EQ(std::accumulate(r.begin(), r.end(), 0), 45);
}
