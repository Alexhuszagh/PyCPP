//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Memory-mapped file I/O unittests.
 */

#include <pycpp/filesystem.h>
#include <pycpp/stream/mmap.h>
#include <warnings/push.h>
#include <warnings/narrowing-conversions.h>
#include <gtest/gtest.h>
#if defined(HAVE_WFOPEN)
#   include <io.h>
#endif

#if defined(HAVE_MMAP) || defined(OS_WINDOWS)           // MMAP

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
    void standard(const String &path)
    {
        std::string expected = "Single line";

        OStream ofs1(path, ios_base::out);
        ofs1.map(0, expected.size());
        for (size_t i = 0; i < expected.size(); ++i) {
            ofs1[i] = expected[i];
        }
        ofs1.flush(false);
        ofs1.unmap();
        ofs1.close();

        IStream ifs1(path, ios_base::in);
        ifs1.map(0);
        EXPECT_TRUE(ifs1.has_mapping());
        std::string actual(ifs1.data(), ifs1.size());
        ifs1.close();

        EXPECT_EQ(actual, expected);
        EXPECT_TRUE(remove_file(path));
    }

    template <typename String>
    void move(const String &path)
    {
        std::string expected = "Single line";

        OStream ofs1(path, ios_base::out);
        ofs1.map(0, expected.size());
        OStream osf2(std::move(ofs1));
        for (size_t i = 0; i < expected.size(); ++i) {
            osf2[i] = expected[i];
        }
        osf2.flush(false);
        osf2.unmap();
        osf2.close();

        IStream ifs1(path, ios_base::in);
        ifs1.map(0);
        IStream ifs2(std::move(ifs1));
        EXPECT_TRUE(ifs2.has_mapping());
        std::string actual(ifs2.data(), ifs2.size());
        ifs2.close();

        EXPECT_EQ(actual, expected);
        EXPECT_TRUE(remove_file(path));
    }

    template <typename String>
    void operator()(const String &path)
    {
        standard(path);
        move(path);
    }
};


// TESTS
// -----


TEST(mmap_fstream, mmap_fstream)
{
    using tester = test_stream<mmap_fstream, mmap_fstream>;

    tester()(UTF8_ENGLISH);
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH);
    tester()(UTF16_KOREAN);
#else                           // POSIX
    tester()(UTF8_KOREAN);
#endif
}


TEST(mmap_fstream, mmap_iofstream)
{
    using tester = test_stream<mmap_ifstream, mmap_ofstream>;

    tester()(UTF8_ENGLISH);
#if defined(HAVE_WFOPEN)         // WINDOWS
    tester()(UTF16_ENGLISH);
    tester()(UTF16_KOREAN);
#else                           // POSIX
    tester()(UTF8_KOREAN);
#endif
}

#endif                                                  // MMAP

#include <warnings/pop.h>
