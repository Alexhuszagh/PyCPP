//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PyCPP
 *  \brief String manipulation unittests.
 */

#include <pycpp/string/string.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(string, split)
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


TEST(string, quoted_split)
{
    auto data = quoted_split("';';0", ';', '\'', '\\');
    ASSERT_EQ(data.size(), 2);
    EXPECT_EQ(data[0], ";");
    EXPECT_EQ(data[1], "0");
}


TEST(string, rsplit)
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


TEST(string, join)
{
    string_list_t data = {"a", "b"};
    string_wrapper_list_t wrapper = {data[0], data[1]};
    EXPECT_EQ(join({"a", "c"}, " "), "a c");
    EXPECT_EQ(join(data), "a b");
    EXPECT_EQ(join(wrapper), "a b");
    EXPECT_EQ(join(data, "  "), "a  b");
}


TEST(string, capitalize)
{
    EXPECT_EQ(capitalize("this is"), "This is");
    EXPECT_EQ(capitalize(".this is"), ".this is");
}


TEST(string, expandtabs)
{
    std::string str = "This is a message";
    EXPECT_EQ(expandtabs(str), str);

    str = "This\t";
    EXPECT_EQ(expandtabs(str, 4), "This    ");

    str = "This\tis\ta\tmessage";
    EXPECT_EQ(expandtabs(str, 1), "This is a message");
}


TEST(string, startswith)
{
    EXPECT_TRUE(startswith("This", "T"));
    EXPECT_TRUE(startswith("This", "Thi"));
    EXPECT_FALSE(startswith("This", "t"));
    EXPECT_FALSE(startswith("This", "s"));
}


TEST(string, endswith)
{
    EXPECT_TRUE(endswith("This", "s"));
    EXPECT_TRUE(endswith("This", "his"));
    EXPECT_FALSE(endswith("This", "T"));
    EXPECT_FALSE(endswith("This", "t"));
}


TEST(string, ltrim)
{
    EXPECT_EQ(ltrim("ThisihT", "T"), "hisihT");
    EXPECT_EQ(ltrim("ThisihT", "Ti"), "hisihT");
    EXPECT_EQ(ltrim("ThisihT", "Tih"), "sihT");
    EXPECT_EQ(ltrim("ThisihT", "Tish"), "");
}


TEST(string, rtrim)
{
    EXPECT_EQ(rtrim("ThisihT", "T"), "Thisih");
    EXPECT_EQ(rtrim("ThisihT", "Ti"), "Thisih");
    EXPECT_EQ(rtrim("ThisihT", "Tih"), "This");
    EXPECT_EQ(rtrim("ThisihT", "Tish"), "");
}


TEST(string, trim)
{
    EXPECT_EQ(trim("ThisihT", "T"), "hisih");
    EXPECT_EQ(trim("ThisihT", "Ti"), "hisih");
    EXPECT_EQ(trim("ThisihT", "Tih"), "s");
    EXPECT_EQ(trim("ThisihT", "Tish"), "");
}


TEST(string, find)
{
    EXPECT_EQ(find("This", "i"), 2);
    EXPECT_EQ(find("This", "is"), 2);
    EXPECT_EQ(find("This", "igg"), SIZE_MAX);
}


TEST(string, rfind)
{
    EXPECT_EQ(rfind("ThisThis", "i"), 6);
    EXPECT_EQ(rfind("ThisThis", "is"), 6);
    EXPECT_EQ(rfind("This", "igg"), SIZE_MAX);
}


TEST(string, index)
{
    EXPECT_EQ(index("This", "i"), 2);
    EXPECT_EQ(index("This", "is"), 2);
    try {
        index("This", "igg");
    } catch (...) {
        return;
    }
    EXPECT_TRUE(false);
}


TEST(string, rindex)
{
    EXPECT_EQ(rindex("ThisThis", "i"), 6);
    EXPECT_EQ(rindex("ThisThis", "is"), 6);
    try {
        rindex("This", "igg");
    } catch (...) {
        return;
    }
    EXPECT_TRUE(false);
}


TEST(string, count)
{
    EXPECT_EQ(count("ThisThis", "x"), 0);
    EXPECT_EQ(count("ThisThis", "i"), 2);
    EXPECT_EQ(count("ThisThis", "is"), 2);
    EXPECT_EQ(count("ThisThis", "isT"), 1);
}


TEST(string, lower)
{
    EXPECT_EQ(lower("ThisThis"), "thisthis");
}


TEST(string, upper)
{
    EXPECT_EQ(upper("ThisThis"), "THISTHIS");
}


TEST(string, replace)
{
    EXPECT_EQ(replace("ThisThis", "is", "IS"), "ThISThIS");
}
