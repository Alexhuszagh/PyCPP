//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief High-level regular-expression interface unittests.
 */

#include <pycpp/re/re.h>
#include <pycpp/stl/deque.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(re, re_search)
{
    string data = "These are a bunch of words";
    auto m = re_search("\\w+", data);
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 0);
    EXPECT_EQ(m.end(), 5);
    EXPECT_EQ(m.group(), string_view("These"));
    EXPECT_EQ(m.lastindex(), 0);

    data = "...~/.'' Words";
    m = re_search("\\w+", data);
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 9);
    EXPECT_EQ(m.end(), 14);
    EXPECT_EQ(m.group(), string_view("Words"));
    EXPECT_EQ(m.lastindex(), 0);
}


TEST(re, re_match)
{
    string data = "These are a bunch of words";
    auto m = re_match("\\w+", data);
    EXPECT_EQ(m.start(), 0);
    EXPECT_EQ(m.end(), 5);
    EXPECT_EQ(m.group(), string_view("These"));
    EXPECT_EQ(m.lastindex(), 0);

    data = "...~/.'' Words";
    m = re_match("\\w+", data);
    ASSERT_FALSE(bool(m));
}


TEST(re, syntax)
{
    // Need to check various assumptions made about the regular
    // expression are accurate.

    string data = "These are a bunch of words";
    auto m = re_match("\\w+$", data);
    ASSERT_FALSE(bool(m));

    m = re_match("^\\w+", data);
    ASSERT_TRUE(bool(m));
}


TEST(re, re_findall)
{
    string data = "These are a bunch of words";
    auto words = re_findall("\\w+", data);
    EXPECT_EQ(words.size(), 6);
    EXPECT_EQ(words.front(), string_view("These"));
}


TEST(re, re_finditer)
{
    string data = "These are a bunch of words";
    deque<string> actual;
    deque<string> expected = {
        "These",
        "are",
        "a",
        "bunch",
        "of",
        "words",
    };
    for (auto &match: re_finditer("\\w+", data)) {
        actual.emplace_back(string(match.group(0)));
    }
    EXPECT_EQ(actual, expected);
}


TEST(re, re_split)
{
    string data = "These are a bunch of words";

    // first example
    auto whitespace = re_split("\\w+", data);
    ASSERT_EQ(whitespace.size(), 7);
    EXPECT_EQ(whitespace[0], string_view(""));
    EXPECT_EQ(whitespace[1], string_view(" "));
    EXPECT_EQ(whitespace[2], string_view(" "));
    EXPECT_EQ(whitespace[3], string_view(" "));
    EXPECT_EQ(whitespace[4], string_view(" "));
    EXPECT_EQ(whitespace[5], string_view(" "));
    EXPECT_EQ(whitespace[6], string_view(""));
}


TEST(re, re_sub)
{
    EXPECT_EQ(re_sub("(\\w+)", "+\\1", "These are a bunch of words"), "+These +are +a +bunch +of +words");
}


TEST(re, re_escape)
{
    EXPECT_EQ(re_escape(string_view("\0", 1)), string("\\\0", 2));
}


TEST(re, re_purge)
{
    re_purge();
}
