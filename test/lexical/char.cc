//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Lexical char conversion unittests.
 */

#include <pycpp/lexical/char.h>
#include <pycpp/stl/stdexcept.h>
#include <pycpp/stl/utility.h>
#include <pycpp/stl/vector.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// DATA
// ----

static vector<pair<char, std::string>> DATA = {
    {'\0', std::string("\0", 1)},
    {'\b', "\b"},
    {'\f', "\f"},
    {'a', "a"},
};

// TESTS
// -----


TEST(lexical_char_formatter, lexical_char_formatter)
{
    auto TEST_CHAR  = [](char c, const string_view& expected) {
        EXPECT_EQ(lexical_char_formatter(c).str(), expected);
    };
    auto TEST_CONST_CHAR  = [](const char b, const string_view& expected) {
        EXPECT_EQ(lexical_char_formatter(b).str(), expected);
    };
    auto TEST_VOLATILE_CHAR  = [](volatile char b, const string_view& expected) {
        EXPECT_EQ(lexical_char_formatter(b).str(), expected);
    };
    auto TEST_CV_CHAR  = [](volatile char b, const string_view& expected) {
        EXPECT_EQ(lexical_char_formatter(b).str(), expected);
    };
    auto TEST_UCHAR  = [](unsigned char c, const string_view& expected) {
        EXPECT_EQ(lexical_char_formatter(c).str(), expected);
    };
    auto TEST_CONST_UCHAR  = [](const unsigned char b, const string_view& expected) {
        EXPECT_EQ(lexical_char_formatter(b).str(), expected);
    };
    auto TEST_VOLATILE_UCHAR  = [](volatile unsigned char b, const string_view& expected) {
        EXPECT_EQ(lexical_char_formatter(b).str(), expected);
    };
    auto TEST_CV_UCHAR  = [](volatile unsigned char b, const string_view& expected) {
        EXPECT_EQ(lexical_char_formatter(b).str(), expected);
    };

    for (const auto& pair: DATA) {
        TEST_CHAR(pair.first, pair.second);
        TEST_CONST_CHAR(pair.first, pair.second);
        TEST_VOLATILE_CHAR(pair.first, pair.second);
        TEST_CV_CHAR(pair.first, pair.second);
        TEST_UCHAR(pair.first, pair.second);
        TEST_CONST_UCHAR(pair.first, pair.second);
        TEST_VOLATILE_UCHAR(pair.first, pair.second);
        TEST_CV_UCHAR(pair.first, pair.second);
    }
}


TEST(lexical_char_extractor, lexical_char_extractor)
{
    EXPECT_THROW(lexical_char_extractor(""), runtime_error);
    EXPECT_EQ(char(lexical_char_extractor("c")), 'c');
    EXPECT_EQ(char(lexical_char_extractor("*")), '*');
    EXPECT_EQ(char(lexical_char_extractor("\n")), '\n');
    EXPECT_EQ(char(lexical_char_extractor("\t")), '\t');
    EXPECT_THROW(lexical_char_extractor("cc"), runtime_error);
}
