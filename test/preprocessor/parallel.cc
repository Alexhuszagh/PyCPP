//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Parallel execution policy unittests.
 */

#include <pycpp/preprocessor/parallel.h>
#include <pycpp/stl/numeric.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE;

// TESTS
// -----


TEST(parallel, accumulate)
{
    vector<int> v = {1, 2, 3, 4, 5};
    auto sum = accumulate(PARALLEL_EXECUTION v.begin(), v.end(), 0);
    EXPECT_EQ(sum, 15);
}
