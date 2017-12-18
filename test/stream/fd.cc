//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief File descriptor stream unittests.
 */

#include <pycpp/filesystem.h>
#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/preprocessor/os.h>
#include <pycpp/stl/vector.h>
#include <pycpp/stream/fd.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <gtest/gtest.h>
#if defined(HAVE_WFOPEN)
#   include <io.h>
#endif

PYCPP_USING_NAMESPACE

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
    template <typename String>
    void standard(const String &path, size_t seekg, mode_t permissions, io_access_pattern access)
    {
        fd_t fd;
        std::string expected = "Single line";

        fd = fd_open(path, ios_base::out, permissions, access);
        OStream ofs1(fd, true);
        ofs1 << expected << endl;
        ofs1.close();

        fd = fd_open(path, ios_base::in, permissions, access);
        IStream ifs1(fd, true);
        ifs1.seekg(seekg);
        std::string result;
        std::getline(ifs1, result);
        ifs1.close();

        EXPECT_EQ(result, expected.substr(seekg));
        EXPECT_TRUE(remove_file(path));
    }

    template <typename String>
    void move(const String &path, size_t seekg, mode_t permissions, io_access_pattern access)
    {
        fd_t fd;
        std::string expected = "Single line";

        fd = fd_open(path, ios_base::out, permissions, access);
        OStream ofs1(fd, true);
        OStream ofs2(std::move(ofs1));
        ofs2 << expected << endl;
        ofs2.close();

        fd = fd_open(path, ios_base::in, permissions, access);
        IStream ifs1(fd, true);
        IStream ifs2(std::move(ifs1));
        ifs2.seekg(seekg);
        std::string result;
        std::getline(ifs2, result);
        ifs2.close();

        EXPECT_EQ(result, expected.substr(seekg));
        EXPECT_TRUE(remove_file(path));
    }

    template <typename String>
    void operator()(const String &path, size_t seekg = 0, mode_t permissions = S_IWR_USR_GRP, io_access_pattern access = access_normal)
    {
        standard(path, seekg, permissions, access);
        move(path, seekg, permissions, access);
    }
};

// TESTS
// -----


TEST(fd_streambuf, fd_streambuf)
{
    string path("sample_path");
    size_t size = 11;
    std::string in("Single Line");
    vector<char> out(size);

    // open
    fd_t fd = fd_open(path, ios_base::in | ios_base::out);
    fd_streambuf buf(ios_base::in | ios_base::out, fd);

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
    using tester = test_stream<fd_stream, fd_stream>;

    tester()(UTF8_ENGLISH);
    tester()(UTF8_ENGLISH, 0, S_IWR_USR_GRP, access_normal);
    tester()(UTF8_ENGLISH, 0, S_IWR_USR_GRP, access_sequential);
    tester()(UTF8_ENGLISH, 0, S_IWR_USR_GRP, access_random);
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH);
    tester()(UTF16_KOREAN);
#else                           // POSIX
    tester()(UTF8_KOREAN);
#endif
}


TEST(fd_stream, iostream)
{
    using tester = test_stream<fd_istream, fd_ostream>;

    tester()(UTF8_ENGLISH);
    tester()(UTF8_ENGLISH, 0, S_IWR_USR_GRP, access_normal);
    tester()(UTF8_ENGLISH, 0, S_IWR_USR_GRP, access_sequential);
    tester()(UTF8_ENGLISH, 0, S_IWR_USR_GRP, access_random);
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH);
    tester()(UTF16_KOREAN);
#else                           // POSIX
    tester()(UTF8_KOREAN);
#endif
}


TEST(fd_stream, seek)
{
    using tester = test_stream<fd_istream, fd_ostream>;

    tester()(UTF8_ENGLISH, 4);
    tester()(UTF8_ENGLISH, 4, S_IWR_USR_GRP, access_normal);
    tester()(UTF8_ENGLISH, 4, S_IWR_USR_GRP, access_sequential);
    tester()(UTF8_ENGLISH, 4, S_IWR_USR_GRP, access_random);
}
