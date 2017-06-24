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
    struct timeval t1, t2;
    gettimeofday(&t1, NULL);
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    gettimeofday(&t2, NULL);

    EXPECT_GE(t2.tv_usec - t1.tv_usec, 500);
}
