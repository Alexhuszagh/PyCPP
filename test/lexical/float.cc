//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Lexical floating point number conversion unittests.
 */

#include <pycpp/lexical/float.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----
// Don't test extensively: the back-end is
// already extensively tested.

TEST(lexical_float_formatter, lexical_float_formatter)
{
    EXPECT_EQ(lexical_float_formatter(1.).string(), "1");
    EXPECT_EQ(lexical_float_formatter(-1.).string(), "-1");
}


TEST(lexical_float_extractor, lexical_float_extractor)
{
    EXPECT_FLOAT_EQ(float(lexical_float_extractor("-1")), -1);
    EXPECT_DOUBLE_EQ(double(lexical_float_extractor("-1")), -1);
}
