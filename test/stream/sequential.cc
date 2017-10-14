//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Sequential file I/O unittests.
 */

#include <pycpp/filesystem.h>
#include <pycpp/stream/sequential.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <gtest/gtest.h>
#if defined(HAVE_WFOPEN)
#   include <io.h>
#endif

 PYCPP_USING_NAMESPACE

static const std::string UTF8_ENGLISH = {69, 110, 103, 108, 105, 115, 104};
static const std::string UTF8_KOREAN = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
#if defined(HAVE_WFOPEN)
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::u16string UTF16_ENGLISH = {69, 110, 103, 108, 105, 115, 104};
static const std::u16string UTF16_KOREAN = {54620, 44397, 50612};
#else
static const std::u16string UTF16_ENGLISH = {23765, 28077, -19259};
static const std::u16string UTF16_KOREAN = {23765,  28077, -19259};
#endif
#endif

// HELPERS
// -------

template <typename IStream, typename OStream>
struct test_stream
{
    template <typename String>
    void operator()(const String &path)
    {
        std::string expected = "Single line";

        OStream ostream(path, std::ios_base::out);
        ostream << expected << std::endl;
        ostream.close();

        IStream istream(path, std::ios_base::in);
        std::string result;
        std::getline(istream, result);
        istream.close();

        EXPECT_EQ(result, expected);
        EXPECT_EQ(result[0], expected[0]);
        EXPECT_TRUE(remove_file(path));
    }
};


// TESTS
// -----


TEST(sequential_fstream, sequential_fstream)
{
    typedef test_stream<sequential_fstream, sequential_fstream> tester;

    tester()(UTF8_ENGLISH);
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH);
    tester()(UTF16_KOREAN);
#else                           // POSIX
    tester()(UTF8_KOREAN);
#endif
}


TEST(sequential_fstream, sequential_iofstream)
{
    typedef test_stream<sequential_ifstream, sequential_ofstream> tester;

    tester()(UTF8_ENGLISH);
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH);
    tester()(UTF16_KOREAN);
#else                           // POSIX
    tester()(UTF8_KOREAN);
#endif
}

#include <warnings/pop.h>
