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
    typedef std::chrono::high_resolution_clock Clock;

    struct timeval tv1, tv2;
    auto tc1 = Clock::now();
    gettimeofday(&tv1, NULL);
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    auto tc2 = Clock::now();
    gettimeofday(&tv2, NULL);

    // just want them in the same ballpark...
    auto highres = std::chrono::duration_cast<std::chrono::microseconds>(tc2 - tc1).count();
    EXPECT_NEAR(tv2.tv_usec - tv1.tv_usec, highres, 0.5*highres);
}
