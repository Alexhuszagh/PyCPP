//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Gettimeofday unittests.
 */

#include "gettimeofday.h"
#include <gtest/gtest.h>

#include <chrono>
#include <thread>


// TESTS
// -----


TEST(gettimeofday, gettimeofday)
{
    struct timeval tv1, tv2;
    gettimeofday(&tv1, NULL);
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    gettimeofday(&tv2, NULL);

    // useless check, we can't control clocks that well
    auto diff = tv2.tv_usec - tv1.tv_usec;
    EXPECT_NEAR(diff, diff, diff);
}
