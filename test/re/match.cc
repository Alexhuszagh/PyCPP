//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Regular expression match object unittests.
 */

#include <pycpp/re/regex.h>
#include <gtest/gtest.h>
#include <deque>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(re, match_iterator_t)
{
    std::string data = "These are a bunch of words";
    regexp_t regex("\\w+");
    match_iterator_t first(regex, data);
    match_iterator_t last;

    std::deque<std::string> actual;
    std::deque<std::string> expected = {
        "These",
        "are",
        "a",
        "bunch",
        "of",
        "words",
    };
    EXPECT_NE(first, last);
    for (; first != last; ++first) {
        actual.emplace_back(std::string(first->group(0)));
    }
    EXPECT_EQ(actual, expected);
}
