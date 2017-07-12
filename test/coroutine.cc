//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Coroutine unittests.
 */

#include <pycpp/coroutine.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// HELPERS
// -------
//
//COROUTINE_DECL(int, range, int start, int stop, int step)
//{
//    for (int i = start; i < stop; i += step) {
// //       COROUTINE_YIELD(i);
//    }
//}


struct x {};


struct range: generator<int>
{
    int start = 0;
    int stop = 50;
    int step = 1;
    int i = start;

    virtual void operator()() override
    {
        CORO_REENTER(coro_) {
            for (; i < stop; i += step) {
                CORO_YIELD store(i);
            }
        }
    }
};


// TESTS
// -----


TEST(coroutine, coroutine)
{
    int sum = 0;
    range r;
    for (int i: r) {
        std::cout << i << std::endl;
    }

    // TODO: need to implement...
    EXPECT_EQ(sum, 0);
    exit(0);
}
