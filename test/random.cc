//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Random unittests.
 */

#include "random.h"
#include <gtest/gtest.h>


// TESTS
// -----


TEST(random, sysrandom)
{
    // cryptographic random, should be completely unique
    auto reference = sysrandom(8);
    for (int i = 0; i < 200; ++i) {
        EXPECT_NE(sysrandom(8), reference);
    }
}


TEST(random, pseudorandom)
{
    // all are seeded the same, should produce same output
    auto reference = pseudorandom(8, true);
    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(pseudorandom(8, true), reference);
    }
}
