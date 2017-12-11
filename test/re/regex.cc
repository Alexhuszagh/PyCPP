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


TEST(regex, search)
{
    regexp_t regex("\\w+");
    std::string data = "These are a bunch of words";

    // first example
    auto m = regex.search(data);
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 0);
    EXPECT_EQ(m.end(), 5);
    EXPECT_EQ(m.group(), string_view("These"));
    EXPECT_EQ(m.lastindex(), 0);

    m = regex.search(data, m.end());
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 6);
    EXPECT_EQ(m.end(), 9);
    EXPECT_EQ(m.group(), string_view("are"));
    EXPECT_EQ(m.lastindex(), 0);
}


TEST(regex, match)
{
    regexp_t regex("\\w+");
    std::string data = "These are a bunch of words";

    // first example
    auto m = regex.match(data);
    ASSERT_TRUE(bool(m));
    EXPECT_EQ(m.start(), 0);
    EXPECT_EQ(m.end(), 5);
    EXPECT_EQ(m.group(), string_view("These"));
    EXPECT_EQ(m.lastindex(), 0);

    m = regex.match(data, m.end());
    ASSERT_FALSE(bool(m));
}


TEST(regex, split)
{
    regexp_t regex("\\w+");
    std::string data = "These are a bunch of words";

    // first example
    auto whitespace = regex.split(data);
    ASSERT_EQ(whitespace.size(), 7);
    EXPECT_EQ(whitespace[0], string_view(""));
    EXPECT_EQ(whitespace[1], string_view(" "));
    EXPECT_EQ(whitespace[2], string_view(" "));
    EXPECT_EQ(whitespace[3], string_view(" "));
    EXPECT_EQ(whitespace[4], string_view(" "));
    EXPECT_EQ(whitespace[5], string_view(" "));
    EXPECT_EQ(whitespace[6], string_view(""));

    // limit split
    whitespace = regex.split(data, 1);
    ASSERT_EQ(whitespace.size(), 2);
    EXPECT_EQ(whitespace[0], string_view(""));
    EXPECT_EQ(whitespace[1], string_view(" are a bunch of words"));
}


TEST(regex, sub)
{
    regexp_t regex("(\\w+)");
    EXPECT_EQ(regex.sub("+\\1", "These are a bunch of words"), "+These +are +a +bunch +of +words");
}
