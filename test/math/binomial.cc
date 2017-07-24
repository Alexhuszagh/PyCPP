//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Binomial coefficient unittests.
 */

#include <pycpp/math/binomial.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(math, binomial)
{
    EXPECT_EQ(binomial(5, -1), 0);
    EXPECT_EQ(binomial(5, 0), 1);
    EXPECT_EQ(binomial(5, 3), 10);
    EXPECT_EQ(binomial(10, 3), 120);
    EXPECT_EQ(binomial(40, 6), 3838380);

    try {
        binomial(-1, 3);
    } catch (std::exception) {
        return;
    }

    EXPECT_TRUE(false);
}
