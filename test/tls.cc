//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Thread-local storage unittests.
 */

#include "tls.h"
#include <gtest/gtest.h>

#include <thread>
#include <vector>

// STORAGE
// -------

thread_local_storage int tls = 0;

// FUNCTIONS
// ---------


void test_value()
{
    EXPECT_EQ(tls, 0);
    tls = 5;
}

// TESTS
// -----


TEST(thread_local_storage, thread_local_storage)
{
    std::vector<std::thread*> threads;
    threads.reserve(30);

    for (size_t i = 0; i < 30; ++i) {
        threads.emplace_back(new std::thread(test_value));
    }
    for (auto *thread: threads) {
        thread->join();
        delete thread;
    }
}
