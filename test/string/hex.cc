//  :copyright: (c) 2017 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.
/*
 *  \addtogroup Tests
 *  \brief Hex representation unittests.
 */

#include <pycpp/preprocessor/byteorder.h>
#include <pycpp/string/hex.h>
#include <gtest/gtest.h>

PYCPP_USING_NAMESPACE

// HELPERS
// -------

static void test_lowlevel(const std::string& input, const std::string& expected, hex_lowlevel_callback cb)
{
    const char* src = input.data();
    char* dst = nullptr;

    try {
        dst = new char[20];
        const void* src_first = src;
        void* dst_first = dst;
        cb(src_first, input.size(), dst_first, 20);
        EXPECT_EQ(std::distance(dst, (char*) dst_first), expected.size());
        EXPECT_EQ(strncmp(dst, expected.data(), expected.size()), 0);
    } catch (...) {
        delete[] dst;
        throw;
    }

    delete[] dst;
}

// TESTS
// -----


TEST(hex, hex8)
{
    // high-level
    std::string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    std::string expected = "0001020304050607";
    EXPECT_EQ(hex(bytes, 1), expected);
    EXPECT_EQ(hex_i8(bytes), expected);

    // low-level
    test_lowlevel(bytes, expected, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        hex_i8(src, srclen, dst, dstlen);
    });
}


TEST(hex, hex32)
{
    // high-level
    std::string expected = "0302010007060504";
    std::string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(hex(bytes, 4), expected);
    EXPECT_EQ(hex_i32(bytes), expected);

    // low-level
    test_lowlevel(bytes, expected, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        hex_i32(src, srclen, dst, dstlen);
    });
}


TEST(unhex, unhex8)
{
    // high-level
    std::string bytes = "0001020304050607";
    std::string expected = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(unhex(bytes, 1), expected);
    EXPECT_EQ(unhex_i8(bytes), expected);

    // low-level
    test_lowlevel(bytes, expected, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        unhex_i8(src, srclen, dst, dstlen);
    });
}


TEST(unhex, unhex32)
{
    // high-level
    std::string bytes = "0302010007060504";
    std::string expected = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(unhex(bytes, 4), expected);
    EXPECT_EQ(unhex_i32(bytes), expected);

    // low-level
    test_lowlevel(bytes, expected, [](const void*& src, size_t srclen, void*& dst, size_t dstlen) {
        unhex_i32(src, srclen, dst, dstlen);
    });
}
