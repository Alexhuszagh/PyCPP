//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Transformable stream unittests.
 */

#include <pycpp/stl/algorithm.h>
#include <pycpp/stl/sstream.h>
#include <pycpp/stl/string.h>
#include <pycpp/stream/filter.h>
#include <gtest/gtest.h>
#if BUILD_FILESYSTEM
#   include <pycpp/filesystem.h>
#endif          // BUILD_FILESYSTEM

PYCPP_USING_NAMESPACE

// FUNCTIONS
// ---------

void doublechars(const void*& src, size_t srclen,
    void*& dst, size_t dstlen,
    size_t char_size)
{
    size_t bytes = min(srclen, dstlen / 2) * char_size;
    const char* src_ = reinterpret_cast<const char*>(src);
    char* dst_ = reinterpret_cast<char*>(dst);

    // copy bytes
    while (bytes--) {
        *dst_++ = *src_;
        *dst_++ = *src_++;
    }

    // reassign to buffer
    src = (const void*) src_;
    dst = (void*) dst_;
}

// TESTS
// -----

// STREAMBUF

TEST(filter_streambuf, swap)
{
    filter_streambuf sb1(ios_base::in);
    filter_streambuf sb2(ios_base::in);
    sb1.swap(sb2);
    EXPECT_NE(&sb1, &sb2);

    filter_streambuf sb3(ios_base::in, nullptr, doublechars);
    sb1.swap(sb3);
    EXPECT_NE(&sb1, &sb3);
}

// ISTREAM

TEST(filter_istream, nocallback)
{
    istringstream sstream("This is a message");
    filter_istream stream(sstream);
    std::string line;

    std::getline(stream, line);
    EXPECT_EQ(line, "This is a message");
}


TEST(filter_istream, doublechars)
{
    istringstream sstream("This is a message");
    filter_istream stream(sstream, doublechars);
    std::string line;

    std::getline(stream, line);
    EXPECT_EQ(line, "TThhiiss  iiss  aa  mmeessssaaggee");
}

// OSTREAM

TEST(filter_ostream, nocallback)
{
    ostringstream sstream;
    {
        filter_ostream stream(sstream);
        stream << "This is a message";
    }
    EXPECT_EQ(sstream.str(), "This is a message");
}


TEST(filter_ostream, doublechars)
{
    ostringstream sstream;
    {
        filter_ostream stream(sstream, doublechars);
        stream << "This is a message";
    }
    EXPECT_EQ(sstream.str(), "TThhiiss  iiss  aa  mmeessssaaggee");
}

#if BUILD_FILESYSTEM

// IFSTREAM

TEST(filter_ifstream, null_constructor)
{
    filter_ifstream s1;
    EXPECT_FALSE(s1.is_open());
}


TEST(filter_ifstream, doublechars)
{
    // write data
    string path("sample_filter_ifstream.txt");
    ASSERT_FALSE(exists(path));
    {
        ofstream stream(path);
        stream << "This is a message";
    }
    ASSERT_TRUE(exists(path));

    // read data
    {
        filter_ifstream s1(path, ios_base::in, doublechars);
        ASSERT_TRUE(s1.is_open());
        std::string line;
        std::getline(s1, line);
        EXPECT_EQ(line, "TThhiiss  iiss  aa  mmeessssaaggee");
    }

    // cleanup
    EXPECT_TRUE(remove_file(path));
}


TEST(filter_ifstream, move)
{
    // write data
    string path("sample_filter_ifstream.txt");
    ASSERT_FALSE(exists(path));
    {
        ofstream stream(path);
        stream << "This is a message";
    }
    ASSERT_TRUE(exists(path));

    // read data
    {
        // TODO: restore
//        filter_ifstream s1(path, ios_base::in, doublechars);
//        filter_ifstream s2(std::move(s1));
//        ASSERT_FALSE(s1.is_open());
//        ASSERT_TRUE(s2.is_open());
//        std::string line;
//        std::getline(s1, line);
//        EXPECT_EQ(line, "TThhiiss  iiss  aa  mmeessssaaggee");
    }

    // cleanup
    EXPECT_TRUE(remove_file(path));
}

// OFSTREAM

TEST(filter_ofstream, null_constructor)
{
    filter_ofstream s1;
    EXPECT_FALSE(s1.is_open());
}


TEST(filter_ofstream, doublechars)
{
    // write data
    string path("sample_filter_ofstream.txt");
    ASSERT_FALSE(exists(path));
    {
        filter_ofstream s1(path, ios_base::out, doublechars);
        ASSERT_TRUE(s1.is_open());
        s1 << "This is a message";
    }
    ASSERT_TRUE(exists(path));

    // read data
    {
        ifstream stream(path);
        std::string line;
        std::getline(stream, line);
        EXPECT_EQ(line, "TThhiiss  iiss  aa  mmeessssaaggee");
    }

    // cleanup
    EXPECT_TRUE(remove_file(path));
}


TEST(filter_ofstream, move)
{
    // TODO: implement...
}

#endif                  // BUILD_FILESYSTEM
