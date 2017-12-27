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

static void test_lowlevel(const string& input,
    const string& expected,
    hex_lowlevel_callback cb)
{
    byte_allocator alloc;
    const char* src = input.data();
    char* dst = nullptr;
    size_t dstlen = 20;

    try {
        dst = (char*) alloc.allocate(dstlen);
        const void* src_first = src;
        void* dst_first = dst;
        cb(src_first, input.size(), dst_first, dstlen, alloc);
        EXPECT_EQ(distance(dst, (char*) dst_first), expected.size());
        EXPECT_EQ(strncmp(dst, expected.data(), expected.size()), 0);
    } catch (...) {
        if (dst) {
            alloc.deallocate((byte*) dst, dstlen);
        }
        throw;
    }

    alloc.deallocate((byte*) dst, dstlen);
}

// TESTS
// -----


TEST(hex, hex8)
{
    // high-level
    string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    string expected = "0001020304050607";
    EXPECT_EQ(hex(bytes, byte_allocator(), 1), expected);
    EXPECT_EQ(hex_i8(bytes), expected);

    // low-level
    test_lowlevel(bytes, expected, hex_lowlevel_callback(hex_i8));
}


TEST(hex, hex32)
{
    // high-level
    string expected = "0302010007060504";
    string bytes = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(hex(bytes, byte_allocator(), 4), expected);
    EXPECT_EQ(hex_i32(bytes), expected);

    // low-level
    test_lowlevel(bytes, expected, hex_lowlevel_callback(hex_i32));
}


TEST(unhex, unhex8)
{
    // high-level
    string bytes = "0001020304050607";
    string expected = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(unhex(bytes, byte_allocator(), 1), expected);
    EXPECT_EQ(unhex_i8(bytes), expected);

    // low-level
    test_lowlevel(bytes, expected, hex_lowlevel_callback(unhex_i8));
}


TEST(unhex, unhex32)
{
    // high-level
    string bytes = "0302010007060504";
    string expected = {0, 1, 2, 3, 4, 5, 6, 7};
    EXPECT_EQ(unhex(bytes, byte_allocator(), 4), expected);
    EXPECT_EQ(unhex_i32(bytes), expected);

    // low-level
    test_lowlevel(bytes, expected, hex_lowlevel_callback(unhex_i32));
}
