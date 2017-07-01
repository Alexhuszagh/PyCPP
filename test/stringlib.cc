//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup funxx
 *  \brief String manipulation unittests.
 */

#include <stringlib.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(stringlib, split)
{
    auto data = split("This,Is,A,String", ",");
    EXPECT_EQ(data.size(), 4);
    EXPECT_EQ(data[0], "This");
    EXPECT_EQ(data[1], "Is");
    EXPECT_EQ(data[2], "A");
    EXPECT_EQ(data[3], "String");

    data = split("This,Is,A,String", "\t");
    EXPECT_EQ(data.size(), 1);
    EXPECT_EQ(data[0], "This,Is,A,String");

    data = split("This\tIs\tA\tString", "\t");
    EXPECT_EQ(data.size(), 4);
    EXPECT_EQ(data[0], "This");
    EXPECT_EQ(data[1], "Is");
    EXPECT_EQ(data[2], "A");
    EXPECT_EQ(data[3], "String");

    data = split("This\tIs\tA\tString", ",");
    EXPECT_EQ(data.size(), 1);
    EXPECT_EQ(data[0], "This\tIs\tA\tString");

    data = split(";;0", ";");
    EXPECT_EQ(data.size(), 3);
    EXPECT_EQ(data[0], "");
    EXPECT_EQ(data[1], "");
    EXPECT_EQ(data[2], "0");
}
