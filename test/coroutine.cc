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

COROUTINE_DECL(int, myroutine)
{
    for (int i = 0; i < 500; ++i) {
        COROUTINE_YIELD(i);
    }
}

// TESTS
// -----


TEST(coroutine, coroutine)
{
    // TODO: implement..
}
