//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief CSV unittests.
 */

#include <pycpp/csv.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----

TEST(csvpunct, delimiter)
{
    csvpunct punct;
    EXPECT_EQ(punct.delimiter(), ',');
}


TEST(csvpunct, quote)
{
    csvpunct punct;
    EXPECT_EQ(punct.quote(), '"');
}


TEST(csvpunct, escape)
{
    csvpunct punct;
    EXPECT_EQ(punct.escape(), '\\');
}