//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief String manipulation unittests.
 */

#include <pycpp/stringlib.h>
#include <gtest/gtest.h>

// TESTS
// -----


TEST(stringlib, split)
{
    auto data = split("This,Is,A,String", ",");
    ASSERT_EQ(data.size(), 4);
    EXPECT_EQ(data[0], "This");
    EXPECT_EQ(data[1], "Is");
    EXPECT_EQ(data[2], "A");
    EXPECT_EQ(data[3], "String");

    data = split("This,Is,A,String", "\t");
    ASSERT_EQ(data.size(), 1);
    EXPECT_EQ(data[0], "This,Is,A,String");

    data = split("This\tIs\tA\tString", "\t");
    ASSERT_EQ(data.size(), 4);
    EXPECT_EQ(data[0], "This");
    EXPECT_EQ(data[1], "Is");
    EXPECT_EQ(data[2], "A");
    EXPECT_EQ(data[3], "String");

    data = split("This\tIs\tA\tString", ",");
    ASSERT_EQ(data.size(), 1);
    EXPECT_EQ(data[0], "This\tIs\tA\tString");

    data = split(";;0", ";");
    ASSERT_EQ(data.size(), 3);
    EXPECT_EQ(data[0], "");
    EXPECT_EQ(data[1], "");
    EXPECT_EQ(data[2], "0");

    data = split(";;0", ";", 1);
    ASSERT_EQ(data.size(), 2);
    EXPECT_EQ(data[0], "");
    EXPECT_EQ(data[1], ";0");

    data = split(";;0", [](char c) {
        return c == ';';
    }, 1);
    ASSERT_EQ(data.size(), 2);
    EXPECT_EQ(data[0], "");
    EXPECT_EQ(data[1], ";0");

    data = split(";;;", ";");
    ASSERT_EQ(data.size(), 4);
    EXPECT_EQ(data[0], "");
    EXPECT_EQ(data[1], "");
    EXPECT_EQ(data[2], "");
    EXPECT_EQ(data[3], "");
}


TEST(stringlib, rsplit)
{
    auto data = split(";;0", ";");
    ASSERT_EQ(data.size(), 3);
    EXPECT_EQ(data[0], "");
    EXPECT_EQ(data[1], "");
    EXPECT_EQ(data[2], "0");

    data = rsplit(";;0", ";");
    ASSERT_EQ(data.size(), 3);
    EXPECT_EQ(data[0], "");
    EXPECT_EQ(data[1], "");
    EXPECT_EQ(data[2], "0");

    data = rsplit(";;0", ";", 1);
    ASSERT_EQ(data.size(), 2);
    EXPECT_EQ(data[0], ";");
    EXPECT_EQ(data[1], "0");

    data = rsplit(";;0", [](char c) {
        return c == ';';
    }, 1);
    ASSERT_EQ(data.size(), 2);
    EXPECT_EQ(data[0], ";");
    EXPECT_EQ(data[1], "0");
}


TEST(stringlib, join)
{
    string_list_t data = {"a", "b"};
    EXPECT_EQ(join(data), "a b");
    EXPECT_EQ(join(data, "  "), "a  b");
}


TEST(stringlib, capitalize)
{
    // TODO: here..
}


TEST(stringlib, expandtabs)
{
    std::string str = "This is a message";
    EXPECT_EQ(expandtabs(str), str);

    str = "This\t";
    EXPECT_EQ(expandtabs(str, 4), "This    ");

    str = "This\tis\ta\tmessage";
    EXPECT_EQ(expandtabs(str, 1), "This is a message");
}


TEST(stringlib, find)
{
    EXPECT_EQ(find("This", "i"), 2);
    EXPECT_EQ(find("This", "is"), 2);
    EXPECT_EQ(find("This", "igg"), SIZE_MAX);
}


TEST(stringlib, rfind)
{
    EXPECT_EQ(rfind("ThisThis", "i"), 6);
// TODO: bug here...
//    EXPECT_EQ(rfind("ThisThis", "is"), 6);
//    EXPECT_EQ(rfind("This", "igg"), SIZE_MAX);
}


TEST(stringlib, index)
{
    // TODO: here..
}


TEST(stringlib, rindex)
{
    // TODO: here..
}


TEST(stringlib, count)
{
    // TODO: here..
}


TEST(stringlib, lower)
{
    // TODO: here..
}


TEST(stringlib, upper)
{
    // TODO: here..
}


TEST(stringlib, replace)
{
    // TODO: here..
}
