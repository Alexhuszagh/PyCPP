//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Coroutine unittests.
 */

#include <pycpp/coroutine.h>
#include <gtest/gtest.h>
#include <numeric>

PYCPP_USING_NAMESPACE

// HELPERS
// -------


generator<int> custom_range(int start, int stop, int step = 1)
{
    return generator<int>([=](generator<int>& gen) mutable {
        COROUTINE_REENTER(gen.coroutine()) {
            for (; start < stop; start += step) {
                COROUTINE_YIELD gen.store(start);
            }
        }
    });
}

// TESTS
// -----


TEST(coroutine, coroutine)
{
    auto range = custom_range(1, 10, 1);
    EXPECT_EQ(std::accumulate(range.begin(), range.end(), 0), 45);
}
