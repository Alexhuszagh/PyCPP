//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief High-level regular-expression interface unittests.
 */

#include <pycpp/re/re.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(re, search)
{
    std::string data = "These are a bunch of words";
    auto m = search("\\w+", data);
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 0);
    EXPECT_EQ(m.end(), 5);
    EXPECT_EQ(m.group(), "These");
    EXPECT_EQ(m.lastindex(), 0);

    data = "...~/.'' Words";
    m = search("\\w+", data);
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 9);
    EXPECT_EQ(m.end(), 14);
    EXPECT_EQ(m.group(), "Words");
    EXPECT_EQ(m.lastindex(), 0);
}


TEST(re, match)
{
    std::string data = "These are a bunch of words";
    auto m = match("\\w+", data);
    EXPECT_EQ(m.start(), 0);
    EXPECT_EQ(m.end(), 5);
    EXPECT_EQ(m.group(), "These");
    EXPECT_EQ(m.lastindex(), 0);

    data = "...~/.'' Words";
    m = match("\\w+", data);
    ASSERT_FALSE(bool(m));
}


TEST(re, syntax)
{
    // Need to check various assumptions made about the regular
    // expression are accurate.

    std::string data = "These are a bunch of words";
    auto m = match("\\w+$", data);
    ASSERT_FALSE(bool(m));

    m = match("^\\w+", data);
    ASSERT_TRUE(bool(m));
}


TEST(re, escape)
{
    EXPECT_EQ(escape(string_view("\0", 1)), std::string("\\\0", 2));
}


TEST(re, purge)
{
    purge();
}
