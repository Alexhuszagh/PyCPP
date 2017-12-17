//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Getline unittests.
 */

#include <pycpp/lexical.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/string/getline.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(getline, unix)
{
    stringstream stream("line1\nline2\nline3");
    std::string line;
    int counter = 1;
    while (getline(stream, line)) {
        EXPECT_EQ(line, "line" + lexical(counter++));
    }
    EXPECT_EQ(counter, 4);
}


TEST(getline, windows)
{
    stringstream stream("line1\r\nline2\r\nline3");
    std::string line;
    int counter = 1;
    while (getline(stream, line)) {
        EXPECT_EQ(line, "line" + lexical(counter++));
    }
    EXPECT_EQ(counter, 4);
}


TEST(getline, macos9)
{
    stringstream stream("line1\rline2\rline3");
    std::string line;
    int counter = 1;
    while (getline(stream, line)) {
        EXPECT_EQ(line, "line" + lexical(counter++));
    }
    EXPECT_EQ(counter, 4);
}
