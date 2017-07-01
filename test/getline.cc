//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Getline unittests.
 */

#include <getline.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(getline, unix)
{
    std::stringstream stream("line1\nline2\nline3");
    std::string line;
    int counter = 1;
    while (getline(stream, line)) {
        EXPECT_EQ(line, "line" + std::to_string(counter++));
    }
    EXPECT_EQ(counter, 4);
}


TEST(getline, windows)
{
    std::stringstream stream("line1\r\nline2\r\nline3");
    std::string line;
    int counter = 1;
    while (getline(stream, line)) {
        EXPECT_EQ(line, "line" + std::to_string(counter++));
    }
    EXPECT_EQ(counter, 4);
}


TEST(getline, macos9)
{
    std::stringstream stream("line1\rline2\rline3");
    std::string line;
    int counter = 1;
    while (getline(stream, line)) {
        EXPECT_EQ(line, "line" + std::to_string(counter++));
    }
    EXPECT_EQ(counter, 4);
}
