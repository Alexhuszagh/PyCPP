//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief File-based stream unittests.
 */

#include <pycpp/byteorder.h>
#include <pycpp/os.h>
#include <pycpp/stream/fstream.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <gtest/gtest.h>
#if defined(HAVE_WFOPEN)
#   include <io.h>
#endif

// CONSTANTS
// ---------

static const std::string UTF8_ENGLISH = {69, 110, 103, 108, 105, 115, 104};
static const std::string UTF8_KOREAN = {-19, -107, -100, -22, -75, -83, -20, -106, -76};
#if defined(HAVE_WFOPEN)
#if BYTE_ORDER == LITTLE_ENDIAN
static const std::wstring UTF16_ENGLISH = {69, 110, 103, 108, 105, 115, 104};
static const std::wstring UTF16_KOREAN = {54620, 44397, 50612};
#else
static const std::wstring UTF16_ENGLISH = {23765, 28077, -19259};
static const std::wstring UTF16_KOREAN = {23765,  28077, -19259};
#endif
#endif

// HELPERS
// -------

template <typename IStream, typename OStream>
struct test_stream
{
    template <typename String, typename RemoveFile>
    void operator()(const String &path, RemoveFile remove_file)
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
        EXPECT_TRUE(remove_file(path));
    }
};

// TESTS
// -----


TEST(fstream, fstream)
{
    typedef test_stream<fstream, fstream> tester;

    tester()(UTF8_ENGLISH, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    });

#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH, [](const std::wstring& path) {
        return _wunlink(path.data()) == 0;
    });
    tester()(UTF16_KOREAN, [](const std::wstring& path) {
        return _wunlink(path.data()) == 0;
    });
#else                           // POSIX
    tester()(UTF8_KOREAN, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    });
#endif
}


TEST(fstream, iofstream)
{
    typedef test_stream<ifstream, ofstream> tester;

    tester()(UTF8_ENGLISH, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    });

#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH, [](const std::wstring& path) {
        return _wunlink(path.data()) == 0;
    });
    tester()(UTF16_KOREAN, [](const std::wstring& path) {
        return _wunlink(path.data()) == 0;
    });
#else                           // POSIX
    tester()(UTF8_KOREAN, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    });
#endif
}

#include <warnings/pop.h>
