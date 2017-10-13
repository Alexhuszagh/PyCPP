//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief File descriptor stream unittests.
 */

#include <pycpp/filesystem.h>
#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stream/fd.h>
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
    template <typename String, typename RemoveFile>
    void operator()(const String &path, RemoveFile remove_file, size_t seekg = 0)
    {
        fd_t fd;
        std::string expected = "Single line";

        fd = fd_open(path, std::ios_base::out);
        OStream ostream(fd, true);
        ostream << expected << std::endl;
        ostream.close();

        fd = fd_open(path, std::ios_base::in);
        IStream istream(fd, true);
        istream.seekg(seekg);
        std::string result;
        std::getline(istream, result);
        istream.close();

        EXPECT_EQ(result, expected.substr(seekg));
        EXPECT_TRUE(remove_file(path));
    }
};

// TESTS
// -----


TEST(fd_streambuf, fd_streambuf)
{
    std::string path("sample_path");
    size_t size = 11;
    std::string in("Single Line");
    std::vector<char> out(size);

    // open
    fd_t fd = fd_open(path, std::ios_base::in | std::ios_base::out);
    fd_streambuf buf(std::ios_base::in | std::ios_base::out, fd);

    // write
    EXPECT_EQ((size_t) buf.sputn(in.data(), in.size()), size);

    // seek
    EXPECT_EQ((size_t) buf.pubseekpos(0), 0);

    // read
    EXPECT_EQ((size_t) buf.sgetn(out.data(), out.size()), size);
    EXPECT_EQ(in, std::string(out.data(), out.size()));

    // cleanup
    buf.close();
    fd_close(fd);
    EXPECT_TRUE(remove_file(path));
}


TEST(fd_stream, fd_stream)
{
    typedef test_stream<fd_stream, fd_stream> tester;

    tester()(UTF8_ENGLISH, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    });
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH, [](const std::u16string& path) {
        return _wunlink(reinterpret_cast<const wchar_t*>(path.data())) == 0;
    });
    tester()(UTF16_KOREAN, [](const std::u16string& path) {
        return _wunlink(reinterpret_cast<const wchar_t*>(path.data())) == 0;
    });
#else                           // POSIX
    tester()(UTF8_KOREAN, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    });
#endif
}


TEST(fd_stream, iostream)
{
    typedef test_stream<fd_istream, fd_ostream> tester;

    tester()(UTF8_ENGLISH, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    });

#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH, [](const std::u16string& path) {
        return _wunlink(reinterpret_cast<const wchar_t*>(path.data())) == 0;
    });
    tester()(UTF16_KOREAN, [](const std::u16string& path) {
        return _wunlink(reinterpret_cast<const wchar_t*>(path.data())) == 0;
    });
#else                           // POSIX
    tester()(UTF8_KOREAN, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    });
#endif
}


TEST(fd_stream, seek)
{
    typedef test_stream<fd_istream, fd_ostream> tester;

    tester()(UTF8_ENGLISH, [](const std::string& path) {
        return std::remove(path.data()) == 0;
    }, 4);
}
