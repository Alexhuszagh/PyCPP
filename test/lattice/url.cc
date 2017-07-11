//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#include <pycpp/lattice/url.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// TESTS
// -----


TEST(url_t, constructors)
{
    url_t url;
    url = url_t("http://example.com");
    url = url_t("http://example.com", 18);
    url = url_t(std::string("http://example.com"));
    url = url_t {104, 116, 116, 112, 58, 47, 47, 101, 120, 97, 109, 112, 108, 101, 46, 99, 111, 109};
}


TEST(url_t, getters)
{
    url_t url("http://example.com/path/to/file");
    EXPECT_EQ("http", url.service());
    EXPECT_EQ("example.com", url.host());
    EXPECT_EQ("/path/to/file", url.path());
    EXPECT_EQ("path/to", url.directory());
    EXPECT_EQ("file", url.file());
}


TEST(url_t, setters)
{
    url_t url("http://example.com/path/to/file");

    // SERVICE
    url.set_service("https");
    EXPECT_EQ("https://example.com/path/to/file", url);

    // HOST
    url.set_host("example.net");
    EXPECT_EQ("https://example.net/path/to/file", url);

    // PATH
    url.set_path("/new/path/to/file");
    EXPECT_EQ("https://example.net/new/path/to/file", url);

    // DIRECTORY
    url.set_directory("new/directory");
    EXPECT_EQ("https://example.net/new/directory/file", url);

    // FILE
    url.set_file("document");
    EXPECT_EQ("https://example.net/new/directory/document", url);
}


TEST(url_t, properties)
{
    url_t url("http://example.com/path/to/file");
    EXPECT_FALSE(url.relative());
    EXPECT_TRUE(url.absolute());

    url = url_t("/path/to/file");
    EXPECT_TRUE(url.relative());
    EXPECT_FALSE(url.absolute());
}


static std::string string_to_hex(const std::string& input)
{
    static const char* const lut = "0123456789abcdef";

    std::string output;
    output.reserve(4 * input.length());
    for (size_t i = 0; i < input.length(); ++i)
    {
        const unsigned char c = input[i];
        output.push_back('\\');
        output.push_back('x');
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
}


TEST(url_t, unicode)
{
    // "http://räksmörgås.josefsson.org/"
    url_t url = {
        104, 116, 116, 112, 58, 47, 47, 114, -61, -92,
        107, 115, 109, -61, -74, 114, 103, -61, -91,
        115, 46, 106, 111, 115, 101, 102, 115, 115, 111,
        110, 46, 111, 114, 103, 47
    };

    EXPECT_EQ("http", url.service());
    EXPECT_EQ("xn--rksmrgs-5wao1o.josefsson.org", url.host());
    EXPECT_EQ("/", url.path());
    EXPECT_EQ("", url.directory());
    EXPECT_EQ("", url.file());
}
