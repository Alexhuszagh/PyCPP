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

// OBJECTS
// -------


struct filter_istream_wrapper: filter_istream
{
    // Export ::swap publicly for unittests
    using filter_istream::filter_istream;
    using filter_istream::swap;
};


struct filter_ostream_wrapper: filter_ostream
{
    // Export ::swap publicly for unittests
    using filter_ostream::filter_ostream;
    using filter_ostream::swap;
};


template <typename OStream>
struct test_ostream
{
    template <typename T, typename Checker, typename ... Ts>
    void standard(T &t, const string_view& message, Checker checker, Ts... ts)
    {
        {
            OStream s1(t, ts...);
            s1 << message;
        }
        checker();
    }

    template <typename T, typename Checker, typename ... Ts>
    void move(T &t, const string_view& message, const Checker checker, Ts... ts)
    {
        {
            OStream s1(t, ts...), s2;
            s1.swap(s2);
            s2 << message;
        }
        checker();
    }

    template <typename T, typename Checker, typename ... Ts>
    void operator()(T &t, const string_view& message, Checker checker, Ts... ts)
    {
        standard(t, message, checker, ts...);
        move(t, message, checker, ts...);
    }
};


template <typename IStream>
struct test_istream
{
    template <typename T, typename Checker, typename ... Ts>
    void standard(T &t, Checker checker, Ts... ts)
    {
        IStream s1(t, ts...);
        std::string line;
        std::getline(s1, line);
        checker(line);
    }

    template <typename T, typename Checker, typename ... Ts>
    void move(T &t, const Checker checker, Ts... ts)
    {
        IStream s1(t, ts...), s2;
        s1.swap(s2);
        std::string line;
        std::getline(s2, line);
        checker(line);
    }

    template <typename T, typename Checker, typename ... Ts>
    void operator()(T &t, Checker checker, Ts... ts)
    {
        standard(t, checker, ts...);
        move(t, checker, ts...);
    }
};


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
    using tester = test_istream<filter_istream_wrapper>;

    istringstream sstream("This is a message");
    std::string actual = "This is a message";
    tester()(sstream, [&sstream, &actual](const std::string& result) {
        EXPECT_EQ(result, actual);
        sstream.seekg(0);
    }, nullptr);
}


TEST(filter_istream, doublechars)
{
    using tester = test_istream<filter_istream_wrapper>;

    istringstream sstream("This is a message");
    std::string actual = "TThhiiss  iiss  aa  mmeessssaaggee";
    tester()(sstream, [&sstream, &actual](const std::string& result) {
        EXPECT_EQ(result, actual);
        sstream.seekg(0);
    }, doublechars);
}

// OSTREAM

TEST(filter_ostream, nocallback)
{
    using tester = test_ostream<filter_ostream_wrapper>;

    ostringstream sstream;
    std::string message = "This is a message";
    std::string actual = message;
    tester()(sstream, message, [&sstream, &actual]() {
        EXPECT_EQ(sstream.str(), actual);
        sstream.seekp(0);
    }, nullptr);
}


TEST(filter_ostream, doublechars)
{
    using tester = test_ostream<filter_ostream_wrapper>;

    ostringstream sstream;
    std::string message = "This is a message";
    std::string actual = "TThhiiss  iiss  aa  mmeessssaaggee";
    tester()(sstream, message, [&sstream, &actual]() {
        EXPECT_EQ(sstream.str(), actual);
        sstream.seekp(0);
    }, doublechars);
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
    using tester = test_istream<filter_ifstream>;
    std::string actual = "TThhiiss  iiss  aa  mmeessssaaggee";
    tester()(path, [&actual](const std::string& result) {
        EXPECT_EQ(result, actual);
    }, ios_base::in, doublechars);

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
    // read data
    using tester = test_ostream<filter_ofstream>;
    string path("sample_filter_ofstream.txt");
    std::string message = "This is a message";
    std::string actual = "TThhiiss  iiss  aa  mmeessssaaggee";
    tester()(path, message, [&path, &actual]() {
        // check contents
        {
            ifstream ifs(path);
            std::string line;
            std::getline(ifs, line);
            EXPECT_EQ(line, actual);
        }
        // cleanup
        ASSERT_TRUE(exists(path));
        EXPECT_TRUE(remove_file(path));
    }, ios_base::out, doublechars);
}

#endif                  // BUILD_FILESYSTEM
