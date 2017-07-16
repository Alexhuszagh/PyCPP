//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Regular expression object unittests.
 */

#include <pycpp/re/regex.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(re, regex)
{
    regexp_t regex("\\w+");
    std::string data = "These are a bunch of words";

    // first example
    auto m = regex.search(data);
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 0);
    EXPECT_EQ(m.end(), 5);
    EXPECT_EQ(m.group(), "These");
    EXPECT_EQ(m.lastindex(), 0);

    m = regex.search(data, m.end());
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 6);
    EXPECT_EQ(m.end(), 9);
    EXPECT_EQ(m.group(), "are");
    EXPECT_EQ(m.lastindex(), 0);
}
