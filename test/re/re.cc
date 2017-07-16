//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief High-level regular-expression interface unittests.
 */

#include <pycpp/re/re.h>
#include <gtest/gtest.h>
#include <deque>

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


TEST(re, findall)
{
    std::string data = "These are a bunch of words";
    auto words = findall("\\w+", data);
    EXPECT_EQ(words.size(), 6);
    EXPECT_EQ(words.front(), "These");
}


TEST(re, finditer)
{
    std::string data = "These are a bunch of words";
    std::deque<std::string> actual;
    std::deque<std::string> expected = {
        "These",
        "are",
        "a",
        "bunch",
        "of",
        "words",
    };
    for (auto &match: finditer("\\w+", data)) {
        actual.emplace_back(std::string(match.group(0)));
    }
    EXPECT_EQ(actual, expected);
}


TEST(re, split)
{
    std::string data = "These are a bunch of words";

    // first example
    auto whitespace = split("\\w+", data);
    ASSERT_EQ(whitespace.size(), 7);
    EXPECT_EQ(whitespace[0], "");
    EXPECT_EQ(whitespace[1], " ");
    EXPECT_EQ(whitespace[2], " ");
    EXPECT_EQ(whitespace[3], " ");
    EXPECT_EQ(whitespace[4], " ");
    EXPECT_EQ(whitespace[5], " ");
    EXPECT_EQ(whitespace[6], "");
}


TEST(re, sub)
{
    EXPECT_EQ(sub("(\\w+)", "+\\1", "These are a bunch of words"), "+These +are +a +bunch +of +words");
}


TEST(re, escape)
{
    EXPECT_EQ(escape(string_view("\0", 1)), std::string("\\\0", 2));
}


TEST(re, purge)
{
    purge();
}
